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
#include <iostream>
#include <map>
#include <utility>

namespace cynthia {
namespace core {

class Closure {
private:
  // a sorted array
  const std::vector<logic::ltlf_ptr> from_id_to_subformula;
  const std::map<logic::ltlf_ptr, size_t> from_subformula_to_id;

public:
  explicit Closure(
      const std::map<logic::ltlf_ptr, size_t>& from_subformula_to_id)
      : from_id_to_subformula{utils::from_index_map_to_vector(
            from_subformula_to_id)},
        from_subformula_to_id{from_subformula_to_id} {};
  size_t get_id(const logic::ltlf_ptr& formula) const;
  const logic::ltlf_ptr& get_formula(size_t index) const;
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

public:
  std::map<logic::ltlf_ptr, size_t> from_subformula_to_id{};
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
};

Closure closure(const logic::LTLfFormula& f);

inline bool ClosureVisitor::insert_if_not_already_present_(
    const logic::LTLfFormula& formula) {
  auto formula_ptr = formula.shared_from_this();
  if (from_subformula_to_id.find(formula_ptr) == from_subformula_to_id.end()) {
    size_t new_id = from_subformula_to_id.size();
    from_subformula_to_id[formula_ptr] = new_id;
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
inline void
ClosureVisitor::apply_to_unary_op_(const logic::LTLfUnaryOp& formula) {
  auto result = insert_if_not_already_present_(formula);
  if (result) {
    apply(*formula.arg);
  }
}

} // namespace core
} // namespace cynthia
