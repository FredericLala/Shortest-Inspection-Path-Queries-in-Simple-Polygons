#ifndef TwoPointQuery_H
#define TwoPointQuery_H

#include "ShortestPath.h"
#include "OnePointQuery.h"
#include "generalcase.h"
#include "common.h"
#include "cmath"

class TwoPointQuery
{
public:
	TwoPointQuery();

	QVector<QPointF> convertToQT(std::vector<Point_3> points);

	QVector<QPointF> shortestPathToSegment(QPointF start, QLineF segment, Polygon_2& polygon, Surface_mesh& mesh);

	bool dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath);

	double calculatePathLength(const QVector<QPointF>& path);


	void executeTwoPointQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh);

	void intersectionCase(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	void dominationCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh mesh);

	void computeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	struct IntersectionResult {
		QVector<QPointF> intersectionPath1;
		QVector<QPointF> intersectionPath2;
		QVector<QPointF> intersectionPath3;
		QVector<QPointF> optimalPath;
	};

	struct DominationResult {
		QVector<QPointF> optimalPath;
	};

	enum Q2CASE { QNONE, INTERSECTION, DOMINATION, GENERAL };

	struct QueryResult {
		bool visibility = false;
		OnePointQuery::QueryResult resultQ1;
		QLineF window1;
		QLineF window2;
		Q2CASE currentCase = QNONE;
	};

	GeneralCase::GeneralCaseResult getGeneralCaseResult();
	IntersectionResult getIntersectionResult();
	DominationResult getDominationResult();
	QueryResult getQ2Result();


private:
	ShortestPath m_shortestPathHandler;
	OnePointQuery m_onePointHandler;
	GeneralCase m_generalCaseHandler;

	OnePointQuery::QueryResult resultQ1;

	Q2CASE currentCase;

	GeneralCase::GeneralCaseResult resultGeneral;

	IntersectionResult resultIntersection;

	DominationResult resultDomination;

	QueryResult resultQ2;
};

#endif
