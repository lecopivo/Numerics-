#include <iostream>

#include "../Algebra/AlgebraicOperations.h"
#include "../Algebra/AlgebraicStructures.h"
#include "../Algebra/ContainerFunctor.h"

#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

using namespace Numerics;
using namespace Numerics::Algebra;

class A {
public:
  using haha = double;
};

int main() {

  std::cout << "Hello template Hell!" << std::endl;

  std::cout << is_magma<double>(addition) << std::endl;
  std::cout << is_magma<double>(division) << std::endl;
  std::cout << is_magma<double>(multiplication) << std::endl;
  std::cout << is_magma<double>(addition) << std::endl;

  // std::cout << is_group<double>(addition,negative) << std::endl;
  // std::cout << is_group<double>(multiplication,[](auto &&a){return 1/a;}) <<
  // std::endl;

  A a, b, c;

  std::cout << is_dynamic_container<std::vector<double>> << std::endl;
  std::cout
      << is_dynamic_container<std::tuple<double, int, float>> << std::endl;
  std::cout
      << is_dynamic_container<std::variant<double, int, float>> << std::endl
      << std::endl;
  ;

  std::cout << is_static_container<std::vector<double>> << std::endl;
  std::cout << is_static_container<std::tuple<double, int, float>> << std::endl;
  std::cout
      << is_static_container<std::variant<double, int, float>> << std::endl;

  // v3 = container_functor(addition)(v1,v2);
  // t3 = container_functor(addition)(t1,t2);

  // container_functor<is_magma(addition),v1>;
  // container_functor<is_magma(addition),t1>;

  std::tuple<double, int>    t1{0.1, 2}, t2{0.4, 3};
  std::tuple<double, double> t3{0.1, 0.5};
  std::tuple<A, A>           tA;

  std::array<double, 3> a1{1, 2, 3};
  std::array<int, 3>    a2{-3, 1, 0};
  std::array<double, 2> a3{1, 2};

  auto print_tuple = [](auto const &t) {
    std::apply(
        [](auto const &... x) {
          ((std::cout << typeid(x).name() << ": " << x << " | "), ...);
        },
        t);
  };

  print_tuple(t1);

  std::cout << std::endl;

  auto tuple_add = static_container_functor(addition);
  print_tuple(tuple_add(t1, t2));

  std::cout << std::endl;

  auto tuple_sub = static_container_functor(subtraction);
  print_tuple(tuple_sub(t1, t3));

  std::cout << std::endl;

  auto tuple_div = static_container_functor(division);
  print_tuple(tuple_div(t1, t3));

  std::cout << std::endl;

  print_tuple(tuple_add(a1,a2));

  // constexpr auto s = Utils::integral_sequence<0,10>;

  // std::cout << std::get<std::get<8>(s)>(s);

  return 0;
}
