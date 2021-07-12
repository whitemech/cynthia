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

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file
#include <catch.hpp>
#include <cynthia/logic/base.hpp>
#include <cynthia/parser/driver.hpp>

namespace cynthia {
namespace parser {
namespace Test {
TEST_CASE("Parsing tt", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto actual_formula = context->make_tt();
  std::istringstream fstring("tt");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
TEST_CASE("Parsing ff", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto actual_formula = context->make_ff();
  std::istringstream fstring("ff");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
TEST_CASE("Parsing atom", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto actual_formula = context->make_atom("a");
  std::istringstream fstring("a");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
TEST_CASE("Parsing not", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom = context->make_atom("a");
  auto actual_formula = context->make_not(atom);
  std::istringstream fstring("!a");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
TEST_CASE("Parsing and", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto actual_formula = context->make_and(logic::vec_ptr{atom_1, atom_2});
  std::istringstream fstring("a & b");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
TEST_CASE("Parsing or", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto actual_formula = context->make_or(logic::vec_ptr{atom_1, atom_2});
  std::istringstream fstring("a | b");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(actual_formula == expected_formula);
}
} // namespace Test
} // namespace parser
} // namespace cynthia