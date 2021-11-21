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

#include <cfloat>
#include <cynthia/search/algorithms/heuristic.hpp>
#include <cynthia/search/heuristics/base.hpp>
#include <cynthia/search/node.hpp>
#include <cynthia/search/search_connector.hpp>
#include <cynthia/utils.hpp>

namespace cynthia {
namespace search {

class SearchNode : public AbstractNode {
private:
  int depth;
  int branching_factor = 0;

  bool is_goal_node_;
  bool is_dead_node_;

protected:
  /**
   * Incoming connectors.
   */
  utils::deref_set<std::shared_ptr<SearchConnector>> incoming_connectors;

  /**
   * Outgoing connectors.
   */
  utils::deref_set<std::shared_ptr<SearchConnector>> outgoing_connectors;

  std::shared_ptr<SearchConnector> marked_connector = nullptr;

  double heuristic;

public:
  std::shared_ptr<SearchNode> parent{};
  explicit SearchNode(std::shared_ptr<State> state,
                      std::shared_ptr<HeuristicSearch> search_mgr, int depth)
      : AbstractNode(state) {
    this->depth = depth;
    if (search_mgr->get_heuristic() != nullptr)
      this->heuristic = search_mgr->get_heuristic()->get_heuristic(*state);

    if (this->state->is_goal_state()) {
      this->is_goal_node_ = true;
    } else if (this->is_dead_end_node())
      this->is_dead_node_ = true;
  }

  bool is_dead_end_node() const { return heuristic == DBL_MAX; }
  int get_depth() const { return this->depth; }

  hash_t compute_hash_() const override {
    // TODO
    throw std::logic_error("not implemented");
  }

  bool is_equal(const utils::SimpleComparable& o) const override {
    // TODO
    throw std::logic_error("not implemented");
  }

  int compare_(const utils::SimpleComparable& o) const override {
    // TODO
    throw std::logic_error("not implemented");
  }
};

} // namespace search
} // namespace cynthia
