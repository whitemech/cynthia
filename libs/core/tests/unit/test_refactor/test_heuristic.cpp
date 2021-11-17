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
#include <cynthia/heuristic_hamming_distance.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("heuristic of 'a U X b'") {
  logic::Context context;
  auto partition = InputOutputPartition({"dummy"}, {"a", "b"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto X_b = context.make_next(b);
  auto a_U_X_b = context.make_until({a, X_b});
  auto problem = Problem(a_U_X_b, partition);
  auto heuristic = HeuristicHammingDistance(problem);
  auto h_value = heuristic.get_h(problem.init_state());
  REQUIRE(h_value == 1);
}



} // namespace Test
} // namespace core
} // namespace cynthia