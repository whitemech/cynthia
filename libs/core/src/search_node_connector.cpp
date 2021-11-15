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

#include <cynthia/core.hpp>
#include <cynthia/eval.hpp>
#include <cynthia/logic/nnf.hpp>
#include <cynthia/logic/print.hpp>
#include <cynthia/one_step_realizability.hpp>
#include <cynthia/one_step_unrealizability.hpp>
#include <cynthia/sdd_to_formula.hpp>
#include <cynthia/sddcpp.hpp>
#include <cynthia/strip_next.hpp>
#include <cynthia/to_sdd.hpp>
#include <cynthia/vtree.hpp>
#include <cynthia/xnf.hpp>

namespace cynthia {
namespace core {

ForwardSynthesis::SearchNode::SearchNode(Problem problem,
                                         const logic::ltlf_ptr formula,
                                         const SearchNode* parent)
    : problem_(problem), formula(formula), parent_(parent) {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  sdd = SddNodeWrapper(to_sdd(*formula, problem_), problem_.manager);
  index = sdd.get_id();
}

ForwardSynthesis::SearchConnector::SearchConnector(
    Problem problem, const ForwardSynthesis::SearchNode* parent,
    const std::vector<std::pair<SddNodeWrapper, SddNodeWrapper>> children)
    : problem_(problem), parent_(parent), children_(children) {
  cost_ = 0;
}

} // namespace core
} // namespace cynthia