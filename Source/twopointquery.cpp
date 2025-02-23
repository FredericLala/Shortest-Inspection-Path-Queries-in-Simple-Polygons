#include "twopointquery.h"

TwoPointQuery::TwoPointQuery()
{
}

QVector<QPointF> TwoPointQuery::convertToQT(std::vector<Point_3> points)
{
	QVector<QPointF> qtPoints;
	qtPoints.reserve(points.size());

	for (const auto& point : points)
	{
		qtPoints.append(QPointF(point.x(), point.y()));
	}

	return qtPoints;
}

QVector<QPointF> TwoPointQuery::shortestPathToSegment(QPointF start, QLineF segment, Polygon_2& polygon, Surface_mesh& mesh)
{
	QPointF a = segment.p1();
	QPointF b = segment.p2();

	QVector<QPointF> pathStartToA = m_shortestPathHandler.findShortestPath(start, a, polygon, mesh);
	QVector<QPointF> pathStartToB = m_shortestPathHandler.findShortestPath(start, b, polygon, mesh);

	QPointF lca = m_shortestPathHandler.getLCA(pathStartToA, pathStartToB);

	QVector<QPointF> pathRootToA = m_shortestPathHandler.findShortestPath(lca, a, polygon, mesh);
	QVector<QPointF> pathRootToB = m_shortestPathHandler.findShortestPath(lca, b, polygon, mesh);


	const QPointF c = m_onePointHandler.computeOptimalPoint(pathRootToA, pathRootToB, lca, segment);

	const QVector<QPointF> pathSC = m_shortestPathHandler.findShortestPath(start, c, polygon, mesh);
	return pathSC;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERSECTION
double TwoPointQuery::calculatePathLength(const QVector<QPointF>& path) {
	double length = 0.0;
	for (int i = 1; i < path.size(); ++i) {
		// Distance between consecutive points
		double dx = path[i].x() - path[i - 1].x();
		double dy = path[i].y() - path[i - 1].y();
		length += std::sqrt(dx * dx + dy * dy);
	}
	return length;
}

double TwoPointQuery::calculateNormalizedPathLength(const QVector<QPointF>& path) {
	double length = calculatePathLength(path);
	return length / 10;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOMINATION
bool TwoPointQuery::dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath)
{
	QPointF originalB = m_onePointHandler.unsnapPointInPolygon(window.p2(), window.p1());
	QLineF originalWindow = QLineF(window.p1(), originalB);

	for (size_t i = 0; i < shortestPath.size() - 1; ++i) {
		QLineF segment(shortestPath[i], shortestPath[i + 1]);
		QLineF::IntersectionType intersection = segment.intersects(originalWindow, nullptr);
		if (intersection == QLineF::BoundedIntersection)
		{
			return true; // Intersection found
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TwoPointQuery::executeTwoPointQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh)
{
	QElapsedTimer timer;
	timer.start();

	resultQ2.currentCase = Q2CASE::QNONE;
	bool visibilitySQ1 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint1, polygon);
	bool visibilitySQ2 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint2, polygon);
	resultQ2.visibilityQ1 = visibilitySQ1;
	resultQ2.visibilityQ2 = visibilitySQ2;

	if (visibilitySQ1 && visibilitySQ2)
	{
		std::cout << "Both Query Points are Visible from the Starting Point \n";
		return;
	}
	else if (visibilitySQ1)
	{
		std::cout << "Q1 is visible from the Starting Point \n";
		m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint2, polygon, mesh);
		resultQ2.resultQ1 = m_onePointHandler.getResult();
		return;
	}
	else if (visibilitySQ2)
	{
		std::cout << "Q2 is visible from the Starting Point \n";
		m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint1, polygon, mesh);
		resultQ2.resultQ1 = m_onePointHandler.getResult();
		return;
	}


	QVector<QPointF> shortestPathSQ1 = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint1, polygon, mesh);
	QVector<QPointF> shortestPathSQ2 = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint2, polygon, mesh);

	QLineF window1 = m_onePointHandler.calculateWindow(shortestPathSQ1, queryPoint1, polygon);
	std::cout << "window1 endpoints" << window1.p1().x() << ", " << window1.p1().y() << "; " << window1.p2().x() << ", " << window1.p2().y() << "\n";
	resultQ2.window1 = window1;
	QLineF window2 = m_onePointHandler.calculateWindow(shortestPathSQ2, queryPoint2, polygon);
	resultQ2.window2 = window2;

	currentCase = Q2CASE::QNONE;

	intersectionCase(startingPoint, queryPoint1, queryPoint2, window1, window2, polygon, mesh);

	if (currentCase == Q2CASE::QNONE) {
		dominationCase(startingPoint, window1, window2, polygon, mesh);
	}

	if (currentCase == Q2CASE::QNONE) {
		computeGeneralCase(startingPoint, window1, window2, polygon, mesh);
	}

	qint64 elapsedTime = timer.elapsed();
	resultQ2.time = elapsedTime;
	std::cout << "TwoPointQuery completed in " << elapsedTime << " ms \n";
	std::cout << "TwoPointQuery optimal path length " << resultQ2.optimalPathLength << "\n";
}

