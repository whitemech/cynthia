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
#include <cynthia/logic/ltlf.hpp>

// TODO instead of make_shared, use "signature" of object to instantiate
//        so to avoid heap allocation.

namespace cynthia {
namespace logic {
Context::Context() {
  table_ = utils::make_unique<HashTable>();

  tt = std::make_shared<const LTLfTrue>(*this);
  table_->insert_if_not_available(tt);

  ff = std::make_shared<const LTLfFalse>(*this);
  table_->insert_if_not_available(ff);

  true_ = std::make_shared<const LTLfPropTrue>(*this);
  table_->insert_if_not_available(true_);

  false_ = std::make_shared<const LTLfPropFalse>(*this);
  table_->insert_if_not_available(false_);

  end = std::make_shared<const LTLfAlways>(*this, ff);
  table_->insert_if_not_available(end);

  last = std::make_shared<const LTLfWeakNext>(*this, ff);
  table_->insert_if_not_available(last);
}

ltlf_ptr Context::make_tt() { return tt; }
ltlf_ptr Context::make_ff() { return ff; }
ltlf_ptr Context::make_prop_true() { return true_; }
ltlf_ptr Context::make_prop_false() { return false_; }
ltlf_ptr Context::make_end() { return end; }
ltlf_ptr Context::make_last() { return last; }
ltlf_ptr Context::make_bool(bool value) {
  return value ? make_tt() : make_ff();
}

ltlf_ptr Context::make_atom(const std::string& name) {
  auto atom = std::make_shared<const LTLfAtom>(*this, name);
  auto actual_atom = table_->insert_if_not_available(atom);
  return actual_atom;
}

ltlf_ptr Context::make_not(const ltlf_ptr& arg) {
  auto negation = std::make_shared<const LTLfNot>(*this, arg);
  auto actual = table_->insert_if_not_available(negation);
  return actual;
}

ltlf_ptr Context::make_prop_not(const ltlf_ptr& arg) {
  // !(!a) = a
  if (is_a<LTLfPropositionalNot>(*arg)) {
    return table_->insert_if_not_available(
        std::static_pointer_cast<const LTLfPropositionalNot>(arg)->arg);
  }
  // argument must be an atom
  auto negation = std::make_shared<const LTLfPropositionalNot>(*this, arg);
  auto actual = table_->insert_if_not_available(negation);
  return actual;
}

ltlf_ptr Context::make_and(const vec_ptr& args) {
  auto setified_args = utils::setify(args);
  if (setified_args.size() == 1) {
    auto actual = table_->insert_if_not_available(setified_args[0]);
    return actual;
  }
  auto and_ = std::make_shared<const LTLfAnd>(*this, setified_args);
  auto actual = table_->insert_if_not_available(and_);
  return actual;
}

ltlf_ptr Context::make_or(const vec_ptr& args) {
  auto setified_args = utils::setify(args);
  if (setified_args.size() == 1) {
    auto actual = table_->insert_if_not_available(setified_args[0]);
    return actual;
  }
  auto or_ = std::make_shared<const LTLfOr>(*this, args);
  auto actual = table_->insert_if_not_available(or_);
  return actual;
}

ltlf_ptr Context::make_implies(const vec_ptr& args) {
  auto implies = std::make_shared<const LTLfImplies>(*this, args);
  auto actual = table_->insert_if_not_available(implies);
  return actual;
}

ltlf_ptr Context::make_equivalent(const vec_ptr& args) {
  auto equivalent = std::make_shared<const LTLfEquivalent>(*this, args);
  auto actual = table_->insert_if_not_available(equivalent);
  return actual;
}

ltlf_ptr Context::make_xor(const vec_ptr& args) {
  auto equivalent = std::make_shared<const LTLfXor>(*this, args);
  auto actual = table_->insert_if_not_available(equivalent);
  return actual;
}

ltlf_ptr Context::make_next(const ltlf_ptr& arg) {
  auto next = std::make_shared<const LTLfNext>(*this, arg);
  auto actual = table_->insert_if_not_available(next);
  return actual;
}

ltlf_ptr Context::make_weak_next(const ltlf_ptr& arg) {
  auto next = std::make_shared<const LTLfWeakNext>(*this, arg);
  auto actual = table_->insert_if_not_available(next);
  return actual;
}

ltlf_ptr Context::make_until(const vec_ptr& args) {
  auto until = std::make_shared<const LTLfUntil>(*this, args);
  auto actual = table_->insert_if_not_available(until);
  return actual;
}

ltlf_ptr Context::make_release(const vec_ptr& args) {
  auto release = std::make_shared<const LTLfRelease>(*this, args);
  auto actual = table_->insert_if_not_available(release);
  return actual;
}

ltlf_ptr Context::make_eventually(const ltlf_ptr& arg) {
  auto eventually = std::make_shared<const LTLfEventually>(*this, arg);
  auto actual = table_->insert_if_not_available(eventually);
  return actual;
}

ltlf_ptr Context::make_always(const ltlf_ptr& arg) {
  auto always = std::make_shared<const LTLfAlways>(*this, arg);
  auto actual = table_->insert_if_not_available(always);
  return actual;
}

} // namespace logic
} // namespace cynthia