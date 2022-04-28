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

class Graph {
private:
  std::map<size_t, std::map<size_t, size_t>> transitions;
  std::map<size_t, std::map<size_t, size_t>> backward_transitions;
  void insert_with_default_(std::map<size_t, std::map<size_t, size_t>>& m,
                            size_t start, size_t action, size_t end);
  std::map<size_t, size_t>
  get_or_empty_(const std::map<size_t, std::map<size_t, size_t>>& m,
                size_t key) const;

public:
  void add_transition(size_t start, size_t action, size_t end);
  std::map<size_t, size_t> get_successors(size_t start) const;
  std::map<size_t, size_t> get_predecessors(size_t end) const;
};

} // namespace core
} // namespace cynthia
