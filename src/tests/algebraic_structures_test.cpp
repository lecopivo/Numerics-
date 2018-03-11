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
void group_like_structure_check(std::string operation_name = "") {
  std::cout << "(" << typeid(T).name() << "," << operation_name
            << ") is:" << std::endl;
  std::cout << "is consistent: "
            << is_consistent<T, OperationType, false> << std::endl;
  std::cout << "is semigroup:  " << is_semigroup<T, OperationType> << std::endl;
  std::cout << "is monoid:     " << is_monoid<T, OperationType> << std::endl;
  std::cout << "is group:      " << is_group<T, OperationType> << std::endl;
}

template <typename T, template <typename...> typename Add,
          template <typename...> typename Mul>
void ring_like_structure_check(std::string add_name = "+",
                               std::string mul_name = "*") {
  std::cout << "(" << typeid(T).name() << "," << add_name << "," << mul_name
            << ") is:" << std::endl;
  std::cout << "is ring:       " << is_ring<T, Add, Mul> << std::endl;
  std::cout << "is com. ring:  "
            << is_commutative_ring<T, Add, Mul> << std::endl;
  std::cout << "is div. ring:  "
            << is_commutative_ring<T, Add, Mul> << std::endl;
  std::cout << "is field:      "
            << is_commutative_ring<T, Add, Mul> << std::endl;
}

template <class... T>
void test_types() {

  Utils::static_for<0, sizeof...(T)>([](auto I) {
    using type = Utils::get_type<I.value, T...>;

    std::cout << std::endl;

    group_like_structure_check<type, addition_t>("+");
    group_like_structure_check<type, multiplication_t>("*");
    ring_like_structure_check<type, addition_t, multiplication_t>("+", "*");
  });
};

int main() {

  test_types<int, double>();

  test_types<std::complex<int>, std::complex<double>>();

  test_types<Eigen::Vector2i, Eigen::Vector2d>();
  test_types<Eigen::Matrix2i, Eigen::Matrix2d>();

  using at_add = algebraic_traits<Eigen::Matrix2d,addition_t>;
  using at_mul = algebraic_traits<Eigen::Matrix2d,multiplication_t>;

  auto zero = at_add::unit;
  auto one = at_mul::unit;

  // std::cout << std::endl << zero << std::endl;
  // std::cout << std::endl << one << std::endl;
  // std::cout << std::endl << at_mul::inverse(4*one) << std::endl;
  // std::cout << std::endl << at_add::inverse(4*one) << std::endl;
  // Eigen::Array2d>();
}
