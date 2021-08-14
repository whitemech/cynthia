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

#include <cynthia/logic/ltlf.hpp>

namespace cynthia {
namespace logic {

template <typename Function1, typename Function2>
inline ltlf_ptr forward_call_to_arguments(const LTLfBinaryOp& formula,
                                          Function1 mapping_function,
                                          Function2 factory_function) {
  const auto& container = formula.args;
  auto new_container = vec_ptr(container.size());
  std::transform(container.begin(), container.end(), new_container.begin(),
                 mapping_function);
  return factory_function(new_container);
}
} // namespace logic
} // namespace cynthia