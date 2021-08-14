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
#include <cynthia/utils.hpp>
#include <fstream>
#include <unistd.h>
#include <utility>

namespace cynthia {
namespace core {

std::runtime_error
InputOutputPartition::bad_file_format_exception(std::size_t line_number) {
  return std::runtime_error("Incorrect format in line " +
                            std::to_string(line_number) +
                            " of the partition file.");
}

InputOutputPartition::InputOutputPartition(
    const std::vector<std::string>& input_variables,
    const std::vector<std::string>& output_variables)
    : input_variables{input_variables}, output_variables{output_variables} {
  if (input_variables.empty() or output_variables.empty()) {
    throw std::invalid_argument("inputs/outputs set cannot be empty");
  }
  build_from_var_to_type_map_();
}

InputOutputPartition
InputOutputPartition::read_from_file(const std::string& filename) {

  std::ifstream in(filename);

  std::size_t line_number = 1;
  std::string line;
  std::getline(in, line);

  std::vector<std::string> input_substr;
  input_substr = utils::split_with_delimiter(line, ":");

  if (input_substr.size() != 2 || input_substr[0] != ".inputs") {
    throw bad_file_format_exception(line_number);
  }
  const auto& input_variables =
      utils::split_with_delimiter(input_substr[1], " ");

  ++line_number;
  std::getline(in, line);

  std::vector<std::string> output_substr;
  output_substr = utils::split_with_delimiter(line, ":");

  if (output_substr.size() != 2 || output_substr[0] != ".outputs") {
    throw bad_file_format_exception(line_number);
  }

  const auto& output_variables =
      utils::split_with_delimiter(output_substr[1], " ");

  return {input_variables, output_variables};
}

void InputOutputPartition::build_from_var_to_type_map_() {
  for (const auto& var : input_variables) {
    from_var_to_type[var] = true;
  }
  for (const auto& var : output_variables) {
    from_var_to_type[var] = false;
  }
}

} // namespace core
} // namespace cynthia
