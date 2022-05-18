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
#include <cynthia/eval.hpp>
#include <cynthia/logic/nnf.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/one_step_realizability.hpp>
#include <cynthia/one_step_unrealizability.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {

ISynthesis::ISynthesis(const logic::ltlf_ptr& formula,
                       const InputOutputPartition& partition)
    : formula{formula}, partition{partition} {}

bool ForwardSynthesis::is_realizable() {
  bool result = forward_synthesis_();
  return result;
}

bool ForwardSynthesis::forward_synthesis_() {
  auto path = Path{};

  context_.logger.info("Check zero-step realizability");
  if (eval(*context_.nnf_formula)) {
    context_.logger.info("Zero-step realizability check successful");
    return true;
  }

  context_.logger.info("Check one-step realizability");
  auto pair_rel_result =
      one_step_realizability(*context_.nnf_formula, context_);
  if (pair_rel_result.second) {
    context_.logger.info("One-step realizability check successful");
    return true;
  }
  context_.logger.info("Check one-step unrealizability");
  auto is_unrealizable =
      one_step_unrealizability(*context_.nnf_formula, context_);
  if (!is_unrealizable) {
    context_.logger.info("One-step unrealizability check successful");
    return false;
  }

  context_.logger.info("Building the root SDD node...");
  auto root_sdd_node = to_sdd(*context_.xnf_formula, context_);
  auto sdd_formula_id = sdd_id(root_sdd_node);
  context_.logger.info("Starting first system move...");
  auto strategy = system_move_(context_.xnf_formula, path);
  bool result = strategy[sdd_formula_id] != sdd_manager_false(context_.manager);
  context_.logger.info("Explored states: {}",
                       context_.statistics_.nb_visited_nodes());
  return result;
}

std::map<std::string, size_t> ForwardSynthesis::compute_prop_to_id_map(
    const Closure& closure, const InputOutputPartition& partition) {
  std::map<std::string, size_t> result;
  size_t offset = closure.nb_formulas();
  size_t i = offset;
  for (const auto& p : partition.input_variables) {
    result[p] = i++;
  }
  for (const auto& p : partition.output_variables) {
    result[p] = i++;
  }
  return result;
}

