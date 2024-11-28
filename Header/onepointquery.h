#ifndef ONEPOINTQUERY_H
#define ONEPOINTQUERY_H

#include "common.h"

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/AABB_segment_primitive_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Line_2.h>

typedef K::Point_3 Point;
typedef K::Segment_2 Segment_2;
typedef K::Line_2 Line_2;

typedef std::vector<Segment_2>::iterator Iterator;

// AABB Tree setup
typedef CGAL::AABB_segment_primitive_2<K, Iterator> Primitive;
typedef CGAL::AABB_traits_2<K, Primitive> AABB_traits;
typedef CGAL::AABB_tree<AABB_traits> AABB_tree;
typedef std::optional<AABB_tree::Intersection_and_primitive_id<Segment_2>::Type> Segment_intersection;

//
typedef K::Ray_2 Ray;
typedef std::optional<AABB_tree::Intersection_and_primitive_id<Ray>::Type> Ray_intersection;
//

typedef CGAL::Orientation Orientation;

class OnePointQuery
{
public:
	OnePointQuery();
	void setStartingPoint(const QPointF& point);
	void setQueryPoint(const QPointF& point);
	void clearPoints();
	QPointF getStartingPoint() const;
	QPointF getQueryPoint() const;

	// Check if points are set
	bool isStartingPointSet() const;
	bool isQueryPointSet() const;

	bool checkVisibilty(const QPointF& point1, const QPointF& point2, Polygon_2& polygon);
	void clearTree();
	void shootRayExtended(const QPointF& point1, const QPointF& point2, Polygon_2& polygon);
	Point_2 convertToCGALPoint(const QPointF& qtPoint);
	QPointF convertToQTPoint(const Point_2& cgalPoint);
	Point_2 getIntersection();
	double calculateFunnelAngle(const QPointF& point1, const QPointF& point2, const QPointF& a, const QPointF& b);
	QPointF calculateWindowIntersection(const QPointF& pathPoint, const QPointF& windowStart, const QPointF& windowEnd);

	QPointF computeOptimalPoint(QVector<QPointF>& pathRA, QVector<QPointF>& pathRB, QLineF& window);

	int binarySearchByAngle(QVector<QPointF>& path, QPointF& a, QPointF& b);

private:
	QPointF m_startingPoint;
	QPointF m_queryPoint;
	bool m_startSelected; // Whether the starting point is set
	bool m_querySelected; // Whether the first query point is set
	bool visibilty;       // Whether q is visible from s
	Point_2 b;
	AABB_tree tree;
	std::vector<Segment_2> edges;
	double calculateAngle(const K::Vector_2& v1, const K::Vector_2& v2);
};

#endif