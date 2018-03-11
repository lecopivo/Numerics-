#pragma once

#include "../Utils/MetaUtils.h"

#include <complex>
#include <eigen3/Eigen/Dense>
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

struct basic_assignment_2 {

  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t = s;
  }

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() = std::declval<S>());

};

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
auto addition_assignment       = [](auto &&a, auto &&b) { return a += b; };
auto subtraction_assignment    = [](auto &&a, auto &&b) { return a -= b; };
auto multiplication_assignment = [](auto &&a, auto &&b) { return a *= b; };
auto division_assignment       = [](auto &&a, auto &&b) { return a /= b; };

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

template <typename T, typename S>
using addition_assignment_t = decltype(std::declval<T>() += std::declval<S>());

template <typename T, typename S>
using subtraction_assignment_t =
    decltype(std::declval<T>() -= std::declval<S>());

template <typename T, typename S>
using multiplication_assignment_t =
    decltype(std::declval<T>() *= std::declval<S>());

//

template <template <typename...> typename OperationType, typename... T>
constexpr bool has_operation = Utils::is_detected<OperationType, T...>{};

}; // namespace Algebra
} // namespace Numerics
