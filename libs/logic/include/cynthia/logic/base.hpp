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
#include <cynthia/logic/visitable.hpp>
#include <cynthia/utils.hpp>
#include <memory>
#include <utility>

namespace cynthia {
namespace logic {

class Context;

class AstNode : public Visitable,
                public Hashable,
                public Comparable,
                public std::enable_shared_from_this<const LTLfFormula> {
private:
  Context* m_ctx_;

public:
  explicit AstNode(Context& ctx) : m_ctx_{&ctx} {}
  Context& ctx() const { return *m_ctx_; }
  friend void check_context(AstNode const& a, AstNode const& b) {
    assert(a.m_ctx_ == b.m_ctx_);
  };
};

class Context {
private:
  std::unique_ptr<HashTable> table_;

  ltlf_ptr tt;
  ltlf_ptr ff;
  ltlf_ptr true_;
  ltlf_ptr false_;
  ltlf_ptr end;
  ltlf_ptr not_end;
  ltlf_ptr last;

public:
  Context();
  ltlf_ptr make_tt();
  ltlf_ptr make_ff();
  ltlf_ptr make_prop_true();
  ltlf_ptr make_prop_false();
  ltlf_ptr make_end();
  ltlf_ptr make_not_end();
  ltlf_ptr make_last();
  ltlf_ptr make_bool(bool value);
  ltlf_ptr make_atom(const std::string& name);
  ltlf_ptr make_not(const ltlf_ptr& arg);
  ltlf_ptr make_prop_not(const ltlf_ptr& arg);
  ltlf_ptr make_and(const vec_ptr& args);
  ltlf_ptr make_or(const vec_ptr& arg);
  ltlf_ptr make_implies(const vec_ptr& arg);
  ltlf_ptr make_equivalent(const vec_ptr& arg);
  ltlf_ptr make_xor(const vec_ptr& arg);
  ltlf_ptr make_next(const ltlf_ptr& arg);
  ltlf_ptr make_weak_next(const ltlf_ptr& arg);
  ltlf_ptr make_until(const vec_ptr& args);
  ltlf_ptr make_release(const vec_ptr& args);
  ltlf_ptr make_eventually(const ltlf_ptr& args);
  ltlf_ptr make_always(const ltlf_ptr& args);
};

template <typename T, typename caller, typename True, typename False,
          typename Not, typename And, typename Or>
std::shared_ptr<T>
and_or(Context& context, const vec_ptr& s, bool op_x_notx,
       std::shared_ptr<T> (Context::*const& fun_ptr)(bool x)) {
  std::set<std::shared_ptr<T>, utils::Deref::Less> args;
  for (auto& a : s) {
    // handle the case when a subformula is true
    if (is_a<True>(*a)) {
      if (op_x_notx)
        return a;
      else
        continue;
    }
    // handle the case when a subformula is false
    else if (is_a<False>(*a)) {
      if (!op_x_notx)
        return a;
      else
        continue;
    }
    // handle the case when a subformula is of the same type of the caller
    else if (is_a<caller>(*a)) {
      const auto& to_insert = dynamic_cast<const caller&>(*a);
      const auto& container = to_insert.args;
      args.insert(container.begin(), container.end());
      continue;
    } else {
      args.insert(a);
    }
  }
  vec_ptr final_args =
      utils::setify<ltlf_ptr, utils::Deref::Equal, utils::Deref::Less>(
          vec_ptr(args.begin(), args.end()));
  if (final_args.size() == 1)
    return *(args.begin());
  if (final_args.empty())
    return (context.*fun_ptr)(not op_x_notx);
  return std::make_shared<caller>(context, final_args);
}

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