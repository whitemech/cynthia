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
namespace logic {

/*
 * Abstract base class for generic hashable immutable objects.
 */
class Hashable {
private:
  // The hash_ is defined as mutable, because its value is initialized to 0
  // in the constructor and then it can be changed in Basic::hash() to the
  // current hash (which is always the same for the given instance). The
  // state of the instance does not change, so we define hash_ as mutable
  mutable hash_t hash_; // This holds the hash value

  /*!
  Calculates the hash of the given Lydia class.
  Use Basic.hash() which gives a cached version of the hash.
  \return 64-bit integer value for the hash
  */
  inline virtual hash_t compute_hash_() const = 0;

public:
  // Destructor must be explicitly defined as virtual here to avoid problems
  // with undefined behavior while deallocating derived classes.
  virtual ~Hashable() = default;

  Hashable() : hash_{0} {}

  /*! Returns the hash of the Basic class:
      This method caches the value.

      Use `std::hash` to get the hash. Example:

           const Symbol x = symbol("x");
           std::hash<Basic> hash_fn;
           std::cout << hash_fn(*x);

      \return 64-bit integer value for the hash
  */
  inline hash_t hash() const {
    if (hash_ == 0) {
      auto hash_v = compute_hash_();
      assert(hash_v != 0);
      hash_ = hash_v;
    }
    return hash_;
  }
};

template <class T> void hash_combine(hash_t& seed, const T& v);

//! Templatised version to combine hash
template <typename T>
inline void hash_combine_impl(
    hash_t& seed, const T& v,
    typename std::enable_if<std::is_base_of<Hashable, T>::value>::type* =
        nullptr) {
  hash_combine(seed, v.hash());
}

template <typename T>
inline void hash_combine_impl(
    hash_t& seed, const T& v,
    typename std::enable_if<std::is_integral<T>::value>::type* = nullptr) {
  seed ^= hash_t(v) + hash_t(0x9e3779b9) + (seed << 6) + (seed >> 2);
}

inline void hash_combine_impl(hash_t& seed, const std::string& s) {
  for (const char& c : s) {
    hash_combine<hash_t>(seed, static_cast<hash_t>(c));
  }
}

template <class T> void hash_combine(hash_t& seed, const T& v);
inline void hash_combine_impl(hash_t& seed, const double& s) {
  union {
    hash_t h;
    double d;
  } u{};
  u.h = 0u;
  u.d = s;
  hash_combine(seed, u.h);
}
template <class T> inline void hash_combine(hash_t& seed, const T& v) {
  hash_combine_impl(seed, v);
}

} // namespace logic
} // namespace cynthia