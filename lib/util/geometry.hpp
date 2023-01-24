template <int N>
struct Factorial
{
  static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0>
{
  static constexpr int value = 1;
};

template <>
struct BOOST_GEO_RIGHT
{
  static constexpr int value = 1;
}

template <>
struct BOOST_GEO_LEFT
{
  static constexpr int value = 0;
}
