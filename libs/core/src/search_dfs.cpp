#pragma once
/*
 * This file is part of Cynthia.
 *
 * Cynthia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cynthia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cynthia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cynthia/core.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/logic/types.hpp>
#include <cynthia/one_step_realizability.hpp>
#include <cynthia/one_step_unrealizability.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/search_dfs.hpp>

extern "C" {
#include "sddapi.h"
}

namespace cynthia {
namespace core {

SearchDFS::SearchDFS(Problem* problem, Heuristic* heuristic)
    : problem_{problem}, heuristic_{heuristic} {
  init_node_ = new SearchNode(problem->init_state(), nullptr, 0);
  auto h_value = heuristic_->get_h(init_node_->state_);
  init_node_->set_heuristic(h_value);
  context_ = problem_->get_context();
}

std::vector<SearchConnector*> SearchDFS::expand_(SearchNode* node) {
  std::vector<SearchConnector*> connectors;
  std::vector<SddNodeWrapper> ops = node->state_->compute_ops();
  connectors.reserve(ops.size());

  for (const auto& op : ops) {
    std::vector<State*> states = node->state_->apply_op(op);
    std::set<SearchNode*> children;
    for (const auto& state : states) {
      SearchNode* new_node = new SearchNode(state, node, 0);
      auto h_value = heuristic_->get_h(new_node->state_);
      new_node->set_heuristic(h_value);
      children.insert(new_node);
    }
    SearchConnector* connector = new SearchConnector(node, children, op);
    connectors.emplace_back(connector);
  }

  return connectors;
}

bool SearchDFS::forward_search() {
  auto path = std::set<SddSize>{};

  context_->logger.info("Check zero-step realizability");
  if (this->init_node_->state_->is_goal_state()) {
    context_->logger.info("Zero-step realizability check successful");
    return true;
  }

  context_->logger.info("Check one-step realizability");
  auto pair_rel_result = one_step_realizability(
      *(init_node_->state_->get_nnf_formula()), *context_);
  if (pair_rel_result.second) {
    context_->logger.info("One-step realizability check successful");
    return true;
  }
  context_->logger.info("Check one-step unrealizability");
  auto is_unrealizable = one_step_unrealizability(
      *(init_node_->state_->get_nnf_formula()), *context_);
  if (!is_unrealizable) {
    context_->logger.info("One-step unrealizability check successful");
    return false;
  }

  context_->logger.info("Building the root SDD node...");
  init_node_->state_->instantiate();
  context_->logger.info("Starting first system move...");
  auto strategy = do_search_(init_node_, path);
  bool result =
      strategy[init_node_->get_index()] != sdd_manager_false(context_->manager);
  context_->logger.info("Explored states: {}",
                        context_->statistics_.nb_visited_nodes());
  return result;
}

strategy_t SearchDFS::do_search_(SearchNode* node, std::set<SddSize>& path) {
  strategy_t success_strategy, failure_strategy;
  context_->indentation += 1;
  auto sdd_formula_id = node->get_index();
  context_->statistics_.visit_node(sdd_formula_id);

  success_strategy[sdd_formula_id] = sdd_manager_true(context_->manager);
  failure_strategy[sdd_formula_id] = sdd_manager_false(context_->manager);
  context_->print_search_debug("State {}", sdd_formula_id);

  if (context_->discovered.find(sdd_formula_id) != context_->discovered.end()) {
    context_->indentation -= 1;
    bool is_success = context_->discovered[sdd_formula_id];
    if (is_success) {
      context_->print_search_debug("{} already discovered, success",
                                   sdd_formula_id);
      return success_strategy;
    } else {
      context_->print_search_debug("{} already discovered, failure",
                                   sdd_formula_id);
      return failure_strategy;
    }
  }

  if (path.find(sdd_formula_id) != path.end()) {
    context_->print_search_debug("Already visited state {}, failure",
                                 sdd_formula_id);
    context_->discovered[sdd_formula_id] = false;
    context_->indentation -= 1;
    return failure_strategy;
  }

  if (node->is_goal()) {
    context_->print_search_debug("{} accepting!", sdd_formula_id);
    context_->discovered[sdd_formula_id] = true;
    context_->indentation -= 1;
    return success_strategy;
  }

  auto one_step_realizability_result =
      one_step_realizability(*(node->state_->get_nnf_formula()), *context_);
  if (one_step_realizability_result.second) {
    strategy_t strategy;
    strategy[sdd_formula_id] = one_step_realizability_result.first;
    context_->discovered[sdd_formula_id] = true;
    context_->indentation -= 1;
    return strategy;
  }
  auto is_unrealizable =
      one_step_unrealizability(*(node->state_->get_nnf_formula()), *context_);
  if (!is_unrealizable) {
    context_->discovered[sdd_formula_id] = false;
    context_->indentation -= 1;
    return failure_strategy;
  }

  path.insert(sdd_formula_id);
  strategy_t final_strategy;
  std::vector<SearchConnector*> connectors = expand_(node);
  for (const auto connector : connectors) {
    auto system_move = connector->get_operator();
    auto system_move_str =
        logic::to_string(*sdd_to_formula(system_move.get_raw(), *context_));
    context_->print_search_debug("checking system move: {}", system_move_str);
    auto children = connector->get_children();
    for (const auto s : children) {
      s->state_->instantiate();
      auto strategy = do_search_(s, path);
      if (strategy[s->get_index()] == sdd_manager_false(context_->manager)) {
        context_->indentation -= 1;
        return strategy_t{};
      }
      final_strategy.insert(strategy.begin(), strategy.end());
    }
  }
  return final_strategy;
}

} // namespace core
} // namespace cynthia