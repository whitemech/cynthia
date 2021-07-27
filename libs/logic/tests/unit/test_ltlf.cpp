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

namespace cynthia {
namespace logic {
namespace Test {
TEST_CASE("Test Symbol", "[logic][ltlf]") {
  auto context = Context();
  auto s1 = context.make_symbol("symbol_1");
  auto s2 = context.make_symbol("symbol_2");
  auto s3 = context.make_symbol("symbol_1");

  // same value, same pointer
  REQUIRE(s1 == s3);
  REQUIRE(*s1 == *s3);

  // different value, different pointer
  REQUIRE(s1 != s2);
}

TEST_CASE("tt", "[logic][ltlf]") {
  auto context = Context();

  auto tt1 = context.make_tt();
  auto tt2 = context.make_bool(true);

  REQUIRE(tt1 == tt2);
  REQUIRE(*tt1 == *tt2);
}

TEST_CASE("ff", "[logic][ltlf]") {
  auto context = Context();

  auto ff1 = context.make_ff();
  auto ff2 = context.make_bool(false);

  REQUIRE(ff1 == ff2);
  REQUIRE(*ff1 == *ff2);
}

TEST_CASE("atom", "[logic][ltlf]") {
  auto context = Context();

  auto expeted_atom1 = context.make_atom("a");
  auto expeted_atom2 = context.make_atom("a");
  REQUIRE(expeted_atom1 == expeted_atom2);
  REQUIRE(*expeted_atom1 == *expeted_atom2);
}

TEST_CASE("negation", "[logic][ltlf]") {
  auto context = Context();

  auto atom1 = context.make_atom("a");
  auto expected1 = context.make_not(atom1);

  auto atom2 = context.make_atom("a");
  auto expected2 = context.make_not(atom2);
  REQUIRE(expected1 == expected2);
  REQUIRE(*expected1 == *expected2);
}

TEST_CASE("conjunction", "[logic][ltlf]") {
  auto context = Context();

  auto atom1 = context.make_atom("a");
  auto atom2 = context.make_atom("a");
  auto atom3 = context.make_atom("b");
  auto atom4 = context.make_atom("b");
  auto expected1 = context.make_and(vec_ptr{atom1, atom2, atom3});
  auto expected2 = context.make_and(vec_ptr{atom1, atom3, atom4});
  REQUIRE(expected1 == expected2);
  REQUIRE(*expected1 == *expected2);
}

TEST_CASE("disjunction", "[logic][ltlf]") {
  auto context = Context();

  auto atom1 = context.make_atom("a");
  auto atom2 = context.make_atom("a");
  auto atom3 = context.make_atom("b");
  auto atom4 = context.make_atom("b");
  auto expected1 = context.make_or(vec_ptr{atom1, atom2, atom3});
  auto expected2 = context.make_or(vec_ptr{atom1, atom3, atom4});
  REQUIRE(expected1 == expected2);
  REQUIRE(*expected1 == *expected2);
}

TEST_CASE("implication", "[logic][ltlf]") {
  auto context = Context();

  auto atom1 = context.make_atom("a");
  auto atom2 = context.make_atom("a");
  auto atom3 = context.make_atom("b");
  auto atom4 = context.make_atom("b");
  auto aab1 = context.make_implies(vec_ptr{atom1, atom2, atom4});
  auto aab2 = context.make_implies(vec_ptr{atom1, atom2, atom3});
  auto abb = context.make_implies(vec_ptr{atom1, atom3, atom4});
  REQUIRE(aab1 == aab2);
  REQUIRE(*aab1 == *aab2);

  REQUIRE(aab1 != abb);
  REQUIRE(*aab1 != *abb);
}

TEST_CASE("equivalence", "[logic][ltlf]") {
  auto context = Context();

  auto atom1 = context.make_atom("a");
  auto atom2 = context.make_atom("a");
  auto atom3 = context.make_atom("b");
  auto atom4 = context.make_atom("b");
  auto aa = context.make_equivalent(vec_ptr{atom1, atom2});
  auto bb = context.make_equivalent(vec_ptr{atom3, atom4});
  REQUIRE(aa != bb);
  REQUIRE(*aa != *bb);

  REQUIRE(aa == aa);
  REQUIRE(*aa == *aa);

  auto aa_bb = context.make_equivalent(vec_ptr{aa, bb});
  auto bb_aa = context.make_equivalent(vec_ptr{bb, aa});
  REQUIRE(aa_bb != bb_aa);
  REQUIRE(*aa_bb != *bb_aa);
}

} // namespace Test
} // namespace logic
} // namespace cynthia