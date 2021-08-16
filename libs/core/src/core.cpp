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
  auto root_sdd_node = to_sdd(*context_.xnf_formula, context_);
  auto sdd_formula_id = sdd_id(root_sdd_node);
  auto strategy = system_move_(context_.xnf_formula, path);
  bool result = strategy[sdd_formula_id] != sdd_manager_false(context_.manager);
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

strategy_t ForwardSynthesis::system_move_(const logic::ltlf_ptr& formula_,
                                          std::set<SddSize>& path) {
  strategy_t strategy;
  auto formula = xnf(*formula_);
  auto formula_str = logic::to_string(*formula);
  auto sdd = SddNodeWrapper(to_sdd(*formula, context_));
  auto sdd_formula_id = sdd_id(sdd.get_raw());
  if (eval(*formula)) {
    strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
    context_.discovered.insert(sdd_formula_id);
    return strategy;
  }
  if (path.find(sdd_formula_id) != path.end() or
      context_.discovered.find(sdd_formula_id) != context_.discovered.end()) {
    strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
    context_.discovered.insert(sdd_formula_id);
    return strategy;
  }

  path.insert(sdd_formula_id);
  if (!sdd.is_decision()) {
    auto new_strategy = env_move_(sdd, path);
    if (!new_strategy.empty()) {
      path.erase(sdd_formula_id);
      new_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
      context_.discovered.insert(sdd_formula_id);
      return new_strategy;
    }
  } else if (sdd.get_raw()->vtree->parent != NULL) {
    // not at the vtree root; it means that system choice is irrelevant
    auto env_state_node = sdd;
    auto new_strategy = env_move_(env_state_node, path);
    if (!new_strategy.empty()) {
      path.erase(sdd_formula_id);
      // all system moves are OK, since it does not have control
      new_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
      context_.discovered.insert(sdd_formula_id);
      return new_strategy;
    }
  } else {
    // is a decision node
    auto child_it = sdd.begin();
    auto children_end = sdd.end();

    if (child_it == children_end) {
      path.erase(sdd_formula_id);
      strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
      context_.discovered.insert(sdd_formula_id);
      return strategy;
    }

    while (child_it != children_end) {
      auto system_move = child_it.get_prime();
      auto env_state_node = SddNodeWrapper(child_it.get_sub());
      ++child_it;
      if (sdd_node_is_false(system_move))
        continue;
      auto new_strategy = env_move_(env_state_node, path);
      if (!new_strategy.empty()) {
        path.erase(sdd_formula_id);
        new_strategy[sdd_formula_id] = system_move;
        context_.discovered.insert(sdd_formula_id);
        return new_strategy;
      }
    }
  }

  path.erase(sdd_formula_id);
  strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
  context_.discovered.insert(sdd_formula_id);
  return strategy;
}

strategy_t ForwardSynthesis::env_move_(SddNodeWrapper& wrapper,
                                       std::set<SddSize>& path) {
  logic::ltlf_ptr next_state, sdd_formula;
  if (!wrapper.is_decision()) {
    sdd_formula = sdd_to_formula(wrapper.get_raw(), context_);
    auto sdd_formula_str = logic::to_string(*sdd_formula);
    next_state = xnf(*strip_next(*sdd_formula));
    auto next_state_str = logic::to_string(*next_state);
    auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
    auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
    auto strategy = system_move_(next_state, path);
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else if (wrapper.get_raw()->vtree->parent->parent != NULL) {
    // parent is not the vtree root; it means that environment choice is
    // irrelevant
    sdd_formula = sdd_to_formula(wrapper.get_raw(), context_);
    next_state = xnf(*strip_next(*sdd_formula));
    auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
    auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
    auto strategy = system_move_(next_state, path);
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
      auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
      auto sdd_next_state_id = sdd_id(sdd_next_state.get_raw());
      auto strategy = system_move_(next_state, path);
      if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
        return strategy_t{};
      final_strategy.insert(strategy.begin(), strategy.end());
    }
    return final_strategy;
  }
}

ForwardSynthesis::Context::Context(const logic::ltlf_ptr& formula,
                                   const InputOutputPartition& partition)
    : formula{formula}, partition{partition}, ast_manager{&formula->ctx()} {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  Closure closure_object = closure(*xnf_formula);
  closure_ = closure_object;
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager = sdd_manager_new(vtree_);
  prop_to_id = compute_prop_to_id_map(closure_, partition);
}
} // namespace core
} // namespace cynthia
