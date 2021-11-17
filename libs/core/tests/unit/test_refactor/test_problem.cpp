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
#include <cynthia/search_dfs.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("forward synthesis of 'a & b'") {
  logic::Context context;
  auto partition = InputOutputPartition({"dummy"}, {"a", "b"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and({a, b});
  auto problem = Problem(a_and_b, partition);

  SECTION("problem construction") {
    auto init_formula = problem.init_state()->get_formula();
    REQUIRE(init_formula == a_and_b);
  }

  SECTION("state instantiation") {
    auto state = problem.init_state();
    REQUIRE(!state->is_instantiated());
  }
}

} // namespace Test
} // namespace core
} // namespace cynthia