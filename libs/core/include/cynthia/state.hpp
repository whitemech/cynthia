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

extern "C" {
#include "sddapi.h"
}
namespace cynthia {
namespace core {

class State {
public:
  State(ForwardSynthesis::Context* context, const logic::ltlf_ptr formula);

  void instantiate();
  std::vector<State*> apply_op(SddNodeWrapper op);
  std::vector<SddNodeWrapper> compute_ops();


  bool is_deadend();
  inline bool is_goal_state() { return is_goal_state_; }
  inline bool is_init_state() { return is_init_state_; }
  inline bool is_instantiated() { return instantiated_; }
  inline void set_init_state() { is_init_state_ = true; }
  inline ForwardSynthesis::Context* get_context() { return context_; }
  inline logic::ltlf_ptr get_formula() { return formula_; }
  inline logic::ltlf_ptr get_nnf_formula() { return nnf_formula_; }
  inline logic::ltlf_ptr get_xnf_formula() { return formula_; }
  inline SddSize get_id() { return id_; }




private:
  ForwardSynthesis::Context* context_{};
  const logic::ltlf_ptr formula_;
  logic::ltlf_ptr nnf_formula_;
  logic::ltlf_ptr xnf_formula_;
  SddNodeWrapper sdd_;
  SddSize id_;

  std::vector<SddNodeWrapper> ops_;
  std::map<SddSize, SddNodeWrapper> op_to_effects_;
  std::map<SddSize, SddNodeWrapper> op_to_action_;
  bool instantiated_ = false;
  bool is_goal_state_ = false;
  bool is_init_state_ = false;

};


} // namespace core
} // namespace cynthia
