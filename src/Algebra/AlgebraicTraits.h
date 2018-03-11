#pragma once

#include <type_traits>

#include "../Utils/MetaUtils.h"
#include "AlgebraicOperations.h"

namespace Numerics {
namespace Algebra {

template <typename T, template <typename...> typename OperationType,
          typename = void>
struct algebraic_traits {
  static constexpr bool is_associative = false;
  static constexpr bool is_commutative = false;
};

namespace detail {

template <template <typename...> typename Op>
struct detect {

  template <typename T>
  using is_associative_t = decltype(algebraic_traits<T, Op>::is_associative);

  template <typename T>
  using is_commutative_t = decltype(algebraic_traits<T, Op>::is_commutative);

  template <typename T>
  using unit_t = decltype(algebraic_traits<T, Op>::unit);

  template <typename T>
  using all_invertible_t = decltype(algebraic_traits<T, Op>::all_invertible);

  template <typename T>
  using is_invertible_t =
      decltype(algebraic_traits<T, Op>::is_invertible(std::declval<T>()));

  template <typename T>
  using inverse_t =
      decltype(algebraic_traits<T, Op>::inverse(std::declval<T>()));

  template <typename T>
  using is_division_ring_t = decltype(algebraic_traits<T, Op>::is_commutative);
};

} // namespace detail

template <typename T, template <typename...> typename Op>
constexpr bool has_unit =
    Utils::is_detected<detail::detect<Op>::template unit_t, T>{};

template <typename T, template <typename...> typename Op>
constexpr bool all_invertible = []() {
  if constexpr (Utils::is_detected<
                    detail::detect<Op>::template all_invertible_t, T>{}) {
    return algebraic_traits<T, Op>::all_invertible;
  } else {
    return false;
  }
}();

template <typename T, template <typename...> typename Op, bool Diagnose = true>
constexpr bool is_consistent = []() {

  if constexpr /* Operation Op is invalid on type T */ (!Utils::is_detected<
                                                            Op, T, T>{}) {

    static_assert(!Diagnose, "Invalid operation on give type!");
    return false;

  } else if constexpr /* Operation is not closed */
      (!std::is_assignable_v<T &, Op<T, T>>) {

    static_assert(!Diagnose, "Operation is not closed!");
    return false;
  }

  if constexpr /* Associativity is not defined */
      (!Utils::is_detected<detail::detect<Op>::template is_associative_t,
                           T>{}) {

    static_assert(!Diagnose, "Associativity is not defined");
    return false;
  }

  if constexpr /* Commutativity is not defined */
      (!Utils::is_detected<detail::detect<Op>::template is_commutative_t,
                           T>{}) {

    static_assert(!Diagnose, "Commutativity is not defined");
    return false;
  }

  if constexpr /* Unit is defined */
      (has_unit<T, Op>) {

    if constexpr /* Unit is of incorrect type */
        (!std::is_assignable_v<T &, decltype(algebraic_traits<T, Op>::unit)>) {

      static_assert(!Diagnose, "Unit is of incorect type");
      return false;
    }

    if constexpr /* Invertibility information */
        (!(Utils::is_detected<detail::detect<Op>::template all_invertible_t,
                              T>{} |
           Utils::is_detected<detail::detect<Op>::template is_invertible_t,
                              T>{})) {

      static_assert(!Diagnose,
                    "Incorrectly spcified which elements are invertible");
      return false;
    }

    if constexpr /* Inverse is not defined */
        (!Utils::is_detected<detail::detect<Op>::template inverse_t, T>{}) {

      static_assert(!Diagnose, "Inverse is not defined");
      return false;

    } else if constexpr /* Inverse is of incorrect type */
        (!std::is_assignable_v<T &, decltype(algebraic_traits<T, Op>::inverse(
                                        std::declval<T>()))>) {

      static_assert(!Diagnose, "Inverse is of incorrect type!");
      return false;
    }
  }
  return true;
}();

// (Z,+) or (R,+)
template <typename T>
struct algebraic_traits<T, addition_t,
                        std::enable_if_t<std::is_arithmetic_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr T    unit           = 0;
  static constexpr bool all_invertible = std::is_signed_v<T>;
  static constexpr T    inverse(T const &x) { return -x; }
};

// (Z,*)
//
template <typename T>
struct algebraic_traits<T, multiplication_t,
                        std::enable_if_t<std::is_integral_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr T    unit = 1;
  static constexpr bool is_invertible(T const &x) {
    if (x * x == unit)
      return true;
    return false;
  }
  static constexpr T inverse(T const &x) { return x; }
};

