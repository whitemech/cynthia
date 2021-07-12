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

#include <cynthia/logic/base.hpp>
#include <cynthia/logic/visitable.hpp>
#include <utility>

namespace cynthia {
namespace logic {

class Symbol : public AstNode {
public:
  const std::string name;
  const static TypeID type_code_id = TypeID::t_Symbol;
  Symbol(Context& ctx, const std::string& name) : AstNode(ctx), name{name} {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfFormula : public AstNode {
public:
  explicit LTLfFormula(Context& c) : AstNode(c) {}
};

class LTLfTrue : public LTLfFormula {
public:
  const static TypeID type_code_id = TypeID::t_LTLfTrue;
  explicit LTLfTrue(Context& ctx) : LTLfFormula(ctx) {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfFalse : public LTLfFormula {
public:
  const static TypeID type_code_id = TypeID::t_LTLfFalse;
  explicit LTLfFalse(Context& ctx) : LTLfFormula(ctx) {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfAtom : public LTLfFormula {
public:
  const symbol_ptr symbol;
  const static TypeID type_code_id = TypeID::t_LTLfAtom;
  LTLfAtom(Context& ctx, const std::string& name)
      : LTLfFormula(ctx), symbol{ctx.make_symbol(name)} {}
  LTLfAtom(Context& ctx, symbol_ptr symbol)
      : LTLfFormula(ctx), symbol{std::move(symbol)} {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfNot : public LTLfFormula {
public:
  const ltlf_ptr arg;
  const static TypeID type_code_id = TypeID::t_LTLfNot;
  LTLfNot(Context& ctx, ltlf_ptr arg) : LTLfFormula(ctx), arg{std::move(arg)} {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

} // namespace logic
} // namespace cynthia