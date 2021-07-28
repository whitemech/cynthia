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

#include <memory>
#include <string>
#include <vector>

namespace cynthia {
namespace utils {

/*
 * make_unique function, like the one in the standards >C++14.
 * In C++11, that is not included.
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * ltrim function, like the one in the boost API function boost::ltrim().
 * \brief remove leading whitespace
 */
std::string ltrim(const std::string& s);

/**
 * rtrim function, like the one in the boost API function boost::rtrim().
 * \brief remove trailing whitespace
 */
std::string rtrim(const std::string& s);

/**
 * trim function, like the one in the boost API function boost::trim().
 * \brief remove leading and trailing whitespace
 */
std::string trim(const std::string& s);

/**
 * split_with_delimiter function, like the one in the boost API function
 * boost::split().
 *
 * \param str The string that will be split.
 * \param delimiter The delimiter used to follow for splitting.
 * \return A vector of substrs after split.
 */
std::vector<std::string> split_with_delimiter(std::string str,
                                              std::string delimiter);

} // namespace utils
} // namespace cynthia