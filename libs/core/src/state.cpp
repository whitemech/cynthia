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
#include <cynthia/state.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/xnf.hpp>
#include <cynthia/eval.hpp>
#include <cynthia/logic/nnf.hpp>
#include <cynthia/logic/print.hpp>

namespace cynthia {
namespace core {

State::State(ForwardSynthesis::Context* context, const logic::ltlf_ptr formula)
    : context_(context), formula(formula) {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  is_goal_state_ = eval(*formula);
}

void State::instantiate() {
  sdd = context_->formula_to_sdd_(nnf_formula);
  id = sdd.get_id();
  instantiated_ = true;
}

bool State::is_deadend(){
  return ops_.size()>0? false : true;
}

std::vector<SddNodeWrapper> State::compute_ops(){
  if (sdd.get_type() == SddNodeType::STATE) {
    auto system_move_f = sdd_to_formula(sdd.get_raw(), *context_);
    auto system_move_str =
        logic::to_string(*system_move_f);
    context_->print_search_debug("system move (unique): {}", system_move_str);
    auto op_id = sdd.get_id();
    op_to_effects_[op_id] = sdd;
    op_to_action_[op_id] = sdd;
    ops_.emplace_back(sdd);
  } else if (sdd.get_type() == SddNodeType::ENV_STATE) {
    // not at the vtree root; it means that system choice is irrelevant
    context_->print_search_debug("system choice is irrelevant");
    auto op_id = sdd.get_id();
    op_to_effects_[op_id] = sdd;
    op_to_action_[op_id] = sdd;
    ops_.emplace_back(sdd);
  } else {
    // is a decision node
    auto child_it = sdd.begin();
    auto children_end = sdd.end();
    if (child_it == children_end) {
      context_->print_search_debug("No children, {} is failure", id);
    }

    context_->print_search_debug("Processing {} system node's children nodes",
                                 sdd.nb_children());
    ops_.reserve(sdd.nb_children());
    for (; child_it != children_end; ++child_it) {
      auto system_move = SddNodeWrapper(child_it.get_prime(), context_->manager);
      auto env_state_node =
          SddNodeWrapper(child_it.get_sub(), context_->manager);
      auto op_id = system_move.get_id();
      op_to_action_[op_id] = system_move;
      op_to_effects_[op_id] = env_state_node;
      ops_.emplace_back(sdd);
    }
  }
  return ops_;
}

std::vector<State*> State::apply_op(SddNodeWrapper op) {
  auto wrapper = op_to_effects_[op.get_id()];
  std::vector<State*> states;
  if (wrapper.get_type() == SddNodeType::STATE) {
    auto formula_next_state = context_->next_state_formula_(wrapper.get_raw());
    auto sdd_next_state = context_->formula_to_sdd_(formula_next_state);
    auto sdd_next_state_id = sdd_next_state.get_id();
    context_->print_search_debug("env move forced to next state {}",
                                 sdd_next_state_id);
    auto state = new State(context_, formula_next_state);
    states.emplace_back(state);
  } else if (wrapper.get_type() == SddNodeType::SYSTEM_STATE) {
    // parent is not the vtree root; it means that environment choice is
    // irrelevant
    context_->print_search_debug("env choice is irrelevant");
    auto formula_next_state = context_->next_state_formula_(wrapper.get_raw());
    auto state = new State(context_, formula_next_state);
    states.emplace_back(state);
  } else {
    assert(wrapper.get_type() == ENV_STATE);
    auto child_it = wrapper.begin();
    auto children_end = wrapper.end();
    context_->print_search_debug("Processing {} env node's children nodes",
                                 wrapper.nb_children());
    states.reserve(wrapper.nb_children());
    for (; child_it != children_end; ++child_it) {
      // TODO we better store the env_node as well
      auto env_node = SddNodeWrapper(child_it.get_prime(), context_->manager);
      auto state_node = SddNodeWrapper(child_it.get_sub(), context_->manager);
      assert(state_node.get_type() == STATE);
      auto formula_next_state = context_->next_state_formula_(state_node.get_raw());
      auto state = new State(context_, formula_next_state);
      states.emplace_back(state);
    }
  }
  return states;
}



} // namespace core
} // namespace cynthia