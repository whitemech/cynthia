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

#include <algorithm>
#include <cynthia/vtree.hpp>
#include <fstream>
#include <sstream>
#include <stack>

namespace cynthia {
namespace core {

void VTreeBuilder::check_partition_superset_of_atoms() const {
  std::vector<logic::atom_ptr>::const_iterator it;
  for (it = closure_.begin_atoms(); it != closure_.end_atoms(); ++it) {
    logic::atom_ptr atom = *it;
    bool is_atom_in_partition = partition_.is_var(atom->name);
    assert(is_atom_in_partition);
  }
}

VTreeBuilder::VTreeBuilder(const Closure& closure,
                           const InputOutputPartition& partition)
    : closure_{closure}, partition_{partition} {
  check_partition_superset_of_atoms();
}

Vtree* VTreeBuilder::get_vtree() {
  if (executed) {
    return result;
  }
  executed = true;
  size_t offset = 1;
  // build the state root
  auto state_leaves = build_leaves(closure_.nb_formulas(), offset);
  auto state_root = build_binary_tree_from_list(state_leaves);

  // build the env root
  offset = offset + state_leaves.size();
  auto env_leaves = build_leaves(partition_.input_variables.size(), offset);
  auto env_root = build_binary_tree_from_list(env_leaves);

  // build the system root
  offset = offset + env_leaves.size();
  auto system_leaves = build_leaves(partition_.output_variables.size(), offset);
  auto system_root = build_binary_tree_from_list(system_leaves);

  // build the final root
  auto env_state_root = std::make_shared<VTreeNode>();
  env_state_root->left = env_root;
  env_state_root->right = state_root;
  auto root = std::make_shared<VTreeNode>();
  root->left = system_root;
  root->right = env_state_root;

  // save to a temporary file
  auto vtree_string = print_vtree(root);
  std::string filename = std::tmpnam(nullptr);
  std::ofstream temp_file;
  temp_file.open(filename);
  temp_file << vtree_string;
  temp_file.close();
  result = sdd_vtree_read(filename.c_str());
  return result;
}

vtree_node_ptr VTreeBuilder::build_binary_tree_from_list(
    const std::vector<vtree_node_ptr>& leaves) {
  if (leaves.empty()) {
    throw std::invalid_argument("vector of leaves must be non-empty");
  }
  auto queue = std::queue<vtree_node_ptr>();
  for (const auto& node : leaves)
    queue.push(node);

  while (queue.size() > 1) {
    auto left_node = queue.front();
    queue.pop();
    auto right_node = queue.front();
    queue.pop();
    auto new_node = std::make_shared<VTreeNode>();

    new_node->left = left_node;
    new_node->right = right_node;
    queue.push(new_node);
  }

  auto result = queue.front();
  queue.pop();
  return result;
}

std::vector<vtree_node_ptr> VTreeBuilder::build_leaves(size_t size,
                                                       size_t offset) {
  std::vector<vtree_node_ptr> leaves(size);
  size_t index = offset;
  std::transform(leaves.begin(), leaves.end(), leaves.begin(),
                 [&index](vtree_node_ptr& _) {
                   return std::make_shared<VTreeNode>(index++);
                 });
  return leaves;
}

std::string VTreeBuilder::print_vtree(const vtree_node_ptr& root) {
  vtree_node_ptr current_node;
  size_t current_index;
  size_t node_index = 0;
  std::queue<std::pair<vtree_node_ptr, size_t>> queue;
  std::stack<std::string> lines;
  queue.push(std::make_pair(root, 0));
  while (!queue.empty()) {
    auto pair = queue.front();
    current_node = pair.first;
    current_index = pair.second;
    queue.pop();

    if (current_node->is_leaf()) {
      lines.push("L " + std::to_string(current_index) + " " +
                 std::to_string(current_node->value));
    } else {
      auto left_index = node_index + 1;
      auto right_index = node_index + 2;
      queue.push(std::make_pair(current_node->left, left_index));
      queue.push(std::make_pair(current_node->right, right_index));
      lines.push("I " + std::to_string(current_index) + " " +
                 std::to_string(left_index) + " " +
                 std::to_string(right_index));
      node_index += 2;
    }
  }
  std::string result = "vtree " + std::to_string(node_index + 1) + "\n";
  while (!lines.empty()) {
    auto current_line = lines.top();
    lines.pop();
    result += current_line + "\n";
  }
  return result;
}

} // namespace core
} // namespace cynthia
