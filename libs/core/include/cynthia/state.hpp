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
  ForwardSynthesis::Context* context_{};
  const logic::ltlf_ptr formula;
  logic::ltlf_ptr nnf_formula;
  logic::ltlf_ptr xnf_formula;
  SddNodeWrapper sdd;
  SddSize id;


  State(ForwardSynthesis::Context* context, const logic::ltlf_ptr formula);

  std::vector<State*> apply_op(SddNodeWrapper op);
  std::vector<SddNodeWrapper> compute_ops();
  bool is_deadend();
  bool is_goal_state();
  bool is_init_state();
  inline void set_init_state() { is_init_state_ = true; }
  void instantiate();

private:
  std::vector<SddNodeWrapper> ops_;
  std::map<SddSize, SddNodeWrapper> op_to_effects_;
  std::map<SddSize, SddNodeWrapper> op_to_action_;
  bool instantiated_ = false;
  bool is_goal_state_ = false;
  bool is_init_state_ = false;
};


} // namespace core
} // namespace cynthia