void TwoPointQuery::intersectionCase(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh) {
	QPointF visibleEndpoint1;
	QPointF visibleEndpoint2;

	QPointF invisibleEndpoint1;
	QPointF invisibleEndpoint2;

	QPointF intersectionPoint;

	QVector<QPointF> optimalPath;

	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QLineF::IntersectionType intersection = window1.intersects(window2, &intersectionPoint);

	if (intersection == QLineF::BoundedIntersection)
	{
		// Check if the intersection point is not a shared endpoint
		if (intersectionPoint != window1.p1() && intersectionPoint != window1.p2() &&
			intersectionPoint != window2.p1() && intersectionPoint != window2.p2())
		{
			std::cout << "The windows intersect \n";

			if (m_onePointHandler.checkVisibilty(a1, queryPoint2, polygon))
			{
				visibleEndpoint1 = a1;
				invisibleEndpoint1 = b1;
			}
			else
			{
				visibleEndpoint1 = b1;
				invisibleEndpoint1 = a1;
			}

			if (m_onePointHandler.checkVisibilty(a2, queryPoint1, polygon))
			{
				visibleEndpoint2 = a2;
				invisibleEndpoint2 = b2;
			}
			else
			{
				visibleEndpoint2 = b2;
				invisibleEndpoint2 = a2;
			}

			QVector<QPointF> intersectionPath1 = shortestPathToSegment(startingPoint, QLineF(visibleEndpoint1, intersectionPoint), polygon, mesh);
			resultIntersection.intersectionPath1 = intersectionPath1;
			double sizePath1 = calculatePathLength(intersectionPath1);

			QVector<QPointF> intersectionPath2 = shortestPathToSegment(startingPoint, QLineF(visibleEndpoint2, intersectionPoint), polygon, mesh);
			resultIntersection.intersectionPath2 = intersectionPath2;
			double sizePath2 = calculatePathLength(intersectionPath2);

			QLineF segWindow1 = QLineF(invisibleEndpoint1, intersectionPoint);
			QLineF segWindow2 = QLineF(invisibleEndpoint2, intersectionPoint);

			computeGeneralCase(startingPoint, segWindow1, segWindow2, polygon, mesh);
			GeneralCase::GeneralCaseResult generalCaseResult = resultGeneral;
			QVector<QPointF> intersectionPath3 = generalCaseResult.optimalPath;
			resultIntersection.intersectionPath3 = intersectionPath3;
			double sizePath3 = calculatePathLength(intersectionPath3);

			double minPath = std::min({ sizePath1, sizePath2, sizePath3 });
			if (minPath == sizePath1 && minPath == sizePath2) {
				optimalPath = intersectionPath1;
				std::cout << "Path 1 and 2 are shortest \n";
			}
			else if (minPath == sizePath1) {
				optimalPath = intersectionPath1;
				std::cout << "Path 1 is shortest \n";
			}
			else if (minPath == sizePath2) {
				optimalPath = intersectionPath2;
				std::cout << "Path 2 is shortest \n";
			}
			else if (minPath == sizePath3) {
				optimalPath = intersectionPath3;
				std::cout << "Path 3 is shortest \n";
			}

			resultIntersection.optimalPath = optimalPath;

			std::cout << "Size of Path 1: " << sizePath1 << "\n";
			std::cout << "Size of Path 2: " << sizePath2 << "\n";
			std::cout << "Size of Path 3: " << sizePath3 << "\n";
			currentCase = INTERSECTION;
			resultQ2.currentCase = INTERSECTION;
            resultQ2.optimalPathLength = calculatePathLength(optimalPath);
			return;
		}
	}
}

