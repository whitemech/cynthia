#pragma once
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

#include <stdexcept>
#include <vector>

namespace cynthia {
namespace core {
/**
 * \brief A partition of variables into input and output variables.
 */
class InputOutputPartition {
private:
  static std::runtime_error bad_file_format_exception(std::size_t line_number);

public:
  std::vector<std::string> input_variables;
  std::vector<std::string> output_variables;

  /**
   * \brief Creates a partition with no variables.
   */
  InputOutputPartition();

  /**
   * \brief Constructs a partition from a file.
   *
   * The file should look like
   *   .inputs: X1 X2 X3 X4
   *   .outputs: Y1 Y2 Y3
   *
   * \param filename The name of the partition file.
   * \return A partition with the input and output variables listed in the file
   */
  static InputOutputPartition read_from_file(const std::string& filename);
};

} // namespace core
} // namespace cynthia
