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

	struct HourglassStruct {
		QVector<QPointF> hourglassSide1;
		QVector<QPointF> hourglassSide2;
		bool isOpen = false;
	};

	struct FunnelStruct {
		QVector<QPointF> funnelSideA;
		QVector<QPointF> funnelSideB;
	};

	enum FailureCondition { NONE, FUNNEL, HOURGLASS, FUNNEL_HOURGLASS, VISIBILITY };

	struct TangentStruct {
		FailureCondition failure = NONE;
		int funnelIntersections = 0;
		int hourglassIntersections = 0;
		QVector<QPointF> tangentPath;
	};

	struct ConcatenatedSideStruct {
		QVector<QPointF> concatenatedSide;
		int mFunnelIndex = 0;
		int mHourglassIndex = 0;
		QPointF mFunnelPoint;
		QPointF mHourglassPoint;
	};

	struct FunnelStar {
		QVector<QPointF> funnelStarSide1;
		QVector<QPointF> funnelStarSide2;
		QPointF m1;
		QPointF m2;
		QPointF m3;
		QPointF m4;
		int m1Index = 0;
		int m2Index = 0;
		int m3Index = 0;
		int m4Index = 0;
		bool isRootInFunnel = false;
	};

	enum SearchArea {FIRST, SECOND, POINT};

	struct SearchAreaStruct {
		SearchArea searchArea;
		int pointIndex = 0;
	};

	TwoPointQuery::HourglassStruct constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	QVector<QPointF> convertToQT(std::vector<Point_3> points);

	QVector<QPointF> shortestPathToSegment(QPointF start, QLineF segment, Polygon_2& polygon, Surface_mesh& mesh);

	bool dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath);

	double calculatePathLength(const QVector<QPointF>& path);

	bool isHourglassOpenCheck(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2);

	TwoPointQuery::TangentStruct findTangent(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, Polygon_2& funnelPolygon, Polygon_2& hourglassPolygon);

	QPointF mirrorPoint(const QPointF& point, const QLineF& window);

	ConcatenatedSideStruct concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent);
	ConcatenatedSideStruct concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);
	ConcatenatedSideStruct concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);
	FunnelStar concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass);
	FunnelStar concatenateBlockedOpenHourglass(QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, FunnelStruct& funnel, HourglassStruct& hourglass);
	FunnelStar concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass);

	SearchAreaStruct searchFirstHalfASide(QPointF& m, int mIndex, QVector<QPointF>& concatenatedSide, QPointF& mirrorPoint, QLineF& window2);

	SearchAreaStruct searchFirstHalfBSide(QPointF& m, int mIndex, QVector<QPointF>& concatenatedSide, QPointF& mirrorPoint, QLineF& window2);

	QVector<QPointF> mirrorFunnelPart(QVector<QPointF>& funnelStarSide, int mIndex, QLineF& window1);

	QVector<QPointF> mirrorFunnelPart(FunnelStar& funnelStar, QLineF& window1);

	QPointF computeOptimalPoint(FunnelStar& funnelStar, QLineF& window1, QLineF& window2);

	QVector<QPointF> computeOptimalPoint(QVector<QPointF>& funnelStarSide1, QVector<QPointF>& funnelStarSide2, QPointF& m1, QPointF& m2, QPointF& m3, QPointF& m4, QLineF& window1);


	bool searchFirstHalf(QPointF& m, int mIndex, QVector<QPointF>& concatenatedSide, QPointF& mirrorPoint, QLineF& window2);

	QVector<QPointF> computeOptimalPathQ2(QVector<QPointF>& pathRA2, QVector<QPointF>& pathRB2, QPointF& m1, QPointF& m2, QPointF& m3, QPointF& m4, QLineF& window1, bool searchFirstHalf1, bool searchFirstHalf2);

	void executeTwoPointQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh);

	void intersectionCase(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	void dominationCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh mesh);

	void computeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	void generalCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);


	QVector<QPointF> computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot);
	QVector<QPointF> computeOptimalPathRootInHourglass(QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot);

	QLineF extendLine(const QLineF& line, const double extensionFactor);
	TangentStruct findTangentEdgeHourglass(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, Polygon_2& funnelPolygon);
	TangentStruct findTangentEdgeFunnel(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, Polygon_2& hourglassPolygon);
	TangentStruct findTangentEdgeBoth(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon);

	TwoPointQuery::TangentStruct findTangentEdgeWindow(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, Polygon_2& funnelPolygon, Polygon_2& hourglassPolygon);

	int numberOfIntersections(const QLineF& line, const QVector<QPointF>& side);

	QVector<QPointF> tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, Polygon_2& funnelPolygon, Polygon_2& hourglassPolygon);

	QVector<QPointF> removeElementFromVector(QVector<QPointF> vector, int index);


	bool isTangent(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon, const Polygon_2& structurePolygon, const QPointF& newEndpoint);

	bool doSegmentsOverlap(const QLineF& line1, const QLineF& line2);

	struct GeneralCaseResult {
		QPointF funnelRoot;
		QVector<QPointF> funnelSideA;
		QVector<QPointF> funnelSideB;
		QVector<QPointF> hourglassSide1;
		QVector<QPointF> hourglassSide2;
		QVector<QPointF> tangent1;
		QVector<QPointF> tangent2;
		QVector<QPointF> tangent3;
		QVector<QPointF> tangent4;
		QVector<QPointF> concatenatedSide1;
		QVector<QPointF> concatenatedSide2;
		QPointF m1;
		QPointF m2;
		QPointF m3;
		QPointF m4;
		QVector<QPointF> optimalPath;
		QPointF optimalPoint;
	};

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

	GeneralCaseResult getGeneralCaseResult();
	IntersectionResult getIntersectionResult();
	DominationResult getDominationResult();
	QueryResult getQ2Result();

	QPointF findWaistPoint(const QVector<QPointF>& side1, const QVector<QPointF>& side2);

	///////////
	//TEST
	QLineF line;
	QLineF firstWindow;
	QVector<QPointF> funnelSideTest;
	QVector<QPointF> funnelVecSideTest;
	QPointF starRoot;
	QVector<QPointF> PRAT;
	QVector<QPointF> PRBT;
	///////////

private:
	ShortestPath m_shortestPathHandler;
	OnePointQuery m_onePointHandler;

	OnePointQuery::QueryResult resultQ1;

	Q2CASE currentCase;

	GeneralCaseResult resultGeneral;

	IntersectionResult resultIntersection;

	DominationResult resultDomination;

	QueryResult resultQ2;
};

#endif