void TwoPointQuery::dominationCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh mesh) {
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> shortestPathSA1 = m_shortestPathHandler.findShortestPath(startingPoint, a1, polygon, mesh);
	QVector<QPointF> shortestPathSB1 = m_shortestPathHandler.findShortestPath(b1, startingPoint, polygon, mesh);
	QVector<QPointF> shortestPathSA2 = m_shortestPathHandler.findShortestPath(startingPoint, a2, polygon, mesh);
	QVector<QPointF> shortestPathSB2 = m_shortestPathHandler.findShortestPath(b2, startingPoint, polygon, mesh);
	QVector<QPointF> optimalPath;

	if (dominateWindowCheck(window2, shortestPathSA1) && dominateWindowCheck(window2, shortestPathSB1))
	{
		std::cout << "Window1 lies behind Window2 \n";
		currentCase = DOMINATION;
		resultQ2.currentCase = DOMINATION;
		optimalPath = shortestPathToSegment(startingPoint, window1, polygon, mesh);
	}
	else if (dominateWindowCheck(window1, shortestPathSA2) && dominateWindowCheck(window1, shortestPathSB2))
	{
		std::cout << "Window2 lies behind Window1";
		currentCase = DOMINATION;
		resultQ2.currentCase = DOMINATION;
		optimalPath = shortestPathToSegment(startingPoint, window2, polygon, mesh);
	}

	resultDomination.optimalPath = optimalPath;
    resultQ2.optimalPathLength = calculatePathLength(optimalPath);
	return;
}


void TwoPointQuery::computeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh) {
	std::cout << "General case: " << "\n";
	currentCase = GENERAL;
	resultQ2.currentCase = GENERAL;

	GeneralCase::GeneralCaseResult firstResult = m_generalCaseHandler.executeGeneralCase(startingPoint, window1, window2, polygon, mesh);
	double tempOptimalPathLength1 = calculatePathLength(firstResult.optimalPath);

	GeneralCase::GeneralCaseResult secondResult = m_generalCaseHandler.executeGeneralCase(startingPoint, window2, window1, polygon, mesh);
	double tempOptimalPathLength2 = calculatePathLength(secondResult.optimalPath);
	if (tempOptimalPathLength2 >= tempOptimalPathLength1) {
		std::cout << "General case: First is shorter" << "\n";
		resultGeneral = firstResult;
        resultQ2.optimalPathLength = calculatePathLength(firstResult.optimalPath);
		return;
	}
	//resultGeneral = firstResult;
	std::cout << "General case: Second is shorter" << "\n";
    resultQ2.optimalPathLength = calculatePathLength(secondResult.optimalPath);
	resultGeneral = secondResult;
}


GeneralCase::GeneralCaseResult TwoPointQuery::getGeneralCaseResult() {
	return resultGeneral;
}

TwoPointQuery::IntersectionResult TwoPointQuery::getIntersectionResult() {
	return resultIntersection;
}

TwoPointQuery::DominationResult TwoPointQuery::getDominationResult() {
	return resultDomination;
}

TwoPointQuery::QueryResult TwoPointQuery::getQ2Result() {
	return resultQ2;
}




