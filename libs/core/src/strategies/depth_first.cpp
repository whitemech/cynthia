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
#include <cynthia/eval.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/strategies/depth_first.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {

bool ForwardDfsSynthesis::is_realizable() {
  bool result = forward_synthesis_();
  return result;
}

bool ForwardDfsSynthesis::forward_synthesis_() {
  auto path = std::set<SddSize>{};
  auto root_sdd_node = to_sdd(*context_.xnf_formula, context_);
  auto sdd_formula_id = sdd_id(root_sdd_node);
  auto strategy = system_move_(context_.xnf_formula, path);
  bool result = strategy[sdd_formula_id] != sdd_manager_false(context_.manager);
  context_.logger.info("Explored states: {}",
                       context_.statistics_.nb_visited_nodes());
  return result;
}

strategy_t ForwardDfsSynthesis::system_move_(const logic::ltlf_ptr& formula,
                                             std::set<SddSize>& path) {
  strategy_t success_strategy, failure_strategy;
  context_.indentation += 1;
  auto formula_str = logic::to_string(*formula);
  auto sdd = SddNodeWrapper(to_sdd(*formula, context_));
  auto sdd_formula_id = sdd_id(sdd.get_raw());
  context_.statistics_.visit_node(sdd_formula_id);
  success_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
  failure_strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
  context_.print_search_debug("State {}", sdd_formula_id);
  if (eval(*formula)) {
    context_.print_search_debug("{} accepting!", sdd_formula_id);
    context_.discovered[sdd_formula_id] = true;
    context_.indentation -= 1;
    return success_strategy;
  }

  if (context_.discovered.find(sdd_formula_id) != context_.discovered.end()) {
    context_.indentation -= 1;
    bool is_success = context_.discovered[sdd_formula_id];
    if (is_success) {
      context_.print_search_debug("{} already discovered, success",
                                  sdd_formula_id);
      return success_strategy;
    } else {
      context_.print_search_debug("{} already discovered, failure",
                                  sdd_formula_id);
      return failure_strategy;
    }
  } else if (path.find(sdd_formula_id) != path.end()) {
    context_.print_search_debug("Already visited state {}, failure",
                                sdd_formula_id);
    context_.discovered[sdd_formula_id] = false;
    context_.indentation -= 1;
    return failure_strategy;
  }

  path.insert(sdd_formula_id);
  if (!sdd.is_decision()) {
    auto system_move_str =
        logic::to_string(*sdd_to_formula(sdd.get_raw(), context_));
    context_.print_search_debug("system move (unique): {}", system_move_str);
    auto new_strategy = env_move_(sdd, path);
    if (!new_strategy.empty()) {
      path.erase(sdd_formula_id);
      context_.discovered[sdd_formula_id] = true;
      context_.indentation -= 1;
      return success_strategy;
    }
  } else if (!sdd.at_vtree_root()) {
    // not at the vtree root; it means that system choice is irrelevant
    context_.print_search_debug("system choice is irrelevant");
    auto env_state_node = sdd;
    auto new_strategy = env_move_(env_state_node, path);
    if (!new_strategy.empty()) {
      context_.print_search_debug("Any system move is a success from state {}!",
                                  sdd_formula_id);
      path.erase(sdd_formula_id);
      // all system moves are OK, since it does not have control
      context_.discovered[sdd_formula_id] = true;
      context_.indentation -= 1;
      return success_strategy;
    }
  } else {
    // is a decision node
    auto child_it = sdd.begin();
    auto children_end = sdd.end();

    if (child_it == children_end) {
      path.erase(sdd_formula_id);
      context_.discovered[sdd_formula_id] = false;
      context_.indentation -= 1;
      return failure_strategy;
    }

    while (child_it != children_end) {
      auto system_move = child_it.get_prime();
      auto system_move_str =
          logic::to_string(*sdd_to_formula(system_move, context_));
      context_.print_search_debug("checking system move: {}", system_move_str);
      auto env_state_node = SddNodeWrapper(child_it.get_sub());
      ++child_it;
      if (sdd_node_is_false(system_move))
        continue;
      auto new_strategy = env_move_(env_state_node, path);
      if (!new_strategy.empty()) {
        context_.print_search_debug(
            "System move {} from state {} is successful", sdd_formula_id,
            system_move_str);
        path.erase(sdd_formula_id);
        new_strategy[sdd_formula_id] = system_move;
        context_.discovered[sdd_formula_id] = true;
        context_.indentation -= 1;
        return new_strategy;
      }
    }
  }

  context_.print_search_debug("State {} is failure", sdd_formula_id);
  path.erase(sdd_formula_id);
  context_.discovered[sdd_formula_id] = false;
  context_.indentation -= 1;
  return failure_strategy;
}

strategy_t ForwardDfsSynthesis::env_move_(SddNodeWrapper& wrapper,
                                          std::set<SddSize>& path) {
  logic::ltlf_ptr next_state, sdd_formula;
  context_.indentation += 1;
  if (!wrapper.is_decision()) {
    sdd_formula = sdd_to_formula(wrapper.get_raw(), context_);
    auto sdd_formula_str = logic::to_string(*sdd_formula);
    next_state = xnf(*strip_next(*sdd_formula));
    auto next_state_str = logic::to_string(*next_state);
    auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
    auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
    context_.print_search_debug("env move forced to next state {}",
                                sdd_next_state_id);
    auto strategy = system_move_(next_state, path);
    context_.indentation -= 1;
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else if (!wrapper.parent_at_vtree_root()) {
    // parent is not the vtree root; it means that environment choice is
    // irrelevant
    context_.print_search_debug("env choice is irrelevant");
    sdd_formula = sdd_to_formula(wrapper.get_raw(), context_);
    next_state = xnf(*strip_next(*sdd_formula));
    auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
    auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
    auto strategy = system_move_(next_state, path);
    context_.indentation -= 1;
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else {
    auto child_it = wrapper.begin();
    auto children_end = wrapper.end();
    strategy_t final_strategy;
    for (; child_it != children_end; ++child_it) {
      auto env_action = sdd_to_formula(child_it.get_prime(), context_);
      auto env_action_str = logic::to_string(*env_action);
      sdd_formula = sdd_to_formula(child_it.get_sub(), context_);
      next_state = xnf(*strip_next(*sdd_formula));
      auto next_state_str = logic::to_string(*next_state);
      auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
      auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
      context_.print_search_debug("env move: {}", env_action_str);
      auto strategy = system_move_(next_state, path);
      if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager)) {
        context_.indentation -= 1;
        return strategy_t{};
      }
      final_strategy.insert(strategy.begin(), strategy.end());
    }
    context_.indentation -= 1;
    return final_strategy;
  }
}

} // namespace core
} // namespace cynthia
