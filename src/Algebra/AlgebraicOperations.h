#pragma once

#include "../Utils/MetaUtils.h"

#include <complex>
#include <string>
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

struct basic_assignment {
  static constexpr char name[] = "basic_assignment";

  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t = s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() = std::declval<S>());
};

struct addition {
  static constexpr char name[] = "addition";
  template <typename T, typename... S>
  auto operator()(T &&t, S &&... s) {
    return (t + ... + s);
  };

  template <typename T, typename... S>
  using result_type = decltype((std::declval<T>() + ... + std::declval<S>()));
};

struct subtraction {
  static constexpr char name[] = "subtraction";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t - s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() - std::declval<S>());
};

struct unary_plus {
  static constexpr char name[] = "unary_plus";
  
  template <typename T>
  auto operator()(T &&t) {
    return +t;
  };

  template <typename T>
  using result_type = decltype(+std::declval<T>());
};

struct unary_minus {
  static constexpr char name[] = "unary_minus";
  
  template <typename T>
  auto operator()(T &&t) {
    return -t;
  };

  template <typename T>
  using result_type = decltype(-std::declval<T>());
};

struct multiplication {
  static constexpr char name[] = "multiplication";
  
  template <typename T, typename... S>
  auto operator()(T &&t, S &&... s) {
    return (t * ... * s);
  };

  template <typename T, typename... S>
  using result_type = decltype((std::declval<T>() * ... * std::declval<S>()));
};

struct division {
  static constexpr char name[] = "division";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t / s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() / std::declval<S>());
};

struct modulo {
  static constexpr char name[] = "modulo";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t % s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() % std::declval<S>());
};

struct increment_prefix {
  static constexpr char name[] = "increment_prefix";
  
  template <typename T>
  auto operator()(T &&t) {
    return ++t;
  };

  template <typename T>
  using result_type = decltype(++std::declval<T>());
};

struct increment_postfix {
  static constexpr char name[] = "increment_postfix";
  
  template <typename T>
  auto operator()(T &&t) {
    return t++;
  };

  template <typename T>
  using result_type = decltype(std::declval<T>()++);
};

struct decrement_prefix {
  static constexpr char name[] = "decrement_prefix";
  
  template <typename T>
  auto operator()(T &&t) {
    return --t;
  };

  template <typename T>
  using result_type = decltype(--std::declval<T>());
};

struct decrement_postfix {
  static constexpr char name[] = "decrement_postfix";
  
  template <typename T>
  auto operator()(T &&t) {
    return t--;
  };

  template <typename T>
  using result_type = decltype(std::declval<T>()--);
};

struct equal_to {
  static constexpr char name[] = "equal_to";
  
  template <typename T, typename... S>
  auto operator()(T &&t, S &&... s) {
    return (t == ... == s);
  };

  template <typename T, typename... S>
  using result_type = decltype((std::declval<T>() == ... == std::declval<S>()));
};

struct not_equal_to {
  static constexpr char name[] = "not_equal_to";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return (t != s);
  };

  template <typename T, typename S>
  using result_type = decltype((std::declval<T>() != std::declval<S>()));
};

struct greater_then {
  static constexpr char name[] = "greater_then";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t > s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() > std::declval<S>());
};

struct less_then {
  static constexpr char name[] = "less_then";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t < s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() < std::declval<S>());
};

struct greater_then_or_equal_to {
  static constexpr char name[] = "greater_then_or_equal_to";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t >= s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() >= std::declval<S>());
};

struct less_then_or_equal_to {
  static constexpr char name[] = "less_then_or_equal_to";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t <= s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() <= std::declval<S>());
};

struct addition_assignment {
  static constexpr char name[] = "addition_assignment";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t += s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() += std::declval<S>());
};

struct subtraction_assignment {
  static constexpr char name[] = "subtraction_assignment";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t -= s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() -= std::declval<S>());
};

struct multiplication_assignment {
  static constexpr char name[] = "multiplication_assignment";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t *= s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() *= std::declval<S>());
};

struct division_assignment {
  static constexpr char name[] = "division_assignment";
  
  template <typename T, typename S>
  auto operator()(T &&t, S &&s) {
    return t /= s;
  };

  template <typename T, typename S>
  using result_type = decltype(std::declval<T>() /= std::declval<S>());
};

//

template <typename Op, typename... T>
constexpr bool has_operation =
    Utils::is_detected<Op::template result_type, T...>{};

}; // namespace Algebra
} // namespace Numerics
