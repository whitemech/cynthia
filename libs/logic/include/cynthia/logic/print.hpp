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

#include <cstdint>
#include <memory>

#include <cynthia/logic/utils.hpp>
#include <cynthia/logic/visitor.hpp>

namespace cynthia {
namespace logic {

class PrintVisitor : public Visitor {
private:
  void binary_op_to_string(const LTLfBinaryOp& formula,
                           const std::string& op_symbol);
  void unary_op_to_string(const LTLfUnaryOp& formula,
                          const std::string& op_symbol);

public:
  std::string result;

  void visit(const LTLfTrue&) override;
  void visit(const LTLfFalse&) override;
  void visit(const LTLfPropTrue&) override;
  void visit(const LTLfPropFalse&) override;
  void visit(const LTLfAtom&) override;
  void visit(const LTLfNot&) override;
  void visit(const LTLfPropositionalNot&) override;
  void visit(const LTLfAnd&) override;
  void visit(const LTLfOr&) override;
  void visit(const LTLfImplies&) override;
  void visit(const LTLfEquivalent&) override;
  void visit(const LTLfXor&) override;
  void visit(const LTLfNext&) override;
  void visit(const LTLfWeakNext&) override;
  void visit(const LTLfUntil&) override;
  void visit(const LTLfRelease&) override;
  void visit(const LTLfEventually&) override;
  void visit(const LTLfAlways&) override;

  std::string apply(const LTLfFormula& f);
};

std::string to_string(const LTLfFormula& f);

} // namespace logic
} // namespace cynthia