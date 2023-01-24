#include <string>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>

using namespace boost::geometry;

struct PointWithUUID : public boost::geometry::model::point<CoordinateType, 2, boost::geometry::cs::cartesian>
{
public:
  std::string uuid;
};
