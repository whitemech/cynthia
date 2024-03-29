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
#include <cynthia/logic/types.hpp>

namespace cynthia {
namespace parser {
class AbstractDriver {
public:
  std::shared_ptr<logic::Context> context = nullptr;
  AbstractDriver() : context{std::make_shared<logic::Context>()} {}
  AbstractDriver(std::shared_ptr<logic::Context> c) : context{std::move(c)} {}
  virtual void parse(const char* const filename) = 0;
  virtual void parse(std::istream& iss) = 0;
  virtual logic::ltlf_ptr get_result() = 0;
};
} // namespace parser
} // namespace cynthia
