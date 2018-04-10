#include "../Category/CategoryBase.h"
#include <cmath>
#include <iostream>

template <typename Obj> void object_test(Obj obj) {
  std::cout << type_name<Obj>() << std::endl;
  std::cout << "universal object: " << concepts::is_universal_object<Obj>
            << std::endl;
  std::cout << "set object:       " << concepts::is_set_object<Obj>
            << std::endl;
  std::cout << "type object:      " << concepts::is_type_object<Obj>
            << std::endl;
  std::cout << std::endl;
}

template <typename Morph> void morphism_test(Morph morph) {

  std::cout << type_name<Morph>() << std::endl;
  std::cout << "universal moprhism: "
            << concepts::is_universal_morphism<Morph,
                                               universal_category> << std::endl;
  std::cout << "set moprhism: "
            << concepts::is_set_morphism<Morph, set_category> << std::endl;

  std::cout << "type moprhism: "
            << concepts::is_type_morphism<Morph, type_category> << std::endl;

  std::cout << std::endl;
}

int main() {

  object_test(type_object<float>{});
  object_test(float{});

  auto sine   = [](float x) -> float { return sin(x); };
  auto cosine = [](float x) -> float { return cos(x); };
  auto um     = universal_morphism<float, float>{};
  auto sm =
      set_morphism<type_object<float>, type_object<float>, decltype(sine)>{
          std::move(sine)};
  auto tm = type_morphism<float, float, decltype(cosine)>{std::move(cosine)};

  morphism_test(um);
  morphism_test(sm);
  morphism_test(tm);

  std::cout << set_product<type_object<float>, type_object<int>>::is_element(
                   std::tuple{1.23f, 1})
            << std::endl;

  static_assert(type_object<float>::is_element(1.23f) == true, "Error");
  static_assert(type_object<int>::is_element(1) == true, "Error");
  static_assert(type_object<std::tuple<float, int>>::is_element(
                    std::tuple{1.23f, 1}) == true,
                "Error");

  return 0;
}
