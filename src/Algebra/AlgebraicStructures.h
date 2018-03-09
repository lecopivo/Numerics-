#pragma once

#include <type_traits>

namespace Numerics {
namespace Algebra {

// One operation structures //
// --------------------------- //

// Magma
template <typename T, typename Op> constexpr bool is_magma(Op &&op) {
  return std::is_invocable_v<Op, T, T> &&
         std::is_assignable_v<T &, std::invoke_result_t<Op, T, T>>;
}

// Monoid
template <typename T, typename Op, typename Inv>
constexpr bool is_monoid(Op &&op, T identity) {
  return is_magma<T>(op);
}

// Semigroup

// Group
template <typename T, typename Op, typename Inv>
constexpr bool is_group(Op &&op, Inv &&inv, T identity) {
  return is_magma<T>(op) && std::is_invocable_v<Inv, T> &&
         std::is_assignable_v<T &, std::invoke_result_t<Inv, T>>;
}

// Two operation structures //
// ------------------------ //

// Ring
template <typename T, typename Add, typename Neg, typename Mul>
constexpr bool is_group(Add &&add, Neg &&neg, Mul &&mul, T zero, T one) {
  return is_group<T>(add, neg, zero) && is_monoid<T>(mul, one);
}

// Field

// Module

// Vector Space

} // namespace Algebra
} // namespace Numerics
