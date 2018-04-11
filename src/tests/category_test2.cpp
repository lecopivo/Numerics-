#include "../Category/CategoryBase.h"
#include <cmath>
#include <iostream>

template <typename Obj> void object_test(Obj obj) {
  std::cout << type_name<Obj>() << std::endl;

  std::cout << "universal object: " << universal_category::is_object(obj)
            << std::endl;
  std::cout << "set object:       " << set_category::is_object(obj)
            << std::endl;
  std::cout << "type object:      " << type_category::is_object(obj)
            << std::endl;
  std::cout << std::endl;
}

template <typename Morph> void morphism_test(Morph morph) {

  std::cout << type_name<Morph>() << std::endl;
  std::cout << "universal moprhism: " << universal_category::is_morphism(morph)
            << std::endl;
  std::cout << "set moprhism: " << set_category::is_morphism(morph)
            << std::endl;

  std::cout << "type moprhism: " << type_category::is_morphism(morph)
            << std::endl;

  std::cout << std::endl;
}

int main() {

  auto to = type_object<float>{};
  auto go = float{};

  object_test(to);
  object_test(go);

  static_assert(concepts::is_type_object(to),
                "type_object<float> is not a type object but should be");
  static_assert(concepts::is_universal_object(go),
                "float is not an universal object but should be");

  auto sine   = [](float x) -> float { return sin(x); };
  auto cosine = [](float x) -> float { return cos(x); };
  auto um     = universal_morphism<float, float>{};
  auto sine_m =
      set_morphism<type_object<float>, type_object<float>, decltype(sine)>{
          sine};
  auto cos_m =
      type_morphism<type_object<float>, type_object<float>, decltype(cosine)>{
          cosine};

  morphism_test(um);
  morphism_test(sine_m);
  morphism_test(cos_m);

  auto um1 = universal_morphism<float, int>{};
  auto um2 = universal_morphism<int, double>{};
  auto um3 = universal_morphism<double, int>{};

  auto cum = composed_universal_morphism{um1, um2, um3};
  morphism_test(cum);

  auto csm = composed_set_morphism(sine_m, cos_m);

  auto o = csm(1.0f);
  
  std::cout << type_name<decltype(o)>() << " "<<  o << std::endl;

  // std::cout << set_product<type_object<float>, type_object<int>>::is_element(
  //                  std::tuple{1.23f, 1})
  //           << std::endl;

  // static_assert(type_object<float>::is_element(1.23f) == true, "Error");
  // static_assert(type_object<int>::is_element(1) == true, "Error");
  // static_assert(type_object<std::tuple<float, int>>::is_element(
  //                   std::tuple{1.23f, 1}) == true,
  //               "Error");

  return 0;
}
