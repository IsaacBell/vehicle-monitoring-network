# QuadTree

## For a layman:
A quadtree is a data structure that is used to organize data points in a grid-like structure. It is used to efficiently store and retrieve data points that are located in a two-dimensional space. This data structure can be used to represent areas of a map, images, or any other data that has a location in a 2D space.

## For an experienced developer:
A quadtree is a hierarchical spatial data structure that recursively subdivides a two-dimensional space into four quadrants or regions. Each internal node in the quadtree represents a region, and leaf nodes represent individual data points. The quadtree allows for efficient insertion, deletion, and search operations for data points by reducing the number of points that need to be searched as the tree is traversed. It is particularly useful for handling large datasets with many points and for data that changes frequently.

## In terms of time complexity and performance:
Quadtrees have a time complexity of O(log n) for insertion, deletion, and search operations, where n is the number of data points in the tree. This makes them an efficient data structure for large datasets, as the number of points that need to be searched decreases as the tree is traversed. The space complexity of a quadtree is O(n) as it stores all the data points in the tree. The performance of a quadtree can be affected by the balance of the tree, if the tree becomes unbalanced it may require more traversals to find a specific point. To avoid this, quadtrees can use balance algorithms such as k-d trees to keep the tree balanced which increases the time complexity of insertion and deletion to O(log^2 n) but keeps search complexity as O(log n)

## Template Parameters:

  - CoordinateType:
    - The type of the coordinates used by the quadtree.
    - This type must be an arithmetic type (e.g. int, float, double, etc.)
  - RecordType:
    - The type of the records that will be stored in the quadtree.
  - isProd:
    - A boolean value that determines whether the quadtree is in production or not.
  - MAX_N:
    - The maximum number of records that can be stored in a single quadtree node.
    - The value of this parameter will be different depending on whether the quadtree is in production or not.

  The std::enable_if check ensures that CoordinateType is an arithmetic type and will cause a compile error if the wrong type is passed.

### Example Usage

```
  QuadTree<double, std::string> tree(BoxType(PointType(0, 0), PointType(100, 100)), 10);
  tree.insert(50, 50, "my point");
```

```
  struct Building {
    std::string uuid;
    std::string name;
    double lat;
    double lon;
    // other fields...
  };

  QuadTree<double, Building> tree(BoxType(PointType(0, 0), PointType(100, 100)), 10);
  Building building;
  building.uuid = "uuid-uuid-uuid-uuid-uuid";
  building.name = "Sweat Hotel";
  building.lat = 1.1;
  building.lon = 50;
  tree2.insert(building.lat, building.lon, building);
```
