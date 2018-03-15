#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

static auto compose = [](auto &&f, auto &&g) {
  return [ f, g ](auto &&obj)
             -> std::enable_if_t<
                 std::is_invocable_v<decltype(g), decltype(obj)> &&
                     std::is_invocable_v<decltype(f), decltype(g(obj))>,
                 decltype(f(g(obj)))> {
    return f(g(std::forward<decltype(obj)>(obj)));
  };
};

