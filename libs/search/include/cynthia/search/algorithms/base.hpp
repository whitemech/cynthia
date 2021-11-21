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

#include <climits>
#include <cynthia/search/context.hpp>
#include <cynthia/search/node.hpp>
#include <memory>

namespace cynthia {
namespace search {

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

enum SearchFlag {
  GOAL,
  DEAD_END,
  NO_POLICY,
  VISITED,
};

class AbstractSearch {
  static const unsigned long NO_TIMEOUT = ULLONG_MAX;

protected:
  unsigned long starttime;
  unsigned long endtime;
  unsigned long timeout = NO_TIMEOUT;
  std::shared_ptr<AbstractNode> initial_node;
  std::shared_ptr<Problem> problem;

public:
  explicit AbstractSearch(std::shared_ptr<Problem> problem) : problem{problem} {
    RECURSION_COUNTER = 0;
    NODE_EXPANSIONS = 0;
    NODES = 0;
  }
  //  std::shared_ptr<Policy> get_policy() = 0;

  virtual Result run() = 0;
  virtual void do_iteration() = 0;

  /**
   * Counter for recursion.
   */
  static int RECURSION_COUNTER;

  /**
   * Counter for node expansions.
   */
  static int NODE_EXPANSIONS;

  /**
   * Counter for nodes.
   */
  static int NODES;
};

} // namespace search
} // namespace cynthia
