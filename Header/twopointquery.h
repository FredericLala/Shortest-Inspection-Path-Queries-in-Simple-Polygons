#ifndef TwoPointQuery_H
#define TwoPointQuery_H

#include "ShortestPath.h"
#include "OnePointQuery.h"
#include "common.h"
#include "cmath"

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

	bool dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath);

	double calculatePathLength(const QVector<QPointF>& path);

	bool hourglassOpen(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2);

	QPointF mirrorPoint(const QPointF& point, const QLineF& window);

	QVector<QPointF> concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent);

	QVector<QPointF> concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);

	QVector<QPointF> concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);

	QPointF getMFunnel();

	QPointF getMHourglass();

	int getFunnelIndex();

	int getHourglassIndex();

	bool searchFirstHalf(QPointF& m, int& mIndex, QVector<QPointF>& concatenatedSide, QPointF& mirrorPoint, QLineF& window2);

	QVector<QPointF> computeOptimalPathQ2(QVector<QPointF>& pathRA2, QVector<QPointF>& pathRB2, QPointF& m1, QPointF& m2, QPointF& m3, QPointF& m4, QLineF& window1, bool searchFirstHalf1, bool searchFirstHalf2);

	//void generalCase(QLineF& window1, QLineF& window2, Polygon_2& polygon);

	void findTangent(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	void findTangentEdgeHourglass(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	void findTangentEdgeFunnel(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	void findTangentEdgeBoth(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	void findTangentEdgeCase(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	int numberOfIntersections(const QLineF& line, const QVector<QPointF>& side);

	QVector<QPointF> tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	QVector<QPointF> removeElementFromVector(QVector<QPointF> vector, int index);

	bool isTangent(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon);

	bool doSegmentsOverlap(const QLineF& line1, const QLineF& line2);

	////////////////////
	// FOR TESTING
	QVector<QPointF> pathSA1;
	QVector<QPointF> pathSB1;
	QVector<QPointF> tangent1;
	QVector<QPointF> tangent2;
	QVector<QPointF> tangent3;
	QVector<QPointF> tangent4;

	QVector<QPointF> funnelSide1;
	QVector<QPointF> funnelSide2;

	QVector<QPointF> hourglassSide1;
	QVector<QPointF> hourglassSide2;
	/////////////////


private:
	QPointF m_startingPoint;
	QPointF m_queryPoint1;
	QPointF m_queryPoint2;
	bool m_startSelected;
	bool m_query1Selected;
	bool m_query2Selected;
	ShortestPath shortestPathHandler;
	OnePointQuery onePointHandler;
	QVector<QPointF> m_tangent;

	enum FailureCondition { NONE, FUNNEL, HOURGLASS, FUNNEL_HOURGLASS, VISIBILITY};
	FailureCondition failure;
	int funnelIntersections;
	int hourglassIntersections;

	QPointF mFunnel;
	QPointF mHourglass;
	int mFunnelIndex;
	int mHourglassIndex;

};

#endif // TwoPointQuery_H
