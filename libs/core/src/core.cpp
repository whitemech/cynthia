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
#include <cynthia/logic/nnf.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {

ISynthesis::ISynthesis(const logic::ltlf_ptr& formula,
                       const InputOutputPartition& partition)
    : formula{formula}, partition{partition} {}

bool ForwardSynthesis::is_realizable() {
  bool result = forward_synthesis_();
  return result;
}

bool ForwardSynthesis::forward_synthesis_() {
  auto path = std::set<SddSize>{};
  context_.logger.info("Building the root SDD node...");
  auto root_sdd_node = to_sdd(*context_.xnf_formula, context_);
  auto sdd_formula_id = sdd_id(root_sdd_node);
  context_.logger.info("Starting first system move...");
  auto strategy = system_move_(context_.xnf_formula, path);
  bool result = strategy[sdd_formula_id] != sdd_manager_false(context_.manager);
  context_.logger.info("Explored states: {}",
                       context_.statistics_.nb_visited_nodes());
  return result;
}

std::map<std::string, size_t> ForwardSynthesis::compute_prop_to_id_map(
    const Closure& closure, const InputOutputPartition& partition) {
  std::map<std::string, size_t> result;
  size_t offset = closure.nb_formulas();
  size_t i = offset;
  for (const auto& p : partition.input_variables) {
    result[p] = i++;
  }
  for (const auto& p : partition.output_variables) {
    result[p] = i++;
  }
  return result;
}