strategy_t ForwardSynthesis::system_move_(const logic::ltlf_ptr& formula,
                                          Path& path) {
  strategy_t success_strategy, failure_strategy;
  context_.indentation += 1;
  auto sdd = SddNodeWrapper(to_sdd(*formula, context_), context_.manager);
  auto sdd_formula_id = sdd.get_id();
  context_.statistics_.visit_node(sdd_formula_id);

  success_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
  failure_strategy[sdd_formula_id] = sdd_manager_false(context_.manager);
  context_.print_search_debug("State {}", sdd_formula_id);

  if (context_.discovered.find(sdd_formula_id) != context_.discovered.end()) {
    context_.indentation -= 1;
    bool is_success = context_.discovered[sdd_formula_id];
    if (is_success) {
      context_.print_search_debug("{} already discovered, success",
                                  sdd_formula_id);
      return strategy_t{
          {sdd_formula_id, context_.winning_moves[sdd_formula_id]}};
    } else {
      context_.print_search_debug("{} already discovered, failure",
                                  sdd_formula_id);
      return failure_strategy;
    }
  }

  if (path.contains(sdd_formula_id)) {
    context_.print_search_debug("Loop detected for node {}, tagging the node",
                                sdd_formula_id);
    context_.loop_tags.insert(sdd_formula_id);
    context_.indentation -= 1;
    return failure_strategy;
  }

  if (eval(*formula)) {
    context_.print_search_debug("{} accepting!", sdd_formula_id);
    context_.discovered[sdd_formula_id] = true;
    context_.winning_moves[sdd_formula_id] = sdd_manager_true(context_.manager);
    context_.indentation -= 1;
    return success_strategy;
  }

  auto one_step_realizability_result =
      one_step_realizability(*formula, context_);
  if (one_step_realizability_result.second) {
    strategy_t strategy;
    strategy[sdd_formula_id] = one_step_realizability_result.first;
    context_.discovered[sdd_formula_id] = true;
    context_.winning_moves[sdd_formula_id] =
        one_step_realizability_result.first;
    context_.indentation -= 1;
    return strategy;
  }
  auto is_unrealizable = one_step_unrealizability(*formula, context_);
  if (!is_unrealizable) {
    context_.discovered[sdd_formula_id] = false;
    context_.indentation -= 1;
    return failure_strategy;
  }

  path.push(sdd_formula_id);
  if (sdd.get_type() == SddNodeType::STATE) {
    // both system and env moves are irrelevant
    auto system_move_str =
        logic::to_string(*sdd_to_formula(sdd.get_raw(), context_));
    context_.print_search_debug("system move (unique): {}", system_move_str);
    auto new_strategy = env_move_(sdd, path);
    if (!new_strategy.empty()) {
      path.pop();
      context_.discovered[sdd_formula_id] = true;
      context_.winning_moves[sdd_formula_id] =
          sdd_manager_true(context_.manager);
      context_.indentation -= 1;
      return success_strategy;
    }
  } else if (sdd.get_type() == SddNodeType::ENV_STATE) {
    // not at the vtree root; it means that system choice is irrelevant
    // (but env has several choices)
    context_.print_search_debug("system choice is irrelevant");
    auto env_state_node = sdd;
    auto new_strategy = env_move_(env_state_node, path);
    if (!new_strategy.empty()) {
      context_.print_search_debug("Any system move is a success from state {}!",
                                  sdd_formula_id);
      path.pop();
      // all system moves are OK, since it does not have control
      context_.discovered[sdd_formula_id] = true;
      context_.winning_moves[sdd_formula_id] =
          sdd_manager_true(context_.manager);
      context_.indentation -= 1;
      new_strategy[sdd_formula_id] = sdd_manager_true(context_.manager);
      return new_strategy;
    }
  } else { // is a decision node
    auto child_it = sdd.begin();
    auto children_end = sdd.end();

    if (child_it == children_end) {
      context_.print_search_debug("No children, {} is failure", sdd_formula_id);
      path.pop();
      context_.discovered[sdd_formula_id] = false;
      context_.indentation -= 1;
      return failure_strategy;
    }

    context_.print_search_debug("Processing {} system node's children nodes",
                                sdd.nb_children());
    std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> new_children;
    new_children.reserve(sdd.nb_children());
    // process all children, looking for OR-nodes
    // do the one-step-lookahead:
    // - if it is not an OR node, add to the new_children list so to be
    // processed later;
    // - if it is an OR node:
    //    - if already discovered: if success, return, otherwise ignore and
    //    continue
    //    - if one-step-realizability succeeds, return
    //    - if one-step-unrealizability succeeds, ignore and continue
    for (; child_it != children_end; ++child_it) {
      auto system_move = SddNodeWrapper(child_it.get_prime(), context_.manager);
      auto env_state_node =
          SddNodeWrapper(child_it.get_sub(), context_.manager);
      if (env_state_node.get_type() == STATE) {
        // OR->OR transition
        auto formula_next_state = next_state_formula_(env_state_node.get_raw());
        auto next_state = formula_to_sdd_(formula_next_state);
        auto next_state_id = next_state.get_id();
        add_transition_(sdd, system_move.get_raw(), next_state);
        auto next_state_result_it =
            context_.discovered.find(next_state.get_id());
        if (next_state_result_it != context_.discovered.end()) {
          auto next_state_is_success = next_state_result_it->second;
          if (next_state_is_success) {
            context_.print_search_debug(
                "system look-ahead: next state {} already discovered, success",
                next_state.get_id());
            path.pop();
            context_.indentation -= 1;
            strategy_t strategy;
            strategy[sdd_formula_id] = system_move.get_raw();
            return strategy;
          } else {
            context_.print_search_debug(
                "system look-ahead: next state {} already "
                "discovered, failure, ignoring",
                next_state.get_id());
            continue;
          }
        }
        auto one_step_realizability_result =
            one_step_realizability(*formula_next_state, context_);
        if (one_step_realizability_result.second) {
          context_.print_search_debug("system look-ahead: one-step "
                                      "realizability check was successful");
          strategy_t strategy;
          strategy[sdd_formula_id] = system_move.get_raw();
          strategy[next_state_id] = one_step_realizability_result.first;
          context_.discovered[next_state_id] = true;
          context_.discovered[sdd_formula_id] = true;
          context_.winning_moves[sdd_formula_id] = system_move.get_raw();
          context_.winning_moves[next_state_id] =
              one_step_realizability_result.first;
          context_.indentation -= 1;
          return strategy;
        }
        auto is_unrealizable =
            one_step_unrealizability(*formula_next_state, context_);
        if (!is_unrealizable) {
          context_.print_search_debug("system look-ahead: one-step "
                                      "unrealizability check was successful");
          context_.discovered[next_state_id] = false;
          continue;
        }
        context_.print_search_debug(
            "system look-ahead: next state {} not discovered yet ",
            next_state.get_id());
        new_children.emplace_back(system_move, env_state_node);
      } else {
        // one-step lookahead check inconclusive, need to take env action.
        // OR-AND transition.
        assert(env_state_node.get_type() == ENV_STATE);
        context_.print_search_debug("system look-ahead: {} is not a state node",
                                    env_state_node.get_id());
        new_children.emplace_back(system_move, env_state_node);
      }
    }

    // process the new_children list of AND nodes, populated in the previous
    // loop.
    for (const auto& pair : new_children) {
      auto system_move = pair.first;
      auto env_state_node = pair.second;
      auto system_move_str =
          logic::to_string(*sdd_to_formula(system_move.get_raw(), context_));
      context_.print_search_debug("checking system move: {}", system_move_str);
      ++child_it;
      if (system_move.is_false())
        continue;
      auto new_strategy = env_move_(env_state_node, path);
      if (!new_strategy.empty()) {
        context_.print_search_debug(
            "System move {} from state {} is successful", sdd_formula_id,
            system_move_str);
        path.pop();
        new_strategy[sdd_formula_id] = system_move.get_raw();
        context_.discovered[sdd_formula_id] = true;
        context_.winning_moves[sdd_formula_id] = system_move.get_raw();
        if (context_.loop_tags.find(sdd_formula_id) !=
            context_.loop_tags.end()) {
          context_.print_search_debug("trigger backward search to update "
                                      "success tag of predecessors of {}",
                                      sdd_formula_id);
          backprop_success(sdd, new_strategy);
        }
        context_.indentation -= 1;
        return new_strategy;
      }
    }
  }

  context_.print_search_debug("State {} is failure", sdd_formula_id);
  path.pop();
  context_.discovered[sdd_formula_id] = false;
  context_.indentation -= 1;
  return failure_strategy;
}

