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

#include <cynthia/search/algorithms/base.hpp>
#include <cynthia/search/heuristics/base.hpp>
#include <cynthia/search/node.hpp>
#include <map>

namespace cynthia {
namespace search {

class SearchNode;

class HeuristicSearch : public AbstractSearch,
                        public std::enable_shared_from_this<HeuristicSearch> {
protected:
  std::shared_ptr<Heuristic> heuristic;
  std::map<size_t, std::shared_ptr<SearchNode>> state_to_node;

public:
  HeuristicSearch(std::shared_ptr<Problem> problem,
                  std::shared_ptr<Heuristic> heuristic)
      : heuristic{heuristic}, AbstractSearch(problem) {}

  std::shared_ptr<Heuristic> get_heuristic() { return heuristic; }
};

} // namespace search
} // namespace cynthia
