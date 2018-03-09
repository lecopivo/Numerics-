#pragma once

#include <tuple>
#include <utility>

namespace Numerics {
namespace Utils {

template <class U, class... T> constexpr bool is_all_same() {
  return (std::is_same_v<U, T> && ... && true);
}

template <int I, typename... Ts>
using get_type = typename std::tuple_element<I, std::tuple<Ts...>>::type;

template <int I, class... Ts> constexpr decltype(auto) get(Ts &&... ts) {
  return std::get<I>(std::forward_as_tuple(ts...));
}

template <int First, int Last, class Lambda>
inline void static_for(Lambda const &f) {

  if constexpr (First < Last) {
    f(std::integral_constant<int, First>{});
    static_for<First + 1, Last>(f);
  }
}

template <std::size_t... I>
constexpr auto integral_sequence_impl(std::index_sequence<I...>) {
  return std::make_tuple((std::integral_constant<std::size_t, I>{})...);
}

template <std::size_t N, typename Indices = std::make_index_sequence<N>>
constexpr auto integral_sequence = integral_sequence_impl(Indices{});

// template <typename> struct ApplyNTimes_impl;

// template <std::size_t... I> struct
// ApplyNTimes_impl<std::index_sequence<I...>> {
//   template <typename V, int N> using type = V;

//   template <template <typename...> typename T, typename V>
//   using apply = T<type<V, I>...>;
// };

// template <int N>
// using ApplyNTimes = ApplyNTimes_impl<std::make_index_sequence<N>>;

// The "safer" way for
// template<typename... Ts> using void_t = void;
template <typename... Ts> struct make_void { using type = void; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

struct nonesuch {
  nonesuch()                 = delete;
  ~nonesuch()                = delete;
  nonesuch(nonesuch const &) = delete;
  void operator=(nonesuch const &) = delete;
};

namespace detail {
template <class Default, class AlwaysVoid, template <class...> class Op,
          class... Args>
struct detector {
  using value_t = std::false_type;
  using type    = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...> {
  using value_t = std::true_type;
  using type    = Op<Args...>;
};

} // namespace detail

// For details look at https://stackoverflow.com/a/38798091/1258071
template <template <class...> class Op, class... Args>
using is_detected =
    typename detail::detector<nonesuch, void, Op, Args...>::value_t;

////////////////////////////////////////////

} // namespace Utils
} // namespace Numerics
