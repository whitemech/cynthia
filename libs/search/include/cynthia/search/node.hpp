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

#include <cynthia/comparable.hpp>
#include <cynthia/hashable.hpp>
#include <cynthia/search/context.hpp>
#include <cynthia/search/state.hpp>

namespace cynthia {
namespace search {

class AbstractNode : public utils::Hashable, public utils::SimpleComparable {
public:
  std::shared_ptr<State> state{};
  explicit AbstractNode(std::shared_ptr<State> state) : state{state} {}
  bool is_goal_node() const { return state->is_goal_state(); }
  inline size_t state_id() const { return state->get_unique_id(); }
};

} // namespace search
} // namespace cynthia
