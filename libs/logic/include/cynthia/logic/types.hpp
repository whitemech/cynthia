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

#include <cstdint>
#include <memory>
#include <vector>

namespace cynthia {
namespace logic {
class AstNode;
class Symbol;
class LTLfFormula;

typedef std::size_t hash_t;
typedef std::shared_ptr<const AstNode> ast_ptr;
typedef std::shared_ptr<const Symbol> symbol_ptr;
typedef std::shared_ptr<const LTLfFormula> ltlf_ptr;
typedef std::vector<ltlf_ptr> vec_ptr;
} // namespace logic

} // namespace cynthia
