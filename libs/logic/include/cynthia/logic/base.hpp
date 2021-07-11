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

#include <cassert>
#include <cynthia/logic/comparable.hpp>
#include <cynthia/logic/hashable.hpp>
#include <cynthia/logic/hashtable.hpp>
#include <cynthia/logic/visitor.hpp>
#include <cynthia/utils.hpp>
#include <memory>
#include <utility>

namespace cynthia {
namespace logic {

class Context;
typedef std::shared_ptr<Context> context_ptr;

class AstNode : public Visitable, public Hashable, public Comparable {
private:
  const context_ptr m_ctx_;

public:
  explicit AstNode(context_ptr ctx) : m_ctx_{std::move(ctx)} {}
  Context& ctx() const { return *m_ctx_; }
  friend void check_context(AstNode const& a, AstNode const& b) {
    assert(a.m_ctx_ == b.m_ctx_);
  };
};

typedef std::shared_ptr<const AstNode> ast_ptr;

class Context {
private:
  std::unique_ptr<BaseHashTable<AstNode>> table_;

public:
  Context() { table_ = utils::make_unique<HashTable<AstNode>>(); };
};

} // namespace logic
} // namespace cynthia

// injected in namespace std custom specialization of std::hash for AstNode
namespace std {
template <> struct hash<cynthia::logic::AstNode> {
  std::size_t operator()(cynthia::logic::AstNode const& n) const noexcept {
    return n.hash();
  }
};
} // namespace std