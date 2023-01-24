#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include "cassandra.hpp"
#include "../util/security.hpp"

class User
{
private:
public:
  std::string username_;
  std::string email_;
  std::string address_;
  std::string address2_;
  u_int8_t zipcode_;
  double lat_;
  double lon_;
  std::string phone_;
  std::string_view uuid_;

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

  bool save()
  {
    return Cassandra.save(*this);
  }

  const u_int8_t zipcode() const { return zipcode_; }
  const double lat() const { return lat_; }
  const double lon() const { return lon_; }
  const std::string &username() const { return username_; }
  const std::string &email() const { return email_; }
  const std::string &address() const { return address_; }
  const std::string &address2() const { return address2_; }
  const std::string &phone() const { return phone_; }
  const std::string_view &uuid() const { return uuid_; }
};
