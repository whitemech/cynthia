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

#include <cynthia/problem.hpp>
#include <cynthia/state.hpp>

namespace cynthia {
namespace core {

Problem::Problem(const logic::ltlf_ptr& formula,
                 const InputOutputPartition& partition, bool enable_gc) {
  synthesis_ = new ForwardSynthesis(formula, partition, enable_gc);
  context_ = &(synthesis_->context_);
  init_state_ = new State(this->get_context(), formula);
  init_state_->set_init_state();
}

} // namespace core
} // namespace cynthia