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
#include <cynthia/state.hpp>

extern "C" {
#include "sddapi.h"
}
namespace cynthia {
namespace core {

class SearchNode {
public:
  ForwardSynthesis::Context* context_{};
  State* state_{};

  size_t depth_;
  size_t branching_factor_ = 0;
  const SearchNode* parent_;

  SearchNode(State* state, const SearchNode* parent, size_t depth);

  void set_heuristic(size_t heuristic);

  inline SddSize get_index() { return index_; }
  inline bool is_init() { return is_init_node_; }
  inline bool is_goal() { return is_goal_node_; }
  inline bool is_deadend() { return is_deadend_; }
  inline size_t get_heuristic() { return heuristic_value_; }

private:
  SddSize index_;
  size_t heuristic_value_ = 0;
  bool is_init_node_ = false;
  bool is_goal_node_ = false;
  bool is_deadend_ = false;
};


} // namespace core
} // namespace cynthia
