#include <cstdlib>
#include <iostream>
#include <string>
#include <random>

class User
{
public:
  std::string username_;
  std::string email_;
  std::string address_;
  std::string address2_;
  u_int8_t zipcode_;
  double lat_;
  double lon_;
  std::string_view uuid_;
  std::string phone_;

  User(std::string username, std::string email, std::string address, std::string address2, u_int8_t zipcode, double lat, double lon, std::string phone)
      : username_(username),
        email_(email),
        address_(address),
        address2_(address2),
        lat_(lat),
        lon_(lon),
        phone_(phone),
        zipcode_(zipcode),
        uuid_(generate_uuid()) {}

  const std::string &username() const { return username_; }
  const std::string &email() const { return email_; }
  const std::string &address() const { return address_; }
  const std::string &address2() const { return address2_; }
  double lat() const { return lat_; }
  double lon() const { return lon_; }
  const std::string_view &uuid() const { return uuid_; }
  const std::string &phone() const { return phone_; }

private:
  const std::string_view generate_uuid() const
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
};
