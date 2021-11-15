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

#include <cynthia/closure.hpp>
#include <cynthia/core.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/logger.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/logic/types.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>

extern "C" {
#include "sddapi.h"
}

namespace cynthia {
namespace core {

ForwardSynthesis::Search::Search(Problem& problem,
                                 ForwardSynthesis& forwardSynthesis)
    : problem_{problem}, init_node{problem.formula}, forwardSynthesis_{
                                                         forwardSynthesis} {}

std::vector<ForwardSynthesis::SearchConnector>
ForwardSynthesis::Search::expand_(SearchNode* node) {
  auto sdd = node->sdd;
  std::vector<ForwardSynthesis::SearchConnector> connectors;
  // is a decision node
  auto child_it = sdd.begin();
  auto children_end = sdd.end();

  if (child_it == children_end) {
    return connectors;
  }

  problem_.print_search_debug("Processing {} system node's children nodes",
                              sdd.nb_children());
  std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> new_children;
  new_children.reserve(sdd.nb_children());
  for (; child_it != children_end; ++child_it) {
    auto system_move = SddNodeWrapper(child_it.get_prime(), problem_.manager);
    auto env_state_node = SddNodeWrapper(child_it.get_sub(), problem_.manager);

    new_children.emplace_back(system_move, env_state_node);
  }

  for (const auto& pair : new_children) {

    auto system_move = pair.first;
    auto env_state_node = pair.second;
    auto system_move_str =
        logic::to_string(*sdd_to_formula(system_move.get_raw(), problem_));
    problem_.print_search_debug("checking system move: {}", system_move_str);
    ++child_it;
    std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> env_children;

    if (env_state_node.get_type() != SddNodeType::ENV_STATE) {
      auto formula_next_state =
          forwardSynthesis_.next_state_formula_(env_state_node.get_raw());
      auto sdd_next_state =
          forwardSynthesis_.formula_to_sdd_(formula_next_state);
      auto sdd_next_state_id = sdd_next_state.get_id();
      problem_.print_search_debug("env move forced to next state {}",
                                  sdd_next_state_id);
      std::pair<SddNodeWrapper, SddNodeWrapper> env_child;
      env_child.first =
          forwardSynthesis_.formula_to_sdd_(problem_.ast_manager->make_tt());
      ;
      env_child.second = sdd_next_state;
      env_children.emplace_back(env_child);
      ForwardSynthesis::SearchConnector connector =
          ForwardSynthesis::SearchConnector(problem_, node, env_children);
      connectors.emplace_back(connector);
    } else {
      auto env_child_it = env_state_node.begin();
      auto env_children_end = env_state_node.end();
      problem_.print_search_debug("Processing {} env node's children nodes",
                                  env_state_node.nb_children());
      env_children.reserve(env_state_node.nb_children());
      for (; env_child_it != env_children_end; ++env_child_it) {
        bool ignore = false;
        auto env_node =
            SddNodeWrapper(env_child_it.get_prime(), problem_.manager);
        auto state_node =
            SddNodeWrapper(env_child_it.get_sub(), problem_.manager);
        std::pair<SddNodeWrapper, SddNodeWrapper> env_child;
        env_child.first = env_node;
        env_child.second = state_node;
        env_children.emplace_back(env_child);
      }
      ForwardSynthesis::SearchConnector connector =
          ForwardSynthesis::SearchConnector(problem_, node, env_children);
      connectors.emplace_back(connector);
    }
  }

  return connectors;
}

strategy_t ForwardSynthesis::Search::do_search_(const logic::ltlf_ptr& formula,
                                                std::set<SddSize>& path) {
  strategy_t success_strategy;
  return success_strategy;
}

} // namespace core
} // namespace cynthia