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

#include <cynthia/logic/visitor.hpp>

namespace cynthia {
namespace logic {

class Print : public Visitor {
public:
  std::string result;

  void visit(const Symbol&) override;
  void visit(const LTLfTrue&) override;
  void visit(const LTLfFalse&) override;
  void visit(const LTLfAtom&) override;
  void visit(const LTLfNot&) override;
};

std::string to_string(const LTLfFormula& f);

} // namespace logic
} // namespace cynthia
