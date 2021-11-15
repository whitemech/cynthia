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
#include <cynthia/eval.hpp>
#include <cynthia/logic/nnf.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/one_step_realizability.hpp>
#include <cynthia/one_step_unrealizability.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {

ForwardSynthesis::Problem::Problem(const logic::ltlf_ptr& formula,
                                   const InputOutputPartition& partition,
                                   bool use_gc, float gc_threshold)
    : logger{"cynthia"}, formula{formula}, partition{partition}, use_gc{use_gc},
      gc_threshold{gc_threshold}, ast_manager{&formula->ctx()} {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  Closure closure_object = closure(*xnf_formula);
  closure_ = closure_object;
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager = sdd_manager_new(vtree_);
  prop_to_id = compute_prop_to_id_map(closure_, partition);
  statistics_ = Statistics();
  initialie_maps_();
}

logic::ltlf_ptr ForwardSynthesis::Problem::get_formula(size_t index) const {
  if (index < closure_.nb_formulas()) {
    return closure_.get_formula(index);
  }
  index -= closure_.nb_formulas();
  if (index < partition.input_variables.size()) {
    return ast_manager->make_atom(partition.input_variables[index]);
  }
  index -= partition.input_variables.size();
  return ast_manager->make_atom(partition.output_variables[index]);
}

void ForwardSynthesis::Problem::call_gc_vtree() const {
  if (use_gc) {
    auto before_live = sdd_manager_live_size(manager);
    auto before_dead = sdd_manager_dead_size(manager);
    int result = sdd_manager_garbage_collect_if(gc_threshold, manager);
    if (result) {
      auto after_live = sdd_manager_live_size(manager);
      auto after_dead = sdd_manager_dead_size(manager);
      print_search_debug("vtree garbage collection with threshold {} completed",
                         gc_threshold);
      print_search_debug("== before garbage collection:");
      print_search_debug("  live sdd size = {}", before_live);
      print_search_debug("  dead sdd size = {}", before_dead);
      print_search_debug("== after garbage collection:");
      print_search_debug("  live sdd size = {}", after_live);
      print_search_debug("  dead sdd size = {}", after_dead);
    }
  }
}

void ForwardSynthesis::Problem::initialie_maps_() {
  const auto nb_variables = closure_.nb_formulas() + closure_.nb_atoms();
  controllable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());
  uncontrollable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());

  auto controllable_offset = closure_.nb_formulas();
  auto uncontrollable_offset =
      closure_.nb_formulas() + partition.input_variables.size();
  for (int i = 0; i < nb_variables; ++i) {
    if (i < controllable_offset) {
      controllable_map[i] = 0;
      uncontrollable_map[i] = 0;
    } else if (i >= controllable_offset and i < uncontrollable_offset) {
      controllable_map[i] = 1;
      uncontrollable_map[i] = 0;
    } else {
      // uncontrollable
      controllable_map[i] = 0;
      uncontrollable_map[i] = 1;
    }
  }
}

} // namespace core
} // namespace cynthia