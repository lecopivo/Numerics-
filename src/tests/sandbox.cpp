#include <iostream>

#include "../Algebra/AlgebraicOperations.h"
#include "../Algebra/AlgebraicStructures.h"
#include "../Algebra/ContainerFunctor.h"

#include <complex>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include <eigen3/Eigen/Dense>

using namespace Numerics;
using namespace Numerics::Algebra;

class A {
public:
  using haha = double;
};

template <typename T> struct B {
  static void foo() {
    std::cout << "Foo called: " << typeid(T).name() << std::endl;
  }
};

template <typename T> void static_container_check() {
  if constexpr (is_static_container<T>) {
    std::cout << "Is static container:     " << typeid(T).name() << std::endl;
  } else {
    std::cout << "Is not static container: " << typeid(T).name() << std::endl;
  }
}

int main() {

  static_container_check<std::vector<double>>();
  static_container_check<std::tuple<double, int, float>>();
  static_container_check<std::variant<double, int, float>>();

  std::tuple<double, int>    t1{0.1, 2}, t2{0.4, 3};
  std::tuple<double, double> t3{0.1, 0.5};
  std::tuple<A, A>           tA;

  std::array<double, 3> a1{1, 2, 3};
  std::array<int, 3>    a2{-3, 1, 0};
  std::array<double, 2> a3{1, 2};

  float f;
  auto  print = [&f](auto const &x) -> void {
    std::cout << "| " << typeid(decltype(x)).name() << ": " << x << " |";
  };

  auto print_tuple = static_container_functor(print);

  print_tuple(t1);

  std::cout << std::endl << "add" << std::endl;

  auto tuple_add = static_container_functor(addition{});
  print_tuple(tuple_add(t1, t2));

  std::cout << std::endl << "sub " << std::endl;

  auto tuple_sub = static_container_functor(subtraction{});
  print_tuple(tuple_sub(t1, t3));

  std::cout << std::endl << "div" << std::endl;

  auto tuple_div = static_container_functor(division{});
  print_tuple(tuple_div(t1, t3));

  std::cout << std::endl;

  // constexpr auto s = Utils::integral_sequence<0,10>;

  // std::cout << std::get<std::get<8>(s)>(s);

  std::cout << std::endl;
  // std::cout << sizeof(Utils::integral_sequence<200>);
  std::cout << sizeof(std::make_index_sequence<200>);
  std::cout << std::endl;

  // bool val = has_addition<A,A>(addition);
  // bool val2 = has_operation<A,A>(addition);

  // std::cout << val << std::endl;
  // std::cout << val2 << std::endl;

  Eigen::Vector2f u, v;
  u << 1, 2;
  v << 3, 4;
  auto vt = std::make_tuple(u, v);

  auto ta = tuple_add(vt, vt);
  
  print_tuple(ta);

  return 0;
}
