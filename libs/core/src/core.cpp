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
#include <cynthia/logic/nnf.hpp>
#include <cynthia/vtree.hpp>

namespace cynthia {
namespace core {

ISynthesis::ISynthesis(const logic::ltlf_ptr& formula,
                       const InputOutputPartition& partition)
    : formula{formula}, partition{partition} {}

bool ForwardSynthesis::is_realizable() {
  bool result = false;

  const auto& formula_nnf = logic::to_nnf(*formula);
  closure_ = closure(*formula_nnf);
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager_ = sdd_manager_new(vtree_);
  sdd_vtree_free(vtree_);
  sdd_manager_free(manager_);
  return result;
}

} // namespace core
} // namespace cynthia
