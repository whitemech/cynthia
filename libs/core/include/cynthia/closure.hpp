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

#include <cynthia/logic/visitor.hpp>
#include <cynthia/utils.hpp>
#include <iostream>
#include <map>
#include <set>
#include <utility>

namespace cynthia {
namespace core {

class Closure {
private:
  std::vector<logic::atom_ptr> atoms;
  logic::vec_ptr from_id_to_subformula;
  friend Closure closure(const logic::LTLfFormula& f);
  explicit Closure(const logic::set_ptr& formulas);

public:
  Closure() = default;
  size_t get_id(const logic::ltlf_ptr& formula) const;
  const logic::ltlf_ptr& get_formula(size_t index) const;
  inline size_t nb_formulas() const { return from_id_to_subformula.size(); };
  inline size_t nb_atoms() const { return atoms.size(); };
  logic::vec_ptr::const_iterator begin_formulas() const;
  logic::vec_ptr::const_iterator end_formulas() const;
  std::vector<logic::atom_ptr>::const_iterator begin_atoms() const;
  std::vector<logic::atom_ptr>::const_iterator end_atoms() const;

  void find_atoms_();
};

class ClosureVisitor : public logic::Visitor {
private:
  /**
   * Insert a formula in the index, but only if it is not already present.
   * @param formula the formula to add
   * @return true if the formula was added (for the first time), false otherwise
   */
  inline bool insert_if_not_already_present_(const logic::LTLfFormula& formula);
  inline void apply_to_binary_op_(const logic::LTLfBinaryOp& formula);
  inline void apply_to_unary_op_(const logic::LTLfUnaryOp& formula);
  inline void add_end_and_not_end_(logic::Context& context);
  friend Closure closure(const logic::LTLfFormula& f);

public:
  logic::set_ptr formulas;
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

  void apply(const logic::LTLfFormula& f);

  template <typename FactoryFunction>
  inline void
  apply_to_right_associative_binary_op_(const logic::LTLfBinaryOp& formula,
                                        FactoryFunction function);
};

Closure closure(const logic::LTLfFormula& f);

inline bool ClosureVisitor::insert_if_not_already_present_(
    const logic::LTLfFormula& formula) {
  auto formula_ptr = formula.shared_from_this();
  if (formulas.find(formula_ptr) == formulas.end()) {
    formulas.insert(formula_ptr);
    return true;
  }
  return false;
}

inline void
ClosureVisitor::apply_to_binary_op_(const logic::LTLfBinaryOp& formula) {
  auto result = insert_if_not_already_present_(formula);
  if (!result)
    return;

  for (const auto& arg : formula.args) {
    apply(*arg);
  }
}
template <typename FactoryFunction>
inline void ClosureVisitor::apply_to_right_associative_binary_op_(
    const logic::LTLfBinaryOp& formula, FactoryFunction function) {
  apply_to_binary_op_(formula);
  for (auto it = formula.args.begin() + 1; it != formula.args.end() - 1; ++it) {
    auto subformula = function(logic::vec_ptr(it, formula.args.end()));
    apply(*subformula);
  }
}
inline void
ClosureVisitor::apply_to_unary_op_(const logic::LTLfUnaryOp& formula) {
  auto result = insert_if_not_already_present_(formula);
  if (result) {
    apply(*formula.arg);
  }
}

void ClosureVisitor::add_end_and_not_end_(logic::Context& context) {
  auto& c = context;
  auto not_end = c.make_not_end();
  insert_if_not_already_present_(*not_end);
  insert_if_not_already_present_(*c.make_next(not_end));
  auto end = c.make_end();
  insert_if_not_already_present_(*end);
  insert_if_not_already_present_(*c.make_weak_next(end));
  auto not_last = c.make_next(c.make_tt());
  insert_if_not_already_present_(*not_last);
  auto last = c.make_last();
  insert_if_not_already_present_(*last);
}

} // namespace core
} // namespace cynthia
