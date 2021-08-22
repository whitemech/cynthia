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

namespace cynthia {
namespace core {

class ForwardDfsSynthesis : public ISynthesis {
public:
  ForwardDfsSynthesis(const logic::ltlf_ptr& formula,
                      const InputOutputPartition& partition)
      : context_{formula, partition}, ISynthesis(formula, partition){};

  bool is_realizable() override;

  bool forward_synthesis_();

private:
  Context context_;

  template <typename Arg1, typename... Args>
  inline void print_search_debug(const char* fmt, const Arg1& arg1,
                                 const Args&... args) {
    context_.logger.debug(
        (std::string(context_.indentation, '\t') + fmt).c_str(), arg1, args...);
  };

  inline void print_search_debug(const char* fmt) {
    context_.logger.debug(
        (std::string(context_.indentation, '\t') + fmt).c_str());
  };

  strategy_t system_move_(const logic::ltlf_ptr& formula,
                          std::set<SddSize>& path);

  strategy_t env_move_(SddNodeWrapper& wrapper, std::set<SddSize>& path);
};

} // namespace core
} // namespace cynthia