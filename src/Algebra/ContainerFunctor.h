#pragma once

#include "../Utils/MetaUtils.h"
#include "AlgebraicOperations.h"

#include <iostream>
#include <tuple>
#include <typeinfo>

namespace Numerics {
namespace Algebra {

template <typename T>
using static_container_detector =
    decltype(std::get<0>(std::declval<std::remove_reference_t<T>>()),
             std::tuple_size_v<std::remove_reference_t<T>>);

template <typename T>
constexpr bool is_static_container =
    Utils::is_detected<static_container_detector, T>{};

template <typename Op>
auto static_container_functor(Op &&op) {
  // if constexpr (sizeof(Op) > 8) {
  //     #warning "Operation seams to hold data larger than a single pointer.
  //     Operation is assumed to be very light weighted object. Consider
  //     modifying your operation."
  // }
  return [op](auto &&... c) {
    static_assert((is_static_container<decltype(c)> && ... && true),
                  "All of the arguments must be static containers!");

    constexpr int N = std::tuple_size_v<
        std::remove_reference_t<decltype(Utils::get<0>(c...))>>;

    static_assert(
        (true == ... ==
         (N == std::tuple_size_v<std::remove_reference_t<decltype(c)>>)),
        "All of the arguments must have the same length!");

    auto implementation = [&](auto... I) {
      auto slice = [&](auto idx) {
        return std::forward_as_tuple(std::get<idx>(c)...);
      };

      auto result = [&](auto idx) {
        return std::apply(op, std::forward<decltype(slice(idx))>(slice(idx)));
      };
      auto zero = std::integral_constant<size_t, 0>{};

      if constexpr /* Are all return types are void? Return void*/
          ((std::is_same_v<void, decltype(result(I))> && ... && true)) {

        (result(I), ...);
        return;

      } else if constexpr /* Are all return types the same? Return std::array */
          ((std::is_same_v<decltype(result(zero)), decltype(result(I))> &&
            ... && true)) {

        using ReturnType = std::array<decltype(result(zero)), N>;
        return ReturnType{result(I)...};

      } else /* All other cases. Return std::tuple  */ {

        using ReturnType = std::tuple<decltype(result(I))...>;
        return ReturnType{result(I)...};
      }
    };
    return std::apply(implementation, Utils::integral_sequence<N>);
  };
}

} // namespace Algebra
} // namespace Numerics
