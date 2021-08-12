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

TEST_CASE("Test closure of tt", "[core][SDD]") {
  auto context = logic::Context();

  auto tt = context.make_tt();
  auto tt_closure = closure(*tt);

  REQUIRE(tt_closure.nb_formulas() == 1);
  REQUIRE(tt_closure.get_id(tt) == 0);
  REQUIRE(tt_closure.get_formula(0) == tt);
}

TEST_CASE("Test closure of ff", "[core][SDD]") {
  auto context = logic::Context();

  auto ff = context.make_ff();
  auto ff_closure = closure(*ff);

  REQUIRE(ff_closure.nb_formulas() == 1);
  REQUIRE(ff_closure.get_id(ff) == 0);
  REQUIRE(ff_closure.get_formula(0) == ff);
}

TEST_CASE("Test closure of true", "[core][SDD]") {
  auto context = logic::Context();

  auto true_ = context.make_prop_true();
  auto true_closure = closure(*true_);

  REQUIRE(true_closure.nb_formulas() == 3);

  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto eventually_tt = context.make_eventually(tt);

  REQUIRE(true_closure.get_id(tt) == 0);
  REQUIRE(true_closure.get_formula(0) == tt);
  REQUIRE(true_closure.get_id(true_) == 1);
  REQUIRE(true_closure.get_formula(1) == true_);
  REQUIRE(true_closure.get_id(next_tt) == 2);
  REQUIRE(true_closure.get_formula(2) == next_tt);
}

TEST_CASE("Test closure of false", "[core][SDD]") {
  auto context = logic::Context();

  auto false_ = context.make_prop_false();
  auto false_closure = closure(*false_);

  REQUIRE(false_closure.nb_formulas() == 3);

  auto ff = context.make_ff();
  auto next_ff = context.make_next(ff);

  REQUIRE(false_closure.get_id(ff) == 0);
  REQUIRE(false_closure.get_formula(0) == ff);
  REQUIRE(false_closure.get_id(false_) == 1);
  REQUIRE(false_closure.get_formula(1) == false_);
  REQUIRE(false_closure.get_id(next_ff) == 2);
  REQUIRE(false_closure.get_formula(2) == next_ff);
}

TEST_CASE("Test closure of a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto a_closure = closure(*a);

  REQUIRE(a_closure.nb_formulas() == 5);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto next_ff = context.make_next(ff);

  REQUIRE(a_closure.get_id(tt) == 0);
  REQUIRE(a_closure.get_formula(0) == tt);
  REQUIRE(a_closure.get_id(ff) == 1);
  REQUIRE(a_closure.get_formula(1) == ff);
  REQUIRE(a_closure.get_id(a) == 2);
  REQUIRE(a_closure.get_formula(2) == a);
  REQUIRE(a_closure.get_id(next_tt) == 3);
  REQUIRE(a_closure.get_formula(3) == next_tt);
  REQUIRE(a_closure.get_id(next_ff) == 4);
  REQUIRE(a_closure.get_formula(4) == next_ff);
}

TEST_CASE("Test closure of !a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto not_a_closure = closure(*not_a);

  REQUIRE(not_a_closure.nb_formulas() == 5);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto next_ff = context.make_next(ff);

  REQUIRE(not_a_closure.get_id(tt) == 0);
  REQUIRE(not_a_closure.get_formula(0) == tt);
  REQUIRE(not_a_closure.get_id(ff) == 1);
  REQUIRE(not_a_closure.get_formula(1) == ff);
  REQUIRE(not_a_closure.get_id(not_a) == 2);
  REQUIRE(not_a_closure.get_formula(2) == not_a);
  REQUIRE(not_a_closure.get_id(next_tt) == 3);
  REQUIRE(not_a_closure.get_formula(3) == next_tt);
  REQUIRE(not_a_closure.get_id(next_ff) == 4);
  REQUIRE(not_a_closure.get_formula(4) == next_ff);
}

TEST_CASE("Test closure of a & b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and(logic::vec_ptr{a, b});
  auto a_and_b_closure = closure(*a_and_b);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto next_ff = context.make_next(ff);

  REQUIRE(a_and_b_closure.nb_formulas() == 7);
  auto actual = logic::vec_ptr(a_and_b_closure.begin_formulas(),
                               a_and_b_closure.end_formulas());
  auto expected = logic::vec_ptr({tt, ff, a, b, a_and_b, next_tt, next_ff});
  REQUIRE(actual == expected);
}

TEST_CASE("Test closure of a | b", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or(logic::vec_ptr{a, b});
  auto a_or_b_closure = closure(*a_or_b);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto next_ff = context.make_next(ff);

  REQUIRE(a_or_b_closure.nb_formulas() == 7);
  auto actual = logic::vec_ptr(a_or_b_closure.begin_formulas(),
                               a_or_b_closure.end_formulas());
  auto expected = logic::vec_ptr({tt, ff, a, b, a_or_b, next_tt, next_ff});
  REQUIRE(actual == expected);
}

TEST_CASE("Test closure of X[!]a", "[core][SDD]") {
  auto context = logic::Context();

  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);
  auto next_a_closure = closure(*next_a);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto next_tt = context.make_next(tt);
  auto next_ff = context.make_next(ff);
  auto eventually_tt = context.make_eventually(tt);

  REQUIRE(next_a_closure.nb_formulas() == 7);
  auto actual = logic::vec_ptr(next_a_closure.begin_formulas(),
                               next_a_closure.end_formulas());
  auto expected =
      logic::vec_ptr({tt, ff, a, next_tt, next_ff, next_a, eventually_tt});
  REQUIRE(actual == expected);
}

} // namespace Test
} // namespace core
} // namespace cynthia