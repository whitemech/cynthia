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

#include <cassert>
#include <cynthia/one_step_unrealizability.hpp>
#include <cynthia/to_sdd.hpp>

namespace cynthia {
namespace core {

void OneStepUnrealizabilityVisitor::visit(const logic::LTLfTrue& formula) {
  result = sdd_manager_true(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfFalse& formula) {
  result = sdd_manager_false(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfPropTrue& formula) {
  result = sdd_manager_true(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfPropFalse& formula) {
  result = sdd_manager_false(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAtom& formula) {
  auto formula_id = problem_.prop_to_id[formula.name];
  result = sdd_manager_literal(formula_id + 1, problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfPropositionalNot& formula) {
  auto formula_id = problem_.prop_to_id[formula.get_atom()->name];
  auto atom_sdd = sdd_manager_literal(formula_id + 1, problem_.manager);
  auto not_atom_sdd = sdd_negate(atom_sdd, problem_.manager);
  sdd_ref(not_atom_sdd, problem_.manager);
  result = not_atom_sdd;
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAnd& formula) {
  result = sdd_boolean_op<OneStepUnrealizabilityVisitor>(
      *this, formula, sdd_manager_true, sdd_conjoin);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfOr& formula) {
  result = sdd_boolean_op<OneStepUnrealizabilityVisitor>(
      *this, formula, sdd_manager_false, sdd_disjoin);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfImplies& formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfEquivalent& formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfXor& formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfNext& formula) {
  result = sdd_manager_true(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfWeakNext& formula) {
  result = sdd_manager_true(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfUntil& formula) {
  result = apply(*formula.ctx().make_or(formula.args));
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfRelease& formula) {
  result = apply(**formula.args.rbegin());
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfEventually& formula) {
  result = sdd_manager_true(problem_.manager);
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAlways& formula) {
  result = apply(*formula.arg);
}

SddNode* OneStepUnrealizabilityVisitor::apply(const logic::LTLfFormula& f) {
  f.accept(*this);
  return result;
}

bool one_step_unrealizability(const logic::LTLfFormula& f,
                              ForwardSynthesis::Problem& problem) {
  auto visitor = OneStepUnrealizabilityVisitor{problem};
  auto result = visitor.apply(f);
  auto wrapper = SddNodeWrapper(result, problem.manager);
  if (wrapper.is_false()) {
    return false;
  }
  if (wrapper.is_true()) {
    return true;
  }

  if (wrapper.get_type() == SddNodeType::SYSTEM) {
    return true;
  }
  if (wrapper.get_type() == SddNodeType::SYSTEM_ENV_STATE) {
    auto child_it = wrapper.begin();
    for (; child_it != wrapper.end(); ++child_it) {
      if (sdd_node_is_true(child_it.get_sub())) {
        return true;
      }
    }
  }
  assert(wrapper.get_type() != SddNodeType::SYSTEM_STATE);
  assert(wrapper.get_type() != SddNodeType::STATE);
  return false;
}

} // namespace core
} // namespace cynthia
