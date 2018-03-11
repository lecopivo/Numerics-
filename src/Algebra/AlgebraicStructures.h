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

// Magma
// template <typename T, template <typename...> typename OperationType>
// constexpr bool is_magma = has_operation<OperationType, T, T>
//     &&         has_operation<basic_assignment_t, T, OperationType<T, T>>;

template <typename T, template <typename...> typename OperationType,
          bool Force = false>
constexpr bool is_magma = []() {

  if constexpr (has_operation<OperationType, T, T>) {
    if constexpr (has_operation<basic_assignment_t, T &, OperationType<T, T>>) {
      return true;
    } else {
      static_assert(!Force,
                    "The type is not closed under the given operation!");
    }
  } else {
    static_assert(!Force, "The type does not have the required operation!");
  }
  return false;
}();

// Monoid
template <typename T, template <typename...> typename OperationType,
          bool Force = false>
constexpr bool is_monoid = []() {
  if constexpr (is_magma<T, OperationType, Force>)
    if constexpr (has_unit<T, OperationType, Force>)
      return true;
  return false;
}();

// Group
template <typename T, template <typename...> typename OperationType,
          bool Force = false>
constexpr bool is_group = []() {
  if constexpr (is_monoid<T, OperationType, Force>) {
    if constexpr (has_inverse<T, OperationType>) {
      return true;
    } else {
      static_assert(!Force,
                    "The type and operation does not define inverse! "
                    "Please see documentation how to define an inverse "
                    "for a type and operation.");
    }
  }
  return false;
}();

// Special types
template <typename T, bool Force = false>
constexpr bool is_abelian_group = []() {
  if constexpr (is_group<T, addition_t, Force>)
    if constexpr /* x+=y */ (has_operation<addition_assignment_t, T &, T>)
      if constexpr /* x-=y */ (has_operation<subtraction_assignment_t, T &, T>)
        if constexpr /* -x */ (has_operation<unary_minus_t, T>)
          // if constexpr /* +x */ (has_operation<unary_plus_t, T>)
          return true;
  return false;
}();

template <typename T, bool Force = false>
constexpr bool is_multiplicative_group = []() {
  if constexpr (is_group<T, multiplication_t, Force>)
    if constexpr /* x*=y */ (has_operation<multiplication_assignment_t, T &, T>)
      // if constexpr /* +x */ (has_operation<unary_plus_t, T>)
      return true;
  return false;
}();

// Two operation structures //
// ------------------------ //

// Ring
// template <typename T, typename Add, typename Neg, typename Mul>
// constexpr bool is_group(Add &&add, Neg &&neg, Mul &&mul, T zero, T one) {
//   return is_group<T>(add, neg, zero) && is_monoid<T>(mul, one);
// }

// Field

// Module

// Vector Space

} // namespace Algebra
} // namespace Numerics
