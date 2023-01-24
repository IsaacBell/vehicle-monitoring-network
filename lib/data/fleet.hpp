#include <boost/property_tree/ptree.hpp>
#include "quadtree.hpp"

#include "../util/security.hpp"

class Fleet
{
private:
  using PointType = boost::geometry::model::point<T, 2, boost::geometry::cs::cartesian>;
  using BoxType = boost::geometry::model::box<PointType>;

  QuadTree<Vehicle &> tree(BoxType(PointType(0, 0), PointType(100, 100)), 10);

  std::string_view uuid_;
  std::string_view ownerUuid_;

public:
  Fleet(User &owner) : uuid(generate_uuid()), ownerUuid_(owner.uuid()) {}

  std::string_view uuid() { return uuid_; }
  std::string_view ownerUuid() { return ownerUuid_; }

  void insert(double lat, double lon, Vehicle &vehicle) { tree.insert(lat, lon, vehicle); }

  void insert(double lat, double lon, Diagnostic &diag)
  {
    Vehicle vehicle(diag, *this);
    vehicle.save();
    tree.insert(lat, lon, vehicle);
  }
};
