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
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace cynthia {
namespace utils {

static inline bool and_(bool b1, bool b2) { return b1 and b2; }
static inline bool or_(bool b1, bool b2) { return b1 and b2; }
static inline bool implies_(bool b1, bool b2) { return not b1 or b2; }
static inline bool equivalent_(bool b1, bool b2) {
  return (b1 and b2) or (not b1 and not b2);
}
static inline bool xor_(bool b1, bool b2) { return b1 xor b2; }

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

  struct Equal {
    template <typename T>
    size_t operator()(std::shared_ptr<const T> const& a,
                      std::shared_ptr<const T> const& b) const {
      return *a == *b;
    }
  };
  struct Less {
    template <typename T>
    size_t operator()(std::shared_ptr<const T> const& a,
                      std::shared_ptr<const T> const& b) const {
      return *a < *b;
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

template <typename T, typename Equal = std::equal_to<T>,
          typename Less = std::less<T>>
std::vector<T> setify(std::vector<T> vec) {
  if (!std::is_sorted(vec.begin(), vec.end(), Less())) {
    std::sort(vec.begin(), vec.end());
  }
  auto last = std::unique(vec.begin(), vec.end(), Equal());
  vec.erase(last, vec.end());
  return vec;
}

template <typename T, typename C>
typename std::vector<T> vectify(std::set<T, C> set) {
  return std::vector<T>(set.begin(), set.end());
}

template <typename T, typename C>
std::vector<T>
from_index_map_to_vector(const std::map<T, size_t, C>& from_element_to_id) {
  auto result = std::vector<T>(from_element_to_id.size());
  for (const auto& pair : from_element_to_id) {
    result[pair.second] = pair.first;
  }
  return result;
}

template <typename T, typename Comparator>
int binary_search_find_index(std::vector<T> v, T data, Comparator compare) {
  auto it = std::lower_bound(v.begin(), v.end(), data, compare);
  if (it == v.end() || *it != data) {
    return -1;
  } else {
    int index = std::distance(v.begin(), it);
    return index;
  }
}

template <typename T> typename std::vector<T> sort(std::vector<T> vec) {
  if (!std::is_sorted(vec.begin(), vec.end())) {
    std::sort(vec.begin(), vec.end());
  }
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

template <class InputIterator1, class InputIterator2>
bool empty_intersection(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2) {
  auto i = first1;
  auto j = first2;
  while (i != last1 && j != last2) {
    if (*i < *j)
      ++i;
    else if (*j < *i)
      ++j;
    else
      return false;
  }
  return true;
}
template <class InputIterator1, class InputIterator2>
bool empty_difference(InputIterator1 first1, InputIterator1 last1,
                      InputIterator2 first2, InputIterator2 last2) {
  auto i = first1;
  auto j = first2;
  while (i != last1 && j != last2) {
    if (*i < *j)
      ++i;
    else if (*j < *i)
      return false;
    else
      ++j;
  }
  return true;
}

} // namespace utils
} // namespace cynthia