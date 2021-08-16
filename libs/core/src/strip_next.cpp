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

#include <cynthia/logic/utils.hpp>
#include <cynthia/strip_next.hpp>

namespace cynthia {
namespace core {
void StripNextVisitor::visit(const logic::LTLfTrue& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfFalse& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfPropTrue& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfPropFalse& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfAtom& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void StripNextVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  result = formula.shared_from_this();
}
void StripNextVisitor::visit(const logic::LTLfAnd& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_and(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfOr& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_or(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfImplies& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_implies(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfEquivalent& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_equivalent(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfXor& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_xor(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfNext& formula) {
  result = formula.ctx().make_and({formula.arg, formula.ctx().make_not_end()});
}
void StripNextVisitor::visit(const logic::LTLfWeakNext& formula) {
  result = formula.ctx().make_or({formula.arg, formula.ctx().make_end()});
}
void StripNextVisitor::visit(const logic::LTLfUntil& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_until(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfRelease& formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr& formula) { return apply(*formula); },
      [formula](const logic::vec_ptr& container) {
        return formula.ctx().make_release(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfEventually& formula) {
  result = formula.ctx().make_eventually(apply(*formula.arg));
}
void StripNextVisitor::visit(const logic::LTLfAlways& formula) {
  result = formula.ctx().make_always(apply(*formula.arg));
}

logic::ltlf_ptr StripNextVisitor::apply(const logic::LTLfFormula& formula) {
  formula.accept(*this);
  return result;
}

logic::ltlf_ptr strip_next(const logic::LTLfFormula& formula) {
  auto visitor = StripNextVisitor{};
  return visitor.apply(formula);
}

} // namespace core
} // namespace cynthia
