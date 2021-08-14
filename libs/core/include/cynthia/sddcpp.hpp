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

#include <cstddef>  // For std::ptrdiff_t
#include <iterator> // For std::forward_iterator_tag
#include <tuple>

extern "C" {
#include "sddapi.h"
}

namespace cynthia {
namespace core {

class SddNodeChildrenIterator;
class SddTransitionIterator;

class SddNodeWrapper {
private:
  SddNode* raw_{};
  SddNodeSize nb_children_ = 0;
  SddNode** children_ = nullptr;

public:
  SddNodeWrapper() = default;
  explicit SddNodeWrapper(SddNode* raw);
  inline SddNode* get_raw() const { return raw_; }
  bool is_true() const;
  bool is_false() const;
  bool is_literal() const;
  bool is_decision() const;
  long node_literal() const;
  long nb_children() const;
  SddNodeChildrenIterator begin() const;
  SddNodeChildrenIterator end() const;
  SddTransitionIterator transitions_begin() const;
  SddTransitionIterator transitions_end() const;
};

struct SddNodeChildrenIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = SddNode*;
  using pointer = value_type*;   // or also value_type*
  using reference = value_type&; // or also value_type&

  SddNodeChildrenIterator() = default;
  explicit SddNodeChildrenIterator(SddNode** ptr) : m_ptr_(ptr) {}

  reference operator*() const { return *m_ptr_; }
  pointer operator->() { return m_ptr_; }
  SddNodeChildrenIterator& operator++() {
    m_ptr_ += 2;
    return *this;
  }
  SddNodeChildrenIterator operator++(int) {
    SddNodeChildrenIterator tmp = *this;
    std::advance(*this, 2);
    return tmp;
  }
  friend bool operator==(const SddNodeChildrenIterator& a,
                         const SddNodeChildrenIterator& b) {
    return a.m_ptr_ == b.m_ptr_;
  }
  friend bool operator!=(const SddNodeChildrenIterator& a,
                         const SddNodeChildrenIterator& b) {
    return a.m_ptr_ != b.m_ptr_;
  }

  SddNode* get_prime() const { return *m_ptr_; }
  SddNode* get_sub() const { return *(m_ptr_ + 1); }

private:
  SddNode** m_ptr_;
};

} // namespace core
} // namespace cynthia
