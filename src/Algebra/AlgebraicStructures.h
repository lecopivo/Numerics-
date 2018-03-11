#pragma once

#include <type_traits>

namespace Numerics {
namespace Algebra {

// One operation structures //
// --------------------------- //

// Magma
// template <typename T, template <typename...> typename OperationType>
// constexpr bool is_magma = has_operation<OperationType, T, T>
//     &&         has_operation<basic_assignment_t, T, OperationType<T, T>>;

template <typename T, template <typename...> typename OperationType>
constexpr bool is_magma = []() {
  // This is a tiny trick because && does is not short-circuted during
  // compilation
  if constexpr (has_operation<OperationType, T, T>)
    if constexpr (has_operation<basic_assignment_t, T &, OperationType<T, T>>)
      return true;
  return false;
}();

// Monoid
// template <typename T, template <typename...> typename OperationType>
// constexpr bool is_monoid =  {
//   return is_magma<T>(op);
// }

// Semigroup

// Group
// template <typename T, typename Op, typename Inv>
// constexpr bool is_group(Op &&op, Inv &&inv, T identity) {
//   return is_magma<T>(op) && std::is_invocable_v<Inv, T> &&
//          std::is_assignable_v<T &, std::invoke_result_t<Inv, T>>;
// }

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
