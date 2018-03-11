#pragma once
#include "AlgebraicOperations.h"
#include "AlgebraicStructures.h"
#include "AlgebraicTraits.h"

#include <eigen3/Eigen/Dense>

namespace Numerics {
namespace Algebra {

#warning "Add support for Eigen::Array types"

// Traits

template <typename T, int N, int M>
struct algebraic_traits<
    Eigen::Matrix<T, N, M>, addition,
    std::enable_if_t<is_abelian_group<T, addition> && (N > 0) && (M > 0)>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = true;

  static const Eigen::Matrix<T, N, M> unit;
  static constexpr bool               all_invertible = std::is_signed_v<T>;
  static constexpr auto inverse(Eigen::Matrix<T, N, M> const &x) { return -x; }
};

template <typename T, int N, int M>
const Eigen::Matrix<T, N, M>
    algebraic_traits<Eigen::Matrix<T, N, M>, addition,
                     std::enable_if_t<is_abelian_group<T, addition> &&
                                      (N > 0) && (M > 0)>>::unit =
        Eigen::Matrix<T, N, M>::Zero();

///////////////////////////////////
// Matrix with com. ring entries //
///////////////////////////////////

template <typename T, int N, int M>
struct algebraic_traits<
    Eigen::Matrix<T, N, M>, multiplication,
    std::enable_if_t<is_commutative_ring<T, addition, multiplication> &&
                     !is_field<T, addition, multiplication> && (N > 0) &&
                     (M > 0) && (N == M)>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = false;

  static const Eigen::Matrix<T, N, M> unit;
  static constexpr bool               all_invertible = false;
  static constexpr auto inverse(Eigen::Matrix<T, N, M> const &x) { return x; }
};

template <typename T, int N, int M>
const Eigen::Matrix<T, N, M> algebraic_traits<
    Eigen::Matrix<T, N, M>, multiplication,
    std::enable_if_t<is_commutative_ring<T, addition, multiplication> &&
                     !is_field<T, addition, multiplication> && (N > 0) &&
                     (M > 0) && (N == M)>>::unit =
    Eigen::Matrix<T, N, M>::Identity();

///////////////////////////////
// Matrix with field entries //
///////////////////////////////
template <typename T, int N, int M>
struct algebraic_traits<
    Eigen::Matrix<T, N, M>, multiplication,
    std::enable_if_t<is_field<T, addition, multiplication> && (N > 0) &&
                     (M > 0) && (N == M)>> {
  static constexpr bool is_associative = true;
  static constexpr bool is_commutative = false;

  static const Eigen::Matrix<T, N, M> unit;
  static constexpr bool is_invertible(Eigen::Matrix<T, N, M> const &x) {
    return x.lu().isInvertible();
  }
  static constexpr auto inverse(Eigen::Matrix<T, N, M> const &x) {
    return x.inverse();
  }
};

template <typename T, int N, int M>
const Eigen::Matrix<T, N, M> algebraic_traits<
    Eigen::Matrix<T, N, M>, multiplication,
    std::enable_if_t<is_field<T, addition, multiplication> && (N > 0) &&
                     (M > 0) && (N == M)>>::unit =
    Eigen::Matrix<T, N, M>::Identity();

// template <typename T, int N, int M>
// struct algebraic_traits<Eigen::Matrix<T, N, M>, multiplication,
//                         std::enable_if_t<std::is_floating_point_v<T> &&
//                                          (N > 0) && (M > 0) && (N == M)>> {
//   static const Eigen::Matrix<T, N, M> unit;
//   static auto inverse(Eigen::Matrix<T, N, M> const &m) { return m.inverse();
//   }
// };

// template <typename T, int N, int M>
// const Eigen::Matrix<T, N, M>
//     algebraic_traits<Eigen::Matrix<T, N, M>, multiplication,
//                      std::enable_if_t<std::is_floating_point_v<T> && (N > 0)
//                      &&
//                                       (M > 0) && (N == M)>>::unit =
//         Eigen::Matrix<T, N, M>::Identity();

// // Structures

// template <typename T, int N, int M, bool Force = false>
// constexpr bool is_magma<Eigen::Matrix<T, N, M>, multiplication, Force> =
//     []() {
//       if constexpr (N == M) {
//         if constexpr (has_operation<multiplication, T>) {
//           return true;
//         } else {
//           static_assert(!Force, "Matrix scalar type cannot be multiplied!");
//         }
//       } else {
//         static_assert(
//             !Force, "Non-square matrices are not closed upon
//             multiplication!");
//       }
//       return false;
//     }();

} // namespace Algebra
} // namespace Numerics
