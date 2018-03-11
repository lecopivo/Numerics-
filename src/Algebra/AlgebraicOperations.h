#pragma once

#include "../Utils/MetaUtils.h"
#include "AlgebraicTraits.h"

#include <complex>
#include <type_traits>

namespace Numerics {
namespace Algebra {

// Concepts:
//
//  1. Operation - lambda preforming given operation
//
//  2. Operation type - wierd templated expression representing the

// Following naming convention from
// https://en.wikipedia.org/wiki/Operators_in_C_and_C

///////////////////////////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////////////////////////

// Should these lambdas have std::forward inside?

// Arithmetic operators
auto basic_assignment  = [](auto &&a, auto &&b) { return a = b; };
auto addition          = [](auto &&a, auto &&... b) { return (a + ... + b); };
auto subtraction       = [](auto &&a, auto &&b) { return a - b; };
auto unary_plus        = [](auto &&a) { return +a; };
auto unary_minus       = [](auto &&a) { return -a; };
auto multiplication    = [](auto &&a, auto &&... b) { return (a * ... * b); };
auto division          = [](auto &&a, auto &&b) { return a / b; };
auto modulo            = [](auto &&a, auto &&b) { return a % b; };
auto increment_prefix  = [](auto &&a) { return ++a; };
auto increment_postfix = [](auto &&a) { return a++; };
auto decrement_prefix  = [](auto &&a) { return --a; };
auto decrement_postfix = [](auto &&a) { return a--; };

// Comparison operators/relational operators
auto equal_to     = [](auto &&a, auto &&... b) { return (a == ... == b); };
auto not_equal_to = [](auto &&a, auto &&b) { return (a != b); };
auto greater_then = [](auto &&a, auto &&b) { return a > b; };
auto less_then    = [](auto &&a, auto &&b) { return a < b; };
auto greater_then_or_equal_to = [](auto &&a, auto &&b) { return a >= b; };
auto less_then_or_equal_to    = [](auto &&a, auto &&b) { return a <= b; };

// Logical operators

// Bitwise operators

// Compound assignment operators
auto addition_assigment       = [](auto &&a, auto &&b) { return a += b; };
auto subtraction_assigment    = [](auto &&a, auto &&b) { return a -= b; };
auto multiplication_assigment = [](auto &&a, auto &&b) { return a *= b; };
auto division_assigment       = [](auto &&a, auto &&b) { return a /= b; };

///////////////////////////////////////////////////////////////////////////////
// Operation Types
///////////////////////////////////////////////////////////////////////////////

// Arithmetic operators
template <typename T, typename S>
using basic_assignment_t = decltype(std::declval<T>() = std::declval<S>());

template <typename T, typename... S>
using addition_t = decltype((std::declval<T>() + ... + std::declval<S>()));

template <typename T, typename S>
using subtraction_t = decltype(std::declval<T>() - std::declval<S>());

template <typename T>
using unary_plus_t = decltype(+std::declval<T>());

template <typename T>
using unary_minus_t = decltype(-std::declval<T>());

template <typename T, typename... S>
using multiplication_t =
    decltype((std::declval<T>() * ... * std::declval<S>()));

template <typename T, typename S>
using division_t = decltype(std::declval<T>() / std::declval<S>());

template <typename T, typename S>
using modulo_t = decltype(std::declval<T>() % std::declval<S>());

template <typename T>
using increment_prefix_t = decltype(++std::declval<T>());

template <typename T>
using increment_postfix_t = decltype(std::declval<T>()++);

template <typename T>
using decrement_prefix_t = decltype(--std::declval<T>());

template <typename T>
using decrement_postfix_t = decltype(std::declval<T>()--);

//

template <template <typename...> typename OperationType, typename... T>
constexpr bool has_operation = Utils::is_detected<OperationType, T...>{};

template <typename T, template <typename...> typename OperationType,
          typename = std::true_type>
struct operation_traits {
  template <typename S>
  using unit_t = decltype(0);
};

template <typename T>
struct operation_traits<
    T, addition_t, std::enable_if_t<std::is_arithmetic_v<T>, std::true_type>> {
  static constexpr T unit = 0;
};

template <typename T>
struct operation_traits<
    T, multiplication_t,
    std::enable_if_t<std::is_arithmetic_v<T>, std::true_type>> {
  static constexpr T unit = 1;
};

// template <typename T, typename = std::true_type>
// struct algebraic_traits {};

// template <typename T>
// struct algebraic_traits<
//     T, std::enable_if_t<std::is_arithmetic_v<T>, std::true_type>> {

//   static constexpr T zero = operation_traits<T, addition_t>::unit;
//   static constexpr T one  = operation_traits<T, multiplication_t>::unit;
// };
namespace detail {
template <template <typename...> typename OperationType>

struct unit_detector {
  template <typename T>
  using unit_t = decltype(operation_traits<T, OperationType>::unit);
};

} // namespace detail

template <typename T>
using haha = detail::unit_detector<addition_t>::unit_t<T>;

template <typename T, template <typename...> typename OperationType>
constexpr bool has_unit =
    Utils::is_detected<haha, T>{};

}; // namespace Algebra
} // namespace Numerics