strategy_t ForwardSynthesis::env_move_(SddNodeWrapper& wrapper, Path& path) {
  context_.indentation += 1;
  if (wrapper.get_type() == SddNodeType::STATE) {
    // env move is irrelevant
    auto formula_next_state = next_state_formula_(wrapper.get_raw());
    auto sdd_next_state = formula_to_sdd_(formula_next_state);
    auto sdd_next_state_id = sdd_next_state.get_id();
    // add OR->? transition
    add_transition_(wrapper, sdd_manager_true(context_.manager),
                    sdd_next_state);
    context_.print_search_debug("env move forced to next state {}",
                                sdd_next_state_id);
    auto strategy = system_move_(formula_next_state, path);
    context_.indentation -= 1;
    if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager))
      return strategy_t{};
    return strategy;
  } else {
    // env move is relevant, checking all moves and successors
    assert(wrapper.get_type() == ENV_STATE);
    auto child_it = wrapper.begin();
    auto children_end = wrapper.end();
    context_.print_search_debug("Processing {} env node's children nodes",
                                wrapper.nb_children());
    std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> new_children;
    new_children.reserve(wrapper.nb_children());
    // process all children, looking for OR-successors
    // do the one-step-lookahead:
    //  - if already discovered: if success, return, otherwise must be visited
    //  later
    //  - if one-step-realizability succeeds, ignore and continue
    //  - if one-step-unrealizability succeeds, return failure
    for (; child_it != children_end; ++child_it) {
      bool ignore = false;
      auto env_node = SddNodeWrapper(child_it.get_prime(), context_.manager);
      auto state_node = SddNodeWrapper(child_it.get_sub(), context_.manager);
      assert(state_node.get_type() == STATE);
      auto formula_next_state = next_state_formula_(state_node.get_raw());
      auto sdd_next_state = formula_to_sdd_(formula_next_state);
      // add AND->? transition
      add_transition_(wrapper, env_node.get_raw(), sdd_next_state);
      auto next_state_id = sdd_next_state.get_id();
      auto next_state_result_it = context_.discovered.find(next_state_id);
      if (next_state_result_it != context_.discovered.end()) {
        auto next_state_is_success = next_state_result_it->second;
        if (next_state_is_success) {
          context_.print_search_debug("env look-ahead: next state {} already "
                                      "discovered, success, ignoring",
                                      next_state_id);
          ignore = true;
        }
        if (!next_state_is_success) {
          context_.print_search_debug(
              "env look-ahead: next state {} already discovered, failure",
              next_state_id);
          context_.indentation -= 1;
          return strategy_t{};
        }
      }
      auto is_unrealizable =
          one_step_unrealizability(*formula_next_state, context_);
      if (!is_unrealizable) {
        context_.print_search_debug("env look-ahead: one-step "
                                    "unrealizability check was successful");
        context_.discovered[next_state_id] = false;
        context_.indentation -= 1;
        return strategy_t{};
      }
      auto one_step_realizability_result =
          one_step_realizability(*formula_next_state, context_);
      if (one_step_realizability_result.second) {
        context_.print_search_debug("env look-ahead: one-step "
                                    "realizability check was successful");
        context_.discovered[next_state_id] = true;
        context_.winning_moves[next_state_id] =
            one_step_realizability_result.first;
        continue;
      }
      if (!ignore) {
        // we don't know, need to take env action
        context_.print_search_debug(
            "env look-ahead: next state {} not discovered yet",
            state_node.get_id());
        new_children.emplace_back(env_node, state_node);
      }
    }

    if (new_children.empty()) {
      // take any successor, it will be a win
      context_.print_search_debug(
          "env look-ahead: taking any env action, system wins");
      auto formula_next_state = next_state_formula_(wrapper.begin().get_sub());
      context_.indentation -= 1;
      return system_move_(formula_next_state, path);
    }
    strategy_t final_strategy;

    // process the new_children list, populated in the previous loop.
    for (const auto& pair : new_children) {
      auto env_move = pair.first;
      auto state_node = pair.second;
      auto env_action = sdd_to_formula(env_move.get_raw(), context_);
      auto env_action_str = logic::to_string(*env_action);
      auto formula_next_state = next_state_formula_(state_node.get_raw());
      auto sdd_next_state = formula_to_sdd_(formula_next_state);
      auto sdd_next_state_id = sdd_next_state.get_id();
      context_.print_search_debug("env move: {}", env_action_str);
      auto strategy = system_move_(formula_next_state, path);
      if (strategy[sdd_next_state_id] == sdd_manager_false(context_.manager)) {
        context_.indentation -= 1;
        return strategy_t{};
      }
      final_strategy.insert(strategy.begin(), strategy.end());
    }
    context_.indentation -= 1;
    return final_strategy;
  }
}

