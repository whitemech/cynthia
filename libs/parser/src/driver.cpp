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

#include <cassert>
#include <fstream>

#include <cynthia/logic/ltlf.hpp>
#include <cynthia/parser/driver.hpp>

namespace cynthia {
namespace parser {
namespace ltlf {

LTLfDriver::~LTLfDriver() {
  delete (scanner);
  scanner = nullptr;
  delete (parser);
  parser = nullptr;
}

void LTLfDriver::parse(const char* const filename) {
  assert(filename != nullptr);
  std::ifstream in_file(filename);
  if (!in_file.good()) {
    exit(EXIT_FAILURE);
  }
  parse_helper(in_file);
}

void LTLfDriver::parse(std::istream& stream) {
  if (!stream.good() && stream.eof()) {
    return;
  }
  parse_helper(stream);
}

void LTLfDriver::parse_helper(std::istream& stream) {
  delete (scanner);
  try {
    scanner = new LTLfScanner(&stream);
  } catch (std::bad_alloc& ba) {
    std::cerr << "Failed to allocate scanner: (" << ba.what()
              << "), exiting!\n";
    exit(EXIT_FAILURE);
  }

  delete (parser);
  try {
    parser = new LTLfParser((*scanner) /* scanner */, (*this) /* driver */);
  } catch (std::bad_alloc& ba) {
    std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!\n";
    exit(EXIT_FAILURE);
  }
  const int accept(0);
  if (parser->parse() != accept) {
    std::cerr << "Parse failed!\n";
    exit(EXIT_FAILURE);
  }
}

logic::ltlf_ptr LTLfDriver::add_LTLfTrue() const { return context->make_tt(); }

logic::ltlf_ptr LTLfDriver::add_LTLfFalse() const { return context->make_ff(); }
logic::ltlf_ptr LTLfDriver::add_LTLfPropTrue() const {
  return context->make_prop_true();
}

logic::ltlf_ptr LTLfDriver::add_LTLfPropFalse() const {
  return context->make_prop_false();
}

logic::ltlf_ptr LTLfDriver::add_LTLfEnd() const {
  return context->make_always(context->make_ff());
}
logic::ltlf_ptr LTLfDriver::add_LTLfLast() const {
  return context->make_weak_next(context->make_ff());
}

logic::ltlf_ptr LTLfDriver::add_LTLfAtom(const std::string& name) const {
  return context->make_atom(name);
}

logic::ltlf_ptr LTLfDriver::add_LTLfNot(const logic::ltlf_ptr& formula) const {
  if (logic::is_a<logic::LTLfAtom>(*formula) ||
      logic::is_a<logic::LTLfPropositionalNot>(*formula)) {
    return context->make_prop_not(formula);
  }
  return context->make_not(formula);
}

logic::ltlf_ptr LTLfDriver::add_LTLfAnd(const logic::ltlf_ptr& lhs,
                                        const logic::ltlf_ptr& rhs) const {
  return context->make_and(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr LTLfDriver::add_LTLfOr(const logic::ltlf_ptr& lhs,
                                       const logic::ltlf_ptr& rhs) const {
  return context->make_or(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr LTLfDriver::add_LTLfImplies(const logic::ltlf_ptr& lhs,
                                            const logic::ltlf_ptr& rhs) const {
  return context->make_implies(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr
LTLfDriver::add_LTLfEquivalent(const logic::ltlf_ptr& lhs,
                               const logic::ltlf_ptr& rhs) const {
  return context->make_equivalent(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr LTLfDriver::add_LTLfXor(const logic::ltlf_ptr& lhs,
                                        const logic::ltlf_ptr& rhs) const {
  return context->make_xor(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr LTLfDriver::add_LTLfNext(const logic::ltlf_ptr& formula) const {
  return context->make_next(formula);
}

logic::ltlf_ptr
LTLfDriver::add_LTLfWeakNext(const logic::ltlf_ptr& formula) const {
  return context->make_weak_next(formula);
}
//
logic::ltlf_ptr
LTLfDriver::add_LTLfEventually(const logic::ltlf_ptr& formula) const {
  return context->make_eventually(formula);
}

logic::ltlf_ptr
LTLfDriver::add_LTLfAlways(const logic::ltlf_ptr& formula) const {
  return context->make_always(formula);
}

logic::ltlf_ptr LTLfDriver::add_LTLfUntil(const logic::ltlf_ptr& lhs,
                                          const logic::ltlf_ptr& rhs) const {
  return context->make_until(logic::vec_ptr{lhs, rhs});
}

logic::ltlf_ptr LTLfDriver::add_LTLfRelease(const logic::ltlf_ptr& lhs,
                                            const logic::ltlf_ptr& rhs) const {
  return context->make_release(logic::vec_ptr{lhs, rhs});
}

std::ostream& LTLfDriver::print(std::ostream& stream) const {
  //  stream << this->result->str() << "\n";
  // TODO
  return (stream);
}

} // namespace ltlf
} // namespace parser
} // namespace cynthia
