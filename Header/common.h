#ifndef COMMON_H
#define COMMOM_H

#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_set_2.h>

#include <QPoint>
#include <QVector>
#include <QLine>

#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::list<Point_2> Container;
typedef CGAL::Polygon_2<K, Container> Polygon_2;
typedef CGAL::Polygon_set_2<K, Container> Polygon_set_2;

#endif