logic::ltlf_ptr ForwardSynthesis::next_state_formula_(SddNode* sdd_ptr) {
  auto sdd_formula = sdd_to_formula(sdd_ptr, context_);
  auto next_state_formula = xnf(*strip_next(*sdd_formula));
  return next_state_formula;
}
SddNodeWrapper
ForwardSynthesis::formula_to_sdd_(const logic::ltlf_ptr& formula) {
  auto wrapper = SddNodeWrapper(to_sdd(*formula, context_), context_.manager);
  return wrapper;
}
SddNodeWrapper ForwardSynthesis::next_state_(const SddNodeWrapper& wrapper) {
  auto next_state_formula = next_state_formula_(wrapper.get_raw());
  auto sdd_next_state = formula_to_sdd_(next_state_formula);
  return sdd_next_state;
}

void ForwardSynthesis::backprop_success(SddNodeWrapper& node,
                                        strategy_t& strategy) {
  auto start_node = Node{node.get_id(), node_type_from_sdd_type_(node)};
  std::queue<Node> queue;
  queue.push(start_node);
  Node current_node{};
  while (!queue.empty()) {
    current_node = queue.front();
    auto all_predecessors = context_.graph.get_predecessors(current_node);
    for (const auto& predecessors_by_action : all_predecessors) {
      auto action = predecessors_by_action.first;
      auto predecessors = predecessors_by_action.second;
      for (const auto& predecessor : predecessors) {
        if (predecessor.type == NodeType::OR) {
          context_.discovered[predecessor.id] = true;
          strategy[predecessor.id] = context_.graph.get_action_by_id(action);
          queue.push(predecessor);
        } else { // if (predecessor.type == NodeType::AND) {
          // TODO check: not all children of the AND node might have been added
          // to the subgraph.
          auto children_by_action = context_.graph.get_successors(predecessor);
          if (std::all_of(
                  children_by_action.begin(), children_by_action.end(),
                  [this](const std::pair<size_t, Node>& action_and_child) {
                    auto is_discovered =
                        context_.discovered.find(action_and_child.second.id);
                    return is_discovered == context_.discovered.end() and
                           is_discovered->second;
                  })) {
            context_.discovered[predecessor.id] = true;
            queue.push(predecessor);
          }
        }
      }
    }
    queue.pop();
  }
}

