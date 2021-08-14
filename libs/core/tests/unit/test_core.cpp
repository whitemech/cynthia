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

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("forward synthesis of 'tt'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto tt = context.make_tt();
  bool result = is_realizable<ForwardSynthesis>(tt, partition);
  REQUIRE(result);
}

TEST_CASE("forward synthesis of 'ff'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto ff = context.make_ff();
  bool result = is_realizable<ForwardSynthesis>(ff, partition);
  REQUIRE(!result);
}

TEST_CASE("forward synthesis of 'true'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto true_ = context.make_prop_true();
  bool result = is_realizable<ForwardSynthesis>(true_, partition);
  REQUIRE(result);
}

TEST_CASE("forward synthesis of 'false'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto false_ = context.make_prop_false();
  bool result = is_realizable<ForwardSynthesis>(false_, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of atom (controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto a = context.make_atom("a");
  bool result = is_realizable<ForwardSynthesis>(a, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of atom (not controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto b = context.make_atom("b");
  bool result = is_realizable<ForwardSynthesis>(b, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of 'a and b' (only one controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and({a, b});
  bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of 'a and b' (both controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"c"}, {"a", "b"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and({a, b});
  bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of 'a or b' (only one controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or({a, b});
  bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of 'a or b' (no one controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"a", "b"}, {"c"});
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or({a, b});
  bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of 'X[!]a' (controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);
  bool result = is_realizable<ForwardSynthesis>(next_a, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of 'X[!]a' (non controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto b = context.make_atom("b");
  auto next_b = context.make_next(b);
  bool result = is_realizable<ForwardSynthesis>(next_b, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of 'X a' (controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto a = context.make_atom("a");
  auto next_a = context.make_weak_next(a);
  bool result = is_realizable<ForwardSynthesis>(next_a, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of 'X a' (non controllable)") {
  logic::Context context;
  auto partition = InputOutputPartition({"b"}, {"a"});
  auto b = context.make_atom("b");
  auto next_b = context.make_weak_next(b);
  bool result = is_realizable<ForwardSynthesis>(next_b, partition);
  // weak next! empty trace reaches the final state
  REQUIRE(result);
}

} // namespace Test
} // namespace core
} // namespace cynthia