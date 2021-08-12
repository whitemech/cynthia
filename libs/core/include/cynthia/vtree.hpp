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

#include <cynthia/closure.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/logic/types.hpp>
#include <cynthia/utils.hpp>
#include <memory>
#include <queue>
#include <set>
extern "C" {
#include "sddapi.h"
}

namespace cynthia {
namespace core {

class VTreeNode;
typedef std::shared_ptr<VTreeNode> vtree_node_ptr;

struct VTreeNode {
public:
  vtree_node_ptr left{};
  vtree_node_ptr right{};
  size_t value{};

  VTreeNode() = default;
  explicit VTreeNode(size_t value) : value{value} {}
  inline bool is_leaf() const { return left == nullptr and right == nullptr; }
};

class VTreeBuilder {
private:
  vtree_node_ptr system_root_;
  vtree_node_ptr env_root_;
  vtree_node_ptr state_root_;
  const Closure& closure_;
  const InputOutputPartition& partition_;

  bool executed = false;
  Vtree* result{};

  /**
   * Check that the set of variables declared in the partition is a superset
   * of the set of variables in the formula.
   */
  void check_partition_superset_of_atoms() const;

public:
  VTreeBuilder(const Closure& closure, const InputOutputPartition& partition);

  Vtree* get_vtree();
  static std::vector<vtree_node_ptr> build_leaves(size_t size, size_t offset);
  static vtree_node_ptr
  build_binary_tree_from_list(const std::vector<vtree_node_ptr>& leaves);
  static std::string print_vtree(const vtree_node_ptr& root);
};

} // namespace core
} // namespace cynthia
