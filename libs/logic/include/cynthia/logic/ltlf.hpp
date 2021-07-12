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
#include <cynthia/logic/base.hpp>
#include <cynthia/logic/visitable.hpp>
#include <utility>

namespace cynthia {
namespace logic {

class Symbol : public AstNode {
private:
  const std::string name_;

public:
  const static TypeID type_code_id = TypeID::t_Symbol;
  Symbol(Context& ctx, const std::string& name) : AstNode(ctx), name_{name} {}

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
  const std::string name;
  const static TypeID type_code_id = TypeID::t_LTLfAtom;
  LTLfAtom(Context& ctx, const std::string& name)
      : LTLfFormula(ctx), name{name} {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfUnaryOp : public LTLfFormula {
public:
  const ltlf_ptr arg;
  LTLfUnaryOp(Context& ctx, ltlf_ptr arg)
      : LTLfFormula(ctx), arg{std::move(arg)} {}

  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfNot : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfNot;
  LTLfNot(Context& ctx, ltlf_ptr arg) : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfCommutativeBinaryOp : public LTLfFormula {
public:
  const vec_ptr args;
  bool const (&fun)(bool, bool);

  LTLfCommutativeBinaryOp(Context& ctx, vec_ptr args,
                          bool const (&fun)(bool, bool))
      : LTLfFormula(ctx), args{utils::setify(args)}, fun{fun} {}

  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfAnd : public LTLfCommutativeBinaryOp {
private:
  static inline bool const and_(bool b1, bool b2) { return b1 and b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfAnd;
  LTLfAnd(Context& ctx, vec_ptr args)
      : LTLfCommutativeBinaryOp(ctx, std::move(args), and_) {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfOr : public LTLfCommutativeBinaryOp {
private:
  static inline bool const or_(bool b1, bool b2) { return b1 or b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfOr;
  LTLfOr(Context& ctx, vec_ptr args)
      : LTLfCommutativeBinaryOp(ctx, std::move(args), or_) {}

  void accept(Visitor* visitor) const override;
  inline TypeID get_type_code() const override;
};

} // namespace logic
} // namespace cynthia