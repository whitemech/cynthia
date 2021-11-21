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

#include <cynthia/search/algorithms/heuristic.hpp>
#include <cynthia/search/operator.hpp>
#include <cynthia/search/search_node.hpp>
#include <queue>

namespace cynthia {
namespace search {

class DfsSearch : public HeuristicSearch {
public:
  DfsSearch(std::shared_ptr<Problem> problem,
            std::shared_ptr<Heuristic> heuristic)
      : HeuristicSearch(problem, heuristic) {}

  Result run() {

    auto initial_search_node =
        lookup_and_insert_node(problem->get_initial_state(), 0);
    assert(
        std::static_pointer_cast<SearchNode>(this->initial_node)->get_depth() ==
        0);
    initial_node = initial_search_node;

    utils::deref_set<std::shared_ptr<SearchNode>> closed_solved;

    auto resultSearch = do_search_(initial_search_node, closed_solved);
  }

  std::shared_ptr<SearchNode>
  lookup_and_insert_node(std::shared_ptr<State> state, int depth) {
    auto node_ptr = state_to_node.find(state->get_unique_id());
    if (node_ptr == state_to_node.end()) {
      auto node =
          std::make_shared<SearchNode>(state, this->shared_from_this(), depth);
      state_to_node[state->get_unique_id()] = node;
      return node;
    }
    return node_ptr->second;
  }

protected:
  utils::deref_set<std::shared_ptr<SearchNode>> closed_dead_ends_nodes;
  utils::deref_set<std::shared_ptr<SearchNode>> closed_visited_nodes;
  utils::deref_set<std::shared_ptr<SearchNode>> closed_solved_nodes;

  std::pair<SearchFlag, utils::deref_set<std::shared_ptr<SearchNode>>>
  do_search_(std::shared_ptr<SearchNode> node,
             utils::deref_set<std::shared_ptr<SearchNode>>& closed_solved) {

    RECURSION_COUNTER++;

    if (node->is_goal_node() ||
        closed_solved.find(node) != closed_solved.end()) {
      closed_solved.insert(closed_visited_nodes.begin(),
                           closed_visited_nodes.end());
      return std::make_pair(GOAL, closed_solved);
    } else if (node->is_dead_end_node() || closed_dead_ends_nodes.find(node) !=
                                               closed_dead_ends_nodes.end()) {
      return std::make_pair(DEAD_END, closed_solved);
    } else if (closed_visited_nodes.find(node) != closed_visited_nodes.end()) {
      return std::make_pair(VISITED, closed_solved);
    }
    closed_visited_nodes.insert(node);

    auto connectors = this->getNodeConnectors(node);

    NODE_EXPANSIONS++;

    //    while (!connectors.isEmpty()) {
    // TODO
  }

  utils::deref_set<std::shared_ptr<SearchConnector>>
  getNodeConnectors(std::shared_ptr<SearchNode> node) {
    utils::deref_set<std::shared_ptr<SearchConnector>> connectors;
    utils::deref_set<std::shared_ptr<Operator>> applicable_ops =
        node->state->get_applicable_ops();
    for (std::shared_ptr<Operator> op : applicable_ops) {
      utils::deref_set<std::shared_ptr<State>> successor_states =
          node->state->apply(*op);
      //      assert !successorStates.isEmpty();
      utils::deref_set<std::shared_ptr<SearchNode>> children;
      for (const std::shared_ptr<State>& successor : successor_states) {
        std::shared_ptr<SearchNode> new_node = this->lookup_and_insert_node(
            successor, (int)(node->get_depth() + op->cost));
        new_node->parent = node;
        children.insert(new_node);
      }
      std::shared_ptr<SearchConnector> connector =
          std::make_shared<SearchConnector>(node, children, op);
      connectors.insert(connector);
    }
    return connectors;
  }
};

} // namespace search
} // namespace cynthia
