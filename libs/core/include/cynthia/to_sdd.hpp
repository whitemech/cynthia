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

namespace cynthia {
namespace core {

class ToSddVisitor : public logic::Visitor {
public:
  ForwardSynthesis::Context& context_;
  SddNode* result{};
  explicit ToSddVisitor(ForwardSynthesis::Context& context)
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

  SddNode* apply(const logic::LTLfFormula& formula);

  inline SddNode* get_sdd_node(const logic::LTLfFormula& formula) const {
    auto formula_id = context_.closure_.get_id(formula.shared_from_this());
    return sdd_manager_literal(formula_id + 1, context_.manager);
  }
};

SddNode* to_sdd(const logic::LTLfFormula& formula,
                ForwardSynthesis::Context& context);

// returns an SDD node representing ( node1 => node2 )
SddNode* sdd_imply(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 <=> node2 )
SddNode* sdd_equiv(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 ^ node2 )
SddNode* sdd_xor(SddNode* node1, SddNode* node2, SddManager* manager);

template <typename T>
inline SddNode* sdd_boolean_op(T& visitor, const logic::LTLfBinaryOp& formula,
                               SddNode* (*const initializer)(const SddManager*),
                               SddNode* (*const& reduce)(SddNode*, SddNode*,
                                                         SddManager*)) {
  SddNode *tmp1, *tmp2;
  tmp1 = initializer(visitor.context_.manager);
  for (const auto& arg : formula.args) {
    auto sdd_arg = visitor.apply(*arg);
    tmp2 = reduce(tmp1, sdd_arg, visitor.context_.manager);
    sdd_ref(tmp2, visitor.context_.manager);
    sdd_deref(sdd_arg, visitor.context_.manager);
    sdd_deref(tmp1, visitor.context_.manager);
    tmp1 = tmp2;
    visitor.context_.call_gc_vtree();
  }
  return tmp1;
}

} // namespace core
} // namespace cynthia
