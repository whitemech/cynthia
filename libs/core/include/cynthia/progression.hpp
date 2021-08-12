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
#include <utility>

namespace cynthia {
namespace core {

class ProgressionManager {

private:
  logic::ltlf_ptr formula;

public:
  explicit ProgressionManager(logic::ltlf_ptr formula)
      : formula{std::move(formula)} {}
};

class Progression : public logic::Visitor {
public:
  logic::ltlf_ptr result;

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

  logic::ltlf_ptr apply(const logic::LTLfFormula& f);
};

logic::ltlf_ptr progress(const logic::LTLfFormula& f);

} // namespace core
} // namespace cynthia
