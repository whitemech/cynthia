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
#include <cynthia/core.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/to_sdd.hpp>

namespace cynthia {
namespace core {

void ToSddVisitor::visit(const logic::LTLfTrue& formula) {
  result = get_sdd_node(formula);
}
void ToSddVisitor::visit(const logic::LTLfFalse& formula) {
  result = get_sdd_node(formula);
}
void ToSddVisitor::visit(const logic::LTLfPropTrue& formula) {
  auto true_sdd = sdd_manager_false(context_.manager);
  auto next_tt = formula.ctx().make_next(formula.ctx().make_tt());
  auto next_tt_id = context_.closure_.get_id(next_tt);
  auto next_tt_sdd = sdd_manager_literal(next_tt_id + 1, context_.manager);
  result = sdd_conjoin(true_sdd, next_tt_sdd, context_.manager);
}
void ToSddVisitor::visit(const logic::LTLfPropFalse& formula) {
  auto false_sdd = sdd_manager_false(context_.manager);
  auto next_ff = formula.ctx().make_next(formula.ctx().make_tt());
  auto next_ff_id = context_.closure_.get_id(next_ff);
  auto next_ff_sdd = sdd_manager_literal(next_ff_id + 1, context_.manager);
  result = sdd_conjoin(false_sdd, next_ff_sdd, context_.manager);
}
void ToSddVisitor::visit(const logic::LTLfAtom& formula) {
  auto formula_id = context_.prop_to_id[formula.name];
  auto atom_sdd = sdd_manager_literal(formula_id + 1, context_.manager);
  auto not_atom_sdd = sdd_negate(atom_sdd, context_.manager);

  auto next_tt = formula.ctx().make_next(formula.ctx().make_tt());
  auto next_tt_id = context_.closure_.get_id(next_tt);
  auto next_tt_sdd = sdd_manager_literal(next_tt_id + 1, context_.manager);
  auto next_ff = formula.ctx().make_next(formula.ctx().make_ff());
  auto next_ff_id = context_.closure_.get_id(next_ff);
  auto next_ff_sdd = sdd_manager_literal(next_ff_id + 1, context_.manager);

  result = sdd_disjoin(sdd_conjoin(atom_sdd, next_tt_sdd, context_.manager),
                       sdd_conjoin(not_atom_sdd, next_ff_sdd, context_.manager),
                       context_.manager);
}
void ToSddVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void ToSddVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  auto tmp = apply(*formula.arg);
  result = sdd_negate(tmp, context_.manager);
}
void ToSddVisitor::visit(const logic::LTLfAnd& formula) {
  SddNode* tmp = sdd_manager_true(context_.manager);
  for (const auto& arg : formula.args) {
    auto sdd_arg = apply(*arg);
    tmp = sdd_conjoin(tmp, sdd_arg, context_.manager);
  }
  result = tmp;
}
void ToSddVisitor::visit(const logic::LTLfOr& formula) {
  SddNode* tmp = sdd_manager_false(context_.manager);
  for (const auto& arg : formula.args) {
    auto sdd_arg = apply(*arg);
    tmp = sdd_disjoin(tmp, sdd_arg, context_.manager);
  }
  result = tmp;
}
void ToSddVisitor::visit(const logic::LTLfImplies& formula) {
  SddNode* tmp = sdd_manager_false(context_.manager);
  for (auto it = formula.args.begin(); it != formula.args.end() - 1; ++it) {
    auto sdd_arg = apply(**it);
    sdd_arg = sdd_negate(sdd_arg, context_.manager);
    tmp = sdd_disjoin(tmp, sdd_arg, context_.manager);
  }
  result = sdd_disjoin(tmp, apply(*formula.args.back()), context_.manager);
}
void ToSddVisitor::visit(const logic::LTLfEquivalent& formula) {
  logic::throw_not_implemented_error();
}
void ToSddVisitor::visit(const logic::LTLfXor& formula) {
  logic::throw_not_implemented_error();
}
void ToSddVisitor::visit(const logic::LTLfNext& formula) {
  result = get_sdd_node(formula);
}
void ToSddVisitor::visit(const logic::LTLfWeakNext& formula) {
  result = get_sdd_node(formula);
}
void ToSddVisitor::visit(const logic::LTLfUntil& formula) {
  logic::throw_expected_xnf();
}
void ToSddVisitor::visit(const logic::LTLfRelease& formula) {
  logic::throw_expected_xnf();
}
void ToSddVisitor::visit(const logic::LTLfEventually& formula) {
  logic::throw_expected_xnf();
}
void ToSddVisitor::visit(const logic::LTLfAlways& formula) {
  logic::throw_expected_xnf();
}

SddNode* ToSddVisitor::apply(const logic::LTLfFormula& formula) {
  formula.accept(*this);
  return result;
}

SddNode* to_sdd(const logic::LTLfFormula& formula,
                ForwardSynthesis::Context& context) {
  ToSddVisitor visitor{context};
  return visitor.apply(formula);
}

} // namespace core
} // namespace cynthia
