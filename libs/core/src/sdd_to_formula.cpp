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

#include <cynthia/logic/types.hpp>
#include <cynthia/sdd_to_formula.hpp>

namespace cynthia {
namespace core {

logic::ltlf_ptr sdd_to_formula(SddNode* sdd_node,
                               ForwardSynthesis::Problem& problem_) {
  auto pair = problem_.sdd_node_id_to_formula.find(sdd_id(sdd_node));
  if (pair != problem_.sdd_node_id_to_formula.end()) {
    return pair->second;
  }
  logic::ltlf_ptr result;
  if (sdd_node_is_false(sdd_node)) {
    result = problem_.ast_manager->make_ff();
  } else if (sdd_node_is_true(sdd_node)) {
    result = problem_.ast_manager->make_tt();
  } else if (sdd_node_is_literal(sdd_node)) {
    auto literal_id = sdd_node_literal(sdd_node);
    auto formula = problem_.get_formula(abs(literal_id) - 1);
    if (literal_id < 0) {
      if (logic::is_a<logic::LTLfAtom>(*formula)) {
        result = problem_.ast_manager->make_prop_not(formula);
      } else {
        // if not an atom ignore, it is a state component
        result = problem_.ast_manager->make_tt();
      }
    } else {
      result = formula;
    }
  } else if (sdd_node_is_decision(sdd_node)) {
    auto wrapper = SddNodeWrapper(sdd_node, problem_.manager);
    std::vector<logic::ltlf_ptr> args;
    args.reserve(wrapper.nb_children());
    for (auto it = wrapper.begin(); it != wrapper.end(); ++it) {
      auto prime = sdd_to_formula(it.get_prime(), problem_);
      auto sub = sdd_to_formula(it.get_sub(), problem_);
      args.push_back(problem_.ast_manager->make_and({prime, sub}));
    }
    result = problem_.ast_manager->make_or(args);
  } else {
    throw std::logic_error("SDD node type not recognized");
  }

  // insert into cache
  problem_.sdd_node_id_to_formula[sdd_id(sdd_node)] = result;
  return result;
}

} // namespace core
} // namespace cynthia
