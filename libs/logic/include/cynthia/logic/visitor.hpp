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

#include <cynthia/logic/ltlf.hpp>

namespace cynthia {
namespace logic {

/*
 * Abstract visitor class.
 */
class Visitor {
public:
  virtual void visit(const Symbol&) = 0;
  virtual void visit(const LTLfTrue&) = 0;
  virtual void visit(const LTLfFalse&) = 0;
  virtual void visit(const LTLfPropTrue&) = 0;
  virtual void visit(const LTLfPropFalse&) = 0;
  virtual void visit(const LTLfAtom&) = 0;
  virtual void visit(const LTLfNot&) = 0;
  virtual void visit(const LTLfPropositionalNot&) = 0;
  virtual void visit(const LTLfAnd&) = 0;
  virtual void visit(const LTLfOr&) = 0;
  virtual void visit(const LTLfImplies&) = 0;
  virtual void visit(const LTLfEquivalent&) = 0;
  virtual void visit(const LTLfXor&) = 0;
  virtual void visit(const LTLfNext&) = 0;
  virtual void visit(const LTLfWeakNext&) = 0;
  virtual void visit(const LTLfUntil&) = 0;
  virtual void visit(const LTLfRelease&) = 0;
  virtual void visit(const LTLfEventually&) = 0;
  virtual void visit(const LTLfAlways&) = 0;
};

} // namespace logic
} // namespace cynthia