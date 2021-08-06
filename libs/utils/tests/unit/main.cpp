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

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file
#include <catch.hpp>

#include <cynthia/utils.hpp>

namespace cynthia {
namespace utils {
namespace Test {
TEST_CASE("Make unique", "[utils]") {
  auto a1 = make_unique<int>(1);
  REQUIRE(*a1.get());
}

TEST_CASE("Split string with colon", "[utils][split]") {
  std::string str = "inputs: a b c_d \n";

  auto tokens = split_with_delimiter(str, ":");
  std::vector<std::string> expected_tokens;
  expected_tokens.push_back("inputs");
  expected_tokens.push_back(" a b c_d");

  REQUIRE(tokens == expected_tokens);
}

TEST_CASE("Split string with whitespace", "[utils][split]") {
  std::string str = " a b c_d\n";

  auto tokens = split_with_delimiter(str, " ");
  std::vector<std::string> expected_tokens;
  expected_tokens.push_back("a");
  expected_tokens.push_back("b");
  expected_tokens.push_back("c_d");

  REQUIRE(tokens == expected_tokens);
}

TEST_CASE("Split string with whitespace and colon", "[utils][split]") {
  std::string str = "inputs: a b c_d \n";

  auto tokens = split_with_delimiter(str, ":");
  auto final_tokens = split_with_delimiter(tokens[1], " ");

  std::vector<std::string> expected_tokens;
  expected_tokens.push_back("a");
  expected_tokens.push_back("b");
  expected_tokens.push_back("c_d");

  REQUIRE(final_tokens == expected_tokens);
}

TEST_CASE("insert_sorted", "[utils]") {
  auto v = std::vector<int>{0, 1, 3};
  insert_sorted(v, 2);
  REQUIRE(v.size() == 4);
  REQUIRE(v.at(2) == 2);
}

TEST_CASE("setify vector", "[utils]") {
  std::vector<int> v{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};

  std::vector<int> expected{1, 2, 3, 4, 5};
  auto actual = setify(v);
  REQUIRE(actual == expected);
}

} // namespace Test
} // namespace utils
} // namespace cynthia