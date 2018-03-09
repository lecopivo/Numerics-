#pragma once

#include "../Utils/MetaUtils.h"
#include "AlgebraicOperations.h"

#include <iostream>
#include <tuple>
#include <typeinfo>

namespace Numerics {
namespace Algebra {

template <typename T>
using dynamic_container_detector =
    decltype(std::declval<T>().begin(), std::declval<T>().end(),
             std::declval<T>().size(), std::declval<T>().resize(0));

template <typename T>
using static_container_detector = decltype(
    std::get<0>(std::declval<std::remove_reference_t<
                    T>>())); //, std::tuple_size_v<std::remove_reference_t<T>>);

template <typename T>
constexpr bool is_dynamic_container =
    Utils::is_detected<dynamic_container_detector, T>{};

template <typename T>
constexpr bool is_static_container =
    Utils::is_detected<static_container_detector, T>{};

template <typename T>
constexpr bool is_container = is_dynamic_container<T> || is_static_container<T>;

template <typename Op> auto static_container_functor(Op &&op) {
  return [&op](auto &&c, auto &&... d) {
    static_assert((is_static_container<decltype(c)> && ... &&
                   is_static_container<decltype(d)>),
                  "All of the arguments must be static containers!");

    constexpr int N = std::tuple_size_v<std::remove_reference_t<decltype(c)>>;

    static_assert(
        (true == ... ==
         (N == std::tuple_size_v<std::remove_reference_t<decltype(d)>>)),
        "All of the arguments must have the same length!");

#warning I should somehow check if the return type should be std::array or std::tuple
    
    auto getIthSlice = [&](auto I) {
      static_assert(std::is_invocable_v<Op, decltype(std::get<I>(c)),
                                        decltype(std::get<I>(d))...>,
                    "Invalid arguments!");
      return std::make_tuple(std::get<I>(c), std::get<I>(d)...);
    };

    auto callOperations = [&](auto... I) {
      return std::make_tuple(std::apply(op, getIthSlice(I))...);
    };

    return std::apply(callOperations, Utils::integral_sequence<N>);
  };
}

// template <typename Op> auto dynamic_container_functor(Op &&op) {
//   return [&op](auto &&c, auto &&... d) {
//     static_assert((is_dynamic_container<decltype(c)> && ... &&
//                    is_dynamic_container<decltype(d)>) ||
//                       (is_static_container<decltype(c)> && ... &&
//                        is_static_container<decltype(d)>),
//                   "All of the arguments must be containers of same
//                   type(either " "dynamic or static)!");
//     if constexpr (is_dynamic_container<decltype(c)>) {
//       assert(
//           (c.size() == ... == d.size())); // containers must have the same
//           size

//       auto idx = make_tuple(c.begin(), d.begin()...);
//       auto end = make_tuple(c.end(), d.end()...);

//       while (static_container_functor(not_equal)(idx, end)) {
//         //
//         std::apply([](auto &&... ptrs) { return op((*ptrs)...); }, idx);

//         static_container_functor
//       }
//     }
//   };
// }

} // namespace Algebra
} // namespace Numerics
