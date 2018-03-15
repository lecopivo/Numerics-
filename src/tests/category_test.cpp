#include "../Category/Category.h"
#include "../Category/Product.h"
#include "../Category/Void.h"

#include <iostream>
#include <string>

using std::tuple;
using namespace Numerics::Category;

template <int I> auto cget = [](auto &&t) { return std::get<I>(t); };

int main() {

  auto f1 = [](int x) -> int { return 2 * x; };
  auto f2 = [](double x) -> double { return x / 2; };
  auto f3 = [](std::string x) -> int { return x.length(); };
  auto f4 = [](double x) -> std::string { return std::to_string(x); };

  std::tuple<int, double, std::string> t{1, 3.1416, "hovno"};

  auto print = [](auto &&x) -> Void {
    std::cout << x << " ";
    return {};
  };
  auto m1 = compose(f1, get<0>);
  auto m2 = compose(f2, get<1>);
  auto m3 = compose(f3, get<2>);

  auto[r1, r2, r3] = product_extension(m1, m2, m3)(t);

  auto product_print = product_extension(
      compose(print, get<0>), compose(print, get<1>), compose(print, get<2>));

  product_print(t);
  std::cout << std::endl;

  std::cout << type_name<decltype(t)> << std::endl;
  std::cout << r1 << std::endl;
  std::cout << r2 << std::endl;
  std::cout << r3 << std::endl;

  tuple<tuple<int, int *, long int>, tuple<float, float *, double>> o;

  auto to = transpose(o);

  std::cout << "Transpose of " << type_name<decltype(o)> << " is "
            << type_name<decltype(to)> << std::endl;

  using T = tuple<int, float, double>;
  T t1{1, 3.1415, -1.0}, t2{-2, 2.71828, 0.0};

  auto add = [](auto &&... x) { return (x + ...); };

  auto t3 = extend_functions(add, add, add)(t1);

  std::cout << type_name<decltype(t3)> << std::endl;

  product_print(t3);
  std::cout << std::endl;
  std::cout << type_name<T> << std::endl;
  std::cout << type_name<decltype(transpose(std::make_tuple(t1)))> << std::endl;

  return 0;
}
