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
    Polygon_2 generateRandomPolygon(int size, double radius);

    Polygon_2 qOneRoot() const;
    Polygon_2 qOneA() const;
    Polygon_2 qOneMiddle() const;
    Polygon_2 qTwoIntersection() const;
    Polygon_2 qTwoDomination() const;
    Polygon_2 qTwoGeneralClosed() const;
    Polygon_2 qTwoGeneralAltOpen() const;
    Polygon_2 qTwoGeneralOpen() const;
    Polygon_2 testPolygon() const;

private:

};

#endif
