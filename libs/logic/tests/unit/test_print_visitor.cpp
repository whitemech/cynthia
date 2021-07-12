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
#include <cynthia/logic/print_visitor.hpp>

namespace cynthia {
namespace logic {
namespace Test {

TEST_CASE("Print visitor tt", "[logic][print_visitor]") {
  auto context = Context();

  auto formula = context.make_tt();
  auto expected_result = "tt";
  auto actual_result = to_string(*formula);
  REQUIRE(expected_result == actual_result);
}

} // namespace Test
} // namespace logic
} // namespace cynthia