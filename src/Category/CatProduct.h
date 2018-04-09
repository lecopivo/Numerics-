#pragma once

#include <tuple>
#include <vector>

#include "../Utils/MetaUtils.h"

using namespace Numerics::Utils;

template <auto Val>
struct Type0 {};

template <typename T>
struct Type1 {};

template <template <typename...> typename T>
struct Type2 {};

template <typename... T>
struct TypeProduct {};

struct Category {

  template <typename T>
  static constexpr bool is_object = false;
};

template <int I>
struct TypeCategory : public Category {

  template <typename T>
  static constexpr bool is_object = false;
};

template <typename... Categories>
struct CategoryProduct : public Category {

  template <typename T>
  static constexpr bool is_object = false;
};

template <>
template <auto T>
constexpr bool TypeCategory<0>::is_object<Type0<T>> = true;

template <>
template <typename T>
constexpr bool TypeCategory<1>::is_object<Type1<T>> = true;

template <>
template <template <typename...> typename T>
constexpr bool TypeCategory<2>::is_object<Type2<T>> = true;

template <typename... Categories>
template <typename... T>
constexpr bool CategoryProduct<Categories...>::is_object<TypeProduct<T...>> =
    []() {
      if constexpr (sizeof...(Categories) != sizeof...(T)) {
        return false;
      } else {
        constexpr int N = sizeof...(T);
        static_for<0, N>([](auto I) {
			   
        });
      }
    }();

constexpr bool is_zero_in_Cat0 = TypeCategory<0>::is_object<Type0<0>>;
constexpr bool is_int_in_Cat0  = TypeCategory<0>::is_object<Type1<int>>;

// template <>
// struct Category<0> {

//   template <auto... vals>
//   struct Product {
//   private:
//     Product(){};
//   };

//   template <typename T>
//   static constexpr bool is_product = false;

//   template <typename T>
//   static constexpr auto get = false;
// };

// template <auto... vals>
// constexpr bool Category<0>::is_product<Category<0>::Product<vals...>> = true;

// template <>
// struct Category<1> {

//   template <typename... T>
//   using Product = std::tuple<T...>;

//   template <typename T>
//   static constexpr bool is_product = false;
// };

// template <>
// struct Category<2> {

//   template <template <typename...> typename... T>
//   struct Product {
//   private:
//     Product() {}
//   };

//   template <typename T>
//   static constexpr bool is_product = false;
// };

// template <>
// struct Category<3> {

//   template <template <typename...> typename... T>
//   struct Product {
//   private:
//     Product() {}
//   };

//   template <typename T>
//   static constexpr bool is_product = false;
// };
