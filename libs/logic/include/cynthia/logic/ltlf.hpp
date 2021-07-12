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
#include <cynthia/logic/visitable.hpp>
#include <utility>

namespace cynthia {
namespace logic {

class Symbol : public AstNode {
private:
  const std::string name_;

public:
  const static TypeID type_code_id = TypeID::t_Symbol;
  explicit Symbol(Context& ctx, const std::string& name)
      : AstNode(&ctx), name_{name} {}

  void accept(Visitor* visitor) const override;
  inline hash_t compute_hash_() const override;
  inline TypeID get_type_code() const override;
  bool is_equal(const Comparable& o) const override;
  int compare_(const Comparable& o) const override;
};

} // namespace logic
} // namespace cynthia