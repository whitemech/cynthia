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
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>

namespace cynthia {
namespace core {

ISynthesis::ISynthesis(const logic::ltlf_ptr& formula,
                       const InputOutputPartition& partition)
    : formula{formula}, partition{partition} {}

bool ForwardSynthesis::is_realizable() {
  bool result = false;
  context_ = ForwardSynthesis::Context(formula, partition);
  auto strategy = forward_synthesis_();

  return result;
}

bool ForwardSynthesis::forward_synthesis_() {
  auto path = logic::set_ptr{};
  auto strategy = system_move_(context_.nnf_formula, path);
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
                                          logic::set_ptr& path) {
  strategy_t strategy;
  if (eval(*formula)) {
    strategy[formula] = sdd_manager_true(context_.manager);
    return strategy;
  }
  if (path.find(formula) != path.end()) {
    strategy[formula] = sdd_manager_false(context_.manager);
    return strategy;
  }

  auto sdd = SddNodeWrapper(to_sdd(*formula, context_));
  auto child_it = sdd.begin();
  auto children_end = sdd.end();

  if (child_it == children_end) {
    strategy[formula] = sdd_manager_false(context_.manager);
    return strategy;
  }

  path.insert(formula);
  while (child_it != children_end) {
    auto system_move = child_it.get_prime();
    if (sdd_node_is_false(system_move))
      continue;
    auto env_state_node = SddNodeWrapper(child_it.get_sub());
    if (env_state_node.nb_children() == 0)
      continue;
    auto new_strategy = env_move_(env_state_node, path);
    if (!new_strategy.empty()) {
      path.erase(formula);
      new_strategy[formula] = system_move;
      return new_strategy;
    }
    ++child_it;
  }

  path.erase(formula);
  strategy[formula] = sdd_manager_false(context_.manager);
  return strategy;
}

strategy_t ForwardSynthesis::env_move_(SddNodeWrapper& wrapper,
                                       logic::set_ptr& path) {
  auto child_it = wrapper.begin();
  auto children_end = wrapper.end();
  strategy_t final_strategy;
  for (; child_it != children_end; ++child_it) {
    logic::ltlf_ptr next_state = sdd_to_formula(*child_it, context_);
    auto strategy = system_move_(next_state, path);
    if (strategy[formula] == sdd_manager_false(context_.manager))
      return strategy_t{};
    final_strategy.insert(strategy.begin(), strategy.end());
  }
  return final_strategy;
}

ForwardSynthesis::Context::Context(const logic::ltlf_ptr& formula,
                                   const InputOutputPartition& partition)
    : formula{formula}, partition{partition}, ast_manager{&formula->ctx()} {
  nnf_formula = logic::to_nnf(*formula);
  Closure closure_object = closure(*nnf_formula);
  closure_ = closure_object;
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager = sdd_manager_new(vtree_);
  prop_to_id = compute_prop_to_id_map(closure_, partition);
}
} // namespace core
} // namespace cynthia
