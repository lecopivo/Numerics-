#pragma once

#include "AlgebraicStructures.h"
#include "AlgebraicTraits.h"

namespace Numerics {
namespace Algebra {

// Traits

template <typename T, int N, int M>
struct algebraic_traits<
    Eigen::Matrix<T, N, M>, addition_t,
    std::enable_if_t<std::is_arithmetic_v<T> && (N > 0) && (M > 0)>> {
  static const Eigen::Matrix<T, N, M> unit;
  static auto inverse(Eigen::Matrix<T, N, M> const &m) { return -m; }
};

template <typename T, int N, int M>
const Eigen::Matrix<T, N, M> algebraic_traits<
    Eigen::Matrix<T, N, M>, addition_t,
    std::enable_if_t<std::is_arithmetic_v<T> && (N > 0) && (M > 0)>>::unit =
    Eigen::Matrix<T, N, M>::Zero();

template <typename T, int N, int M>
struct algebraic_traits<Eigen::Matrix<T, N, M>, multiplication_t,
                        std::enable_if_t<std::is_floating_point_v<T> &&
                                         (N > 0) && (M > 0) && (N == M)>> {
  static const Eigen::Matrix<T, N, M> unit;
  static auto inverse(Eigen::Matrix<T, N, M> const &m) { return m.inverse(); }
};

template <typename T, int N, int M>
const Eigen::Matrix<T, N, M>
    algebraic_traits<Eigen::Matrix<T, N, M>, multiplication_t,
                     std::enable_if_t<std::is_floating_point_v<T> && (N > 0) &&
                                      (M > 0) && (N == M)>>::unit =
        Eigen::Matrix<T, N, M>::Identity();

// Structures

template <typename T, int N, int M, bool Force = false>
constexpr bool is_magma<Eigen::Matrix<T, N, M>, multiplication_t, Force> =
    []() {
      if constexpr (N == M) {
        if constexpr (has_operation<multiplication_t, T>) {
          return true;
        } else {
          static_assert(!Force, "Matrix scalar type cannot be multiplied!");
        }
      } else {
        static_assert(
            !Force,
            "Non-square matrices are not closed upon multiplication!");
      }
      return false;
    }();

} // namespace Algebra
} // namespace Numerics
