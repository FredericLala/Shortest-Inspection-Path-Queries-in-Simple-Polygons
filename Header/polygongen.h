#ifndef POLYGONGEN_H
#define POLYGONGEN_H

#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Random.h>
#include <CGAL/algorithm.h>

#include <list>
#include "common.h"

typedef CGAL::Creator_uniform_2<int, Point_2> Creator;
typedef CGAL::Random_points_in_square_2<Point_2, Creator> Point_generator;

class PolygonGen
{
public:
    PolygonGen();
    Polygon_2 generateRandomPolygon(int size);
    Polygon_2 exampleOne() const;

    Polygon_2 exampleTwo() const;

    Polygon_2 exampleThree() const;

    Polygon_2 exampleFour() const;

    Polygon_2 exampleFive() const;

    Polygon_2 altIsOpen() const;

    Polygon_2 openHourglass() const;

    Polygon_2 testPolygon() const;

private:

};

#endif