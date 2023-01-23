#include <vector>
#include <utility>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>

/*
  QuadTree<double> tree(BoxType(PointType(0, 0), PointType(100, 100)), 10);
  tree.insert(50, 50, "my point");

  struct Report {
    int id;
    std::string name;
    // other fields...
  };

  QuadTree<Report> tree2(BoxType(PointType(0, 0), PointType(100, 100)), 10);
  Report report = {1, "report 1"};
  tree2.insert(50, 50, report);

*/

template <typename T>
class QuadTree
{
public:
  using PointType = boost::geometry::model::point<T, 2, boost::geometry::cs::cartesian>;
  using BoxType = boost::geometry::model::box<PointType>;
  using ElementType = std::pair<PointType, T>;
  using ElementContainer = std::vector<ElementType>;

  QuadTree(const BoxType &b, BoxType b) : box_(b), max_depth_(1000000) {}
  QuadTree(const BoxType &b, size_t max_depth = 0) : box_(b), max_depth_(max_depth) {}

  bool isRoot() { return m_parent == nullptr; }

  double comparableDistance(PointType p)
  {
    PointType center;
    return boost::geometry::distance(p, boost::geometry::return_center(box_));
  }

  void insert(T x, T y, T value)
  {
    PointType point(x, y);
    elements_.emplace_back(point, value);
  }

  // return a container of elements contained within the given bounding box
  ElementContainer search(T xMin, T yMin, T xMax, T yMax)
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
    if (elements_.empty() || current_depth >= max_depth_)
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
        ElementType candidate = child->findNearestNeighbor(point, max_depth_, current_depth + 1);
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
  std::shared_ptr<QuadTree> m_parent;
  const size_t max_depth_;
};
