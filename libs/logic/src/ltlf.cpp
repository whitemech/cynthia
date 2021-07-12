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

#include <cynthia/logic/ltlf.hpp>
#include <cynthia/logic/visitor.hpp>
#include <vector>

namespace cynthia {
namespace logic {

void Symbol::accept(Visitor* visitor) const { visitor->visit(*this); };
inline hash_t Symbol::compute_hash_() const {
  hash_t result = get_type_code();
  hash_combine(result, name_);
  return result;
}
inline TypeID Symbol::get_type_code() const { return TypeID::t_Symbol; }
bool Symbol::is_equal(const Comparable& o) const {
  if (is_a<Symbol>(o))
    return name_ == dynamic_cast<const Symbol&>(o).name_;
  return false;
}
int Symbol::compare_(const Comparable& o) const {
  assert(is_a<Symbol>(o));
  const auto& s = dynamic_cast<const Symbol&>(o);
  if (name_ == s.name_)
    return 0;
  return name_ < s.name_ ? -1 : 1;
}

void LTLfTrue::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfTrue::get_type_code() const { return TypeID::t_LTLfTrue; }
inline hash_t LTLfTrue::compute_hash_() const { return type_code_id; }
bool LTLfTrue::is_equal(const Comparable& o) const { return is_a<LTLfTrue>(o); }
int LTLfTrue::compare_(const Comparable& o) const {
  assert(is_a<LTLfTrue>(o));
  return 0;
}

void LTLfFalse::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfFalse::get_type_code() const { return TypeID::t_LTLfFalse; }
inline hash_t LTLfFalse::compute_hash_() const { return type_code_id; }
bool LTLfFalse::is_equal(const Comparable& o) const {
  return is_a<LTLfFalse>(o);
}
int LTLfFalse::compare_(const Comparable& o) const {
  assert(is_a<LTLfFalse>(o));
  return 0;
}

void LTLfAtom::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfAtom::get_type_code() const { return TypeID::t_LTLfAtom; }
inline hash_t LTLfAtom::compute_hash_() const {
  hash_t result = type_code_id;
  hash_combine(result, name);
  return result;
}
bool LTLfAtom::is_equal(const Comparable& o) const {
  return is_a<LTLfAtom>(o) and name == dynamic_cast<const LTLfAtom&>(o).name;
}
int LTLfAtom::compare_(const Comparable& o) const {
  assert(is_a<LTLfAtom>(o));
  auto n1 = this->name;
  auto n2 = dynamic_cast<const LTLfAtom&>(o).name;
  return n1 == n2 ? 0 : n1 < n2 ? -1 : 1;
}

inline hash_t LTLfUnaryOp::compute_hash_() const {
  hash_t result = get_type_code();
  hash_combine(result, arg->hash());
  return result;
}
bool LTLfUnaryOp::is_equal(const Comparable& o) const {
  return get_type_code() == o.get_type_code() and
         arg->is_equal(*dynamic_cast<const LTLfUnaryOp&>(o).arg);
}
int LTLfUnaryOp::compare_(const Comparable& o) const {
  assert(get_type_code() == o.get_type_code());
  return this->arg->compare(*dynamic_cast<const LTLfUnaryOp&>(o).arg);
}

void LTLfNot::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfNot::get_type_code() const { return TypeID::t_LTLfNot; }

inline hash_t LTLfCommutativeBinaryOp::compute_hash_() const {
  hash_t result = get_type_code();
  auto first = args.begin();
  auto last = args.end();
  for (; first < last; ++first) {
    hash_combine(result, **first);
  }
  return result;
}

bool LTLfCommutativeBinaryOp::is_equal(const Comparable& o) const {
  return get_type_code() == o.get_type_code() and
         std::equal(
             args.begin(), args.end(),
             dynamic_cast<const LTLfCommutativeBinaryOp&>(o).args.begin(),
             utils::Deref::Compare());
}
int LTLfCommutativeBinaryOp::compare_(const Comparable& o) const {
  assert(this->get_type_code() == o.get_type_code());
  return utils::ordered_compare(
      this->args, dynamic_cast<const LTLfCommutativeBinaryOp&>(o).args);
}

void LTLfAnd::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfAnd::get_type_code() const { return TypeID::t_LTLfAnd; }

void LTLfOr::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfOr::get_type_code() const { return TypeID::t_LTLfOr; }

} // namespace logic
} // namespace cynthia