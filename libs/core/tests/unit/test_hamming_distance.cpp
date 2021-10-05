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
#include <cynthia/hamming_distance.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("Test hamming_distance of tt", "[core][SDD]") {
auto context = logic::Context();

auto tt = context.make_tt();
REQUIRE(hamming_distance(*tt) == 0);
}

TEST_CASE("Test hamming_distance of ff", "[core][SDD]") {
auto context = logic::Context();
auto ff = context.make_ff();
REQUIRE(hamming_distance(*ff) == 1);
}

TEST_CASE("Test hamming_distance of true", "[core][SDD]") {
auto context = logic::Context();

auto true_ = context.make_prop_true();
REQUIRE(hamming_distance(*true_) == 1);
}

TEST_CASE("Test hamming_distance of false", "[core][SDD]") {
auto context = logic::Context();

auto false_ = context.make_prop_false();
REQUIRE(hamming_distance(*false_) == 1);
}

TEST_CASE("Test hamming_distance of a", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
REQUIRE(hamming_distance(*a) == 1);
}

TEST_CASE("Test hamming_distance of !a", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
auto not_a = context.make_prop_not(a);
REQUIRE(hamming_distance(*not_a) == 1);
}

TEST_CASE("Test hamming_distance of a & b", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
auto b = context.make_atom("b");
auto a_and_b = context.make_and(logic::vec_ptr{a, b});
REQUIRE(hamming_distance(*a_and_b) == 2);
}

TEST_CASE("Test hamming_distance of a | b", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
auto b = context.make_atom("b");
auto a_or_b = context.make_or(logic::vec_ptr{a, b});
REQUIRE(hamming_distance(*a_or_b) == 1);
}

TEST_CASE("Test hamming_distance of X[!]a", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
auto next_a = context.make_next(a);
REQUIRE(hamming_distance(*next_a) == 1);
}

TEST_CASE("Test hamming_distance of X a", "[core][SDD]") {
auto context = logic::Context();

auto a = context.make_atom("a");
auto weak_next_a = context.make_weak_next(a);
REQUIRE(hamming_distance(*weak_next_a) == 0);
}

} // namespace Test
} // namespace core
} // namespace cynthia