#include <array>
#include <iostream>
#include <tuple>

template <std::size_t... I>
constexpr auto integral_sequence_impl(std::index_sequence<I...>) {
  return std::make_tuple((std::integral_constant<std::size_t, I>{})...);
}

template <std::size_t N, typename Indices = std::make_index_sequence<N>>
constexpr auto integral_sequence = integral_sequence_impl(Indices{});

template <typename Op>
auto static_container_functor(Op &&op) {
  return [&op](auto &&c, auto &&... d) {
    /* here I should add test that c and d... are really tuples */

    constexpr int N = std::tuple_size_v<std::remove_reference_t<decltype(c)>>;
    static_assert(
        (true == ... ==
         (N == std::tuple_size_v<std::remove_reference_t<decltype(d)>>)),
        "All of the arguments must have the same length!");

    auto implementation = [&](auto... I) {
      auto slice = [&](auto idx) {
        return std::forward_as_tuple(std::get<idx>(c), std::get<idx>(d)...);
      };

      auto result = [&](auto idx) {
        return std::apply(op, std::forward<decltype(slice(idx))>(slice(idx)));
      };
      auto zero = std::integral_constant<size_t, 0>{};

      if constexpr /* Are all return types are void? Return void*/
          ((std::is_same_v<void, decltype(result(I))> && ... && true)) {

        (result(I), ...);
        return;

      } else /* All other cases. Return std::tuple  */ {
        using ReturnType = std::tuple<decltype(result(I))...>;
        return ReturnType{result(I)...};
      }
    };

    return std::apply(implementation, integral_sequence<N>);
  };
}

int main() {

  std::tuple<double, int> t1{3.14159, -1}, t2{2.71828, 2};

  auto print = [](auto x) { std::cout << x << " "; };

  auto tuple_print = static_container_functor(print);
  tuple_print(t1); // prints "3.14159 -1 " and returns void

  std::cout << std::endl;

  auto add       = [](auto &&x, auto &&... y) { return (x + ... + y); };
  auto tuple_add = static_container_functor(add);
  auto t3 = tuple_add(t1, t1, t2); // t3 is of type std::tuple<double,int>
  tuple_print(t3);                 // prints "9.00146 0 "

  return 0;
}
