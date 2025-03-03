#ifndef APPROXIMATEQUERY_H
#define APPROXIMATEQUERY_H

#include "common.h"

#include "onepointquery.h"
#include "twopointquery.h"
#include "shortestpath.h"

#include <CGAL/Exact_circular_kernel_2.h>
#include <CGAL/Circular_kernel_intersections.h>
#include <CGAL/Circular_arc_point_2.h>
#include <CGAL/Circular_arc_2.h>
#include <CGAL/Line_arc_2.h>

#include <QElapsedTimer>

typedef CGAL::Exact_circular_kernel_2 CK;
typedef CK::Circle_2 Circle_2;
typedef CK::Line_2 Line;
typedef CK::Point_2 Point;
typedef CK::Circular_arc_point_2 Circular_arc_point_2;
typedef CK::Circular_arc_2 Circular_arc_2;
typedef CK::Line_arc_2 Line_arc_2;

typedef std::variant<std::pair<Circular_arc_point_2, unsigned>, Circular_arc_2, Line_arc_2, Line, Circle_2> var;


class ApproximateQuery
{
public:
	ApproximateQuery();

	void threeApproximateQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh);
	QVector<QPointF> nApproximateQuery(QPointF& startingPoint, QVector<QPointF>& queryPoints, Polygon_2& polygon, Surface_mesh& mesh);
	void nEpsilonApproximateQuery(double epsilon, QPointF& startingPoint, QVector<QPointF>& queryPoints, Polygon_2& polygon, Surface_mesh& mesh);
	QVector<QPointF> getThreeApproximatePath();

	void epsilonApproximateQuery(double epsilon, QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh);

	Line convertToCLine(QLineF& line);

	QVector<QPointF> computeDiscIntersection(QPointF& circleCenter, Circle_2& disc, QLineF& window);

	bool isPointOnLine(const QLineF& line, const QPointF& point);

	double calculatePointToPointDistance(const QPointF& a, const QPointF& b);

	QVector<QPointF> generateEquallySpacedPoints(QLineF& line, double spacedDistance);

	QVector<QPointF> findShortestPathAmongPairs(QVector<QPointF>& spacedPoints1, QVector<QPointF>& spacedPoints2, QPointF& startingPoint, Polygon_2& polygon, Surface_mesh& mesh);

	QVector<QPointF> findShortestPathAmongVectorOfVectors(QVector<QVector<QPointF>>& spacedPointsGroups, QPointF& startingPoint, Polygon_2& polygon, Surface_mesh& mesh);

	struct ApproximateResult {
		QVector<QPointF> threeApproxPath;
		QLineF window1;
		QLineF window2;
		double discRadius;
		QLineF intersectionWindow1;
		QLineF intersectionWindow2;
		QVector<QPointF> equallySpacedPoints1;
		QVector<QPointF> equallySpacedPoints2;
		QVector<QPointF> shortestPath;
	};

	struct NApproximateResult {
		QVector<QPointF> nApproxPath;
		QVector<QLineF> windows;
		double discRadius;
		QVector<QLineF> intersectionWindows;
		QVector<QVector<QPointF>> equallySpacedPointsGroup;
		QVector<QPointF> shortestPath;
        QVector<QPointF> exactShortestPath;
	};

	ApproximateQuery::ApproximateResult getApproximateResult();
	ApproximateQuery::NApproximateResult getNApproximateResult();


private:
	QPointF m_startingPoint;
	QPointF m_queryPoint1;
	QPointF m_queryPoint2;
	bool m_startSelected;
	bool m_query1Selected;
	bool m_query2Selected;

	OnePointQuery m_onePointHandler;
	TwoPointQuery m_twoPointHandler;
	ShortestPath m_shortestPathHandler;

	OnePointQuery::QueryResult resultQ1;
	OnePointQuery::QueryResult resultQ2;
	QVector<QPointF> threeApproximatePath;

	ApproximateResult approximateResult;
	NApproximateResult nApproximateResult;
	QVector<QLineF> windowsVector;
};

#endif
