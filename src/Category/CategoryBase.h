#pragma once

#include <boost/hana.hpp>
#include <iostream>
#include <string_view>
#include <tuple>

template <class T>
constexpr std::string_view type_name() {
  using namespace std;
#ifdef __clang__
  string_view p = __PRETTY_FUNCTION__;
  return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
  string_view p = __PRETTY_FUNCTION__;
#if __cplusplus < 201402
  return string_view(p.data() + 36, p.size() - 36 - 1);
#else
  return string_view(p.data() + 49, p.find(';', 49) - 49);
#endif
#elif defined(_MSC_VER)
  string_view p = __FUNCSIG__;
  return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

template <template <typename...> typename F, typename... T>
struct template_instance {
  static constexpr bool is_instance = false;
};

template <template <typename...> typename F, typename... T>
struct template_instance<F, F<T...>> {
  static constexpr bool is_instance = true;

  template <std::size_t I>
  using get = typename std::tuple_element<I, std::tuple<T...>>::type;
};

template <template <typename...> typename F, typename... T>
constexpr bool is_template_instance_of =
    template_instance<F, F<T...>>::is_instance;

using namespace boost;

namespace concepts {

template <typename T, typename Category>
constexpr bool is_morphism = []() {
  auto has__source =
      hana::is_valid([](auto t) -> decltype(decltype(t)::type::source()) {});
  auto has__target =
      hana::is_valid([](auto t) -> decltype(decltype(t)::type::target()) {});

  if constexpr (has__source(hana::type_c<T>) && has__target(hana::type_c<T>)) {
    return Category::is_object(T::source()) && Category::is_object(T::target());
  }
  return false;
}();

template <typename T>
constexpr bool is_category = []() {
  auto has__is_object = hana::is_valid(
      [](auto t) -> decltype(decltype(t)::type::is_object(0)) {});
  auto has__is_morphism = hana::is_valid(
      [](auto t) -> decltype(decltype(t)::type::is_morphism(0)) {});

  return has__is_object(hana::type_c<T>) && has__is_morphism(hana::type_c<T>);
}();

template <typename T>
constexpr bool is_functor = []() {
  auto has__source =
      hana::is_valid([](auto t) -> typename decltype(t)::source_category{});
  auto has__target =
      hana::is_valid([](auto t) -> typename decltype(t)::target_category{});
}();

} // namespace concepts

auto a = hana::type_c<int>;

struct universal_category {

  template <typename Type>
  struct type_object {
    using type = Type;

    template <typename T>
    static constexpr bool is_element(T &&t) {
      return std::is_same_v<std::remove_reference_t<T>, Type>;
    }
  };

  template <template <typename...> typename T>
  struct template_object {};

  template <typename T>
  static constexpr bool is_object(T const &) {
    return true;
  }

  template <typename T>
  static constexpr bool is_morphism(T const &) {
    return concepts::is_morphism<T>;
  }

  template <typename SourceObj, typename TargetObj>
  struct morphism {
    static constexpr auto source() { return SourceObj{}; }
    static constexpr auto target() { return TargetObj{}; }
  };
};

////////////////////////////////////////////////////////////////////////
//  _____                             _                               //
// |_   _|   _ _ __   ___    ___ __ _| |_ ___  __ _  ___  _ __ _   _  //
//   | || | | | '_ \ / _ \  / __/ _` | __/ _ \/ _` |/ _ \| '__| | | | //
//   | || |_| | |_) |  __/ | (_| (_| | ||  __/ (_| | (_) | |  | |_| | //
//   |_| \__, | .__/ \___|  \___\__,_|\__\___|\__, |\___/|_|   \__, | //
//       |___/|_|                             |___/            |___/  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

struct type_category {

  ///////////////////////////////////
  //        _     _           _    //
  //   ___ | |__ (_) ___  ___| |_  //
  //  / _ \| '_ \| |/ _ \/ __| __| //
  // | (_) | |_) | |  __/ (__| |_  //
  //  \___/|_.__// |\___|\___|\__| //
  //           |__/                //
  //                               //
  ///////////////////////////////////

  template <typename Type>
  using object = universal_category::type_object<Type>;

  template <typename T>
  static constexpr bool is_object(T const &) {
    if constexpr (is_template_instance_of<object, T>) {
      return true;
    } else {
      return false;
    }
  }

  ////////////////////////////////////////////////////////
  //                             _     _                //
  //  _ __ ___   ___  _ __ _ __ | |__ (_)___ _ __ ___   //
  // | '_ ` _ \ / _ \| '__| '_ \| '_ \| / __| '_ ` _ \  //
  // | | | | | | (_) | |  | |_) | | | | \__ \ | | | | | //
  // |_| |_| |_|\___/|_|  | .__/|_| |_|_|___/_| |_| |_| //
  //                      |_|                           //
  //                                                    //
  ////////////////////////////////////////////////////////

  template <typename Fun, typename SourceType, typename TargetType>
  struct morphism {
    morphism(Fun &&_fun, object<SourceType>, object<TargetType>)
        : fun(std::forward<Fun>(_fun)) {
      using namespace hana::literals;
      static_assert(std::is_invocable_r_v<TargetType, Fun, SourceType>,
                    "Invalid arguments"); // type_name<TargetType>() " = "
                                          // type_name< Fun>() "("
                                          // type_name<SourceType>() ") is
                                          // invalid!");
    }

    static constexpr auto source() { return object<SourceType>{}; }
    static constexpr auto target() { return object<TargetType>{}; }

    TargetType operator()(SourceType x) {
      return fun(std::forward<SourceType>(x));
    }

    Fun fun;
  };

  template <typename T>
  static constexpr bool is_morphism(T const &) {
    if constexpr (concepts::is_morphism<T, type_category>) {

      using SrcType = typename decltype(T::source())::type;
      using TrgType = typename decltype(T::target())::type;

      return std::is_invocable_r_v<TrgType, T, SrcType>;
    }
    return false;
  }

  ////////////////////////////////////////////
  //                      _            _    //
  //  _ __  _ __ ___   __| |_   _  ___| |_  //
  // | '_ \| '__/ _ \ / _` | | | |/ __| __| //
  // | |_) | | | (_) | (_| | |_| | (__| |_  //
  // | .__/|_|  \___/ \__,_|\__,_|\___|\__| //
  // |_|                                    //
  //                                        //
  ////////////////////////////////////////////

  struct product /* multi_functor_concept */ {
    template <int I>
    using source_category = type_category;
    using target_category = type_category;

    template <typename... Objs>
    static constexpr auto map(Objs const &... objs) {

      static_assert((is_object(objs) && ...), "Invalid arguments!");

      return object<std::tuple<typename Objs::type...>>{};
    }

    /* canonical projection
     */
    template <std::size_t I, typename ProductObj>
    static auto pi(ProductObj const &obj) {

      static_assert(is_product(obj), "Invalid argument!");

      using Src_t = typename ProductObj::type;
      using Trg_t = typename template_instance<
          std::tuple, typename ProductObj::type>::template get<I>;

      return morphism{[](auto &&s) -> decltype(auto) {

                        static_assert(std::is_same_v<Src_t, decltype(s)>,
                                      "Invalid argument");

                        return std::get<I>(std::forward<decltype(s)>(s));
                      },
                      object<Src_t>{}, object<Trg_t>{}};
    }

    // template <typename... Morphs>
    // static auto fmap(Morphs &&... morphs) {

    //   static_assert((is_morphism(morphs) && ...), "Invalid arguments!");

    //   constexpr int N = sizeof...(morphs);

    //   using SrcObj = decltype(map(morphs.source()...));
    //   using TrgObj = decltype(map(morphs.target()...));
    //   using Src_t  = typename SrcObj::type;
    //   using Trg_t  = typename TrgObj::type;

    //   return morphism{
    //       [morphs...](Src_t &&s) mutable -> decltype(auto) {

    //         auto morp = [&](auto I) { return pack_get<I>(morphs...); };
    //         auto proj = [&](auto I) { return pi<I>(object<Src_t>{}); };
    //         auto fun  = [&](auto I) { return compose(morp(I), proj(I)); };

    //         auto impl = [&](auto... I) {
    //           return std::make_tuple(fun(I)(std::forward<Src_t>(s))...);
    //         };

    //         return std::apply(impl, integral_sequence<N>);
    //       },
    //       object<Src_t>{}, object<Trg_t>{}};
    // }
  };
}; // namespace type_category

struct function_object_category {

  template <typename SourceType, typename TargetType>
  struct object {
    static constexpr auto source() {
      return type_category::object<SourceType>{};
    }
    static constexpr auto target() {
      return type_category::object<TargetType>{};
    }

    template <typename T>
    static constexpr bool is_element(T const &) {
      using TI = template_instance<type_category::morphism, T>;
      if constexpr (TI::is_instance) {
        return std::is_same_v<typename TI::template get<0>, SourceType> &&
               std::is_same_v<typename TI::template get<1>, TargetType>;
      } else {
        return false;
      }
    }
  };

  template <typename T>
  static constexpr bool is_object(T) {
    if constexpr (is_template_instance_of<object, T>) {
      return true;
    } else {
      return false;
    }
  }
};
