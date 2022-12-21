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

#include <cynthia/search/context.hpp>
#include <cynthia/search/operator.hpp>
#include <cynthia/utils.hpp>

namespace cynthia {
namespace search {

class State {
protected:
  std::shared_ptr<Problem> problem{};
  size_t unique_id{};

public:
  explicit State(std::shared_ptr<Problem> problem) : problem{problem} {}

  inline size_t get_unique_id() const { return unique_id; }
  bool is_goal_state() const { return problem->is_goal_state(*this); }
  virtual utils::deref_set<std::shared_ptr<State>>
  apply(const Operator& operator_) = 0;
  virtual utils::deref_set<std::shared_ptr<Operator>> get_applicable_ops() = 0;
};

} // namespace search
} // namespace cynthia
