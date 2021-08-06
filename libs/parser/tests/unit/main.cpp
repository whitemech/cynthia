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
  auto expected_formula = context->make_tt();
  std::istringstream fstring("tt");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing ff", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto expected_formula = context->make_ff();
  std::istringstream fstring("ff");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing true", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto expected_formula = context->make_prop_true();
  std::istringstream fstring("true");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing false", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto expected_formula = context->make_prop_false();
  std::istringstream fstring("false");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing atom", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto expected_formula = context->make_atom("a");
  std::istringstream fstring("a");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing not", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom = context->make_atom("a");
  auto next = context->make_next(atom);
  auto actual_formula = context->make_not(next);
  std::istringstream fstring("!X[!] a");
  driver.parse(fstring);
  auto expected_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing propositional not", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);

  SECTION("parse !a") {
    auto atom = context->make_atom("a");
    auto expected_formula = context->make_prop_not(atom);
    std::istringstream fstring("!a");
    driver.parse(fstring);
    auto actual_formula = driver.result;
    REQUIRE(expected_formula == actual_formula);
  }

  SECTION("parse !!a = a") {
    auto expected_formula = context->make_atom("a");
    std::istringstream fstring("!!a");
    driver.parse(fstring);
    auto actual_formula = driver.result;
    REQUIRE(expected_formula == actual_formula);
  }
}
TEST_CASE("Parsing and", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto expected_formula = context->make_and(logic::vec_ptr{atom_1, atom_2});
  std::istringstream fstring("a & b");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing or", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto expected_formula = context->make_or(logic::vec_ptr{atom_1, atom_2});
  std::istringstream fstring("a | b");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing implies", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto atom_3 = context->make_atom("c");
  // right-associative
  auto expected_formula = context->make_implies(logic::vec_ptr{
      atom_1, context->make_implies(logic::vec_ptr{atom_2, atom_3})});
  std::istringstream fstring("a -> b -> c");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing equivalent", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto atom_3 = context->make_atom("c");
  auto expected_formula = context->make_equivalent(logic::vec_ptr{
      atom_1, context->make_equivalent(logic::vec_ptr{atom_2, atom_3})});
  std::istringstream fstring("a <-> b <-> c");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing xor", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto atom_2 = context->make_atom("b");
  auto atom_3 = context->make_atom("c");
  auto expected_formula = context->make_xor(logic::vec_ptr{
      atom_1, context->make_xor(logic::vec_ptr{atom_2, atom_3})});
  std::istringstream fstring("a ^ b ^ c");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing next", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto expected_formula = context->make_next(atom_1);
  std::istringstream fstring("X[!]a");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing weak next", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto atom_1 = context->make_atom("a");
  auto expected_formula = context->make_weak_next(atom_1);
  std::istringstream fstring("X a");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing until", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto a = context->make_atom("a");
  auto b = context->make_atom("b");
  auto expected_formula = context->make_until(logic::vec_ptr{a, b});
  std::istringstream fstring("a U b");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing release", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto a = context->make_atom("a");
  auto b = context->make_atom("b");
  auto expected_formula = context->make_release(logic::vec_ptr{a, b});
  std::istringstream fstring("a R b");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing eventually", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto a = context->make_atom("a");
  auto expected_formula = context->make_eventually(a);
  std::istringstream fstring("F a");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
TEST_CASE("Parsing always", "[parser][ltlf]") {
  auto context = std::make_shared<logic::Context>();
  auto driver = ltlf::LTLfDriver(context);
  auto a = context->make_atom("a");
  auto expected_formula = context->make_always(a);
  std::istringstream fstring("G a");
  driver.parse(fstring);
  auto actual_formula = driver.result;
  REQUIRE(expected_formula == actual_formula);
}
} // namespace Test
} // namespace parser
} // namespace cynthia