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

template <class Synthesis,
          typename = typename std::enable_if<
              std::is_base_of<ISynthesis, Synthesis>::value>::type,
          typename... Args>
bool is_realizable(const logic::ltlf_ptr& formula,
                   const InputOutputPartition& partition, const Args&... args) {
  auto synthesis = Synthesis(formula, partition, args...);
  return synthesis.is_realizable();
}

class ForwardSynthesis : public ISynthesis {
public:
  class Problem {
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
    std::map<SddSize, logic::ltlf_ptr> sdd_node_id_to_formula;
    std::map<logic::ltlf_ptr, SddNode*> formula_to_sdd_node;
    utils::Logger logger;
    size_t indentation = 0;
    const bool use_gc;
    const float gc_threshold;
    std::vector<int> controllable_map;
    std::vector<int> uncontrollable_map;
    Problem(const logic::ltlf_ptr& formula, const InputOutputPartition& partition,
            bool use_gc = false, float gc_threshold = 0.95);
    ~Problem() {
      if (vtree_) {
        sdd_vtree_free(vtree_);
      }
      if (manager) {
        sdd_manager_free(manager);
      }
    }

    logic::ltlf_ptr get_formula(size_t index) const;
    void call_gc_vtree() const;
    template <typename Arg1, typename... Args>
    inline void print_search_debug(const char* fmt, const Arg1& arg1,
                                   const Args&... args) const {
      logger.debug((std::string(indentation, '\t') + fmt).c_str(), arg1, args...);
    };
    inline void print_search_debug(const char* fmt) const {
      logger.debug((std::string(indentation, '\t') + fmt).c_str());
    };

    void initialie_maps_();
  };

  class SearchNode {
  public:
    Problem problem_;
    const logic::ltlf_ptr formula;
    logic::ltlf_ptr nnf_formula;
    logic::ltlf_ptr xnf_formula;
    SddNodeWrapper sdd;
    SddSize index;
    size_t depth = 0;
    size_t branching_factor = 0;
    const ForwardSynthesis::SearchNode* parent_;

    SearchNode(Problem problem, const logic::ltlf_ptr formula, const SearchNode* parent);
  };

  class SearchConnector {
  public:
    Problem problem_;
    size_t cost_ = 0;
    const ForwardSynthesis::SearchNode* parent_;
    const std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> children_;

    SearchConnector(Problem problem, const ForwardSynthesis::SearchNode* parent,
                    const std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> children);
  };

  Problem problem_;

  ForwardSynthesis(const logic::ltlf_ptr& formula,
                   const InputOutputPartition& partition,
                   bool enable_gc = false)
      : problem_{formula, partition, enable_gc},
        ISynthesis(formula, partition){};

  static std::map<std::string, size_t>
  compute_prop_to_id_map(const Closure& closure,
                         const InputOutputPartition& partition);
  bool is_realizable() override;

  bool forward_synthesis_();

  logic::ltlf_ptr next_state_formula_(SddNode* wrapper);
  SddNodeWrapper formula_to_sdd_(const logic::ltlf_ptr& formula);

  class Search {
  public:
    logic::ltlf_ptr& init_node;
    Problem problem_;
    std::map<SddSize, bool> discovered;

    Search(Problem& problem, ForwardSynthesis& forwardSynthesis);

    strategy_t do_search_(const logic::ltlf_ptr& formula,
                          std::set<SddSize>& path);
    std::vector<SearchConnector> expand_(SearchNode* node);
  private:
    ForwardSynthesis& forwardSynthesis_;
    SearchConnector expand_single_branch_(SearchNode* node);
  };

private:
  strategy_t system_move_(const logic::ltlf_ptr& formula,
                          std::set<SddSize>& path);
  strategy_t env_move_(SddNodeWrapper& wrapper, std::set<SddSize>& path);
  SddNodeWrapper next_state_(const SddNodeWrapper& wrapper);

};

} // namespace core
} // namespace cynthia
