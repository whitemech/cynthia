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
#include <cynthia/closure.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("Test Closure of tt", "[core][SDD]") {
  auto context = logic::Context();

  auto tt = context.make_tt();
  auto tt_closure = closure(*tt);

  REQUIRE(tt_closure.nb_formulas() == 1);
  REQUIRE(tt_closure.get_id(tt) == 0);
  REQUIRE(tt_closure.get_formula(0) == tt);
}

TEST_CASE("Test Closure of ff", "[core][SDD]") {
  auto context = logic::Context();

  auto ff = context.make_ff();
  auto ff_closure = closure(*ff);

  REQUIRE(ff_closure.nb_formulas() == 1);
  REQUIRE(ff_closure.get_id(ff) == 0);
  REQUIRE(ff_closure.get_formula(0) == ff);
}

TEST_CASE("Test Closure of true", "[core][SDD]") {
  auto context = logic::Context();

  auto true_ = context.make_prop_true();
  auto true_closure = closure(*true_);

  REQUIRE(true_closure.nb_formulas() == 1);
  REQUIRE(true_closure.get_id(true_) == 0);
  REQUIRE(true_closure.get_formula(0) == true_);
}

TEST_CASE("Test Closure of false", "[core][SDD]") {
  auto context = logic::Context();

  auto false_ = context.make_prop_false();
  auto false_closure = closure(*false_);

  REQUIRE(false_closure.nb_formulas() == 1);
  REQUIRE(false_closure.get_id(false_) == 0);
  REQUIRE(false_closure.get_formula(0) == false_);
}

TEST_CASE("Test Closure of a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto true_closure = closure(*a);

  REQUIRE(true_closure.nb_formulas() == 1);
  REQUIRE(true_closure.get_id(a) == 0);
  REQUIRE(true_closure.get_formula(0) == a);
}

TEST_CASE("Test Closure of !a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto not_a_closure = closure(*not_a);

  REQUIRE(not_a_closure.nb_formulas() == 2);
  REQUIRE(not_a_closure.get_id(not_a) == 0);
  REQUIRE(not_a_closure.get_formula(0) == not_a);
  REQUIRE(not_a_closure.get_id(a) == 1);
  REQUIRE(not_a_closure.get_formula(1) == a);
}

TEST_CASE("Test Closure of a & b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and(logic::vec_ptr{a, b});
  auto a_and_b_closure = closure(*a_and_b);

  REQUIRE(a_and_b_closure.nb_formulas() == 3);
  REQUIRE(a_and_b_closure.get_id(a_and_b) == 0);
  REQUIRE(a_and_b_closure.get_formula(0) == a_and_b);
  REQUIRE(a_and_b_closure.get_id(a) == 1);
  REQUIRE(a_and_b_closure.get_formula(1) == a);
  REQUIRE(a_and_b_closure.get_id(b) == 2);
  REQUIRE(a_and_b_closure.get_formula(2) == b);
}

TEST_CASE("Test Closure of a | b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or(logic::vec_ptr{a, b});
  auto a_or_b_closure = closure(*a_or_b);

  REQUIRE(a_or_b_closure.nb_formulas() == 3);
  REQUIRE(a_or_b_closure.get_id(a_or_b) == 0);
  REQUIRE(a_or_b_closure.get_formula(0) == a_or_b);
  REQUIRE(a_or_b_closure.get_id(a) == 1);
  REQUIRE(a_or_b_closure.get_formula(1) == a);
  REQUIRE(a_or_b_closure.get_id(b) == 2);
  REQUIRE(a_or_b_closure.get_formula(2) == b);
}

TEST_CASE("Test Closure of X[!]a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);
  auto next_a_closure = closure(*next_a);

  REQUIRE(next_a_closure.nb_formulas() == 2);
  REQUIRE(next_a_closure.get_id(next_a) == 0);
  REQUIRE(next_a_closure.get_formula(0) == next_a);
  REQUIRE(next_a_closure.get_id(a) == 1);
  REQUIRE(next_a_closure.get_formula(1) == a);
}

} // namespace Test
} // namespace core
} // namespace cynthia