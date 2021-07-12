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

namespace cynthia {
namespace logic {
Context::Context() { table_ = utils::make_unique<HashTable>(); }

symbol_ptr Context::make_symbol(const std::string& name) {
  symbol_ptr symbol = std::make_shared<const Symbol>(*this, name);
  symbol_ptr result = table_->insert_if_not_available(symbol);
  return result;
}
} // namespace logic
} // namespace cynthia