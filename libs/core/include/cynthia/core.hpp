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

#include <cynthia/closure.hpp>
#include <cynthia/input_output_partition.hpp>
#include <cynthia/logic/types.hpp>
#include <sddapi.h>

namespace cynthia {
namespace core {

class ISynthesis {
public:
  const logic::ltlf_ptr formula;
  const InputOutputPartition partition;
  ISynthesis(const logic::ltlf_ptr& formula,
             const InputOutputPartition& partition);
  virtual bool is_realizable() = 0;
};

template <class Synthesis, typename = typename std::enable_if<std::is_base_of<
                               ISynthesis, Synthesis>::value>::type>
bool is_realizable(const logic::ltlf_ptr& formula,
                   const InputOutputPartition& partition) {
  auto synthesis = Synthesis(formula, partition);
  return synthesis.is_realizable();
}

class ForwardSynthesis : public ISynthesis {
private:
  Closure closure_;
  Vtree* vtree_;
  SddManager* manager_;

public:
  bool is_realizable() override;

  bool forward_synthesis_();
};

} // namespace core
} // namespace cynthia
