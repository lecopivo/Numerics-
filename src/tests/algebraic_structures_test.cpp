#include <iostream>

#include "../Algebra/AlgebraicStructures.h"
#include "../Algebra/ContainerFunctor.h"
#include "../Algebra/EigenTraits.h"

#include <complex>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include <eigen3/Eigen/Dense>

using namespace Numerics;
using namespace Numerics::Algebra;

class A {};

template <typename T, template <typename...> typename OperationType>
void magma_check(std::string operation_name = "") {
  if constexpr (is_magma<T, OperationType>) {
    std::cout << typeid(T).name() << " is " << operation_name << " magma."
              << std::endl;

  } else {
    std::cout << typeid(T).name() << " is not " << operation_name << " magma."
              << std::endl;
  }
}

template <typename T, template <typename...> typename OperationType>
void monoid_check(std::string operation_name = "") {
  if constexpr (is_monoid<T, OperationType>) {
    std::cout << typeid(T).name() << " is " << operation_name << " monoid."
              << std::endl;
  } else {
    std::cout << typeid(T).name() << " is not " << operation_name << " monoid."
              << std::endl;
  }
}

template <typename T, template <typename...> typename OperationType>
void group_check(std::string operation_name = "") {
  if constexpr (is_group<T, OperationType>) {
    std::cout << typeid(T).name() << " is " << operation_name << " group."
              << std::endl;
  } else {
    std::cout << typeid(T).name() << " is not " << operation_name << " group. "
              << std::endl;
  }
}

template <typename T, template <typename...> typename OperationType>
void structure_check(std::string operation_name = "") {
  magma_check<T, OperationType>(operation_name);
  monoid_check<T, OperationType>(operation_name);
  group_check<T, OperationType>(operation_name);
}

template <>
struct algebraic_traits<Eigen::Vector2f, addition_t> {
  static const Eigen::Vector2f unit;

  static auto inverse(Eigen::Vector2f const &x) { return -x; }
};

const Eigen::Vector2f algebraic_traits<Eigen::Vector2f, addition_t>::unit =
    Eigen::Vector2f::Ones();

int main() {
  std::cout << std::endl;

  structure_check<int, addition_t>("addition");
  structure_check<int, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<double, addition_t>("addition");
  structure_check<double, multiplication_t>("multiplication");

  std::cout << std::endl;
  structure_check<std::complex<int>, addition_t>("addition");
  structure_check<std::complex<int>, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<std::complex<double>, addition_t>("addition");
  structure_check<std::complex<double>, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<Eigen::Vector2i, addition_t>("addition");
  structure_check<Eigen::Vector2i, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<Eigen::Vector2f, addition_t>("addition");
  structure_check<Eigen::Vector2f, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<Eigen::Vector2d, addition_t>("addition");
  structure_check<Eigen::Vector2d, multiplication_t>("multiplication");

  std::cout << std::endl;

  structure_check<Eigen::Matrix2d, addition_t>("addition");
  structure_check<Eigen::Matrix2d, multiplication_t>("multiplication");

  std::cout << std::endl;

  std::cout << std::endl << zero<double> << std::endl;

  std::cout << std::endl << one<double> << std::endl;

  std::cout << std::endl << zero<Eigen::Matrix2d> << std::endl;

  std::cout << std::endl << one<Eigen::Matrix2d> << std::endl;
}
