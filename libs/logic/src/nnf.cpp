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

#include <cynthia/logic/duality.hpp>
#include <cynthia/logic/ltlf.hpp>
#include <cynthia/logic/nnf.hpp>

namespace cynthia {
namespace logic {

void NNFTransformer::visit(const LTLfTrue& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfFalse& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfPropTrue& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfPropFalse& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfAtom& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfNot& formula) {
  result = apply_negation(*formula.arg);
}

void NNFTransformer::visit(const LTLfPropositionalNot& formula) {
  result = formula.shared_from_this();
}

void NNFTransformer::visit(const LTLfAnd& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_and(container);
      });
}

void NNFTransformer::visit(const LTLfOr& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_or(container);
      });
}

void NNFTransformer::visit(const LTLfImplies& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_implies(container);
      });
}

void NNFTransformer::visit(const LTLfEquivalent& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_equivalent(container);
      });
}

void NNFTransformer::visit(const LTLfXor& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_xor(container);
      });
}

void NNFTransformer::visit(const LTLfNext& formula) {
  result = formula.ctx().make_next(apply(*formula.arg));
}

void NNFTransformer::visit(const LTLfWeakNext& formula) {
  result = formula.ctx().make_weak_next(apply(*formula.arg));
}

void NNFTransformer::visit(const LTLfUntil& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_until(container);
      });
}

void NNFTransformer::visit(const LTLfRelease& formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr& formula) { return apply(*formula); },
      [formula](const vec_ptr& container) {
        return formula.ctx().make_release(container);
      });
}

void NNFTransformer::visit(const LTLfEventually& formula) {
  result = formula.ctx().make_eventually(apply(*formula.arg));
}

void NNFTransformer::visit(const LTLfAlways& formula) {
  result = formula.ctx().make_always(apply(*formula.arg));
}

ltlf_ptr NNFTransformer::apply(const LTLfFormula& f) {
  f.accept(*this);
  return result;
}

ltlf_ptr to_nnf(const LTLfFormula& f) {
  auto visitor = NNFTransformer{};
  return visitor.apply(f);
}

} // namespace logic
} // namespace cynthia