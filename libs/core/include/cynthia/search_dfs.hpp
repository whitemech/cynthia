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
#include <cynthia/heuristic.hpp>
#include <cynthia/problem.hpp>
#include <cynthia/search_connector.hpp>
#include <cynthia/search_node.hpp>

extern "C" {
#include "sddapi.h"
}
namespace cynthia {
namespace core {

class SearchDFS {
public:
  SearchDFS(Problem* problem, Heuristic* heuristic);
  bool forward_search();

  strategy_t do_search_(SearchNode* node, std::set<SddSize>& path);

  inline SearchNode* init_node() { return init_node_; }
  std::vector<SearchConnector*> expand_(SearchNode* node);

private:
  Problem* problem_{};
  Heuristic* heuristic_{};
  ForwardSynthesis::Context* context_{};
  SearchNode* init_node_{};
  std::map<SddSize, bool> discovered_;
};

} // namespace core
} // namespace cynthia
