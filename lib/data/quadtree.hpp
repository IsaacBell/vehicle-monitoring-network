#include <map>
#include <memory>
#include <vector>
#include <utility>
#include "point_with_uuid.hpp"
#include "../util/geometry.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>

using namespace boost::geometry;

/*
  See README.md for documentation
*/

template <
    typename CoordinateType,
    typename RecordType,
    bool isProd = false,
    const int MAX_N = std::conditional<isProd, 500, 50>,
    typename = typename std::enable_if<
        std::is_arithmetic<CoordinateType>::value>::type>
class QuadTree
{
public:
  using PointType = PointWithUUID;                        // 2d points
  using BoxType = boost::geometry::model::box<PointType>; // bounding box
  using ElementType = std::pair<PointType, RecordType>;   // Ex: {{1,2}, my_var}
  using ElementContainer = std::vector<std::shared_ptr<ElementType>>;

  template <typename T> // does the RecordType respond to `.uuid()`
  using EnableIfHasUuidMemberFunc = std::enable_if_t<std::is_same<decltype(std::declval<T>().uuid()), std::string>::value>;

  template <typename T> // does the RecordType respond to `.coords_`
  using EnableIfHasCoordsMemberVar = std::enable_if_t<std::is_member_object_pointer<decltype(&T::coords_)>::value>;

  template <typename T>
  using EnableIfHasUuidFuncAndCoordsMemberVar = std::enable_if_t<EnableIfHasUuidMemberFunc<T>::value &&
                                                                 EnableIfHasCoordsMemberVar<T>::value>;

  QuadTree(const BoxType &b) : box_(b) {}

  bool isRoot() const noexcept { return parent_ == nullptr; }

  CoordinateType comparableDistance(PointType &p) { return boost::geometry::distance(p, boost::geometry::return_center(box_)); }

  template <typename T = RecordType, typename = EnableIfHasUuidFuncAndCoordsMemberVar<T>>
  bool insert(CoordinateType x, CoordinateType y, RecordType &record)
  {
    PointType point(x, y);
    point.uuid = record.uuid();
    record.coords_ = make_unique<PointType>(point);
    elements_.emplace_back(point, record);
    uuidLookup[uuid] = point;
    return true;
  }

  template <typename T = RecordType, typename = EnableIfHasUuidFuncAndCoordsMemberVar<T>>
  bool insert(RecordType &record) { return insert(record.lat, record.lon, record); }

  template <typename T = RecordType, typename = EnableIfHasUuidFuncAndCoordsMemberVar<T>>
  bool update(RecordType &record, CoordinateType x, CoordinateType y)
  {
    if (uuidLookup_.count(record.uuid()) &&auto uuid = uuidLookup_[record.uuid()])
    {
      const auto point = uuidLookup_[record.uuid()];
      boost::geometry::set<BOOST_GEO_X_AXIS::value>(point, x);
      boost::geometry::set<BOOST_GEO_Y_AXIS::value>(point, y);
    }
    return false;
  }

  // return a container of elements contained within the given bounding box
  ElementContainer search(CoordinateType xMin, CoordinateType yMin, CoordinateType xMax, CoordinateType yMax)
  {
    ElementContainer result;
    BoxType searchBox(PointType(xMin, yMin), PointType(xMax, yMax));

    for (const auto &element : elements_)
    {
      if (boost::geometry::within(element.first, searchBox))
      {
        result.push_back(element);
      }
    }

    return result;
  }

  /*
    Calculate the center of the bounding box of the current QuadTree using the return_center function from Boost::Geometry.
    Then create four new QuadTrees, one for each of the quadrants of the bounding box,
    by creating four new bounding boxes using the center point and the corners of the current bounding box.

    Store the new QuadTrees in children_
  */
  void split()
  {
    PointType center = boost::geometry::return_center(box_);
    T x = center.get<0>();
    T y = center.get<1>();

    children_[0] = std::make_shared<QuadTree>(*this, BoxType(box_.min_corner(), PointType(x, y)));
    children_[1] = std::make_shared<QuadTree>(*this, BoxType(PointType(x, box_.min_corner().get<1>()), PointType(box_.max_corner().get<0>(), y)));
    children_[2] = std::make_shared<QuadTree>(*this, BoxType(PointType(box_.min_corner().get<0>(), y), PointType(x, box_.max_corner().get<1>())));
    children_[3] = std::make_shared<QuadTree>(*this, BoxType(PointType(x, y), box_.max_corner()));
  }

  ElementType findNearestNeighbor(PointType point, size_t current_depth = 0)
  {
    if (elements_.empty() || current_depth >= MAX_N)
      return {};

    ElementType nearestNeighbor;
    double nearestDistance = std::numeric_limits<double>::max();

    for (const auto &element : elements_)
    {
      double distance = comparableDistance(element.first);
      if (distance < nearestDistance)
      {
        nearestDistance = distance;
        nearestNeighbor = element;
      }
    }

    for (const auto &child : children_)
    {
      if (child)
      {
        ElementType candidate = child->findNearestNeighbor(point, MAX_N, current_depth + 1);
        if (candidate.first == PointType())
        {
          continue;
        }
        double distance = comparableDistance(candidate.first);
        if (distance < nearestDistance)
        {
          nearestDistance = distance;
          nearestNeighbor = candidate;
        }
      }
    }

    return nearestNeighbor;
  }

private:
  BoxType box_;
  ElementContainer elements_;
  std::array<std::shared_ptr<QuadTree>, 4> children_;
  std::map<std::string_view, PointType &> uuidLookup_;
  std::shared_ptr<QuadTree> parent_ = nullptr;
};