strategy_t ForwardSynthesis::system_move_(const logic::ltlf_ptr& formula,
                                          std::set<SddSize>& path) {
  strategy_t success_strategy, failure_strategy;
  context_.indentation += 1;
  auto formula_str = logic::to_string(*formula);
  auto sdd = SddNodeWrapper(to_sdd(*formula, context_));
  auto sdd_formula_id = sdd.get_id();
  context_.statistics_.visit_node(sdd_formula_id);
  success_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
  failure_strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
  this->print_search_debug("State {}", sdd_formula_id);

  if (context_.discovered.find(sdd_formula_id) != context_.discovered.end()) {
    context_.indentation -= 1;
    bool is_success = context_.discovered[sdd_formula_id];
    if (is_success) {
      this->print_search_debug("{} already discovered, success",
                               sdd_formula_id);
      return success_strategy;
    } else {
      this->print_search_debug("{} already discovered, failure",
                               sdd_formula_id);
      return failure_strategy;
    }
  }

  if (path.find(sdd_formula_id) != path.end()) {
    this->print_search_debug("Already visited state {}, failure",
                             sdd_formula_id);
    context_.discovered[sdd_formula_id] = false;
    context_.indentation -= 1;
    return failure_strategy;
  }

  if (eval(*formula)) {
    this->print_search_debug("{} accepting!", sdd_formula_id);
    context_.discovered[sdd_formula_id] = true;
    context_.indentation -= 1;
    return success_strategy;
  }

  path.insert(sdd_formula_id);
  if (sdd.get_type() == SddNodeType::STATE) {
    auto system_move_str =
        logic::to_string(*sdd_to_formula(sdd.get_raw(), context_));
    this->print_search_debug("system move (unique): {}", system_move_str);
    auto new_strategy = env_move_(sdd, path);
    if (!new_strategy.empty()) {
      path.erase(sdd_formula_id);
      context_.discovered[sdd_formula_id] = true;
      context_.indentation -= 1;
      return success_strategy;
    }
  } else if (sdd.get_type() == SddNodeType::ENV_STATE) {
    // not at the vtree root; it means that system choice is irrelevant
    this->print_search_debug("system choice is irrelevant");
    auto env_state_node = sdd;
    auto new_strategy = env_move_(env_state_node, path);
    if (!new_strategy.empty()) {
      this->print_search_debug("Any system move is a success from state {}!",
                               sdd_formula_id);
      path.erase(sdd_formula_id);
      // all system moves are OK, since it does not have control
      context_.discovered[sdd_formula_id] = true;
      context_.indentation -= 1;
      new_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
      return new_strategy;
    }
  } else {
    // is a decision node
    auto child_it = sdd.begin();
    auto children_end = sdd.end();

    if (child_it == children_end) {
      this->print_search_debug("No children, {} is failure", sdd_formula_id);
      path.erase(sdd_formula_id);
      context_.discovered[sdd_formula_id] = false;
      context_.indentation -= 1;
      return failure_strategy;
    }

    this->print_search_debug("Processing {} system node's children nodes",
                             sdd.nb_children());
    std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> new_children;
    new_children.reserve(sdd.nb_children());
    for (; child_it != children_end; ++child_it) {
      bool ignore = false;
      auto system_move = SddNodeWrapper(child_it.get_prime());
      auto env_state_node = SddNodeWrapper(child_it.get_sub());
      if (env_state_node.get_type() == STATE) {
        auto next_state = next_state_(env_state_node);
        auto next_state_result_it =
            context_.discovered.find(next_state.get_id());
        if (next_state_result_it != context_.discovered.end()) {
          auto next_state_is_success = next_state_result_it->second;
          if (next_state_is_success) {
            this->print_search_debug(
                "system look-ahead: next state {} already discovered, success",
                next_state.get_id());
            path.erase(sdd_formula_id);
            context_.indentation -= 1;
            strategy_t strategy;
            strategy[sdd_formula_id] = system_move.get_raw();
            return strategy;
          } else {
            this->print_search_debug("system look-ahead: next state {} already "
                                     "discovered, failure, ignoring",
                                     next_state.get_id());
            continue;
          }
        }
        this->print_search_debug(
            "system look-ahead: next state {} not discovered yet ",
            next_state.get_id());
        new_children.emplace_back(system_move, env_state_node);
      } else {
        // we don't know, need to take env action
        this->print_search_debug("system look-ahead: {} is not a state node",
                                 env_state_node.get_id());
        new_children.emplace_back(system_move, env_state_node);
      }
    }

    for (const auto& pair : new_children) {
      auto system_move = pair.first;
      auto env_state_node = pair.second;
      auto system_move_str =
          logic::to_string(*sdd_to_formula(system_move.get_raw(), context_));
      this->print_search_debug("checking system move: {}", system_move_str);
      ++child_it;
      if (system_move.is_false())
        continue;
      auto new_strategy = env_move_(env_state_node, path);
      if (!new_strategy.empty()) {
        this->print_search_debug("System move {} from state {} is successful",
                                 sdd_formula_id, system_move_str);
        path.erase(sdd_formula_id);
        new_strategy[sdd_formula_id] = system_move.get_raw();
        context_.discovered[sdd_formula_id] = true;
        context_.indentation -= 1;
        return new_strategy;
      }
    }
  }

  this->print_search_debug("State {} is failure", sdd_formula_id);
  path.erase(sdd_formula_id);
  context_.discovered[sdd_formula_id] = false;
  context_.indentation -= 1;
  return failure_strategy;
}

