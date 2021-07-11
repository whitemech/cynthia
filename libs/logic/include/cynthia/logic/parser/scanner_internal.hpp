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

#include <cynthia/logic/parser/location.hh>
#include <cynthia/logic/parser/parser.tab.hh>
#include <cynthia/logic/parser/parser_stype.h>

namespace cynthia {
namespace logic {
namespace parsers {
namespace ltlf {

class LTLfScanner : public ltlfFlexLexer {
private:
public:
  /* yyval ptr */
  cynthia::logic::parsers::ltlf::LTLf_YYSTYPE* yylval = nullptr;

  explicit LTLfScanner(std::istream* in) : ltlfFlexLexer(in){};
  virtual ~LTLfScanner(){};

  // get rid of override virtual function warning
  using FlexLexer::yylex;

  virtual int yylex(cynthia::logic::parsers::ltlf::LTLf_YYSTYPE* lval,
                    LTLfParser::location_type* location);
  // YY_DECL defined in lexer.l
  // Method body created by flex in lexer.yy.cc
};

} // namespace ltlf
} // namespace parsers
} // namespace logic
} // namespace cynthia
