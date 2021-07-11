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
#include <string>
#include <utility>

#include <cynthia/logic/parser/abstract_driver.hpp>
#include <cynthia/logic/parser/parser.tab.hh>
#include <cynthia/logic/parser/scanner.hpp>

namespace cynthia {
namespace logic {
namespace parsers {
namespace ltlf {

class LTLfDriver : public AbstractDriver {
private:
  void parse_helper(std::istream& stream);

  LTLfParser* parser = nullptr;
  LTLfScanner* scanner = nullptr;

public:
  //  ldlf_ptr result;

  LTLfDriver() : AbstractDriver() {}

  //  explicit LTLfDriver(std::shared_ptr<AstManager> c) : AbstractDriver(c) {}

  virtual ~LTLfDriver();

  //  ldlf_ptr get_result() override { return result; }

  /**
   * parse - parse from a file
   * @param filename - valid string with input file
   */
  void parse(const char* const filename) override;

  /**
   * parse - parse from a c++ input stream
   * @param is - std::istream&, valid input stream
   */
  void parse(std::istream& iss);

  std::ostream& print(std::ostream& stream) const;
};

} // namespace ltlf
} // namespace parsers
} // namespace logic
} // namespace cynthia