ForwardSynthesis::Context::Context(const logic::ltlf_ptr& formula,
                                   const InputOutputPartition& partition,
                                   bool use_gc, float gc_threshold)
    : logger{"cynthia"}, formula{formula}, partition{partition}, use_gc{use_gc},
      gc_threshold{gc_threshold}, ast_manager{&formula->ctx()} {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  Closure closure_object = closure(*xnf_formula);
  closure_ = closure_object;
  auto builder = VTreeBuilder(closure_, partition);
  vtree_ = builder.get_vtree();
  manager = sdd_manager_new(vtree_);
  prop_to_id = compute_prop_to_id_map(closure_, partition);
  statistics_ = Statistics();
  initialie_maps_();
}

logic::ltlf_ptr ForwardSynthesis::Context::get_formula(size_t index) const {
  if (index < closure_.nb_formulas()) {
    return closure_.get_formula(index);
  }
  index -= closure_.nb_formulas();
  if (index < partition.input_variables.size()) {
    return ast_manager->make_atom(partition.input_variables[index]);
  }
  index -= partition.input_variables.size();
  return ast_manager->make_atom(partition.output_variables[index]);
}

void ForwardSynthesis::Context::call_gc_vtree() const {
  if (use_gc) {
    auto before_live = sdd_manager_live_size(manager);
    auto before_dead = sdd_manager_dead_size(manager);
    int result = sdd_manager_garbage_collect_if(gc_threshold, manager);
    if (result) {
      auto after_live = sdd_manager_live_size(manager);
      auto after_dead = sdd_manager_dead_size(manager);
      print_search_debug("vtree garbage collection with threshold {} completed",
                         gc_threshold);
      print_search_debug("== before garbage collection:");
      print_search_debug("  live sdd size = {}", before_live);
      print_search_debug("  dead sdd size = {}", before_dead);
      print_search_debug("== after garbage collection:");
      print_search_debug("  live sdd size = {}", after_live);
      print_search_debug("  dead sdd size = {}", after_dead);
    }
  }
}

void ForwardSynthesis::Context::initialie_maps_() {
  const auto nb_variables = closure_.nb_formulas() + closure_.nb_atoms();
  controllable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());
  uncontrollable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());

  auto controllable_offset = closure_.nb_formulas();
  auto uncontrollable_offset =
      closure_.nb_formulas() + partition.input_variables.size();
  for (int i = 0; i < nb_variables; ++i) {
    if (i < controllable_offset) {
      controllable_map[i] = 0;
      uncontrollable_map[i] = 0;
    } else if (i >= controllable_offset and i < uncontrollable_offset) {
      controllable_map[i] = 1;
      uncontrollable_map[i] = 0;
    } else {
      // uncontrollable
      controllable_map[i] = 0;
      uncontrollable_map[i] = 1;
    }
  }
}

NodeType
ForwardSynthesis::node_type_from_sdd_type_(const SddNodeWrapper& wrapper) {
  auto sdd_type = wrapper.get_type();
  //  UNDEFINED = 0,
  //  STATE = 1,
  //  ENV = 2,
  //  ENV_STATE = 3,
  //  SYSTEM = 4,
  //  SYSTEM_STATE = 5,
  //  SYSTEM_ENV = 6,
  //  SYSTEM_ENV_STATE = 7,
  assert(sdd_type != UNDEFINED);
  assert(sdd_type != ENV);
  assert(sdd_type != SYSTEM);
  assert(sdd_type != SYSTEM_ENV);
  if (sdd_type == ENV_STATE) {
    return AND;
  }
  // sdd_type == SYSTEM_STATE || sdd_type == SYSTEM_ENV_STATE || sdd_type ==
  // STATE
  return OR;
}

void ForwardSynthesis::add_transition_(const SddNodeWrapper& start,
                                       SddNode* move_node,
                                       const SddNodeWrapper& end) {
  auto start_state_type = node_type_from_sdd_type_(start);
  auto end_state_type = node_type_from_sdd_type_(end);

  auto start_node = Node{start.get_id(), start_state_type};
  auto end_node = Node{end.get_id(), end_state_type};

  context_.print_search_debug(
      "Adding transition ({}, {}, {})", start_node.to_string(),
      std::to_string(sdd_id(move_node)), end_node.to_string());
  context_.graph.add_transition(start_node, move_node, end_node);
}

} // namespace core
} // namespace cynthia
