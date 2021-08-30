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
#include <cynthia/logic/visitor.hpp>
#include <optional>

namespace cynthia {
namespace core {

class OneStepUnrealizabilityVisitor : public logic::Visitor {
public:
  ForwardSynthesis::Context& context_;
  SddNode* result{};
  explicit OneStepUnrealizabilityVisitor(ForwardSynthesis::Context& context)
      : context_{context} {}
  void visit(const logic::LTLfTrue&) override;
  void visit(const logic::LTLfFalse&) override;
  void visit(const logic::LTLfPropTrue&) override;
  void visit(const logic::LTLfPropFalse&) override;
  void visit(const logic::LTLfAtom&) override;
  void visit(const logic::LTLfNot&) override;
  void visit(const logic::LTLfPropositionalNot&) override;
  void visit(const logic::LTLfAnd&) override;
  void visit(const logic::LTLfOr&) override;
  void visit(const logic::LTLfImplies&) override;
  void visit(const logic::LTLfEquivalent&) override;
  void visit(const logic::LTLfXor&) override;
  void visit(const logic::LTLfNext&) override;
  void visit(const logic::LTLfWeakNext&) override;
  void visit(const logic::LTLfUntil&) override;
  void visit(const logic::LTLfRelease&) override;
  void visit(const logic::LTLfEventually&) override;
  void visit(const logic::LTLfAlways&) override;

  SddNode* apply(const logic::LTLfFormula& f);
  inline SddNode* get_sdd_node(const logic::LTLfFormula& formula) const {
    auto formula_id = context_.closure_.get_id(formula.shared_from_this());
    return sdd_manager_literal(formula_id + 1, context_.manager);
  }
};

std::pair<SddNode*, bool>
one_step_unrealizability(const logic::LTLfFormula& f,
                         ForwardSynthesis::Context& context);

} // namespace core
} // namespace cynthia
