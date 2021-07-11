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

#include <string>

namespace cynthia {
namespace parser {
namespace ltlf {

struct LTLf_YYSTYPE {
  //    ldlf_ptr formula;
  void* formula;
  std::string symbol_name;

  // Constructor
  LTLf_YYSTYPE() = default;
  // Destructor
  ~LTLf_YYSTYPE() = default;
  // Copy constructor and assignment
  LTLf_YYSTYPE(const LTLf_YYSTYPE&) = default;
  LTLf_YYSTYPE& operator=(const LTLf_YYSTYPE&) = default;
  // Move constructor and assignment
  LTLf_YYSTYPE(LTLf_YYSTYPE&&) = default;
  LTLf_YYSTYPE& operator=(LTLf_YYSTYPE&&) = default;
};

} // namespace ltlf
} // namespace parser
} // namespace cynthia
