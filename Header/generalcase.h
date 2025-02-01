#ifndef GeneralCase_H
#define GeneralCase_H

#include "ShortestPath.h"
#include "OnePointQuery.h"
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

	GeneralCase::HourglassStruct constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);
	bool isHourglassOpenCheck(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2);
	QPointF findWaistPoint(const QVector<QPointF>& side1, const QVector<QPointF>& side2);

	QPointF mirrorPoint(const QPointF& point, const QLineF& window);
	GeneralCase::TangentStruct findTangent(const QPointF& funnelPoint, const QPointF& hourglassPoint, const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, const QVector<QPointF>& hourglassSideHelper);
	bool isTangent(const QVector<QPointF>& path, const QPointF& p1, const QPointF& p2);
	QLineF extendLine(const QLineF& line, const double extensionFactor);
	int numberOfIntersections(const QLineF& line, const QVector<QPointF>& side);
	QVector<QPointF> tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window, Polygon_2& polygon, const QVector<QPointF>& hourglassSideHelper);
	QVector<QPointF> removeElementFromVector(QVector<QPointF> vector, int index);

	ConcatenatedSideStruct concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent);
	ConcatenatedSideStruct concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);
	ConcatenatedSideStruct concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent);
	FunnelStar concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass);
	FunnelStar concatenateBlockedOpenHourglass(QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, FunnelStruct& funnel, HourglassStruct& hourglass);
	FunnelStar concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass);


	QVector<QPointF> mirrorFunnelPart(QVector<QPointF>& funnelStarSide, int mIndex, QLineF& window1);
	QPointF computeOptimalPoint(FunnelStar& funnelStar, QLineF& window1, QLineF& window2);
	QVector<QPointF> computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot);
	QVector<QPointF> computeOptimalPathRootInHourglass(QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot);

	GeneralCase::GeneralCaseResult executeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh);

	///////////
	//TEST
	QLineF line;
	QLineF firstWindow;
	QVector<QPointF> funnelSideTest;
	QVector<QPointF> funnelVecSideTest;
	QPointF starRoot;
	QVector<QPointF> PRAT;
	QVector<QPointF> PRBT;
	QPointF waist;
	Polygon_2 boundTest;
	///////////

private:
	ShortestPath m_shortestPathHandler;
	OnePointQuery m_onePointHandler;
};

#endif
