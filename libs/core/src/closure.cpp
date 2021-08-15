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
  auto result = utils::binary_search_find_index(from_id_to_subformula, formula,
                                                utils::Deref::Less());
  if (result > from_id_to_subformula.size()) {
    throw std::invalid_argument("formula not found");
  }
  return result;
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
  // 'true' in the closure implies 'tt' in the closure
  auto& c = formula.ctx();
  const auto& tt = c.make_tt();
  insert_if_not_already_present_(*tt);
}
void ClosureVisitor::visit(const logic::LTLfPropFalse& formula) {
  insert_if_not_already_present_(formula);
  // 'false' in the closure implies 'ff' in the closure
  auto& c = formula.ctx();
  const auto& ff = c.make_ff();
  insert_if_not_already_present_(*ff);
}
void ClosureVisitor::visit(const logic::LTLfAtom& formula) {
  insert_if_not_already_present_(formula);
  // 'atom' in the closure implies:
  //   (1) 'tt' in the closure (in case of success transition)
  //   (2) 'ff' in the closure (in case of failing transition)
  auto& c = formula.ctx();
  const auto& tt = c.make_tt();
  const auto& ff = c.make_ff();
  insert_if_not_already_present_(*tt);
  insert_if_not_already_present_(*ff);
}
void ClosureVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void ClosureVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  insert_if_not_already_present_(formula);
  // '!atom' in the closure implies:
  //   (1) 'tt' in the closure (in case of success transition)
  //   (2) 'ff' in the closure (in case of failing transition)
  auto& c = formula.ctx();
  const auto& tt = c.make_tt();
  const auto& ff = c.make_ff();
  insert_if_not_already_present_(*tt);
  insert_if_not_already_present_(*ff);
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
  apply_to_right_associative_binary_op_(formula,
                                        [&formula](const logic::vec_ptr& args) {
                                          return formula.ctx().make_until(args);
                                        });
  apply(*formula.ctx().make_next(formula.shared_from_this()));
}
void ClosureVisitor::visit(const logic::LTLfRelease& formula) {
  apply_to_right_associative_binary_op_(
      formula, [&formula](const logic::vec_ptr& args) {
        return formula.ctx().make_release(args);
      });
  apply(*formula.ctx().make_weak_next(formula.shared_from_this()));
}
void ClosureVisitor::visit(const logic::LTLfEventually& formula) {
  apply_to_unary_op_(formula);
  apply(*formula.ctx().make_next(formula.shared_from_this()));
}
void ClosureVisitor::visit(const logic::LTLfAlways& formula) {
  apply_to_unary_op_(formula);
  apply(*formula.ctx().make_weak_next(formula.shared_from_this()));
}
void ClosureVisitor::apply(const logic::LTLfFormula& f) { f.accept(*this); }
Closure closure(const logic::LTLfFormula& f) {
  auto visitor = ClosureVisitor{};
  visitor.apply(f);
  visitor.add_end_and_not_end_(f.ctx());
  return Closure{visitor.formulas};
}

logic::vec_ptr::const_iterator Closure::begin_formulas() const {
  return from_id_to_subformula.begin();
}

logic::vec_ptr::const_iterator Closure::end_formulas() const {
  return from_id_to_subformula.end();
}

std::vector<logic::atom_ptr>::const_iterator Closure::begin_atoms() const {
  return atoms.begin();
}

std::vector<logic::atom_ptr>::const_iterator Closure::end_atoms() const {
  return atoms.end();
}

Closure::Closure(const logic::set_ptr& formulas)
    : from_id_to_subformula(utils::vectify(formulas)) {
  find_atoms_();
}

void Closure::find_atoms_() {
  for (const auto& formula : this->from_id_to_subformula) {
    if (logic::is_a<logic::LTLfAtom>(*formula)) {
      atoms.push_back(std::static_pointer_cast<const logic::LTLfAtom>(formula));
    } else if (logic::is_a<logic::LTLfPropositionalNot>(*formula)) {
      atoms.push_back(
          std::static_pointer_cast<const logic::LTLfPropositionalNot>(formula)
              ->get_atom());
    }
  }
  std::sort(atoms.begin(), atoms.end(), utils::Deref::Less());
}

} // namespace core
} // namespace cynthia
