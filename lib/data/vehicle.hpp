#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include "quadtree.hpp"
#include "cassandra.hpp"

std::cout << std::fixed << std::setprecision(9) << std::left;

class Vehicle
{
public:
  std::string_view ownerUuid() { return fleet().ownerUuid_; }

  Vehicle(Fleet &fleet) : fleetUuid(fleet.uuid_) {}

  bool operator==(Diagnostic &diag)
  {
    return false; // todo - ret. true if the incoming attributes are unchanged from current state
  }

  static bool save(Diagnostic &diag)
  {
    auto v = Vehicle.find(diag.vehicleId);
    if (!!v && Vehicle != diag)
    {
      v.last_active_at_ = std::chrono::system_clock::now();
      Cassandra.save(v);
    }
    else if (!!v)
      v.last_active_at_ = std::chrono::system_clock::now();
  }

private:
  std::vector<Diagnostic> diags_;
  std::string_view uuid_;
  std::string_view fleetUuid_;
  std::chrono::time_point last_active_at_;
};
