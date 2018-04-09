#include "../Category/CategoryBase.h"
#include <iostream>
#include <cmath>

int main() {

  constexpr int b = 1;

  using C = universal_category;
  std::cout << type_name<C>() << " is category: " << is_category<C> << std::endl;


  std::cout
      << is_morphism<C::morphism<int, double>> << std::endl;
  std::cout << is_morphism<int> << std::endl;

  std::cout << is_template_instance_of<std::tuple, std::tuple<int,float,double>> << std::endl;

  template_instance<std::tuple, std::tuple<int,float,double>>::get<2> a = 1.2;

  std::cout << type_name<decltype(a)>() << std::endl;

  auto m = type_category::make_morphism<double, double>(sin);

  std::cout << m(3.1415/2) << std::endl;

  return 0;
}
