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
  hash_combine(result, symbol->hash());
  return result;
}
bool LTLfAtom::is_equal(const Comparable& o) const {
  return is_a<LTLfAtom>(o) and
         symbol->is_equal(*dynamic_cast<const LTLfAtom&>(o).symbol);
}
int LTLfAtom::compare_(const Comparable& o) const {
  assert(is_a<LTLfAtom>(o));
  return this->symbol->compare(*dynamic_cast<const LTLfAtom&>(o).symbol);
}

void LTLfNot::accept(Visitor* visitor) const { visitor->visit(*this); };
inline TypeID LTLfNot::get_type_code() const { return TypeID::t_LTLfNot; }
inline hash_t LTLfNot::compute_hash_() const {
  hash_t result = type_code_id;
  hash_combine(result, arg->hash());
  return result;
}
bool LTLfNot::is_equal(const Comparable& o) const {
  return is_a<LTLfNot>(o) and
         arg->is_equal(*dynamic_cast<const LTLfNot&>(o).arg);
}
int LTLfNot::compare_(const Comparable& o) const {
  assert(is_a<LTLfNot>(o));
  return this->arg->compare(*dynamic_cast<const LTLfNot&>(o).arg);
}

} // namespace logic
} // namespace cynthia