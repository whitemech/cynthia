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

#include <catch.hpp>
#include <cynthia/logic/ltlf.hpp>
#include <cynthia/logic/nnf.hpp>

namespace cynthia {
namespace logic {
namespace Test {

TEST_CASE("Test NNF of tt") {
  auto context = Context();
  auto f = context.make_tt();
  auto actual_formula = to_nnf(*f);
  REQUIRE(f == actual_formula);
}

TEST_CASE("Test NNF of ff") {
  auto context = Context();
  auto f = context.make_ff();
  auto actual_formula = to_nnf(*f);
  REQUIRE(f == actual_formula);
}

TEST_CASE("Test NNF of !tt") {
  auto context = Context();
  auto f = context.make_not(context.make_tt());
  auto actual_formula = to_nnf(*f);
  auto expected_formula = context.make_ff();
  REQUIRE(expected_formula == actual_formula);
}

TEST_CASE("Test NNF of !ff") {
  auto context = Context();
  auto f = context.make_not(context.make_ff());
  auto actual_formula = to_nnf(*f);
  auto expected_formula = context.make_tt();
  REQUIRE(expected_formula == actual_formula);
}

TEST_CASE("Test NNF of atomic") {
  auto context = Context();
  auto a = context.make_atom("a");
  auto actual_formula = to_nnf(*a);
  REQUIRE(a == actual_formula);
}

TEST_CASE("Test NNF of negated atomic") {
  auto context = Context();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto actual_formula = to_nnf(*not_a);
  REQUIRE(not_a == actual_formula);
}

TEST_CASE("Test NNF of doubly negated atomic") {
  auto context = Context();
  auto a = context.make_atom("!!a");
  auto not_not_a = context.make_prop_not(context.make_prop_not(a));
  auto actual_formula = to_nnf(*not_not_a);
  REQUIRE(a == actual_formula);
}

} // namespace Test
} // namespace logic
} // namespace cynthia