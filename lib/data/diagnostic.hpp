#include <boost/property_tree/ptree.hpp>

struct Diagnostic
{
  std::string_view vehicleId;
  double speed;
  double engineRpm;
  int fuelLevel;

  Diagnostic(const boost::property_tree::ptree &json)
  {
    vehicleId = (std::string_view)json.get<std::string>("vehicleId");
    speed = json.get<double>("speed");
    engineRpm = json.get<double>("engineRpm");
    fuelLevel = json.get<int>("fuelLevel");
  }

  bool save() { return Vehicle.save(this); }
};
