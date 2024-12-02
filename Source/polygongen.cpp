#include "polygongen.h"

PolygonGen::PolygonGen()
{
}

// Function to generate a random polygon and return the points
Polygon_2 PolygonGen::generateRandomPolygon(int size)
{
    Polygon_2 polygon;
    std::list<Point_2> point_set;
    CGAL::Random rand;

    const double RADIUS = 250;
    const int MAX_POLY_SIZE = 100;

    if (size < 4)
    {
        size = rand.get_int(4, MAX_POLY_SIZE);
    }

    // copy size points from the generator, eliminating duplicates
    CGAL::copy_n_unique(Point_generator(RADIUS), size, std::back_inserter(point_set));

    // Generate the polygon from the points
    CGAL::random_polygon_2(point_set.size(), std::back_inserter(polygon), point_set.begin());

    return polygon;
}

Polygon_2 PolygonGen::exampleOne() const
{
    Polygon_2 p;
    p.push_back(Point_2(18, -52));
    p.push_back(Point_2(96, -98));
    p.push_back(Point_2(46, -189));
    p.push_back(Point_2(141, -251));
    p.push_back(Point_2(-230, -113));
    p.push_back(Point_2(-63, 108));
    p.push_back(Point_2(-195, 115));
    p.push_back(Point_2(-193, 190));
    p.push_back(Point_2(-43, 189));
    p.push_back(Point_2(66, 89));
    p.push_back(Point_2(72, 190));
    p.push_back(Point_2(290, 189));
    p.push_back(Point_2(210, 126));
    p.push_back(Point_2(385, -145));
    p.push_back(Point_2(211, -201));
    p.push_back(Point_2(179, 10));
    p.push_back(Point_2(79, 20));
    p.push_back(Point_2(-48, -23));
    p.push_back(Point_2(-63, -129));
    p.push_back(Point_2(-20, -100));
    p.push_back(Point_2(9, -142));

    return p;
} 
