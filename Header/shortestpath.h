#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "common.h"

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
#include <CGAL/Surface_mesh_shortest_path.h>

#include <boost/lexical_cast.hpp>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>

#include <QLineF>

// Mesh
typedef K::Point_3 Point_3;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Face_handle Face_handle;

// Shortest Path
typedef CGAL::Surface_mesh_shortest_path_traits<K, Surface_mesh> Traits;
typedef CGAL::Surface_mesh_shortest_path<Traits> Surface_mesh_shortest_path;
typedef Surface_mesh_shortest_path::Face_location Face_location;
typedef boost::graph_traits<Surface_mesh> Graph_traits;
typedef Graph_traits::vertex_iterator vertex_iterator;
typedef Graph_traits::face_iterator face_iterator;
typedef Graph_traits::face_descriptor face_descriptor;

#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits_3.h>
typedef CGAL::AABB_face_graph_triangle_primitive<Surface_mesh> AABB_face_graph_primitive;
typedef CGAL::AABB_traits_3<K, AABB_face_graph_primitive> AABB_face_graph_traits;
typedef CGAL::AABB_tree<AABB_face_graph_traits> AABB_Tree;

class ShortestPath
{
public:
    ShortestPath();
    Surface_mesh createMesh(const Polygon_2 &polygon);
    void clearTree();
    bool is_point_on_polygon_edge(const Polygon_2& polygon, const Point_2& point);
    QVector<QPointF> findShortestPath(QPointF source2D, QPointF query2D, const Polygon_2 &polygon, const Surface_mesh& mesh);
    double findShortestPathLength(QPointF source2D, QPointF query2D, const Polygon_2& polygon, const Surface_mesh& mesh);
    Point_2 snapQueryInsidePolygon(QPointF& queryPoint, const Polygon_2& polygon);
    QVector<QPointF> point3VectorToQtVector(std::vector<Point_3>& points);
    const Point_2 &getPenultimate(const std::vector<Point_3> &path, const Polygon_2 &polygon) const;
    QVector<QPointF> reversePath(QVector<QPointF>& path);
    QPointF getLCA(QVector<QPointF>& path1, QVector<QPointF>& path2);

private:
    bool pointsSet;
    CDT cdt;
    std::vector<Point_3> points;
    std::vector<Point_3> shortestPath;
    Point_2 penultimate;
    AABB_Tree tree;
};

#endif
