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

#include <map>

namespace cynthia {
namespace core {

enum NodeType { AND = 0, OR = 1 };

struct Node {
  size_t id;
  NodeType type;

  bool operator<(const Node& node) const { return id < node.id; }
};

class Graph {

private:
  std::map<Node, std::map<size_t, Node>> transitions;
  std::map<Node, std::map<size_t, Node>> backward_transitions;
  static void insert_with_default_(std::map<Node, std::map<size_t, Node>>& m,
                                   Node start, size_t action, Node end);
  static std::map<size_t, Node>
  get_or_empty_(const std::map<Node, std::map<size_t, Node>>& m, Node key);

public:
  void add_transition(Node start, size_t action, Node end);
  std::map<size_t, Node> get_successors(Node start) const;
  std::map<size_t, Node> get_predecessors(Node end) const;
};

} // namespace core
} // namespace cynthia