// (R,*)
//
template <typename T>
struct algebraic_traits<T, multiplication_t,
                        std::enable_if_t<std::is_floating_point_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr T    unit = 1;
  static constexpr bool is_invertible(T const &x) { return x != 0; }
  static constexpr T    inverse(T const &x) { return 1 / x; }
};

// (C,+) or Gaussian integers
//
template <typename T>
struct algebraic_traits<std::complex<T>, addition_t,
                        std::enable_if_t<std::is_arithmetic_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr std::complex<T> unit          = 0;
  static constexpr bool            all_invertible = std::is_signed_v<T>;
  static constexpr std::complex<T> inverse(std::complex<T> const &x) {
    return -x;
  }
};

// Gaussian integers with multiplication
//
template <typename T>
struct algebraic_traits<std::complex<T>, multiplication_t,
                        std::enable_if_t<std::is_integral_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr std::complex<T> unit = 1;
  static constexpr bool            is_invertible(std::complex<T> const &x) {
    return x.norm() == 1;
  }
  static constexpr std::complex<T> inverse(std::complex<T> const &x) {
    return x.conj();
  }
};

// (C,*)
template <typename T>
struct algebraic_traits<std::complex<T>, multiplication_t,
                        std::enable_if_t<std::is_floating_point_v<T>>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static constexpr std::complex<T> unit = 1;
  static constexpr bool            is_invertible(std::complex<T> const &x) {
    return x != 0;
  }
  static constexpr std::complex<T> inverse(std::complex<T> const &x) {
    return 1 / x;
  }
};

// template <typename T, template <typename...> typename OperationType,
//           bool Diagnose = false>
// constexpr bool has_unit = []() {

//   if constexpr /* unit is not defined in `algebraic_traits` */
//       (!Utils::is_detected<
//            detail::operation_detector<OperationType>::template unit_t,
//            T>{})
//            {

//     static_assert(!Diagnose, "Type does not define an unit!");
//     return false;

//   } else if constexpr /* Unit is not assignable to T& */
//       (!std::is_assignable_v<T &, algebraic_traits<T,
//       OperationType>::unit>)
//       {

//     static_assert(!Diagnose, "Defined unit is of an incorrect type!");
//     return false;

//   } else if constexpr /* Inverse element function not defined */
//       (!Utils::is_detected<
//            detail::operation_detector<OperationType>::template
//            inverse_t>{})
//            {

//     static_assert(!Diagnose, "Inverse function is not defined!");
//     return false;

//   } else if constexpr /* Not defined which elements are invertible */
//       (!(Utils::is_detected<detail::operation_detector<
//                                 OperationType>::template all_invertible_t,
//                             T>{} ||
//          Utils::is_detected<detail::operation_detector<OperationType>::
//                                 template conditionally_invertible_t,
//                             T>{})) {

//     static_assert(!Diagnose, "Not specified which elements are
//     invertible!"); return false;

//   } else if constexpr /* Inverse is of incorect type*/
//       (!std::is_assignable_v<T &, algebraic_traits<T,
//       OperationType>::inverse(
//                                       std::declval<T>())>) {

//     static_assert(!Diagnose, "Inverse has incorrect type!");
//     return false;
//   } else {
//     return true;
//   }
// }();

// template <typename T, bool Diagnose = true>
// auto zero = []() {
//   if constexpr (has_unit<T, addition_t, true>) {
//     return algebraic_traits<T, addition_t>::unit;
//   } else {
//     return;
//   }
// }();

// template <typename T>
// auto one = []() {
//   if constexpr (has_unit<T, multiplication_t, true>) {
//     return algebraic_traits<T, multiplication_t>::unit;
//   } else {
//     return;
//   }
// }();

} // namespace Algebra
} // namespace Numerics
