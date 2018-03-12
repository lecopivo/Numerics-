#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>

#include "AlgebraicOperations.h"
#include "AlgebraicTraits.h"

namespace Numerics {
namespace Algebra {

// One operation structures //
// --------------------------- //

// Semigroup
template <typename T, typename Op, bool Force = false>
constexpr bool is_semigroup = []() {
  if constexpr (is_consistent<T, Op>) {

    if constexpr (algebraic_traits<T, Op>::is_associative) {
      return true;
    } else {
      static_assert(!Force, "Not a semigroup: operation is not associative!");
      return false;
    }

  } else {
    return false;
  }
}();

template <typename T, typename Op, bool Force = false>
constexpr bool is_monoid = []() {
  if constexpr (is_semigroup<T, Op, Force> && has_unit<T, Op>) {
    return true;
  } else {
    static_assert(!Force, "Not a monoid: unit element not defined!");
    return false;
  }
}();

template <typename T, typename Op, bool Force = false>
constexpr bool is_group = []() {
  if constexpr (is_monoid<T, Op, Force> && all_invertible<T, Op>) {
    return true;
  } else {
    static_assert(!Force, "Not a group: some elements are not invertible!");
    return false;
  }
}();

template <typename T, typename Op, bool Force = false>
constexpr bool is_abelian_group = []() {
  if constexpr (is_group<T, Op, Force> &&
                algebraic_traits<T, Op>::is_commutative) {
    return true;
  } else {
    static_assert(!Force,
                  "Not an abelian group: operation is not commutative!");
    return false;
  }
}();

template <typename T, typename Add, typename Mul, bool Force = false>
constexpr bool                        is_ring =
    is_abelian_group<T, Add, Force> &&is_monoid<T, Mul, Force>;

template <typename T, typename Add, typename Mul, bool Force = false>
constexpr bool is_commutative_ring = []() {
  if constexpr (is_ring<T, Add, Mul> &&
                algebraic_traits<T, Mul>::is_commutative) {
    return true;
  } else {
    static_assert(!Force,
                  "Not a commutative ring: multiplication is not commutative");
    return false;
  }
}();

template <typename T, typename Add, typename Mul, bool Force = false,
          typename SFINAE = void>
constexpr bool is_division_ring = []() {
  static_assert(!Force, "Not a divition ring!");
  return false;
}();

template <typename T, bool Force>
constexpr bool  is_division_ring<T, addition, multiplication, Force,
                                std::enable_if_t<std::is_floating_point_v<T>>> =
    true;

template <typename T, bool Force>
constexpr bool
    is_division_ring<std::complex<T>, addition, multiplication, Force,
                     std::enable_if_t<std::is_floating_point_v<T>>> = true;

template <typename T, typename Add, typename Mul, bool Force = false,
          typename = void>
constexpr bool                      is_field =
  is_division_ring<T, Add, Mul,Force> &&algebraic_traits<T, Mul>::is_commutative;

} // namespace Algebra
} // namespace Numerics
