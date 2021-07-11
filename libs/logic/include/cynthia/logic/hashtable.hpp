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

#include <unordered_set>

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
 * An abstract hash table class.
 */
template <typename T> class BaseHashTable {
public:
  virtual std::shared_ptr<const T>
  insert_if_not_available(const std::shared_ptr<const T>& ptr) = 0;
  virtual size_t size() = 0;
};

/*
 * A concrete hash table based on STL unordered_set.
 */
template <typename T> class HashTable : BaseHashTable<T> {
private:
  std::unordered_set<std::shared_ptr<const T>, Deref::Hash, Deref::Compare>
      m_table_;

public:
  explicit HashTable() : BaseHashTable<T>() {
    m_table_ = std::unordered_set<std::shared_ptr<const T>, Deref::Hash,
                                  Deref::Compare>{};
  }

  std::shared_ptr<const T>
  insert_if_not_available(const std::shared_ptr<const T>& ptr) {
    auto it = m_table_.find(ptr);
    if (it == m_table_.end()) {
      m_table_.insert(ptr);
      return ptr;
    } else {
      return *it;
    }
  }

  size_t size() { return m_table_.size(); }
};
