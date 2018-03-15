#pragma once

#include <iostream>
#include <tuple>
#include <type_traits>
#include <typeinfo>

#include "../Utils/MetaUtils.h"
#include "Category.h"

namespace Numerics {
namespace Category {

template <typename T>
using product_type_detector =
    decltype(std::get<0>(std::declval<std::remove_reference_t<T>>()),
             std::tuple_size_v<std::remove_reference_t<T>>);

template <typename T>
constexpr bool is_product_type = Utils::is_detected<product_type_detector, T>{};

template <typename T>
constexpr int product_size = std::tuple_size_v<std::remove_reference_t<T>>;

template <std::size_t I>
static decltype(auto) get = [](auto &&x)
    -> std::enable_if_t<is_product_type<decltype(x)>,
                        decltype(std::get<I>(std::forward<decltype(x)>(x)))> {
  return std::get<I>(std::forward<decltype(x)>(x));
};

template <std::size_t I, typename T>
using get_type = decltype(std::get<I>(std::declval<T>()));

template <typename T>
const std::string type_name = []() {
  std::string out = "";

  if constexpr (!is_product_type<T>) {
    out.append(typeid(T).name());
  } else {
    constexpr int N = product_size<T>;

    auto impl = [&](auto I, auto... J) {
      out.append("(");
      out.append(type_name<get_type<0, T>>);
      ((out.append("\u00D7"), out.append(type_name<get_type<J, T>>)), ...);
      out.append(")");
    };

    std::apply(impl, Utils::integral_sequence<N>);
  }

  // this is not working properly :(
  // if constexpr (std::is_lvalue_reference_v<T>)
  //   out.append("&");
  // if constexpr (std::is_rvalue_reference_v<T>)
  //   out.append("&&");

  return out;
}();

static auto make_morphism = [](auto &&fun) {
  return [fun](auto &&obj) -> std::enable_if_t<is_product_type<decltype(obj)>,
                                               decltype(std::apply(fun, obj))> {
    return std::apply(fun, obj);
  };
};

template <typename... Morph> auto product_extension(Morph &&... morph) {
  return [morph...](auto &&obj)
             -> std::enable_if_t<(std::is_invocable_v<Morph, decltype(obj)> &&
                                  ...),
                                 std::tuple<decltype(morph(obj))...>> {
    return std::make_tuple(morph(std::forward<decltype(obj)>(obj))...);
  };
}

static auto transpose = [](auto &&obj) {
  using Obj = decltype(obj);
  static_assert(is_product_type<Obj>, "Argument has to be a product type");

  constexpr int M = product_size<Obj>;
  constexpr int N = product_size<get_type<0, Obj>>;

  auto impl = [&](auto... J) {
    auto slice = [&](auto idx) {
      auto slice_impl = [&](auto... I) {
        return std::make_tuple(std::get<idx>(std::get<I>(obj))...);
      };
      return std::apply(slice_impl, Utils::integral_sequence<M>);
    };

    return std::make_tuple(slice(J)...);
  };

  return std::apply(impl, Utils::integral_sequence<N>);
};

static auto extend_functions = [](auto &&... f) {
  constexpr int N = sizeof...(f);

  auto impl = [f...](auto... I) {
    return product_extension(compose(make_morphism(f), get<I>)...);
  };

  auto fun = std::apply(impl, Utils::integral_sequence<N>);

  return
      [fun](auto &&... obj) { return fun(transpose(std::make_tuple(obj...))); };
};

}; // namespace Category
} // namespace Numerics
