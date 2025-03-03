#ifndef GeneralCase_H
#define GeneralCase_H

#include "shortestpath.h"
#include "onepointquery.h"
#include "common.h"
#include "cmath"

class GeneralCase
{
public:
	GeneralCase();

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

	struct OptimalPointStruct {
		QPointF optimalPoint;
		int perpendicularPointIndex = 0;
		bool onPathRootToA=false;
	};

	GeneralCase::HourglassStruct constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);
	bool isHourglassOpenCheck(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2);
	QPointF findWaistPoint(const QVector<QPointF>& side1, const QVector<QPointF>& side2);
    QPointF findLastWaistPoint(const QVector<QPointF>& side1, const QVector<QPointF>& side2);

	QPointF mirrorPoint(const QPointF& point, const QLineF& window);
	bool areEqual(const QPointF& a, const QPointF b);
	GeneralCase::TangentStruct findTangent(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, const QVector<QPointF>& funnelSideHelper1, const QVector<QPointF>& funnelSideHelper2);
	bool isTangent(const QVector<QPointF>& path, const QPointF& p1, const QPointF& p2);
	QLineF extendLine(const QLineF& line, const double extensionFactor);
	int numberOfIntersections(const QLineF& line, const QVector<QPointF>& side);
	QVector<QPointF> tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, const QVector<QPointF>& funnelSideHelper1, const QVector<QPointF>& funnelSideHelper2);
	QVector<QPointF> removeElementFromVector(QVector<QPointF> vector, int index);

	ConcatenatedSideStruct concatenateClosedHelper(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent);
    FunnelStar concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass, QPointF& waistPoint);
	GeneralCase::ConcatenatedSideStruct concatenateOpenHelper(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);
	FunnelStar concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass);

	int getLCAIndex(QVector<QPointF>& path1, QVector<QPointF>& path2);
	GeneralCase::OptimalPointStruct computeOptimalPoint(QVector<QPointF>& side1, QVector<QPointF>& side2, QLineF& window1, QLineF& window2);
	QVector<QPointF> computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot);
    QVector<QPointF> computeOptimalPathRootInHourglass(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot, QPointF& waistPoint);

	GeneralCase::GeneralCaseResult executeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

private:
	ShortestPath m_shortestPathHandler;
	OnePointQuery m_onePointHandler;
};

#endif
