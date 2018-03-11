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
          bool Diagnose = false>
constexpr bool is_magma = []() {

  if constexpr (has_operation<OperationType, T, T>) {
    if constexpr (has_operation<basic_assignment_t, T &, OperationType<T, T>>) {
      return true;
    } else {
      static_assert(!Diagnose,
                    "The type is not closed under the given operation!");
    }
  } else {
    static_assert(!Diagnose, "The type does not have the required operation!");
  }
  return false;
}();

// Monoid
template <typename T, template <typename...> typename OperationType,
          bool Diagnose = false>
constexpr bool is_monoid = []() {
  if constexpr (is_magma<T, OperationType, Diagnose>)
    if constexpr (has_unit<T, OperationType, Diagnose>)
      return true;
  return false;
}();

// Group
template <typename T, template <typename...> typename OperationType,
          bool Diagnose = false>
constexpr bool is_group = []() {
  if constexpr (is_monoid<T, OperationType, Diagnose>) {
    if constexpr (has_inverse<T, OperationType>) {
      return true;
    } else {
      static_assert(!Diagnose,
                    "The type and operation does not define inverse! "
                    "Please see documentation how to define an inverse "
                    "for a type and operation.");
    }
  }
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
