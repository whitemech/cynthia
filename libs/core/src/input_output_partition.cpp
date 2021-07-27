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

#include <cynthia/input_output_partition.hpp>
#include <fstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace cynthia {
namespace core {

std::runtime_error
InputOutputPartition::bad_file_format_exception(std::size_t line_number) {
  return std::runtime_error("Incorrect format in line " +
                            std::to_string(line_number) +
                            " of the partition file.");
}

InputOutputPartition::InputOutputPartition() {}

InputOutputPartition
InputOutputPartition::read_from_file(const std::string& filename) {
  InputOutputPartition partition;

  std::ifstream in(filename);

  std::size_t line_number = 1;
  std::string line;
  std::getline(in, line);

  std::vector<std::string> input_substr;
  boost::split(input_substr, line, boost::is_any_of(":"));

  if (input_substr.size() != 2 || input_substr[0] != ".inputs") {
    throw bad_file_format_exception(line_number);
  }

  boost::trim(input_substr[1]); // remove leading and trailing whitespace
  boost::split(partition.input_variables, input_substr[1],
               boost::is_any_of(" "));

  ++line_number;
  std::getline(in, line);

  std::vector<std::string> output_substr;
  boost::split(output_substr, line, boost::is_any_of(":"));

  if (output_substr.size() != 2 || output_substr[0] != ".outputs") {
    throw bad_file_format_exception(line_number);
  }

  boost::trim(output_substr[1]); // remove leading and trailing whitespace
  boost::split(partition.output_variables, output_substr[1],
               boost::is_any_of(" "));

  return partition;
}

} // namespace core
} // namespace cynthia
