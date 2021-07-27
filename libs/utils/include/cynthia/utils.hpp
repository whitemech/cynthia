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

#include <algorithm>
#include <memory>
#include <vector>

namespace cynthia {
namespace utils {

/*
 * Dereferenced versions of hashing and comparison.
 */
struct Deref {
  struct Hash {
    template <typename T>
    std::size_t operator()(std::shared_ptr<const T> const& p) const {
      return std::hash<T>()(*p);
    }
  };

  struct Compare {
    template <typename T>
    size_t operator()(std::shared_ptr<const T> const& a,
                      std::shared_ptr<const T> const& b) const {
      return *a == *b;
    }
  };
};

/*
 * make_unique function, like the one in the standards >C++14.
 * In C++11, that is not included.
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/*
 * Insert an element in a sorted vector.
 */
template <typename T>
typename std::vector<T>::iterator insert_sorted(std::vector<T>& vec,
                                                T const& item) {
  return vec.insert(std::upper_bound(vec.begin(), vec.end(), item), item);
}

/*
 * Insert an element in a sorted vector by a specific predicate.
 */
template <typename T, typename Pred>
typename std::vector<T>::iterator insert_sorted(std::vector<T>& vec,
                                                T const& item, Pred pred) {
  return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, pred), item);
}

template <typename T> typename std::vector<T> setify(std::vector<T> vec) {
  if (!std::is_sorted(vec.begin(), vec.end())) {
    std::sort(vec.begin(), vec.end());
  }
  auto last = std::unique(vec.begin(), vec.end());
  vec.erase(last, vec.end());
  return vec;
}

template <class T> inline int ordered_compare(const T& A, const T& B) {
  // Can't be equal if # of entries differ:
  if (A.size() != B.size())
    return A.size() < B.size() ? -1 : 1;

  // Loop over elements in "a" and "b":
  auto a = A.begin();
  auto b = B.begin();
  for (; a != A.end(); ++a, ++b) {
    auto eq = *a == *b;
    if (!eq) {
      return *a < *b ? -1 : 1;
    }
  }
  return 0;
}

} // namespace utils
} // namespace cynthia