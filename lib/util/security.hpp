#include <cstdlib>
#include <iostream>
#include <string>
#include <random>

template <typename T = std::string_view>
const T generate_uuid()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 15);
  std::uniform_int_distribution<> dis2(8, 11);
  std::string uuid = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";

  for (auto &c : uuid)
  {
    if (c == '-')
      continue;
    if (c == 'x')
      c = dis(gen) < 10 ? dis(gen) + '0' : dis(gen) - 10 + 'a';
    else
      c = dis2(gen) < 10 ? dis2(gen) + '0' : dis2(gen) - 10 + 'a';
  }

  return uuid;
}

// select between integer and real uniform distribution, depending on whether
// the type template parameter is of an integral or floating point type
template <typename T,
          typename D = std::conditional_t<
              std::is_integral<T>::value,
              std::uniform_int_distribution<T>,
              std::uniform_real_distribution<T>>,
          typename = typename std::enable_if<
              std::is_arithmetic<T>::value>::type>
std::vector<T> generate_random(T const min, T const max,
                               size_t const size)
{
  std::vector<T> v(size);

  std::random_device rd{};
  std::mt19937 mt{rd()};

  D dist{min, max};

  std::generate(std::begin(v), std::end(v),
                [&dist, &mt]
                { return dist(mt); });

  return v;
}
