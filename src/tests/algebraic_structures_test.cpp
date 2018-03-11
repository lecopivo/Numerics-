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

template <typename T>
void abelian_group_check() {
  if constexpr (is_abelian_group<T>) {
    std::cout << typeid(T).name() << " is abelian group." << std::endl;
  } else {
    std::cout << typeid(T).name() << " is not abelian group." << std::endl;
  }
}

template <typename T>
void multiplicative_group_check() {
  if constexpr (is_multiplicative_group<T>) {
    std::cout << typeid(T).name() << " is multiplicative group." << std::endl;
  } else {
    std::cout << typeid(T).name() << " is not multiplicative group."
              << std::endl;
  }
}

template <typename T, template <typename...> typename OperationType>
void structure_check(std::string operation_name = "") {
  magma_check<T, OperationType>(operation_name);
  monoid_check<T, OperationType>(operation_name);
  group_check<T, OperationType>(operation_name);
}

template <class... T>
void test_types() {

  Utils::static_for<0, sizeof...(T)>([](auto I) {
    using type = Utils::get_type<I.value, T...>;

    std::cout << std::endl;

    structure_check<type, addition_t>("addition");
    structure_check<type, multiplication_t>("multiplicative");
    abelian_group_check<type>();
    multiplicative_group_check<type>();
    std::cout << "Zero is: " << std::endl << zero<type> << std::endl;

    if constexpr (is_multiplicative_group<type>) {
      std::cout << "One is: " << std::endl << one<type> << std::endl;
    }

  });
};

int main() {

  test_types<int, double, std::complex<int>, std::complex<double>,
             Eigen::Vector2i, Eigen::Vector2d, Eigen::Matrix2i,
             Eigen::Matrix2d>();
}
