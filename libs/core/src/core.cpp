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

Context::Context(const logic::ltlf_ptr& formula,
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

std::map<std::string, size_t>
Context::compute_prop_to_id_map(const Closure& closure,
                                const InputOutputPartition& partition) {
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

logic::ltlf_ptr Context::get_formula(size_t index) const {
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

Context::~Context() {
  if (vtree_) {
    sdd_vtree_free(vtree_);
  }
  if (manager) {
    sdd_manager_free(manager);
  }
}

} // namespace core
} // namespace cynthia
