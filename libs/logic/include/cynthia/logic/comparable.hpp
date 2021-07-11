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

#include <cynthia/logic/types.hpp>

namespace cynthia {
namespace logic {

// Type ID for all the concrete classes that inherit from Comparable.
// Alternative: use 'typeid'. However, the order is not guaranteed to be
// deterministic across runs, and that might have an impact with different
// compilers.
enum TypeID {
  t_Symbol,
  t_LTLfTrue,
  t_LTLfFalse,
  t_LTLfAtom,
  t_LTLfAnd,
  t_LTLfOr,
  t_LTLfNot,
  t_LTLfNext,
  t_LTLfWeakNext,
  t_LTLfUntil,
  t_LTLfRelease,
  t_LTLfEventually,
  t_LTLfAlways,
};

/*
 * Abstract base class for generic comparable objects.
 */
class Comparable {
private:
public:
  // Destructor must be explicitly defined as virtual here to avoid problems
  // with undefined behavior while deallocating derived classes.
  virtual ~Comparable() {}

  virtual inline TypeID get_type_code() const = 0;

  virtual bool is_equal(const Comparable& o) const = 0;

  virtual int compare_(const Comparable& o) const = 0;

  int compare(const Comparable& o) const {
    auto a = this->get_type_code();
    auto b = o.get_type_code();
    if (a == b) {
      return this->compare_(o);
    } else {
      // We return the order given by the numerical value of the TypeID enum
      // type.
      // The types don't need to be ordered in any given way, they just need
      // to be ordered.
      return a < b ? -1 : 1;
    }
  }

  bool operator==(const Comparable& o) const { return this->is_equal(o); };

  bool operator!=(const Comparable& o) const { return !(*this == o); };

  //! Comparator operator
  bool operator<(const Comparable& rhs) const {
    return this->compare(rhs) == -1;
  };

  bool operator>(const Comparable& rhs) const { return rhs < *this; }

  bool operator<=(const Comparable& rhs) const { return !(*this > rhs); }

  bool operator>=(const Comparable& rhs) const { return !(*this < rhs); }
};
} // namespace logic
} // namespace cynthia