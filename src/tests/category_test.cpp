#include "../Category/Category.h"
#include "../Category/Product.h"
#include "../Category/Void.h"

#include <iostream>
#include <string>
#include <variant>
#include <vector>

using std::tuple;
using namespace Numerics::Category;

template <int I>
auto cget = [](auto &&t) { return std::get<I>(t); };

struct A {

  float            a;
  double           b;
  std::vector<int> vec;
};

template <template <typename...> typename F>
struct functor /*type class*/ {

  template <typename T>
  static constexpr bool is_member = false;

  // static auto transpose = [](obj&&

  template <typename... T>
  static constexpr auto make = [](auto &&... obj) { return F<T...>{obj...}; };

  // /* lifts functions */
  // static auto lift = [](auto&&...f){
  // 		       return [f...](auto&&...x) ->
  // std::enable_if_t<(is_member<decltype(x)> && ...), transpose(make(x...))>{

  // 		       }
  // 		     };
};

template <template <typename...> typename F>
template <typename... T>
constexpr bool functor<F>::is_member<F<T...>> = true;

template <template <typename...> typename F>
template <typename T>
constexpr auto functor<F>::make<T> =
    [](auto &&... obj) { return F<decltype(obj)...>{obj...}; };

// template <typename R, typename... Args>
// struct function_class {

//   template <typename Fun>
//   static constexpr bool is_member = std::is_invocable_r_v<R, Args...>;
// };

////

template <auto... T>
constexpr auto Type0Product = std::tuple{T...};

template <auto... T>
constexpr auto ValueProduct = Type0Product<T...>;

////

template <typename... T>
class TypeProduct {
private:
  TypeProduct() {}
};

////

template <template <typename...> typename... T>
struct Type1Product {
private:
  Type1Product() {}
};

template <template <typename...> typename... T>
using FunctorProduct = Type1Product<T...>;

////

template <template <template <typename...> typename...> typename... T>
struct Type2Product {
private:
  Type2Product() {}
};

////

namespace detail {
template <typename T>
struct Product {
  using type = std::tuple<T>;
};

template <typename... T>
struct Product<TypeProduct<T...>> {
  using type = std::tuple<T...>;
};

} // namespace detail

template <typename T>
using Product = typename detail::Product<T>::type;

using T1 = TypeProduct<int, double>;
using T2 = Type1Product<std::tuple, std::variant, TypeProduct>;
using T3 = Type2Product<Type1Product, Type1Product>;


int main() {


  Product<int>                      a;
  Product<TypeProduct<int, double>> b;

  bool one = functor<tuple>::is_member<tuple<int &, float>>;
  bool two = functor<tuple>::is_member<int>;

  // auto ahadf = functor<tuple>::make(1, 2, 3.54);

  std::cout << one << std::endl;
  std::cout << two << std::endl;

  auto f1 = [](int x) -> int { return 2 * x; };
  auto f2 = [](double x) -> double { return x / 2; };
  auto f3 = [](std::string x) -> int { return x.length(); };
  auto f4 = [](double x) -> std::string { return std::to_string(x); };

  std::tuple<int, double, std::string> t{1, 3.1416, "hovno"};

  auto print = [](auto &&x) -> int {
    std::cout << x << " ";
    return 0;
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

  using haha = tuple<void, void>;

  using hehe = decltype(std::declval<haha>());
  using ii   = decltype(std::declval<int>());
  // using hoho = product_type_detector<void>;

  // std::cout << "asdf" << std::endl;
  // std::cout << typeid(hehe).name() << std::endl;

  std::variant<int, Void> v = 1;

  return 0;
}
