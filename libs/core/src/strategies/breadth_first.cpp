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

#include <cynthia/eval.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/strategies/breadth_first.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/xnf.hpp>
#include <functional>
#include <numeric>
#include <queue>

namespace cynthia {
namespace core {

bool core::ForwardBfsSynthesis::is_realizable() {
  bool result = forward_synthesis_();
  return result;
}

bool ForwardBfsSynthesis::forward_synthesis_() {
  auto root_formula = context_.xnf_formula;
  bool is_root_terminal = eval(*root_formula);
  if (is_root_terminal)
    return true;

  auto root_sdd_node_raw = to_sdd(*root_formula, context_);
  auto root_sdd_node = SddNodeWrapper(root_sdd_node_raw);
  auto root_node = std::make_shared<node_t>(root_sdd_node);
  std::map<size_t, bool> found;

  context_.graph.add_node(root_node);
  root_node->f = 0;

  while (!root_node->solved() and !root_node->is_cost_infinite() and
         context_.graph.has_next()) {
    node_ptr current_node = context_.graph.next();

    auto sdd_formula_str = logic::to_string(
        *sdd_to_formula(current_node->sdd_node.get_raw(), context_));
    context_.logger.info("Expanding state {}: {}", current_node->id,
                         sdd_formula_str);

    auto children = get_successors(*current_node);
    for (auto& child : children) {
      if (!context_.graph.is_already_added(child->id)) {
        context_.graph.add_node(child);
        child->f = 0;
        child->f_new = 0;
        found[child->id] = true;
      }
    }

    context_.logger.info("successors: ");
    for (const auto& successor : children) {
      auto sdd_formula_str = logic::to_string(
          *sdd_to_formula(successor->sdd_node.get_raw(), context_));
      context_.logger.info("\t{}: {}", successor->id, sdd_formula_str);
    }

    std::vector<size_t> children_ids(children.size());
    std::transform(children.begin(), children.end(), children_ids.begin(),
                   [](const node_ptr& node) { return node->id; });
    context_.graph.add_successors(current_node->id, children_ids);

    auto z_set = std::set<size_t>();
    build_revisable(current_node->id, z_set);
    for (const auto& z_node_id : z_set) {
      found[z_node_id] = false;
    }
    auto o_set = std::set<size_t>();
    init_open(current_node->id, o_set, z_set);
    auto cmp = [](const node_ptr& left, const node_ptr& right) {
      return left->f_new >= right->f_new;
    };
    std::priority_queue<node_ptr, std::vector<node_ptr>, decltype(cmp)> heap(
        cmp);
    for (const auto& element_id : o_set) {
      heap.push(context_.graph.id_to_node[element_id]);
    }
    while (!heap.empty()) {
      auto current_o_node = heap.top();
      heap.pop();
      if (current_o_node->type == NodeType::OR) {
        auto current_o_successors =
            context_.graph.successors[current_o_node->id];
        node_ptr o_node_successor_min =
            context_.graph.id_to_node[current_o_successors[0]];
        for (const auto& o_node_successor_id : current_o_successors) {
          auto o_node_successor =
              context_.graph.id_to_node[o_node_successor_id];
          if (current_o_node->f_new == (1 + o_node_successor->f_new) and
              (o_node_successor_min->f_new > o_node_successor->f_new or
               (o_node_successor_min->f_new == o_node_successor->f_new and
                o_node_successor->solved()))) {
            o_node_successor_min = o_node_successor;
          }
        }
      }
    }
  }

  return root_node->solved();
}

node_ptr ForwardBfsSynthesis::get_state_node(const SddNodeWrapper& wrapper) {
  auto sdd_formula = sdd_to_formula(wrapper.get_raw(), context_);
  auto next_state = xnf(*strip_next(*formula));
  auto sdd_next_state = SddNodeWrapper(to_sdd(*next_state, context_));
  bool is_terminal = eval(*sdd_formula);
  return std::make_shared<node_t>(sdd_next_state, is_terminal = is_terminal);
}

std::vector<node_ptr> ForwardBfsSynthesis::get_successors(node_t& node) {
  std::vector<node_ptr> result;

  if (node.type == NodeType::OR or node.type == NodeType::AND) {
    auto child_it = node.sdd_node.begin();
    auto children_end = node.sdd_node.end();
    for (; child_it != children_end; ++child_it) {
      node_ptr child;
      if (node.type == NodeType::OR) {
        child = std::make_shared<node_t>(SddNodeWrapper(child_it.get_sub()));
      } else { // if (node.type == NodeType::AND)
        child = get_state_node(SddNodeWrapper(child_it.get_sub()));
      }

      child = context_.insert_or_get_node(child);
      result.push_back(child);
    }
    return result;
  }

  // node.type == NodeType::TERMINAL
  auto next_node = get_state_node(node.sdd_node);
  if (next_node->id != node.id) {
    node.set_node_type(NodeType::OR);
    next_node = context_.insert_or_get_node(next_node);
    return {next_node};
  }
  // same state -> return no successors
  node.set_node_type(NodeType::TERMINAL);
  node.label = next_node->solved() ? Label::SOLVED : Label::FAILED;
  return result;
}

void ForwardBfsSynthesis::build_revisable(size_t node_id,
                                          std::set<size_t>& set) {
  for (const auto& predecessor :
       context_.graph.get_marked_predecessors(node_id)) {
    if (set.find(predecessor) != set.end()) {
      set.insert(predecessor);
      build_revisable(predecessor, set);
    }
  }
}

void ForwardBfsSynthesis::init_open(size_t node_id, std::set<size_t>& o_set,
                                    const std::set<size_t>& z_set) {
  auto node = context_.graph.id_to_node[node_id];
  auto successors = context_.graph.successors[node_id];
  if (node->type == NodeType::AND and
      utils::empty_intersection(successors.begin(), successors.end(),
                                z_set.begin(), z_set.end())) {
    size_t total_cost = 0;
    bool is_solved = true;
    std::for_each(successors.begin(), successors.end(),
                  [this, &total_cost, &is_solved](size_t successor_id) {
                    auto successor = context_.graph.id_to_node[successor_id];
                    total_cost += 1 + successor->f;
                    is_solved = is_solved and successor->solved();
                  });
    node->f_new = total_cost;
    if (is_solved)
      node->label = Label::SOLVED;
    o_set.insert(node_id);
    if (node->f_new <= node->f) {
      return;
    }
  } else if (node->type == NodeType::OR and
             !utils::empty_difference(successors.begin(), successors.end(),
                                      z_set.begin(), z_set.end())) {
    std::set<size_t> difference;
    std::set_difference(successors.begin(), successors.end(), z_set.begin(),
                        z_set.end(), difference.begin());
    auto min_cost =
        std::min_element(difference.begin(), difference.end(),
                         [](const node_ptr& left, const node_ptr& right) {
                           return left->f < right->f;
                         });
    node->f_new = *min_cost;
    o_set.insert(node->id);
    if (node->f_new <= node->f) {
      return;
    }
  } else {
    node->f_new = infty();
  }
  if (node->f_new > node->f) {
    for (const auto& predecessor :
         context_.graph.get_marked_predecessors(node->id)) {
      if (o_set.find(predecessor) != o_set.end()) {
        init_open(node_id, o_set, z_set);
      }
    }
  }
}
} // namespace core
} // namespace cynthia
