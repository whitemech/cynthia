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

#include <climits>
#include <memory>

namespace search {
namespace cynthia {
enum Result {
  // problem has been neither solved nor proven unsolvable yet
  UNDECIDED,

  // existence of a winning strategy for the protagonist
  PROVEN,

  // non-existence of a winning strategy for the protagonist
  DISPROVEN,

  EXPANDED_ALL,

  // indicating time-out
  TIMEOUT,

  // indicating out of memory
  OUT_OF_MEMORY
};

class AbstractSearch {
protected:
  unsigned long starttime;
  unsigned long endtime;
  unsigned long timeout = AbstractSearch.NO_TIMEOUT;
  std::shared_ptr<AbstractNode> initialNode;
  std::shared_ptr<Problem> problem;

public:
  static const long NO_TIMEOUT = ULLONG_MAX;
  std::shared_ptr<Policy> get_policy() = 0;
};

} // namespace cynthia
} // namespace search
