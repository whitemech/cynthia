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
TEST_CASE("forward synthesis of atom") {
  logic::Context context;
  auto a = context.make_atom("a");

  SECTION("atom controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(a, partition);
    REQUIRE(result);
  }
  SECTION("atom not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(a, partition);
    REQUIRE(!result);
  }
}

TEST_CASE("forward synthesis of 'a and b'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and({a, b});

  SECTION("left not-controllable, right not-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left not-controllable, right controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left controllable, right not-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left controllable, right controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = is_realizable<ForwardSynthesis>(a_and_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'a or b'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or({a, b});
  SECTION("left not-controllable, right not-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
    REQUIRE(!result);
  }
  SECTION("left not-controllable, right controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
    REQUIRE(result);
  }
  SECTION("left controllable, right not-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
    REQUIRE(result);
  }
  SECTION("left controllable, right controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = is_realizable<ForwardSynthesis>(a_or_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'X[!]a' (controllable)") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);

  SECTION("body controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(next_a, partition);
    REQUIRE(result);
  }
  SECTION("body not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(next_a, partition);
    REQUIRE(!result);
  }
}

TEST_CASE("forward synthesis of 'X a' (controllable)") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto weak_next_a = context.make_weak_next(a);

  SECTION("body controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(weak_next_a, partition);
    REQUIRE(result);
  }
  SECTION("body not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(weak_next_a, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of 'a U b'") {
  logic::Context context;
  auto b = context.make_atom("b");
  auto a = context.make_atom("a");
  auto a_until_b = context.make_until({a, b});

  SECTION("head non-controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = is_realizable<ForwardSynthesis>(a_until_b, partition);
    REQUIRE(!result);
  }
  SECTION("head non-controllable, tail controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(a_until_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(a_until_b, partition);
    REQUIRE(!result);
  }
  SECTION("head controllable, tail controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = is_realizable<ForwardSynthesis>(a_until_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'a R b'") {
  // NOTE: safety formula, always realizable with empty traces
  logic::Context context;
  auto b = context.make_atom("b");
  auto a = context.make_atom("a");
  auto a_release_b = context.make_release({a, b});

  SECTION("head non-controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = is_realizable<ForwardSynthesis>(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head non-controllable, tail controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = is_realizable<ForwardSynthesis>(a_release_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'F a'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto eventually_a = context.make_eventually(a);

  SECTION("tail not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(eventually_a, partition);
    REQUIRE(!result);
  }
  SECTION("tail controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(eventually_a, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'G a'") {
  // NOTE: safety formula, always realizable with empty traces
  logic::Context context;
  auto a = context.make_atom("a");
  auto always_a = context.make_always(a);

  SECTION("tail not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = is_realizable<ForwardSynthesis>(always_a, partition);
    REQUIRE(result);
  }
  SECTION("tail controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = is_realizable<ForwardSynthesis>(always_a, partition);
    REQUIRE(result);
  }
}

} // namespace Test
} // namespace core
} // namespace cynthia