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

#include <cynthia/sddcpp.hpp>
#include <stdexcept>

namespace cynthia {
namespace core {

SddNodeWrapper::SddNodeWrapper(SddNode* raw) : raw_{raw} {
  if (raw_ == nullptr) {
    throw std::invalid_argument("null pointer provided");
  }
  if (is_decision()) {
    children_ = sdd_node_elements(raw);
    nb_children_ = sdd_node_size(raw);
  }
  type_ = get_sdd_node_type_();
}

bool SddNodeWrapper::is_true() const { return sdd_node_is_true(raw_); }

bool SddNodeWrapper::is_false() const { return sdd_node_is_false(raw_); }

bool SddNodeWrapper::is_literal() const { return sdd_node_is_literal(raw_); }

bool SddNodeWrapper::is_decision() const { return sdd_node_is_decision(raw_); }

long SddNodeWrapper::node_literal() const {
  if (not is_literal()) {
    throw std::logic_error("cannot call this method if node is not a literal");
  }
  return sdd_node_literal(raw_);
}
bool SddNodeWrapper::at_vtree_root() const {
  return raw_->vtree->parent == nullptr;
}
bool SddNodeWrapper::parent_at_vtree_root() const {
  return raw_->vtree->parent->parent == nullptr;
}

SddNodeChildrenIterator SddNodeWrapper::begin() const {
  return SddNodeChildrenIterator(children_);
}
SddNodeChildrenIterator SddNodeWrapper::end() const {
  if (children_) {
    return SddNodeChildrenIterator(children_ + 2 * nb_children_);
  } else {
    return SddNodeChildrenIterator(children_);
  }
}

long SddNodeWrapper::nb_children() const { return nb_children_; }

SddNodeType SddNodeWrapper::get_sdd_node_type_() const {
  if (!is_decision()) {
    return STATE;
  } else if (!at_vtree_root()) {
    if (!parent_at_vtree_root()) {
      return STATE;
    }
    return ENV_STATE;
  } else {
    // check parent not at vtree root
    if (begin().get_sub()->vtree->parent->parent != nullptr) {
      return SYSTEM_STATE;
    }
    return SYSTEM_ENV_STATE;
  }
}

} // namespace core
} // namespace cynthia
