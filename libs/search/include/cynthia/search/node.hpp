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

#include <cynthia/search/problem.hpp>
#include <cynthia/search/state.hpp>

namespace search {
namespace cynthia {

class AbstractNode {
public:
  State* state{};
  explicit AbstractNode(State* state) : state{state} {}

private:
  inline bool is_goal_node() { return state->is_goal_state(); }
  inline size_it state_id() { return state->unique_id; }
};

} // namespace cynthia
} // namespace search
