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
#include <cynthia/input_output_partition.hpp>

namespace cynthia {
namespace core {
namespace Test {

TEST_CASE("IOPartition", "[iopartition]") {
  auto partition =
      InputOutputPartition::read_from_file("../libs/core/examples/partfile");

  std::vector<std::string> inputs, outputs;
  inputs.push_back("a");
  inputs.push_back("b");

  outputs.push_back("c");
  outputs.push_back("d");

  // same inputs and same outputs
  REQUIRE(inputs == partition.input_variables);
  REQUIRE(outputs == partition.output_variables);
}
} // namespace Test
} // namespace core
} // namespace cynthia