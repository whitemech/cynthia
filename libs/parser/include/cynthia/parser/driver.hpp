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

#include <cstddef>
#include <istream>
#include <memory>
#include <string>
#include <utility>

#include <cynthia/logic/base.hpp>
#include <cynthia/parser/abstract_driver.hpp>
#include <cynthia/parser/parser.tab.hh>
#include <cynthia/parser/scanner.hpp>

namespace cynthia {
namespace parser {
namespace ltlf {

class LTLfDriver : public AbstractDriver {
private:
  void parse_helper(std::istream& stream);

  LTLfParser* parser = nullptr;
  LTLfScanner* scanner = nullptr;

public:
  logic::ltlf_ptr result;

  LTLfDriver() : AbstractDriver() {}

  explicit LTLfDriver(std::shared_ptr<logic::Context> c) : AbstractDriver(c) {}

  virtual ~LTLfDriver();

  logic::ltlf_ptr get_result() override { return result; }

  /**
   * parse - parse from a file
   * @param filename - valid string with input file
   */
  void parse(const char* const filename) override;

  /**
   * parse - parse from a c++ input stream
   * @param is - std::istream&, valid input stream
   */
  void parse(std::istream& iss) override;

  std::ostream& print(std::ostream& stream) const;

  logic::ltlf_ptr add_LTLfTrue() const;
  logic::ltlf_ptr add_LTLfFalse() const;
  logic::ltlf_ptr add_LTLfPropTrue() const;
  logic::ltlf_ptr add_LTLfPropFalse() const;
  logic::ltlf_ptr add_LTLfEnd() const;
  logic::ltlf_ptr add_LTLfLast() const;
  logic::ltlf_ptr add_LTLfAtom(const std::string& name) const;
  logic::ltlf_ptr add_LTLfNot(const logic::ltlf_ptr& arg) const;
  logic::ltlf_ptr add_LTLfAnd(const logic::ltlf_ptr& lhs,
                              const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfOr(const logic::ltlf_ptr& lhs,
                             const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfEquivalent(const logic::ltlf_ptr& lhs,
                                     const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfImplies(const logic::ltlf_ptr& lhs,
                                  const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfXor(const logic::ltlf_ptr& lhs,
                              const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfNext(const logic::ltlf_ptr& arg) const;
  logic::ltlf_ptr add_LTLfWeakNext(const logic::ltlf_ptr& arg) const;
  logic::ltlf_ptr add_LTLfUntil(const logic::ltlf_ptr& lhs,
                                const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfRelease(const logic::ltlf_ptr& lhs,
                                  const logic::ltlf_ptr& rhs) const;
  logic::ltlf_ptr add_LTLfEventually(const logic::ltlf_ptr& arg) const;
  logic::ltlf_ptr add_LTLfAlways(const logic::ltlf_ptr& arg) const;
};

} // namespace ltlf
} // namespace parser
} // namespace cynthia