strategy_t ForwardSynthesis::env_move_(SddNodeWrapper& wrapper,
                                       std::set<SddSize>& path) {
  context_.indentation += 1;
  auto sdd_formula_id = wrapper.get_id();
  if (wrapper.get_type() == SddNodeType::STATE) {
    auto formula_next_state = next_state_formula_(wrapper.get_raw());
    auto sdd_next_state = formula_to_sdd_(formula_next_state);
    auto sdd_next_state_id = sdd_next_state.get_id();
    this->print_search_debug("env move forced to next state {}",
                             sdd_next_state_id);
    auto strategy = system_move_(formula_next_state, path);
    context_.indentation -= 1;
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else if (wrapper.get_type() == SddNodeType::SYSTEM_STATE) {
    // parent is not the vtree root; it means that environment choice is
    // irrelevant
    this->print_search_debug("env choice is irrelevant");
    auto formula_next_state = next_state_formula_(wrapper.get_raw());
    auto sdd_next_state = formula_to_sdd_(formula_next_state);
    auto sdd_next_state_id = sdd_next_state.get_id();
    auto strategy = system_move_(formula_next_state, path);
    context_.indentation -= 1;
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else {
    assert(wrapper.get_type() == ENV_STATE);
    auto child_it = wrapper.begin();
    auto children_end = wrapper.end();
    this->print_search_debug("Processing {} env node's children nodes",
                             wrapper.nb_children());
    std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> new_children;
    new_children.reserve(wrapper.nb_children());
    for (; child_it != children_end; ++child_it) {
      bool ignore = false;
      auto env_node = SddNodeWrapper(child_it.get_prime());
      auto state_node = SddNodeWrapper(child_it.get_sub());
      assert(state_node.get_type() == STATE);
      auto next_state = next_state_(state_node);
      auto next_state_result_it = context_.discovered.find(next_state.get_id());
      if (next_state_result_it != context_.discovered.end()) {
        auto next_state_is_success = next_state_result_it->second;
        if (next_state_is_success) {
          this->print_search_debug("env look-ahead: next state {} already "
                                   "discovered, success, ignoring",
                                   next_state.get_id());
          ignore = true;
        }
        if (!next_state_is_success) {
          this->print_search_debug(
              "env look-ahead: next state {} already discovered, failure",
              next_state.get_id());
          path.erase(sdd_formula_id);
          context_.indentation -= 1;
          return strategy_t{};
        }
      }
      if (!ignore) {
        // we don't know, need to take env action
        this->print_search_debug(
            "env look-ahead: next state {} not discovered yet",
            state_node.get_id());
        new_children.emplace_back(env_node, state_node);
      }
    }

    if (new_children.empty()) {
      // take any successor, it will be a win
      this->print_search_debug(
          "env look-ahead: taking any env action, system wins");
      auto formula_next_state = next_state_formula_(wrapper.begin().get_sub());
      context_.indentation -= 1;
      return system_move_(formula_next_state, path);
    }
    strategy_t final_strategy;

    for (const auto& pair : new_children) {
      auto env_move = pair.first;
      auto state_node = pair.second;
      auto env_action = sdd_to_formula(env_move.get_raw(), context_);
      auto env_action_str = logic::to_string(*env_action);
      auto formula_next_state = next_state_formula_(state_node.get_raw());
      auto sdd_next_state = formula_to_sdd_(formula_next_state);
      auto sdd_next_state_id = sdd_next_state.get_id();
      this->print_search_debug("env move: {}", env_action_str);
      auto strategy = system_move_(formula_next_state, path);
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

logic::ltlf_ptr ForwardSynthesis::next_state_formula_(SddNode* sdd_ptr) {
  auto sdd_formula = sdd_to_formula(sdd_ptr, context_);
  auto next_state_formula = xnf(*strip_next(*sdd_formula));
  return next_state_formula;
}
SddNodeWrapper
ForwardSynthesis::formula_to_sdd_(const logic::ltlf_ptr& formula) {
  auto wrapper = SddNodeWrapper(to_sdd(*formula, context_));
  return wrapper;
}
SddNodeWrapper ForwardSynthesis::next_state_(const SddNodeWrapper& wrapper) {
  auto next_state_formula = next_state_formula_(wrapper.get_raw());
  auto sdd_next_state = formula_to_sdd_(next_state_formula);
  return sdd_next_state;
}

ForwardSynthesis::Context::Context(const logic::ltlf_ptr& formula,
                                   const InputOutputPartition& partition)
    : logger{"cynthia"}, formula{formula}, partition{partition},
      ast_manager{&formula->ctx()} {
  nnf_formula = logic::to_nnf(*formula);
  auto formula_str = logic::to_string(*nnf_formula);
  xnf_formula = xnf(*nnf_formula);
  Closure closure_object = closure(*xnf_formula);
  closure_ = closure_object;
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager = sdd_manager_new(vtree_);
  prop_to_id = compute_prop_to_id_map(closure_, partition);
  statistics_ = Statistics();
}
} // namespace core
} // namespace cynthia
