#include "../Category/CategoryBase.h"
#include <iostream>
#include <cmath>

int main() {

  constexpr int b = 1;

  using C = universal_category;
  std::cout << type_name<C>() << " is category: " << concepts::is_category<C> << std::endl;


  std::cout
    << concepts::is_morphism<C::morphism<int, double>,C> << std::endl;
  std::cout << concepts::is_morphism<int,C> << std::endl;

  std::cout << is_template_instance_of<std::tuple, std::tuple<int,float,double>> << std::endl;

  template_instance<std::tuple, std::tuple<int,float,double>>::get<2> a = 1.2;

  std::cout << type_name<decltype(a)>() << std::endl;

  auto l = [](double x) {return sin(x);};

  // auto m = type_category::morphism{l,type_category::object<double>{}, type_category::object<double>{}};

  // std::cout << m(3.1415/2) << std::endl;

  // std::cout << type_category::is_morphism(m) << std::endl;

  // std::cout << function_object_category::object<double,double>::is_element(m) << std::endl;
  
  return 0;
}
