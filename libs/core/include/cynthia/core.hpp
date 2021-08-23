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
#include <cynthia/logger.hpp>
#include <cynthia/logic/types.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/statistics.hpp>

extern "C" {
#include "sddapi.h"
}
namespace cynthia {
namespace core {

typedef std::map<SddSize, SddNode*> strategy_t;

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

class Context {
public:
  logic::ltlf_ptr formula;
  logic::Context* ast_manager;
  InputOutputPartition partition;
  logic::ltlf_ptr nnf_formula;
  logic::ltlf_ptr xnf_formula;
  Closure closure_;
  Statistics statistics_;
  std::map<std::string, size_t> prop_to_id;
  std::map<SddSize, bool> discovered;
  Vtree* vtree_ = nullptr;
  SddManager* manager = nullptr;
  utils::Logger logger;
  size_t indentation = 0;
  Context(const logic::ltlf_ptr& formula,
          const InputOutputPartition& partition);
  ~Context();

  logic::ltlf_ptr get_formula(size_t index) const;
  static std::map<std::string, size_t>
  compute_prop_to_id_map(const Closure& closure,
                         const InputOutputPartition& partition);

  template <typename Arg1, typename... Args>
  inline void print_search_debug(const char* fmt, const Arg1& arg1,
                                 const Args&... args) {
    logger.debug((std::string(indentation, '\t') + fmt).c_str(), arg1, args...);
  };

  inline void print_search_debug(const char* fmt) {
    logger.debug((std::string(indentation, '\t') + fmt).c_str());
  };
};

} // namespace core
} // namespace cynthia
