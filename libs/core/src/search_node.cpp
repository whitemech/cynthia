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

#include <cynthia/sddcpp.hpp>
#include <cynthia/search_node.hpp>

namespace cynthia {
namespace core {

SearchNode::SearchNode(State* state, const SearchNode* parent, size_t depth)
    : state_(state), parent_(parent), depth_(depth) {
  context_ = state_->get_context();
  index_ = state_->get_id();
  is_init_node_ = state->is_init_state();
  is_goal_node_ = state->is_goal_state();
  is_deadend_ = state->is_deadend();
}

void SearchNode::set_heuristic(size_t heuristic) {
  heuristic_value_ = heuristic;
}

} // namespace core
} // namespace cynthia