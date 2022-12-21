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

#include <cstddef>
#include <set>
#include <stack>

namespace cynthia {
namespace core {

class Path {
private:
  std::stack<size_t> path;
  std::set<size_t> node_set;

public:
  void push(size_t node);
  size_t pop();
  size_t back();
  bool contains(size_t);
};

} // namespace core
} // namespace cynthia
