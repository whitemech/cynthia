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
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto tt = context.make_tt();
  auto formula_closure = closure(*tt);

  auto expected = logic::vec_ptr({
      tt,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of ff", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto ff = context.make_ff();
  auto formula_closure = closure(*ff);

  auto expected = logic::vec_ptr({
      ff,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of true", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto tt = context.make_tt();
  auto true_ = context.make_prop_true();
  auto formula_closure = closure(*true_);

  auto expected = logic::vec_ptr({
      tt,
      true_,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of false", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto false_ = context.make_prop_false();
  auto ff = context.make_ff();
  auto formula_closure = closure(*false_);

  auto expected = logic::vec_ptr({
      ff,
      false_,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of a", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto tt = context.make_tt();
  auto ff = context.make_ff();
  auto a = context.make_atom("a");
  auto formula_closure = closure(*a);

  auto expected = logic::vec_ptr({
      tt,
      ff,
      a,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of !a", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto tt = context.make_tt();
  auto ff = context.make_ff();
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto next_not_a = context.make_next(not_a);

  auto formula_closure = closure(*not_a);

  auto expected = logic::vec_ptr({
      tt,
      ff,
      not_a,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of a & b", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and(logic::vec_ptr{a, b});
  auto a_and_b_closure = closure(*a_and_b);

  auto formula_closure = closure(*a_and_b);

  auto expected = logic::vec_ptr({
      tt,
      ff,
      a,
      b,
      a_and_b,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of a | b", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or(logic::vec_ptr{a, b});
  auto formula_closure = closure(*a_or_b);

  auto expected = logic::vec_ptr({
      tt,
      ff,
      a,
      b,
      a_or_b,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

TEST_CASE("Test closure of X[!]a", "[core][SDD]") {
  auto context = logic::Context();
  auto end = context.make_end();
  auto not_end = context.make_not_end();
  auto next_not_end = context.make_next(not_end);
  auto wnext_end = context.make_weak_next(end);

  auto ff = context.make_ff();
  auto tt = context.make_tt();
  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);
  auto formula_closure = closure(*next_a);

  auto expected = logic::vec_ptr({
      tt,
      ff,
      a,
      next_a,
      next_not_end,
      wnext_end,
      not_end,
      end,
  });
  auto actual = logic::vec_ptr(formula_closure.begin_formulas(),
                               formula_closure.end_formulas());

  REQUIRE(expected == actual);
}

} // namespace Test
} // namespace core
} // namespace cynthia