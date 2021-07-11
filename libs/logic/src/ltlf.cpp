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

void Symbol::accept(Visitor* visitor) const { visitor->visit(*this); }
inline hash_t Symbol::compute_hash_() const {
  hash_t h1 = get_type_code();
  auto h2 = std::hash<std::string>()(name_);
  hash_combine(h1, h2);
  return h1;
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

} // namespace logic
} // namespace cynthia