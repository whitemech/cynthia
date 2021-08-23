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
#include <queue>

namespace cynthia {
namespace core {

constexpr inline size_t infty() { return std::numeric_limits<size_t>::max(); }

enum Label { SOLVED = 1, UNKNOWN = 0, FAILED = -1 };
enum NodeType {
  UNDEFINED = 0,
  TERMINAL = 1,
  AND = 2,
  OR = 3,
};

enum SddNodeType {
  STATE = 0,
  ENV_STATE = 1,
  SYSTEM_STATE = 2,
  SYSTEM_ENV_STATE = 3,
};

class node_t;
typedef std::shared_ptr<node_t> node_ptr;

struct node_t {
public:
  size_t id;
  SddNodeWrapper sdd_node;
  SddNodeType sdd_node_type;
  NodeType type;
  Label label;
  unsigned long f;
  unsigned long f_new;
  unsigned long f_old;

  explicit node_t(const SddNodeWrapper& sdd_node, bool is_terminal = false) {
    this->sdd_node = sdd_node;
    id = sdd_node.get_id();
    sdd_node_type = get_sdd_node_type_(sdd_node);
    type = get_node_type_(sdd_node_type);
    label = is_terminal ? Label::SOLVED : Label::UNKNOWN;
    f = 0;
  }

  inline bool solved() const { return label == Label::SOLVED; }
  inline bool failed() const { return label == Label::FAILED; }
  inline bool is_cost_infinite() const {
    return f == std::numeric_limits<size_t>::max();
  }
  inline void set_node_type(NodeType new_node_type) {
    if (type != NodeType::UNDEFINED and type != new_node_type) {
      throw std::logic_error("type already set");
    }
    type = new_node_type;
  }

private:
  static inline NodeType get_node_type_(SddNodeType sdd_node_type) {
    if (sdd_node_type == SYSTEM_ENV_STATE or sdd_node_type == SYSTEM_STATE) {
      return NodeType::OR;
    }
    if (sdd_node_type == ENV_STATE) {
      return NodeType::AND;
    }
    return NodeType::UNDEFINED;
  }

  static inline SddNodeType get_sdd_node_type_(const SddNodeWrapper& wrapper) {
    if (!wrapper.is_decision()) {
      return STATE;
    } else if (!wrapper.at_vtree_root()) {
      if (!wrapper.parent_at_vtree_root()) {
        return STATE;
      }
      return ENV_STATE;
    } else {
      if (!SddNodeWrapper((wrapper.begin()).get_sub()).parent_at_vtree_root()) {
        return SYSTEM_STATE;
      }
      return SYSTEM_ENV_STATE;
    }
  }
};

struct node_comparator {
  size_t operator()(node_ptr const& a, node_ptr const& b) const {
    return a->id < b->id;
  }
};

struct graph_t {
  std::map<size_t, node_ptr> id_to_node;
  std::map<size_t, std::vector<size_t>> successors;
  std::map<size_t, std::vector<size_t>> marked_successors;
  std::map<size_t, std::set<size_t>> marked_predecessors;
  std::queue<node_ptr> tips;
  std::set<size_t> tip_set;

  inline void add_node(const node_ptr& node) {
    id_to_node[node->id] = node;
    tips.push(node);
    tip_set.insert(node->id);
  }
  inline void add_successors(size_t node_id,
                             std::vector<size_t> node_successors) {
    std::sort(node_successors.begin(), node_successors.end());
    successors[node_id] = node_successors;
  }

  inline bool is_already_added(size_t node_id) const {
    return id_to_node.find(node_id) != id_to_node.end();
  }
  inline bool has_next() const { return tips.empty(); }
  inline bool is_tip(size_t node_id) const {
    return tip_set.find(node_id) != tip_set.end();
  }
  inline node_ptr next() {
    auto result = tips.front();
    tips.pop();
    tip_set.erase(result->id);
    return result;
  }
  inline std::set<size_t> get_marked_predecessors(size_t node_id) const {
    if (marked_predecessors.find(node_id) == marked_predecessors.end()) {
      return {};
    }
    return marked_predecessors.at(node_id);
  }
  inline void mark_arc(const node_ptr& left, const node_ptr& right) {
    if (left->type == NodeType::OR) {
      auto left_marked_successors = marked_successors[left->id];
      left_marked_successors.clear();
      left_marked_successors.push_back(right->id);
      // TODO
    }
  }
};

class BfsContext : public Context {
public:
  graph_t graph;
  BfsContext(const logic::ltlf_ptr& formula,
             const InputOutputPartition& partition)
      : Context(formula, partition){};
};

class ForwardBfsSynthesis : public ISynthesis {
public:
  ForwardBfsSynthesis(const logic::ltlf_ptr& formula,
                      const InputOutputPartition& partition)
      : context_{formula, partition}, ISynthesis(formula, partition){};

  bool is_realizable() override;

  bool forward_synthesis_();
  node_ptr get_state_node(const SddNodeWrapper& wrapper);
  node_ptr get_env_node(const SddNodeWrapper& wrapper);
  std::vector<node_ptr> get_successors(node_t& node);
  static SddNodeType get_node_type(const SddNodeWrapper& wrapper);

private:
  BfsContext context_;

  void build_revisable(size_t node, std::set<size_t>& z_set);
  void init_open(size_t node, std::set<size_t>& o_set,
                 const std::set<size_t>& z_set);
};

} // namespace core
} // namespace cynthia