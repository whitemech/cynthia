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

#include <cynthia/progression.hpp>

namespace cynthia {
namespace core {

void Progression::visit(const logic::LTLfTrue& formula) {
  result = formula.shared_from_this();
}
void Progression::visit(const logic::LTLfFalse& formula) {
  result = formula.shared_from_this();
}
void Progression::visit(const logic::LTLfPropTrue& formula) {
  result = formula.ctx().make_tt();
}
void Progression::visit(const logic::LTLfPropFalse& formula) {
  result = formula.ctx().make_ff();
}
void Progression::visit(const logic::LTLfAtom& formula) { return; }
void Progression::visit(const logic::LTLfNot& formula) { return; }
void Progression::visit(const logic::LTLfPropositionalNot& formula) { return; }
void Progression::visit(const logic::LTLfAnd& formula) { return; }
void Progression::visit(const logic::LTLfOr& formula) { return; }
void Progression::visit(const logic::LTLfImplies& formula) { return; }
void Progression::visit(const logic::LTLfEquivalent& formula) { return; }
void Progression::visit(const logic::LTLfXor& formula) { return; }
void Progression::visit(const logic::LTLfNext& formula) { return; }
void Progression::visit(const logic::LTLfWeakNext& formula) { return; }
void Progression::visit(const logic::LTLfUntil& formula) { return; }
void Progression::visit(const logic::LTLfRelease& formula) { return; }
void Progression::visit(const logic::LTLfEventually& formula) { return; }
void Progression::visit(const logic::LTLfAlways& formula) { return; }

logic::ltlf_ptr Progression::apply(const logic::LTLfFormula& f) {}

logic::ltlf_ptr progress(const logic::LTLfFormula& f) {}

} // namespace core
} // namespace cynthia
