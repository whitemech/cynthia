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

#include <cynthia/graph.hpp>
#include <stdexcept>

namespace cynthia {
namespace core {

void Graph::insert_with_default_(std::map<Node, std::map<size_t, Node>>& m,
                                 Node start, size_t action, Node end) {
  auto start_item = m.find(start);
  if (start_item == m.end()) {
    auto new_value = std::map<size_t, Node>();
    new_value[action] = end;
    m[start] = new_value;
    return;
  }
  // if already inserted, fail
  if (start_item->second.find(action) == start_item->second.end()) {
    throw std::logic_error("action already set");
  }
  start_item->second[action] = end;
}

std::map<size_t, Node>
Graph::get_or_empty_(const std::map<Node, std::map<size_t, Node>>& m,
                     Node key) {
  auto item_or_end = m.find(key);
  if (item_or_end == m.end()) {
    return {};
  }
  return item_or_end->second;
}

void Graph::add_transition(Node start, size_t action, Node end) {
  insert_with_default_(transitions, start, action, end);
  insert_with_default_(backward_transitions, end, action, start);
}

std::map<size_t, Node> Graph::get_successors(Node start) const {
  return get_or_empty_(transitions, start);
}

std::map<size_t, Node> Graph::get_predecessors(Node end) const {
  return get_or_empty_(backward_transitions, end);
}

} // namespace core
} // namespace cynthia
