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

inline bool is_propositional(const ltlf_ptr& arg);

class Symbol : public AstNode {
private:
  const std::string name_;

public:
  const static TypeID type_code_id = TypeID::t_Symbol;
  Symbol(Context& ctx, const std::string& name) : AstNode(ctx), name_{name} {}

  void accept(Visitor& visitor) const override;
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

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfFalse : public LTLfFormula {
public:
  const static TypeID type_code_id = TypeID::t_LTLfFalse;
  explicit LTLfFalse(Context& ctx) : LTLfFormula(ctx) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfPropTrue : public LTLfFormula {
public:
  const static TypeID type_code_id = TypeID::t_LTLfPropTrue;
  explicit LTLfPropTrue(Context& ctx) : LTLfFormula(ctx) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfPropFalse : public LTLfFormula {
public:
  const static TypeID type_code_id = TypeID::t_LTLfPropFalse;
  explicit LTLfPropFalse(Context& ctx) : LTLfFormula(ctx) {}

  void accept(Visitor& visitor) const override;
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

  void accept(Visitor& visitor) const override;
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

class LTLfPropositionalNot : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfPropNot;
  LTLfPropositionalNot(Context& ctx, ltlf_ptr arg)
      : LTLfUnaryOp(ctx, std::move(arg)) {
    if (!is_propositional(this->arg))
      throw std::invalid_argument(
          "PropositionalNot only accepts LTLfAtom as arguments.");
  }

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfNot : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfNot;
  LTLfNot(Context& ctx, ltlf_ptr arg) : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class BooleanBinaryOp {
public:
  bool const (&fun)(bool, bool);

  BooleanBinaryOp(bool const (&fun)(bool, bool)) : fun{fun} {}
};

class LTLfBinaryOp : public LTLfFormula {
public:
  const vec_ptr args;

  LTLfBinaryOp(Context& ctx, vec_ptr args) : LTLfFormula(ctx), args{args} {}

  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

class LTLfCommutativeBinaryOp : public LTLfBinaryOp {
public:
  LTLfCommutativeBinaryOp(Context& ctx, vec_ptr args)
      : LTLfBinaryOp(
            ctx,
            utils::setify<ltlf_ptr, utils::Deref::Equal, utils::Deref::Less>(
                args)) {}
};

class LTLfAnd : public LTLfCommutativeBinaryOp, public BooleanBinaryOp {
private:
  static inline bool const and_(bool b1, bool b2) { return b1 and b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfAnd;
  LTLfAnd(Context& ctx, vec_ptr args)
      : LTLfCommutativeBinaryOp(ctx, std::move(args)), BooleanBinaryOp(and_) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfOr : public LTLfCommutativeBinaryOp, public BooleanBinaryOp {
private:
  static inline bool const or_(bool b1, bool b2) { return b1 or b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfOr;
  LTLfOr(Context& ctx, vec_ptr args)
      : LTLfCommutativeBinaryOp(ctx, std::move(args)), BooleanBinaryOp(or_) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfImplies : public LTLfBinaryOp, public BooleanBinaryOp {
private:
  static inline bool const implies_(bool b1, bool b2) { return not b1 or b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfImplies;
  LTLfImplies(Context& ctx, vec_ptr args)
      : LTLfBinaryOp(ctx, std::move(args)), BooleanBinaryOp(implies_) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfEquivalent : public LTLfBinaryOp, public BooleanBinaryOp {
private:
  static inline bool const equivalent_(bool b1, bool b2) {
    return (b1 and b2) or (not b1 and not b2);
  }

public:
  const static TypeID type_code_id = TypeID::t_LTLfEquivalent;
  LTLfEquivalent(Context& ctx, vec_ptr args)
      : LTLfBinaryOp(ctx, utils::sort(std::move(args))),
        BooleanBinaryOp(equivalent_) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfXor : public LTLfBinaryOp, public BooleanBinaryOp {
private:
  static inline bool const xor_(bool b1, bool b2) { return b1 xor b2; }

public:
  const static TypeID type_code_id = TypeID::t_LTLfXor;
  LTLfXor(Context& ctx, vec_ptr args)
      : LTLfBinaryOp(ctx, utils::sort(std::move(args))), BooleanBinaryOp(xor_) {
  }

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfNext : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfNext;
  LTLfNext(Context& ctx, ltlf_ptr arg) : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfWeakNext : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfWeakNext;
  LTLfWeakNext(Context& ctx, ltlf_ptr arg) : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfUntil : public LTLfBinaryOp {

public:
  const static TypeID type_code_id = TypeID::t_LTLfUntil;
  LTLfUntil(Context& ctx, vec_ptr args) : LTLfBinaryOp(ctx, std::move(args)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfRelease : public LTLfBinaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfRelease;
  LTLfRelease(Context& ctx, vec_ptr args)
      : LTLfBinaryOp(ctx, std::move(args)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfEventually : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfEventually;
  LTLfEventually(Context& ctx, ltlf_ptr arg)
      : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

class LTLfAlways : public LTLfUnaryOp {
public:
  const static TypeID type_code_id = TypeID::t_LTLfAlways;
  LTLfAlways(Context& ctx, ltlf_ptr arg) : LTLfUnaryOp(ctx, std::move(arg)) {}

  void accept(Visitor& visitor) const override;
  inline TypeID get_type_code() const override;
};

inline bool is_propositional(const ltlf_ptr& arg) {
  return is_a<LTLfAtom>(*arg) || is_a<LTLfPropTrue>(*arg) ||
         is_a<LTLfPropFalse>(*arg);
}

inline hash_t Symbol::compute_hash_() const {
  hash_t result = get_type_code();
  hash_combine(result, name_);
  return result;
}
inline hash_t LTLfTrue::compute_hash_() const { return type_code_id; }
inline hash_t LTLfFalse::compute_hash_() const { return type_code_id; }
inline hash_t LTLfPropTrue::compute_hash_() const { return type_code_id; }
inline hash_t LTLfPropFalse::compute_hash_() const { return type_code_id; }
inline hash_t LTLfAtom::compute_hash_() const {
  hash_t result = type_code_id;
  hash_combine(result, name);
  return result;
}
inline hash_t LTLfUnaryOp::compute_hash_() const {
  hash_t result = get_type_code();
  hash_combine(result, arg->hash());
  return result;
}

inline hash_t LTLfBinaryOp::compute_hash_() const {
  hash_t result = get_type_code();
  auto first = args.begin();
  auto last = args.end();
  for (; first < last; ++first) {
    hash_combine(result, **first);
  }
  return result;
}

inline TypeID Symbol::get_type_code() const { return TypeID::t_Symbol; }
inline TypeID LTLfTrue::get_type_code() const { return TypeID::t_LTLfTrue; }
inline TypeID LTLfFalse::get_type_code() const { return TypeID::t_LTLfFalse; }
inline TypeID LTLfPropTrue::get_type_code() const {
  return TypeID::t_LTLfPropTrue;
}
inline TypeID LTLfPropFalse::get_type_code() const {
  return TypeID::t_LTLfPropFalse;
}
inline TypeID LTLfAtom::get_type_code() const { return TypeID::t_LTLfAtom; }
inline TypeID LTLfNot::get_type_code() const { return TypeID::t_LTLfNot; }
inline TypeID LTLfPropositionalNot::get_type_code() const {
  return TypeID::t_LTLfPropNot;
}
inline TypeID LTLfAnd::get_type_code() const { return TypeID::t_LTLfAnd; }
inline TypeID LTLfOr::get_type_code() const { return TypeID::t_LTLfOr; }
inline TypeID LTLfImplies::get_type_code() const {
  return TypeID::t_LTLfImplies;
}
inline TypeID LTLfEquivalent::get_type_code() const {
  return TypeID::t_LTLfEquivalent;
}
inline TypeID LTLfXor::get_type_code() const { return TypeID::t_LTLfXor; }
inline TypeID LTLfNext::get_type_code() const { return TypeID::t_LTLfNext; }
inline TypeID LTLfWeakNext::get_type_code() const { return TypeID::t_LTLfNext; }
inline TypeID LTLfUntil::get_type_code() const { return TypeID::t_LTLfUntil; }
inline TypeID LTLfRelease::get_type_code() const {
  return TypeID::t_LTLfRelease;
}
inline TypeID LTLfEventually::get_type_code() const {
  return TypeID::t_LTLfEventually;
}
inline TypeID LTLfAlways::get_type_code() const { return TypeID::t_LTLfAlways; }

} // namespace logic
} // namespace cynthia