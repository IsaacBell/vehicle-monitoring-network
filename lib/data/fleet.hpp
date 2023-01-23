#include <boost/property_tree/ptree.hpp>
#include "quadtree.hpp"

class Fleet
{
public:
  void insert(double lat, double lon, Vehicle &vehicle)
  {
    vehicle.fleetUuid_ = uuid;
    tree.insert(lat, lon, vehicle);
  }

  Fleet(User &owner) : ownerUuid_(owner.uuid) {}

private:
  using PointType = boost::geometry::model::point<T, 2, boost::geometry::cs::cartesian>;
  using BoxType = boost::geometry::model::box<PointType>;

  QuadTree<Vehicle &> tree(BoxType(PointType(0, 0), PointType(100, 100)), 10);

  std::string_view uuid;
  std::string_view ownerUuid;
};
