#include <iostream>
#include <tuple>

#define BOOST_PFR_USE_LOOPHOLE 1
#define BOOST_PFR_USE_CPP17 1

#include <boost/pfr/precise.hpp>

using T = std::tuple<int, float>;

namespace boost {
namespace pfr {

template <class T, bool Diagnose = false>
constexpr std::size_t is_simple_struct = []() {
  using type = std::remove_cv_t<T>;

  bool result = true;

  result &= !std::is_reference<type>::value;
  static_assert(!Diagnose || !std::is_reference<type>::value,
                "Attempt to get fields count on a reference. This is "
                "not allowed because that could hide an issue and "
                "different library users expect different behavior in "
                "that case.");

  result &= std::is_copy_constructible<std::remove_all_extents_t<type>>::value;
  static_assert(
      !Diagnose ||
          std::is_copy_constructible<std::remove_all_extents_t<type>>::value,
      "Type and each field in the type must be copy constructible.");

  result &= !std::is_polymorphic<type>::value;
  static_assert(!Diagnose || !std::is_polymorphic<type>::value,
                "Type must have no virtual function, because otherwise it "
                "is not aggregate initializable.");

#ifdef __cpp_lib_is_aggregate
  result &=
      (std::is_aggregate<type>::value || std::is_standard_layout<type>::value);
  static_assert(
      !Diagnose ||
          (std::is_aggregate<type>::value // Does not return `true` for
                                          // build in types.
           || std::is_standard_layout<type>::value), // Does not return `true`
                                                     // for structs that have
                                                     // non standard layout
                                                     // members.
      "Type must be aggregate initializable.");
#endif

  // Can't use the following. See the non_std_layout.cpp test.
  //#if !BOOST_PFR_USE_CPP17
  //    static_assert(
  //        std::is_standard_layout<type>::value,   // Does not return
  //        `true` for structs that have non standard layout members. "Type
  //        must be aggregate initializable."
  //    );
  //#endif

  constexpr std::size_t max_fields_count =
      (sizeof(type) * 8); // We multiply by 8 because we may have bitfields in T
  constexpr std::size_t count = detail::detect_fields_count_dispatch<type>(
      boost::pfr::detail::size_t_<max_fields_count>{}, 1L);

  result &= detail::is_aggregate_initializable_n<type, count>::value;
  static_assert(!Diagnose ||
                    detail::is_aggregate_initializable_n<type, count>::value,
                "Types with user specified constructors (non-aggregate "
                "initializable types) are not supported.");

  result &=
      (count != 0 || std::is_empty<type>::value ||
       std::is_fundamental<type>::value || std::is_reference<type>::value);
  static_assert(!Diagnose || (count != 0 || std::is_empty<type>::value ||
                              std::is_fundamental<type>::value ||
                              std::is_reference<type>::value),
                "Something went wrong. Please report this issue to the "
                "github along with the structure you're reflecting.");

  return result;
}();
} // namespace pfr
} // namespace boost


struct S{
  static constexpr int b = 0;
  int a;
  std::vector<int> v;
};

int main() {

  // S s{1,2};

  T t{1, 3.1415};

  std::cout << boost::pfr::is_simple_struct<T> << std::endl;
  std::cout << boost::pfr::is_simple_struct<S> << std::endl;
  std::cout << boost::pfr::tuple_size_v<S> << std::endl;

  // using U = boost::pfr::detail::;

  // U u;

  // std::cout << u << std::endl;

  return 0;
}
