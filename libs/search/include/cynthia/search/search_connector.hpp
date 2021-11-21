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

#include <cynthia/comparable.hpp>
#include <cynthia/hashable.hpp>
#include <cynthia/search/search_node.hpp>
#include <cynthia/utils.hpp>

namespace cynthia {
namespace search {

class SearchConnector : public utils::Hashable, public utils::SimpleComparable {
public:
  std::shared_ptr<SearchNode> parent;
  utils::deref_set<std::shared_ptr<SearchNode>> children;
  std::shared_ptr<Operator> operator_;

  SearchConnector(std::shared_ptr<SearchNode> parent,
                  utils::deref_set<std::shared_ptr<SearchNode>> children,
                  std::shared_ptr<Operator> operator_)
      : parent{parent}, children{children}, operator_{operator_} {}

  hash_t compute_hash_() const override {
    // TODO
    throw std::logic_error("not implemented");
  }

  bool is_equal(const utils::SimpleComparable& o) const override {
    // TODO
    throw std::logic_error("not implemented");
  }

  int compare_(const utils::SimpleComparable& o) const override {
    // TODO
    throw std::logic_error("not implemented");
  }
};

} // namespace search
} // namespace cynthia