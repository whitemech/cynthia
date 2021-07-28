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

#include <cynthia/utils.hpp>
#include <algorithm>

namespace cynthia {
namespace utils {

std::string ltrim(const std::string &s)
{
  const std::string WHITESPACE = " \n\r\t\f\v";
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
  const std::string WHITESPACE = " \n\r\t\f\v";
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

std::vector<std::string> split_with_delimiter(std::string str, std::string delimiter) {
  std::vector<std::string> tokens;
  std::string trimed_str = trim(str);

  size_t start = 0;
  std::string token;

  auto pos = trimed_str.find(delimiter);
  while (pos != std::string::npos) {
    token = trimed_str.substr(start, pos - start);
    tokens.push_back(token);
    start = pos + delimiter.length();
    pos = trimed_str.find(delimiter, start);
  }
  token = trimed_str.substr(start, pos - start);
  tokens.push_back(token);

  return tokens;
}

} // namespace utils
} // namespace cynthia

