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
#include <cynthia/closure.hpp>

namespace cynthia {
namespace core {

size_t Closure::get_id(const logic::ltlf_ptr& formula) const {
  if (from_subformula_to_id.find(formula) == from_subformula_to_id.end()) {
    throw std::invalid_argument("cannot find formula in closure");
  }
  return from_subformula_to_id.at(formula);
}
const logic::ltlf_ptr& Closure::get_formula(size_t index) const {
  if (index > from_id_to_subformula.size()) {
    throw std::invalid_argument("invalid index");
  }
  return from_id_to_subformula[index];
}

void ClosureVisitor::visit(const logic::LTLfTrue& formula) {
  insert_if_not_already_present_(formula);
}
void ClosureVisitor::visit(const logic::LTLfFalse& formula) {
  insert_if_not_already_present_(formula);
}
void ClosureVisitor::visit(const logic::LTLfPropTrue& formula) {
  insert_if_not_already_present_(formula);
}
void ClosureVisitor::visit(const logic::LTLfPropFalse& formula) {
  insert_if_not_already_present_(formula);
}
void ClosureVisitor::visit(const logic::LTLfAtom& formula) {
  insert_if_not_already_present_(formula);
  auto atom = std::static_pointer_cast<const logic::LTLfAtom>(
      formula.shared_from_this());
  atoms.insert(atom);
}
void ClosureVisitor::visit(const logic::LTLfNot& formula) {
  apply_to_unary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  apply_to_unary_op_(formula);
  auto atom = std::static_pointer_cast<const logic::LTLfAtom>(formula.arg);
  atoms.insert(atom);
}
void ClosureVisitor::visit(const logic::LTLfAnd& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfOr& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfImplies& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfEquivalent& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfXor& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfNext& formula) {
  apply_to_unary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfWeakNext& formula) {
  apply_to_unary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfUntil& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfRelease& formula) {
  apply_to_binary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfEventually& formula) {
  apply_to_unary_op_(formula);
}
void ClosureVisitor::visit(const logic::LTLfAlways& formula) {
  apply_to_unary_op_(formula);
}
void ClosureVisitor::apply(const logic::LTLfFormula& f) { f.accept(*this); }
Closure closure(const logic::LTLfFormula& f) {
  auto visitor = ClosureVisitor{};
  visitor.apply(f);
  return Closure{visitor.from_subformula_to_id,
                 std::vector<std::shared_ptr<const logic::LTLfAtom>>{
                     visitor.atoms.begin(), visitor.atoms.end()}};
}

logic::map_ptr::const_iterator Closure::begin_formulas() const {
  return from_subformula_to_id.begin();
}

logic::map_ptr::const_iterator Closure::end_formulas() const {
  return from_subformula_to_id.end();
}

std::vector<logic::atom_ptr>::const_iterator Closure::begin_atoms() const {
  return atoms.begin();
}

std::vector<logic::atom_ptr>::const_iterator Closure::end_atoms() const {
  return atoms.end();
}

} // namespace core
} // namespace cynthia
