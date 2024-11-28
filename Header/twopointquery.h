#ifndef TwoPointQuery_H
#define TwoPointQuery_H

#include "ShortestPath.h"
#include "OnePointQuery.h"
#include "common.h"

class TwoPointQuery
{
public:
	TwoPointQuery();

	void setStartingPoint(const QPointF& point);
	void setQueryPoint1(const QPointF& point);
	void setQueryPoint2(const QPointF& point);

	void clearPoints();

	QPointF getStartingPoint() const;
	QPointF getQueryPoint1() const;
	QPointF getQueryPoint2() const;

	bool isStartingPointSet() const;
	bool isQueryPoint1Set() const;
	bool isQueryPoint2Set() const;

	QVector<QPointF> convertToQT(std::vector<Point_3> points);
	QVector<QPointF> shortestPathToSegment(QPointF start, QLineF segment, Polygon_2& polygon);

private:
	QPointF m_startingPoint;
	QPointF m_queryPoint1;
	QPointF m_queryPoint2;
	bool m_startSelected;
	bool m_query1Selected;
	bool m_query2Selected;
	ShortestPath shortestPathHandler;
	OnePointQuery onePointHandler;
};

#endif // TwoPointQuery_H
