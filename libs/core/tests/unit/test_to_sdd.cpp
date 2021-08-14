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
#include <cynthia/logic/nnf.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("Test SDD of tt", "[core][SDD]") {
  auto logic_context = logic::Context();
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto tt = logic_context.make_tt();
  auto context = ForwardSynthesis::Context(tt, partition);
  auto sdd = to_sdd(*tt, context);
  REQUIRE(sdd_size(sdd) == 0);
}

TEST_CASE("Test SDD of ff", "[core][SDD]") {
  auto logic_context = logic::Context();
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto ff = logic_context.make_ff();
  auto context = ForwardSynthesis::Context(ff, partition);
  auto sdd = to_sdd(*ff, context);
  REQUIRE(sdd_size(sdd) == 0);
}

TEST_CASE("Test SDD of true", "[core][SDD]") {
  auto logic_context = logic::Context();
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto true_ = logic_context.make_prop_true();
  auto context = ForwardSynthesis::Context(true_, partition);
  auto sdd = to_sdd(*true_, context);
  REQUIRE(sdd_size(sdd) == 0);
}

TEST_CASE("Test SDD of false", "[core][SDD]") {
  auto logic_context = logic::Context();
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto false_ = logic_context.make_prop_true();
  auto context = ForwardSynthesis::Context(false_, partition);
  auto sdd = to_sdd(*false_, context);
  REQUIRE(sdd_size(sdd) == 0);
}
TEST_CASE("Test SDD of a", "[core][SDD]") {
  auto logic_context = logic::Context();
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto a = logic_context.make_atom("a");
  auto context = ForwardSynthesis::Context(a, partition);
  auto sdd = to_sdd(*a, context);
  REQUIRE(sdd_size(sdd) == 2);
  //  sdd_save_as_dot("a_sdd.dot", sdd);
  //  sdd_vtree_save_as_dot("a_vtree.dot", context.vtree_);
}

} // namespace Test
} // namespace core
} // namespace cynthia