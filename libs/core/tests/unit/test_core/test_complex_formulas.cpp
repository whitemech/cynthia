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
#include <cynthia/core.hpp>
#include <cynthia/parser/driver.hpp>
#include <sstream>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("forward synthesis of random formula 1") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring(
      "(((p0) | (G(F(p5)))) & (F(p4))) U  (((p3) & ((~(p1)) | (F(~(p4))))) | "
      "((p1) & (~(p3)) & (G(p4))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p5"}, {"p0", "p1", "p3", "p4"});
  bool result = is_realizable<ForwardSynthesis>(formula, partition);
  REQUIRE(result);
}

} // namespace Test
} // namespace core
} // namespace cynthia