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

enum SddNodeType {
  UNDEFINED = 0,
  STATE = 1,
  ENV = 2,
  ENV_STATE = 3,
  SYSTEM = 4,
  SYSTEM_STATE = 5,
  SYSTEM_ENV = 6,
  SYSTEM_ENV_STATE = 7,
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

class SddNodeWrapper {
private:
  SddSize id_;
  SddNode* raw_{};
  SddNodeSize nb_children_ = 0;
  SddNode** children_ = nullptr;
  SddNodeType type_;
  SddManager* manager_;

  SddNodeType get_sdd_node_type_() const;

public:
  SddNodeWrapper() = default;
  SddNodeWrapper(SddNode* raw, SddManager* manager);
  inline SddNode* get_raw() const { return raw_; }
  inline SddNodeType get_type() const { return type_; }
  inline SddSize get_id() const { return id_; }
  bool is_true() const;
  bool is_false() const;
  bool is_literal() const;
  bool is_decision() const;
  long node_literal() const;
  long nb_children() const;
  bool at_vtree_root() const;
  bool parent_at_vtree_root() const;
  SddNodeChildrenIterator begin() const;
  SddNodeChildrenIterator end() const;
};

} // namespace core
} // namespace cynthia
