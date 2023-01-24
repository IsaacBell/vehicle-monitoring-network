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
struct BOOST_GEO_Y_AXIS
{
  static constexpr int value = 1;
}

template <>
struct BOOST_GEO_X_AXIS
{
  static constexpr int value = 0;
}
