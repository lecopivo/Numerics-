


template <typename...T>
std::tuple<T...> operator+(std::tuple<T...> const& t1, std::tuple<T...> const& t2){
  static_assert((true && ... && HasAddition<T>), "One of the types does not have addition!");
  std::tuple<T...> out;
  static_for<0,sizeof...(T)>([&](auto I){
			       constexpr int i=I.value;
			       std::get<i>(out) = std::get<i>(t1)+std::get<i>(t2);
			     });
  return out;
}

template <typename...T>
std::tuple<T...> operator-(std::tuple<T...> const& t1, std::tuple<T...> const& t2){
  static_assert((true && ... && HasAddition<T>), "One of the types does not have addition!");
  std::tuple<T...> out;
  static_for<0,sizeof...(T)>([&](auto I){
			       constexpr int i=I.value;
			       std::get<i>(out) = std::get<i>(t1)-std::get<i>(t2);
			     });
  return out;
}

