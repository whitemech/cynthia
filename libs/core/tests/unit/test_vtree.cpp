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
#include <cynthia/vtree.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("Test VTree", "[core][SDD]") {
  logic::Context context;

  auto inputs = std::vector<std::string>({"a", "b"});
  auto outputs = std::vector<std::string>({"c", "d"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto c = context.make_atom("c");
  auto d = context.make_atom("d");
  auto ab = context.make_and({a, b});
  auto cd = context.make_and({c, d});
  auto ab_until_cd = context.make_until({ab, cd});
  auto partition = InputOutputPartition(inputs, outputs);

  auto formula_closure = closure(*ab_until_cd);
  auto builder = VTreeBuilder(formula_closure, partition);
  auto vtree = builder.get_vtree();
  REQUIRE(vtree->var_count == 17);
  sdd_vtree_free(vtree);
}
} // namespace Test
} // namespace core
} // namespace cynthia