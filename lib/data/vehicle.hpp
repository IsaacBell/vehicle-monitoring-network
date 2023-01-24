#include <chrono>
#include <memory>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "quadtree.hpp"
#include "cassandra.hpp"

std::cout << std::fixed << std::setprecision(9) << std::left;

class Vehicle
{
public:
  std::string_view ownerUuid() { return fleet().ownerUuid_; }

  Vehicle(Fleet &fleet) : fleetUuid(fleet.uuid()) {}
  Vehicle(Diagnostic &diag, Fleet &fleet) : fleetUuid(fleet.uuid())
  {
    diags_.push_back(std::make_unique<Diagnostic> diag);
    save(diag);
  }

  bool operator==(Diagnostic &diag)
  {
    return false; // todo - ret. true if the incoming attributes are unchanged from current state
  }

  Diagnostic currentDiagnostic() { return diags_.back(); }

  static bool save(Diagnostic &diag)
  {
    auto v = Vehicle.find(diag.vehicleId);
    if (!!v && Vehicle != diag)
    {
      v.last_active_at_ = std::chrono::system_clock::now();
      return Cassandra.save(v);
    }
    if (!!v)
      v.last_active_at_ = std::chrono::system_clock::now();

    return false;
  }

  bool save(Diagnostic &diag)
  {
    last_active_at_ = std::chrono::system_clock::now();
    diags_.push_back(std::make_unique<Diagnostic> diag);
    return Cassandra.save(*this);
  }

  const std::string_view vehicleId() const { return currentDiagnostic().vehicleId; }
  const double engineRpm() const { return currentDiagnostic().engineRpm; }
  const double speed() const { return currentDiagnostic().speed; }
  const int fuelLevel() const { return currentDiagnostic().fuelLevel; }

private:
  std::vector<unique_ptr<Diagnostic>> diags_;
  std::string_view uuid_;
  std::string_view fleetUuid_;
  std::chrono::time_point last_active_at_;
};
