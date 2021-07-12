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

#include <cynthia/logic/print_visitor.hpp>

namespace cynthia {
namespace logic {

void Print::visit(const Symbol& s) { result = s.name; }

void Print::visit(const LTLfTrue& f) { result = "tt"; }

void Print::visit(const LTLfFalse& f) { result = "ff"; }

void Print::visit(const LTLfAtom& f) { result = f.symbol->name; }

void Print::visit(const LTLfNot& f) {
  auto arg_string = to_string(*f.arg);
  result = "~(" + arg_string + ")";
}

std::string to_string(const LTLfFormula& f) {
  auto visitor = Print{};
  f.accept(&visitor);
  return visitor.result;
}

} // namespace logic
} // namespace cynthia
