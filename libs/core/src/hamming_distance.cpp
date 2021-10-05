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

#include <cynthia/hamming_distance.hpp>
#include <cynthia/logic/ltlf.hpp>
#include <algorithm>
#include <numeric>

namespace cynthia {
namespace core {

void HammingDistanceVisitor::visit(const logic::LTLfTrue& formula) { result = 0; }
void HammingDistanceVisitor::visit(const logic::LTLfFalse& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfPropTrue& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfPropFalse& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfAtom& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfNot& formula) {
  logic::throw_expected_nnf();
}
void HammingDistanceVisitor::visit(const logic::LTLfPropositionalNot& formula) {
  result = 1;
}
void HammingDistanceVisitor::visit(const logic::LTLfAnd& formula) {
  std::vector<size_t> values;
  values.resize(formula.args.size());
  std::transform(formula.args.begin(), formula.args.end(), values.begin(), [this](const logic::ltlf_ptr& arg) { return apply(*arg); });
  result =
      std::accumulate(values.begin(), values.end(), 0);
}
void HammingDistanceVisitor::visit(const logic::LTLfOr& formula) {
  std::vector<size_t> values;
  values.resize(formula.args.size());
  std::transform(formula.args.begin(), formula.args.end(), values.begin(), [this](const logic::ltlf_ptr& arg) { return apply(*arg); });
  result =
      *min_element(values.begin(), values.end());
}
void HammingDistanceVisitor::visit(const logic::LTLfImplies& formula) {
  result = apply(*simplify(formula));
}
void HammingDistanceVisitor::visit(const logic::LTLfEquivalent& formula) {
  result = apply(*simplify(formula));
}
void HammingDistanceVisitor::visit(const logic::LTLfXor& formula) {
  result = apply(*simplify(formula));
}
void HammingDistanceVisitor::visit(const logic::LTLfNext& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfWeakNext& formula) { result = 0; }
void HammingDistanceVisitor::visit(const logic::LTLfUntil& formula) { result = 1; }
void HammingDistanceVisitor::visit(const logic::LTLfRelease& formula) { result = 0; }
void HammingDistanceVisitor::visit(const logic::LTLfEventually& formula) {
  result = false;
}
void HammingDistanceVisitor::visit(const logic::LTLfAlways& formula) { result = 0; }

size_t HammingDistanceVisitor::apply(const logic::LTLfFormula& formula) {
  formula.accept(*this);
  return result;
}

size_t hamming_distance(const logic::LTLfFormula& formula) {
  HammingDistanceVisitor visitor{};
  return visitor.apply(formula);
}

} // namespace core
} // namespace cynthia
