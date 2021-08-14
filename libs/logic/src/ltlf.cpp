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

bool LTLfTrue::is_equal(const Comparable& o) const { return is_a<LTLfTrue>(o); }
int LTLfTrue::compare_(const Comparable& o) const {
  assert(is_a<LTLfTrue>(o));
  return 0;
}

bool LTLfFalse::is_equal(const Comparable& o) const {
  return is_a<LTLfFalse>(o);
}
int LTLfFalse::compare_(const Comparable& o) const {
  assert(is_a<LTLfFalse>(o));
  return 0;
}

bool LTLfPropTrue::is_equal(const Comparable& o) const {
  return is_a<LTLfPropTrue>(o);
}
int LTLfPropTrue::compare_(const Comparable& o) const {
  assert(is_a<LTLfPropTrue>(o));
  return 0;
}

bool LTLfPropFalse::is_equal(const Comparable& o) const {
  return is_a<LTLfPropFalse>(o);
}
int LTLfPropFalse::compare_(const Comparable& o) const {
  assert(is_a<LTLfPropFalse>(o));
  return 0;
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

bool LTLfUnaryOp::is_equal(const Comparable& o) const {
  return get_type_code() == o.get_type_code() and
         arg->is_equal(*dynamic_cast<const LTLfUnaryOp&>(o).arg);
}
int LTLfUnaryOp::compare_(const Comparable& o) const {
  assert(get_type_code() == o.get_type_code());
  return this->arg->compare(*dynamic_cast<const LTLfUnaryOp&>(o).arg);
}

bool LTLfBinaryOp::is_equal(const Comparable& o) const {
  return get_type_code() == o.get_type_code() and
         std::equal(args.begin(), args.end(),
                    dynamic_cast<const LTLfBinaryOp&>(o).args.begin(),
                    utils::Deref::Equal());
}
int LTLfBinaryOp::compare_(const Comparable& o) const {
  assert(this->get_type_code() == o.get_type_code());
  return utils::ordered_compare(this->args,
                                dynamic_cast<const LTLfBinaryOp&>(o).args);
}

void Symbol::accept(Visitor& visitor) const {}
void LTLfTrue::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfFalse::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfPropTrue::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfPropFalse::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfAtom::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfNot::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfPropositionalNot::accept(Visitor& visitor) const {
  visitor.visit(*this);
}
void LTLfAnd::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfOr::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfImplies::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfEquivalent::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfXor::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfNext::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfWeakNext::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfUntil::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfRelease::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfEventually::accept(Visitor& visitor) const { visitor.visit(*this); }
void LTLfAlways::accept(Visitor& visitor) const { visitor.visit(*this); }

} // namespace logic
} // namespace cynthia