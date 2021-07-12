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

//#include <cynthia/logic/base.hpp>
#include <cynthia/logic/base.hpp>
#include <cynthia/logic/ltlf.hpp>

namespace cynthia {
namespace logic {
Context::Context() {
  table_ = utils::make_unique<HashTable>();

  tt = std::make_shared<const LTLfTrue>(*this);
  table_->insert_if_not_available(tt);

  ff = std::make_shared<const LTLfFalse>(*this);
  table_->insert_if_not_available(ff);
}

symbol_ptr Context::make_symbol(const std::string& name) {
  symbol_ptr symbol = std::make_shared<const Symbol>(*this, name);
  symbol_ptr result = table_->insert_if_not_available(symbol);
  return result;
}
ltlf_ptr Context::make_tt() { return tt; }
ltlf_ptr Context::make_ff() { return ff; }
ltlf_ptr Context::make_bool(bool value) {
  return value ? make_tt() : make_ff();
}

ltlf_ptr Context::make_atom(const std::string& name) {
  auto symbol = make_symbol(name);
  auto actual_symbol = table_->insert_if_not_available(symbol);
  return make_atom(actual_symbol);
}

ltlf_ptr Context::make_atom(const symbol_ptr& symbol) {
  auto atom = std::make_shared<const LTLfAtom>(*this, symbol);
  auto actual_atom = table_->insert_if_not_available(atom);
  return actual_atom;
}

ltlf_ptr Context::make_not(const ltlf_ptr& arg) {
  auto negation = std::make_shared<const LTLfNot>(*this, arg);
  auto actual = table_->insert_if_not_available(negation);
  return actual;
}

} // namespace logic
} // namespace cynthia