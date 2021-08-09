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

TEST_CASE("Test NNF of next") {
  // ~X[!]a = X(!a | end)
  auto context = Context();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto end = context.make_end();
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto next_a = context.make_next(a);
  auto not_next_a = context.make_not(next_a);
  auto weak_next_not_a = context.make_weak_next(not_a_or_end);
  auto actual = to_nnf(*not_next_a);
  const auto& expected = weak_next_not_a;
  REQUIRE(actual == expected);
}

TEST_CASE("Test NNF of weak next") {
  // ~X a = X[!](!a | end)
  auto context = Context();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto end = context.make_end();
  auto weak_next_a = context.make_weak_next(a);
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto next_not_a = context.make_next(not_a_or_end);
  auto not_weak_next_a = context.make_not(weak_next_a);
  auto actual = to_nnf(*not_weak_next_a);
  const auto& expected = next_not_a;
  REQUIRE(actual == expected);
}

TEST_CASE("Test NNF of until") {
  // ~(a U b) = ((!a | end) R (!b | end))
  auto context = Context();
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto not_a = context.make_prop_not(a);
  auto not_b = context.make_prop_not(b);
  auto end = context.make_end();
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto not_b_or_end = context.make_or(vec_ptr{not_b, end});
  auto a_until_b = context.make_until(vec_ptr{a, b});
  auto not_a_until_b = context.make_not(a_until_b);
  auto not_a_release_not_b =
      context.make_release(vec_ptr{not_a_or_end, not_b_or_end});
  auto actual = to_nnf(*not_a_until_b);
  const auto& expected = not_a_release_not_b;
  REQUIRE(actual == expected);
}

TEST_CASE("Test NNF of release") {
  // ~(a R b) = ((!a | end) U (!b | end))
  auto context = Context();
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto not_a = context.make_prop_not(a);
  auto not_b = context.make_prop_not(b);
  auto end = context.make_end();
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto not_b_or_end = context.make_or(vec_ptr{not_b, end});
  auto a_release_b = context.make_release(vec_ptr{a, b});
  auto not_a_release_b = context.make_not(a_release_b);
  auto not_a_until_not_b =
      context.make_until(vec_ptr{not_a_or_end, not_b_or_end});
  auto actual = to_nnf(*not_a_release_b);
  const auto& expected = not_a_until_not_b;
  REQUIRE(actual == expected);
}

TEST_CASE("Test NNF of eventually") {
  // ~Fa = G(!a | end)
  auto context = Context();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto end = context.make_end();
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto eventually_a = context.make_eventually(a);
  auto not_eventually_a = context.make_not(eventually_a);
  auto always_not_a = context.make_always(not_a_or_end);
  auto actual = to_nnf(*not_eventually_a);
  const auto& expected = always_not_a;
  REQUIRE(actual == expected);
}

TEST_CASE("Test NNF of always") {
  // ~Ga = F(!a | end)
  auto context = Context();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto end = context.make_end();
  auto not_a_or_end = context.make_or(vec_ptr{not_a, end});
  auto always_a = context.make_always(a);
  auto not_always_a = context.make_not(always_a);
  auto eventually_not_a = context.make_eventually(not_a_or_end);
  auto actual = to_nnf(*not_always_a);
  const auto& expected = eventually_not_a;
  REQUIRE(actual == expected);
}

} // namespace Test
} // namespace logic
} // namespace cynthia