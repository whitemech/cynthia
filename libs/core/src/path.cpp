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

#include <cassert>
#include <cynthia/path.hpp>

namespace cynthia {
namespace core {

void Path::push(size_t node_id) {
  assert(!contains(node_id));
  path.push(node_id);
  node_set.insert(node_id);
}
size_t Path::pop() {
  assert(!path.empty());
  auto node_id = path.top();
  path.pop();
  node_set.erase(node_id);
  return node_id;
}
size_t Path::back() { return path.top(); }
bool Path::contains(size_t node_id) {
  return node_set.find(node_id) != node_set.end();
}

} // namespace core
} // namespace cynthia
