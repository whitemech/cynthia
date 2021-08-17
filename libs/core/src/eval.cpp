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

#include <cynthia/eval.hpp>
#include <cynthia/logic/ltlf.hpp>
#include <numeric>

namespace cynthia {
namespace core {

void EvalVisitor::visit(const logic::LTLfTrue& formula) { result = true; }
void EvalVisitor::visit(const logic::LTLfFalse& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfPropTrue& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfPropFalse& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfAtom& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void EvalVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  result = false;
}
void EvalVisitor::visit(const logic::LTLfAnd& formula) {
  result =
      std::all_of(formula.args.begin(), formula.args.end(),
                  [this](const logic::ltlf_ptr& arg) { return apply(*arg); });
}
void EvalVisitor::visit(const logic::LTLfOr& formula) {
  result =
      std::any_of(formula.args.begin(), formula.args.end(),
                  [this](const logic::ltlf_ptr& arg) { return apply(*arg); });
}
void EvalVisitor::visit(const logic::LTLfImplies& formula) {
  result = apply(*simplify(formula));
}
void EvalVisitor::visit(const logic::LTLfEquivalent& formula) {
  result = apply(*simplify(formula));
}
void EvalVisitor::visit(const logic::LTLfXor& formula) {
  result = apply(*simplify(formula));
}
void EvalVisitor::visit(const logic::LTLfNext& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfWeakNext& formula) { result = true; }
void EvalVisitor::visit(const logic::LTLfUntil& formula) { result = false; }
void EvalVisitor::visit(const logic::LTLfRelease& formula) { result = true; }
void EvalVisitor::visit(const logic::LTLfEventually& formula) {
  result = false;
}
void EvalVisitor::visit(const logic::LTLfAlways& formula) { result = true; }

bool EvalVisitor::apply(const logic::LTLfFormula& formula) {
  formula.accept(*this);
  return result;
}

bool eval(const logic::LTLfFormula& formula) {
  EvalVisitor visitor{};
  return visitor.apply(formula);
}

} // namespace core
} // namespace cynthia
