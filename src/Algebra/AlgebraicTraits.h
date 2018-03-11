#pragma once

#include <type_traits>

#include "../Utils/MetaUtils.h"
#include "AlgebraicOperations.h"

namespace Numerics {
namespace Algebra {

template <typename T, template <typename...> typename OperationType,
          typename = void>
struct algebraic_traits {};

template <typename T>
struct algebraic_traits<T, addition_t,
                        std::enable_if_t<std::is_arithmetic_v<T>>> {
  static constexpr T unit = 0;

  static T inverse(T const &x) { return -x; }
};

template <typename T>
struct algebraic_traits<T, multiplication_t,
                        std::enable_if_t<std::is_integral_v<T>>> {
  static constexpr T unit = 1;
};

template <typename T>
struct algebraic_traits<T, multiplication_t,
                        std::enable_if_t<std::is_floating_point_v<T>>> {
  static constexpr T unit = 1;
  static T           inverse(T const &x) { return 1 / x; }
};

template <typename T>
struct algebraic_traits<std::complex<T>, addition_t,
                        std::enable_if_t<std::is_arithmetic_v<T>>> {
  static constexpr std::complex<T> unit = {0, 0};
  static std::complex<T> inverse(std::complex<T> const &x) { return -x; }
};

template <typename T>
struct algebraic_traits<std::complex<T>, multiplication_t,
                        std::enable_if_t<std::is_integral_v<T>>> {
  static constexpr std::complex<T> unit = 1;
};

template <typename T>
struct algebraic_traits<std::complex<T>, multiplication_t,
                        std::enable_if_t<std::is_floating_point_v<T>>> {
  static constexpr std::complex<T> unit = 1;
  static std::complex<T> inverse(std::complex<T> const &x) { return 1 / x; }
};

namespace detail {
template <template <typename...> typename OperationType>

struct operation_detector {
  template <typename T>
  using unit_t = decltype(algebraic_traits<T, OperationType>::unit);

  template <typename T>
  using inverse_t =
      decltype(algebraic_traits<T, OperationType>::inverse(std::declval<T>()));
};

} // namespace detail

template <typename T, template <typename...> typename OperationType,
          bool Diagnose = false>
constexpr bool has_unit = []() {

  if constexpr /* is unit defined in `algebraic_traits` */
      (Utils::is_detected<
           detail::operation_detector<OperationType>::template unit_t, T>{}) {
    using unit_type = decltype(algebraic_traits<T, OperationType>::unit);

    if constexpr /* is unit assignable to T& ? */ (std::is_assignable_v<
                                                       T &, unit_type>) {
      return true;
    } else {
      static_assert(!Diagnose, "Defined unit is of an incorrect type!");
    }
  } else {
    static_assert(!Diagnose, "Type does not a define unit!");
  }
  return false;
}();

template <typename T, template <typename...> typename OperationType>
constexpr bool has_inverse = []() {
  if constexpr /* is inverse defined in algebraic_traits? */
      (Utils::is_detected<
           detail::operation_detector<OperationType>::template inverse_t,
           T>{}) {

    using inverse_type = decltype(
        algebraic_traits<T, OperationType>::inverse(std::declval<T>()));

    if constexpr /* is inverse assignable to T&? */ (std::is_assignable_v<
                                                         T &, inverse_type>) {
      return true;
    }
  }
  return false;
}();

template <typename T, bool Diagnose = true>
auto zero = []() {
  if constexpr (has_unit<T, addition_t, true>) {
    return algebraic_traits<T, addition_t>::unit;
  } else {
    return;
  }
}();

template <typename T>
auto one = []() {
  if constexpr (has_unit<T, multiplication_t, true>) {
    return algebraic_traits<T, multiplication_t>::unit;
  } else {
    return;
  }
}();

} // namespace Algebra
} // namespace Numerics
