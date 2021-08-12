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
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("Test XNF of tt", "[core][SDD]") {
  auto context = logic::Context();

  auto tt = context.make_tt();
  REQUIRE(xnf(*tt) == tt);
}

TEST_CASE("Test XNF of ff", "[core][SDD]") {
  auto context = logic::Context();
  auto ff = context.make_ff();
  REQUIRE(xnf(*ff) == ff);
}

TEST_CASE("Test XNF of true", "[core][SDD]") {
  auto context = logic::Context();

  auto true_ = context.make_prop_true();
  REQUIRE(xnf(*true_) == true_);
}

TEST_CASE("Test XNF of false", "[core][SDD]") {
  auto context = logic::Context();

  auto false_ = context.make_prop_false();
  REQUIRE(xnf(*false_) == false_);
}

TEST_CASE("Test XNF of a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  REQUIRE(xnf(*a) == a);
}

TEST_CASE("Test XNF of !a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  REQUIRE(xnf(*not_a) == not_a);
}

TEST_CASE("Test XNF of a & b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and(logic::vec_ptr{a, b});
  REQUIRE(xnf(*a_and_b) == a_and_b);
}

TEST_CASE("Test XNF of a | b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or(logic::vec_ptr{a, b});
  REQUIRE(xnf(*a_or_b) == a_or_b);
}

TEST_CASE("Test XNF of X[!]a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);
  REQUIRE(xnf(*next_a) == next_a);
}

TEST_CASE("Test XNF of X a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto weak_next_a = context.make_weak_next(a);
  REQUIRE(xnf(*weak_next_a) == weak_next_a);
}

TEST_CASE("Test XNF of a U b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_until_b = context.make_until({a, b});

  auto next_a_until_b = context.make_next(a_until_b);
  auto expected = context.make_or({b, context.make_and({a, next_a_until_b})});
  auto actual = xnf(*a_until_b);
  REQUIRE(actual == expected);
}
TEST_CASE("Test XNF of a R b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_release_b = context.make_release({a, b});

  auto weaknext_a_release_b = context.make_weak_next(a_release_b);
  auto expected =
      context.make_and({b, context.make_or({a, weaknext_a_release_b})});
  auto actual = xnf(*a_release_b);
  REQUIRE(actual == expected);
}

} // namespace Test
} // namespace core
} // namespace cynthia