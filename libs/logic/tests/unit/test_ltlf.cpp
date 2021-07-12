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

  auto symbol = context.make_symbol("a");
  auto expeted_atom2 = context.make_atom(symbol);
  REQUIRE(expeted_atom1 == expeted_atom2);
  REQUIRE(*expeted_atom1 == *expeted_atom2);
}

} // namespace Test
} // namespace logic
} // namespace cynthia