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

QVector<QPointF> TwoPointQuery::shortestPathToSegment(QPointF start, QLineF segment, Polygon_2& polygon)
{
	QPointF a = segment.p1();
	QPointF b = segment.p2();

	QPointF lca = m_shortestPathHandler.getLCA(start, a, b, polygon);

	QVector<QPointF> pathRootToA = m_shortestPathHandler.findShortestPath(lca, a, polygon);
	QVector<QPointF> pathRootToB = m_shortestPathHandler.findShortestPath(lca, b, polygon); 


	const QPointF c = m_onePointHandler.computeOptimalPoint(pathRootToA, pathRootToB, lca, segment);

	const QVector<QPointF> pathSC = m_shortestPathHandler.findShortestPath(start, c, polygon);
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DOMINATION
bool TwoPointQuery::dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath)
{
	for (size_t i = 0; i < shortestPath.size() - 1; ++i) {
		QLineF segment(shortestPath[i], shortestPath[i + 1]);
		QLineF::IntersectionType intersection = segment.intersects(window, nullptr);
		if (intersection == QLineF::BoundedIntersection)
		{
			return true; // Intersection found
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GENERAL CASE
bool TwoPointQuery::hourglassOpen(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2) {
	Polygon_2 polygon;
	for (QPointF p : hourglassSide1) {
		polygon.push_back(Point_2(p.x(), p.y()));
	}

	for (int i = hourglassSide2.size() - 1; i >= 0; --i) {
		polygon.push_back(Point_2(hourglassSide2[i].x(), hourglassSide2[i].y()));
	}

	if (polygon.is_simple()) {
		return true;
	}

	return false;
}


void TwoPointQuery::findTangent(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	failure = NONE;
	m_tangent.clear();

	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);

	QLineF line = QLineF(funnelPoint, mirroredPoint);


	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	if (!isTangent(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon) && !isTangent(QLineF(intersectionPoint, hourglassPoint), hourglassSide, polygon)) {
		failure = FUNNEL_HOURGLASS;
		return;
	}

	// if the line is not tangent to the funnel, it can not be the tangent
	if (!isTangent(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon)) {
		failure = FUNNEL;
		return;
	}

	if (!isTangent(QLineF(intersectionPoint, hourglassPoint), hourglassSide, polygon)) {
		failure = HOURGLASS;
		return;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		failure = VISIBILITY;
		return;
	}

	m_tangent.append(funnelPoint);
	m_tangent.append(intersectionPoint);
	m_tangent.append(hourglassPoint);
}


// edge case for hourglass path endpoints
void TwoPointQuery::findTangentEdgeHourglass(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	failure = NONE;
	m_tangent.clear();

	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);

	QLineF line = QLineF(funnelPoint, mirroredPoint);


	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	// if the line is not tangent to the funnel, it can not be the tangent
	if (!isTangent(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon)) {
		failure = FUNNEL;
		return;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		failure = VISIBILITY;
		return;
	}

	m_tangent.append(funnelPoint);
	m_tangent.append(intersectionPoint);
	m_tangent.append(hourglassPoint);
}


// edge case for funnel path endpoints
void TwoPointQuery::findTangentEdgeFunnel(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	failure = NONE;
	m_tangent.clear();

	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);

	QLineF line = QLineF(funnelPoint, mirroredPoint);


	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	if (!isTangent(QLineF(intersectionPoint, hourglassPoint), hourglassSide, polygon)) {
		failure = HOURGLASS;
		return;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		failure = VISIBILITY;
		return;
	}

	m_tangent.append(funnelPoint);
	m_tangent.append(intersectionPoint);
	m_tangent.append(hourglassPoint);
}


// edge case for hourglass path endpoints
void TwoPointQuery::findTangentEdgeBoth(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	failure = NONE;
	m_tangent.clear();

	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);

	QLineF line = QLineF(funnelPoint, mirroredPoint);


	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		failure = VISIBILITY;
		return;
	}

	m_tangent.append(funnelPoint);
	m_tangent.append(intersectionPoint);
	m_tangent.append(hourglassPoint);
}


int TwoPointQuery::numberOfIntersections(const QLineF& line, const QVector<QPointF>& side) {
	int intersectionCount = 0;
	bool overlap = false;
	QVector<QPointF> intersections;
	// Iterate over the edges of the side
	for (int i = 0; i < side.size() - 1; ++i) {
		QLineF edge(side[i], side[i + 1]); // Create an edge from consecutive points
		QPointF intersectionPoint;

		// Check if line intersects the edge
		QLineF::IntersectType type = line.intersects(edge, &intersectionPoint);

		if (type == QLineF::BoundedIntersection) {
			intersections.append(intersectionPoint);
		}


		/* if (doSegmentsOverlap(line, edge)) {
			overlap = true;
		} */
	}

	for (QPointF intersection : intersections) {
		if (intersection == line.p1() || intersection == line.p2()) {
			continue;
		}
		else {
			intersectionCount++;
		}
	}

	if (overlap) {
		intersectionCount = std::numeric_limits<int>::max();
	}

	return intersectionCount;
}

QVector<QPointF> TwoPointQuery::tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window,
	Polygon_2& polygon) {
	QVector<QPointF> tangent;
	QVector<QPointF> funnelVec = funnelSide;
	QPointF mirroredPoint;
	QLineF line;
	QPointF reflectionPoint;
	QPointF intersectionPoint;
	failure = NONE;
	int test;
	bool funnelTangency = false;
	int funnelIntersections = 0;

	bool edgeFunnel;
	bool edgeHourglass;

	int funnelLeft = 0;
	int funnelRight = funnelVec.size() - 1;

	int hourglassLeft = 0;
	int hourglassRight = hourglassSide.size() - 1;

	while (funnelLeft <= funnelRight) {
		int funnelMid = (funnelLeft + funnelRight) / 2;

		QPointF funnelCandidate = funnelVec[funnelMid];

		hourglassLeft = 0;
		hourglassRight = hourglassSide.size() - 1;
		while (hourglassLeft <= hourglassRight) {
			int hourglassMid = (hourglassLeft + hourglassRight) / 2;
			QPointF hourglassCandidate = hourglassSide[hourglassMid];

			edgeFunnel = (funnelCandidate == funnelSide.begin()[0] || funnelCandidate == funnelSide.rbegin()[0]);
			edgeHourglass = (hourglassCandidate == hourglassSide.begin()[0] || hourglassCandidate == hourglassSide.rbegin()[0]);

			if (edgeFunnel && edgeHourglass) {
				findTangentEdgeBoth(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				//std::cout << "Edge Both: number of intersections: " << funnelIntersections << "\n";
			}
			else if (edgeHourglass) {
				findTangentEdgeHourglass(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				//std::cout << "Edge Hourglass: number of intersections: " << funnelIntersections << "\n";
			}
			else if (edgeFunnel) {
				findTangentEdgeFunnel(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				//std::cout << "Edge Funnel: number of intersections: " << hourglassIntersections << "\n";
			}
			else {
				findTangent(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
			}

			switch (failure)
			{
			case TwoPointQuery::NONE:
				//std::cout << "case a \n";
				tangent = m_tangent; // case a.
				return tangent;
				break;
			case TwoPointQuery::FUNNEL_HOURGLASS:
				if (funnelIntersections == 0 && hourglassIntersections == 0) { // case f.
					//std::cout << "case f \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1 && hourglassIntersections == 0) { // case g.
					//std::cout << "case g \n";
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 0 && hourglassIntersections == 1) { // case h.
					//std::cout << "case h \n";
					funnelRight = funnelMid - 1;
				}

				if (funnelIntersections >= 1 && hourglassIntersections >= 1) { // case i.
					//std::cout << "case i \n";
					QPointF mirroredPoint = mirrorPoint(hourglassCandidate, window);
					QLineF m = QLineF(funnelCandidate, mirroredPoint);
					QLineF lp;
					QLineF lq;
					QPointF intersectionPoint;

					if (funnelMid + 1 < funnelVec.size()) {
						lp = QLineF(funnelCandidate, funnelVec[funnelMid + 1]);
					}
					else {
						lp = QLineF(funnelVec[funnelMid - 1], funnelCandidate);
					}

					if (hourglassMid - 1 >= 0) {
						lq = QLineF(mirroredPoint, mirrorPoint(hourglassSide[hourglassMid - 1], window));
					}
					else {
						lq = QLineF(mirrorPoint(hourglassSide[hourglassMid + 1], window), mirroredPoint);
					}

					QLineF::IntersectType type = lp.intersects(lq, &intersectionPoint);

					Point_2 a = Point_2(window.p1().x(), window.p1().y());
					Point_2 b = Point_2(window.p2().x(), window.p2().y());

					if (CGAL::orientation(a, b, Point_2(intersectionPoint.x(), intersectionPoint.y()))
						== CGAL::orientation(a, b, Point_2(funnelCandidate.x(), funnelCandidate.y()))) { // case i2.
						//std::cout << "case i2 \n";
						funnelLeft = funnelMid + 1;
					}
					else { // case i1.
						//std::cout << "case i1 \n";
						hourglassRight = hourglassMid - 1;
					}
				}
				break;

			case TwoPointQuery::FUNNEL:
				if (funnelIntersections == 0) { // case d.
					//std::cout << "case d \n";
					std::cout << funnelMid - 1 << "\n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1) { // case e.
					//std::cout << "case e \n";
					funnelLeft = funnelMid + 1;
					hourglassLeft = hourglassMid + 1;
				}

				break;
			case TwoPointQuery::HOURGLASS:
				if (hourglassIntersections == 0) { // case b.
					//std::cout << "case b \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (hourglassIntersections == 1) { // case c.
					//std::cout << "case c \n";
					funnelRight = funnelMid - 1;
					hourglassRight = hourglassMid - 1;
				}

				break;
			case TwoPointQuery::VISIBILITY:
				//std::cout << "vis \n";
				hourglassRight = hourglassMid - 1;
				if (hourglassRight <= hourglassLeft) {
					funnelVec.removeAt(funnelMid);
				}

				test = funnelVec.size();
				if (funnelVec.isEmpty()) {
					return tangent;
				}
				break;
			default:
				//std::cout << "default \n";

				break;
			}
		}
	}
	return tangent;
}


QVector<QPointF> TwoPointQuery::removeElementFromVector(QVector<QPointF> vector, int index) {
	QVector<QPointF> newVec;

	if (vector.isEmpty() || vector.size() == 1) {
		return newVec;
	}

	for (size_t i = 0; i < vector.size(); ++i) {
		if (i == index) {
			continue;
		}
		else {
			newVec.append(vector[i]);
		}
	}

	return newVec;
}


bool TwoPointQuery::isTangent(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon) {
	int intersectionCount = numberOfIntersections(line, side);

	if (intersectionCount == 0) {
		// Extend the line slightly
		QPointF direction = line.p2() - line.p1();
		double extendFactor = 0.01; // Small extension factor
		direction *= extendFactor / std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

		QPointF newStart = line.p1() - direction;
		QPointF newEnd = line.p2() + direction;

		QLineF extendedLine(newStart, newEnd);

		if (polygon.has_on_unbounded_side(Point_2(newStart.x(), newStart.y())) || polygon.has_on_unbounded_side(Point_2(newEnd.x(), newEnd.y()))) {
			// If either of the extended endpoints are outside the polygon, it cant be tangent 
			return false;
		}

		return true;
	}

	return false;
}


bool TwoPointQuery::doSegmentsOverlap(const QLineF& line1, const QLineF& line2) {
	if (line1.p1() == line2.p1() && line1.p2() == line2.p2()) {
		return true;
	}
	else {
		return false;
	}
}


QPointF TwoPointQuery::mirrorPoint(const QPointF& point, const QLineF& window) {
	// Extract the coordinates of the point and the window endpoints
	double px = point.x();
	double py = point.y();
	double x1 = window.p1().x();
	double y1 = window.p1().y();
	double x2 = window.p2().x();
	double y2 = window.p2().y();

	// Compute the coefficients of the line equation ax + by + c = 0
	double a = y2 - y1;
	double b = x1 - x2;
	double c = x2 * y1 - x1 * y2;

	// Compute the denominator (a^2 + b^2)
	double denominator = a * a + b * b;

	// Reflect the point across the line
	double xPrime = px - 2 * a * (a * px + b * py + c) / denominator;
	double yPrime = py - 2 * b * (a * px + b * py + c) / denominator;

	// Return the reflected point
	return QPointF(xPrime, yPrime);
}


QVector<QPointF> TwoPointQuery::concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent) {
	QVector<QPointF> concatenatedSide;
	int indexCount = 0;

	for (QPointF funnelPoint : funnelSide) {
		if (funnelPoint != tangent[0]) {
			concatenatedSide.append(funnelPoint);
			++indexCount;
		}
		else {
			mFunnelIndex = indexCount;
			mFunnel = funnelPoint;
			break;
		}
	}

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
		++indexCount;
	}

	bool start = false;
	for (size_t i = 0; i < hourglassSide.size(); ++i) {
		QPointF hourglassPoint = hourglassSide[i];
		if (start) {
			concatenatedSide.append(hourglassPoint);
			++indexCount;
		}

		if (hourglassPoint == tangent.rbegin()[0]) {
			mHourglass = hourglassPoint;
			mHourglassIndex = indexCount;
			start = true;
		}
	}

	return concatenatedSide;
}


QVector<QPointF> TwoPointQuery::concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	QVector<QPointF> concatenatedSide;

	for (QPointF point : funnelSide) {
		if (point != tangent[0]) {
			concatenatedSide.append(point);
		}
		else {
			break;
		}
	}

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}

	mFunnel = tangent[0];
	mHourglass = tangent.rbegin()[0];

	concatenatedSide.append(hourglassSide.rbegin()[0]);

	return concatenatedSide;
}


QVector<QPointF> TwoPointQuery::concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	QVector<QPointF> concatenatedSide;

	concatenatedSide.append(funnelSide[0]);

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}

	mFunnel = tangent[0];
	mHourglass = tangent.rbegin()[0];

	bool start = false;
	for (QPointF hourglassPoint : hourglassSide) {
		if (start) {
			concatenatedSide.append(hourglassPoint);
		}

		if (hourglassPoint == tangent.rbegin()[0]) {
			start = true;
		}
	}


	return concatenatedSide;
}

QPointF TwoPointQuery::getMFunnel() {
	return mFunnel;
}

QPointF TwoPointQuery::getMHourglass() {
	return mHourglass;
}

int TwoPointQuery::getFunnelIndex() {
	return mFunnelIndex;
}

int TwoPointQuery::getHourglassIndex() {
	return mHourglassIndex;
}


bool TwoPointQuery::searchFirstHalf(QPointF& m, int& mIndex, QVector<QPointF>& concatenatedSide, QPointF& mirrorPoint, QLineF& window2) {
	double angle1;
	double angle2;
	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();
	if (mIndex <= concatenatedSide.size() - 2) {
		angle1 = m_onePointHandler.calculateFunnelAngle(m, concatenatedSide[mIndex + 1], a2, b2);
		angle2 = m_onePointHandler.calculateFunnelAngle(mirrorPoint, m, a2, b2);

		if (angle1 > 90 && angle2 > 90) {
			if (angle1 < angle2) {
				return  false;
			}
			else {
				return true;
			}
		}
		else if (angle1 < 90 && angle2 < 90) {
			if (angle1 > angle2) {
				return false;
			}
			else {
				return true;
			}
		}
	}

	return true; // no second half to search in
}


QVector<QPointF> TwoPointQuery::computeOptimalPathQ2(QVector<QPointF>& pathRA2, QVector<QPointF>& pathRB2, QPointF& m1, QPointF& m2, QPointF& m3, QPointF& m4, QLineF& window1,
	bool searchFirstHalf1, bool searchFirstHalf2)
{
	QVector<QPointF> optimalPath;
	QPointF a2 = pathRA2.rbegin()[0];
	QPointF b2 = pathRB2.rbegin()[0];
	QPointF rootStar = pathRA2.first();
	const double angle0 = m_onePointHandler.calculateFunnelAngle(pathRA2.rbegin()[1], pathRA2.rbegin()[0], a2, b2); // theta_0
	if (angle0 > 90)
	{
		std::cout << "c = a2 \n";
		optimalPath = pathRA2;
		return optimalPath;
	}

	const double anglek = m_onePointHandler.calculateFunnelAngle(pathRB2.rbegin()[1], pathRB2.rbegin()[0], a2, b2); // theta_k
	if (anglek < 90)
	{
		std::cout << "c = b2 \n";
		optimalPath = pathRB2;
		return optimalPath;
	}
	//
	//
	QPointF mirrorPoint1;
	QPointF mirrorPoint2;
	QPointF intersectionPointW1;
	QPointF intersectionPointW2;
	QLineF line;

	mirrorPoint1 = mirrorPoint(pathRA2.begin()[0], window1);
	mirrorPoint2 = mirrorPoint(pathRA2.begin()[1], window1);

	const double anglem1 = m_onePointHandler.calculateFunnelAngle(mirrorPoint1, mirrorPoint2, a2, b2); // theta_m-1

	mirrorPoint1 = mirrorPoint(pathRB2.begin()[0], window1);
	mirrorPoint2 = mirrorPoint(pathRB2.begin()[1], window1);

	const double anglem = m_onePointHandler.calculateFunnelAngle(mirrorPoint1, mirrorPoint2, a2, b2);  // theta_m
	if (anglem1 == 90 || anglem == 90 || (anglem1 <= 90 && 90 < anglem))
	{
		std::cout << "c is at the foot of the reflected perpendicular from root*";
		line = QLineF(rootStar, mirrorPoint(rootStar, window1));
		QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
		intersectionPointW2 = m_onePointHandler.calculateWindowIntersection(mirrorPoint(rootStar, window1), a2, b2);

		optimalPath.append(rootStar);
		optimalPath.append(intersectionPointW1);
		optimalPath.append(intersectionPointW2);
		return optimalPath;
	}
	//
	//
	QVector<QPointF> firstHalfPath;
	QVector<QPointF> mirrorPath;
	QVector<QPointF> secondHalfPath;
	int vertex;
	int v;
	bool start = false;

	if (anglem1 > 90)
	{
		if (searchFirstHalf1) {
			mirrorPath.clear();
			for (QPointF point : pathRA2) {
				if (point != m1) {
					firstHalfPath.append(point);
					mirrorPath.append(mirrorPoint(point, window1));
				}
				else {
					firstHalfPath.append(point);
					mirrorPath.append(mirrorPoint(point, window1));
					break;
				}
			}

			vertex = m_onePointHandler.binarySearchByAngle(mirrorPath, a2, b2);
			v = (pathRA2.size() - 1) - vertex; // root would be lowest, but needs to be highest

			std::cout << "c is at the foot of the perpendicular from reflected v" + v;

			line = QLineF(firstHalfPath[vertex], mirrorPath[vertex]);
			QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
			intersectionPointW2 = m_onePointHandler.calculateWindowIntersection(intersectionPointW1, a2, b2);

			for (size_t i = 0; i <= vertex; ++i) {
				optimalPath.append(firstHalfPath[i]);
			}
			optimalPath.append(intersectionPointW1);
			optimalPath.append(intersectionPointW2);
			return optimalPath;

		}
		else {
			start = false;
			secondHalfPath.clear();
			for (QPointF point : pathRA2) {
				if (point == m2) {
					start = true;
				}

				if (start) {
					secondHalfPath.append(point);
				}
			}

			vertex = m_onePointHandler.binarySearchByAngle(secondHalfPath, a2, b2);
			v = (pathRA2.size() - 1) - vertex; // root would be lowest, but needs to be highest

			for (QPointF point : pathRA2) {
				if (point != secondHalfPath[vertex]) {
					optimalPath.append(point);
				}
				else {
					optimalPath.append(point);
					break;
				}
			}

			optimalPath.append(m_onePointHandler.calculateWindowIntersection(secondHalfPath[vertex], a2, b2));

			std::cout << "c is at the foot of the perpendicular from v" + v;
			return optimalPath;
		}
	}
	else
	{
		if (searchFirstHalf2) {
			mirrorPath.clear();
			for (QPointF point : pathRB2) {
				if (point != m3) {
					firstHalfPath.append(point);
					mirrorPath.append(mirrorPoint(point, window1));
				}
				else {
					firstHalfPath.append(point);
					mirrorPath.append(mirrorPoint(point, window1));
					break;
				}
			}

			vertex = m_onePointHandler.binarySearchByAngle(mirrorPath, a2, b2);
			v = vertex + (pathRB2.size() - 1);
			std::cout << "c is at the foot of the perpendicular from reflected v" + v;

			line = QLineF(firstHalfPath[vertex], mirrorPath[vertex]);
			QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
			intersectionPointW2 = m_onePointHandler.calculateWindowIntersection(intersectionPointW1, a2, b2);

			for (size_t i = 0; i <= vertex; ++i) {
				optimalPath.append(firstHalfPath[i]);
			}
			optimalPath.append(intersectionPointW1);
			optimalPath.append(intersectionPointW2);
			return optimalPath;
		}
		else {
			start = false;
			secondHalfPath.clear();
			for (QPointF point : pathRB2) {
				if (point == m4) {
					start = true;
				}

				if (start) {
					secondHalfPath.append(point);
				}
			}

			vertex = m_onePointHandler.binarySearchByAngle(secondHalfPath, a2, b2);
			v = vertex + (pathRB2.size() - 1);
			std::cout << "c is at the foot of the perpendicular from v" + v;

			for (QPointF point : pathRB2) {
				if (point != secondHalfPath[vertex]) {
					optimalPath.append(point);
				}
				else {
					optimalPath.append(point);
					break;
				}
			}

			optimalPath.append(m_onePointHandler.calculateWindowIntersection(secondHalfPath[vertex], a2, b2));

			return optimalPath;
		}
	}
}

void TwoPointQuery::executeTwoPointQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon)
{
	std::cout << "start 2 query \n";
	std::cout << "startingPoint: " << startingPoint.x() << ", " << startingPoint.y() << "\n";
	std::cout << "queryPoint1: " << queryPoint1.x() << ", " << queryPoint1.y() << "\n";
	std::cout << "queryPoint2: " << queryPoint2.x() << ", " << queryPoint2.y() << "\n";
	// visibility check
	resultQ2.visibility = false;
	resultQ2.currentCase = Q2CASE::QNONE;
	bool visibilitySQ1 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint1, polygon);
	bool visibilitySQ2 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint2, polygon);
	if (visibilitySQ1 && visibilitySQ2)
	{
		std::cout << "Both Query Points are Visible from the Starting Point \n";
		resultQ2.visibility = true;
		return;
	}
	else if (visibilitySQ1)
	{
		std::cout << "Q1 is visible from the Starting Point \n";
		m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint2, polygon);
		resultQ2.resultQ1 = m_onePointHandler.getResult();
		return;
	}
	else if (visibilitySQ2)
	{
		std::cout << "Q2 is visible from the Starting Point \n";
		m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint1, polygon);
		resultQ2.resultQ1 = m_onePointHandler.getResult();
		return;
	}


	QVector<QPointF> shortestPathSQ1 = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint1, polygon);
	QVector<QPointF> shortestPathSQ2 = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint2, polygon);

	QLineF window1 = m_onePointHandler.calculateWindow(shortestPathSQ1, queryPoint1, polygon);
	std::cout << "window1 endpoints" << window1.p1().x() << ", " << window1.p1().y() << "; " << window1.p2().x() << ", " << window1.p2().y() << "\n";
	resultQ2.window1 = window1;
	QLineF window2 = m_onePointHandler.calculateWindow(shortestPathSQ2, queryPoint2, polygon);
	resultQ2.window2 = window2;

	currentCase = Q2CASE::QNONE;

	intersectionCase(startingPoint, queryPoint1, queryPoint2, window1, window2, polygon);

	if (currentCase == Q2CASE::QNONE) {
		dominationCase(startingPoint, window1, window2, polygon);
	}

	if (currentCase == Q2CASE::QNONE) {
		computeGeneralCase(startingPoint, window1, window2, polygon);
	}
}

void TwoPointQuery::intersectionCase(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, QLineF& window1, QLineF& window2, Polygon_2& polygon) {
	QPointF visibleEndpoint1;
	QPointF visibleEndpoint2;

	QPointF invisibleEndpoint1;
	QPointF invisibleEndpoint2;

	QPointF intersectionPoint;

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

			QVector<QPointF> intersectionPath1 = shortestPathToSegment(startingPoint, QLineF(visibleEndpoint1, intersectionPoint), polygon);
			resultIntersection.intersectionPath1 = intersectionPath1;
			double sizePath1 = calculatePathLength(intersectionPath1);

			QVector<QPointF> intersectionPath2 = shortestPathToSegment(startingPoint, QLineF(visibleEndpoint2, intersectionPoint), polygon);
			resultIntersection.intersectionPath2 = intersectionPath2;
			double sizePath2 = calculatePathLength(intersectionPath2);

			QLineF segWindow1 = QLineF(invisibleEndpoint1, intersectionPoint);
			QLineF segWindow2 = QLineF(invisibleEndpoint2, intersectionPoint);

			computeGeneralCase(startingPoint, window1, window2, polygon);
			GeneralCaseResult generalCaseResult = resultGeneral;
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
			return;
		}
	}
}

void TwoPointQuery::dominationCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon) {
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> shortestPathSA1 = m_shortestPathHandler.findShortestPath(startingPoint, a1, polygon);
	QVector<QPointF> shortestPathSB1 = m_shortestPathHandler.findShortestPath(b1, startingPoint, polygon);
	QVector<QPointF> shortestPathSA2 = m_shortestPathHandler.findShortestPath(startingPoint, a2, polygon);
	QVector<QPointF> shortestPathSB2 = m_shortestPathHandler.findShortestPath(b2, startingPoint, polygon);
	QVector<QPointF> optimalPath;

	if (dominateWindowCheck(window2, shortestPathSA1) && dominateWindowCheck(window2, shortestPathSB1))
	{
		std::cout << "Window1 lies behind Window2 \n";
		currentCase = DOMINATION;
		resultQ2.currentCase = DOMINATION;
		optimalPath = shortestPathToSegment(startingPoint, window1, polygon);
	}
	else if (dominateWindowCheck(window1, shortestPathSA2) && dominateWindowCheck(window1, shortestPathSB2))
	{
		std::cout << "Window2 lies behind Window1";
		currentCase = DOMINATION;
		resultQ2.currentCase = DOMINATION;
		optimalPath = shortestPathToSegment(startingPoint, window2, polygon);
	}

	resultDomination.optimalPath = optimalPath;
	return;
}


void TwoPointQuery::computeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon) {
	generalCase(startingPoint, window1, window2, polygon);
	GeneralCaseResult firstWindow1 = resultGeneral;
	double tempOptimalPathLength1 = calculatePathLength(firstWindow1.optimalPath);

	generalCase(startingPoint, window2, window1, polygon);
	GeneralCaseResult firstWindow2 = resultGeneral;
	double tempOptimalPathLength2 = calculatePathLength(firstWindow2.optimalPath);

	if (tempOptimalPathLength2 >= tempOptimalPathLength1) {
		resultGeneral = firstWindow1;
		return;
	}

	resultGeneral = firstWindow2;
}


void TwoPointQuery::generalCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon)
{
	std::cout << "General case: " << "\n";
	currentCase = GENERAL;
	resultQ2.currentCase = GENERAL;

	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> pathStartToA1 = m_shortestPathHandler.findShortestPath(startingPoint, a1, polygon);
	QVector<QPointF> pathStartToB1 = m_shortestPathHandler.findShortestPath(startingPoint, b1, polygon);

	QPointF penultimatePointSToA1 = pathStartToA1.begin()[1];
	QPointF penultimatePointSToB1 = pathStartToB1.begin()[1];
	QPointF funnelRoot;

	if (penultimatePointSToA1 != penultimatePointSToB1)
	{
		funnelRoot = startingPoint;
	}
	else
	{
		funnelRoot = m_shortestPathHandler.getLCA(startingPoint, a1, b1, polygon);
	}


	QVector<QPointF> funnelSideA = m_shortestPathHandler.findShortestPath(funnelRoot, a1, polygon);
	resultGeneral.funnelSideA = funnelSideA;
	QVector<QPointF> funnelSideB = m_shortestPathHandler.findShortestPath(funnelRoot, b1, polygon);
	resultGeneral.funnelSideB = funnelSideB;

	constructHourglass(window1, window2, polygon);
	resultGeneral.hourglassSide1 = hourglassSide1;
	resultGeneral.hourglassSide2 = hourglassSide2;

	bool isHourglassOpen = m_isHourglassOpen;

	QVector<QPointF> tangent1 = tangentBinary(funnelSideA, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent2 = tangentBinary(funnelSideA, hourglassSide2, window1, polygon);
	QVector<QPointF> tangent3 = tangentBinary(funnelSideB, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent4 = tangentBinary(funnelSideB, hourglassSide2, window1, polygon);

	bool isOuterTangent1Blocked = false;
	bool isOuterTangent2Blocked = false;
	bool rootInFunnel1 = false;

	if (tangent1.isEmpty()) {
		isOuterTangent1Blocked = true;
	}

	if (tangent4.isEmpty()) {
		isOuterTangent2Blocked = true;
	}

	if (!isHourglassOpen) {
		concatenateClosedHourglass(tangent1, tangent2, tangent3, tangent4);
	}
	else {
		if (isOuterTangent1Blocked || isOuterTangent2Blocked) {
			concatenateBlockedOpenHourglass(tangent2, tangent3);
		}
		else {
			concatenateOpenHourglass(tangent1, tangent4);

		}
	}
	resultGeneral.concatenatedSide1 = concatenatedSide1;
	resultGeneral.concatenatedSide2 = concatenatedSide2;

	if (rootInFunnel1) {
		computeOptimalPathRootInFunnel(window1, window2);
	}
	else {
		computeOptimalPathRootInHourglass(window2);
	}

	resultGeneral.optimalPath = optimalPath;
	resultGeneral.optimalPoint = c;
}


void TwoPointQuery::concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4) {
	if (!tangent1.isEmpty()) {
		std::cout << "1 not empty \n";
		concatenatedSide1 = concatenateClosed(resultGeneral.funnelSideA, hourglassSide1, tangent1);
	}
	else if (!tangent2.isEmpty()) {
		std::cout << "2 not empty \n";
		concatenatedSide1 = concatenateClosed(resultGeneral.funnelSideA, hourglassSide2, tangent2);
	}

	m1 = getMFunnel();
	m2 = getMHourglass();

	if (!tangent4.isEmpty()) {
		std::cout << "4 not empty \n";
		concatenatedSide2 = concatenateClosed(resultGeneral.funnelSideB, hourglassSide2, tangent4);
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "3 not empty \n";
		concatenatedSide2 = concatenateClosed(resultGeneral.funnelSideB, hourglassSide1, tangent3);
	}

	m3 = getMFunnel();
	m4 = getMHourglass();

	rootInFunnel1 = false;
}


void TwoPointQuery::concatenateBlockedOpenHourglass(QVector<QPointF>& tangent2, QVector<QPointF>& tangent3) {
	if (!tangent2.isEmpty()) {
		std::cout << "Open: 2 not empty \n";
		concatenatedSide1 = concatenateOpen1(resultGeneral.funnelSideA, hourglassSide1, tangent2);
		m1 = getMFunnel();
		m2 = getMHourglass();
		concatenatedSide2 = concatenateOpen2(resultGeneral.funnelSideB, hourglassSide2, tangent2);
		m3 = getMFunnel();
		m4 = getMHourglass();
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "Open: 3 not empty \n";
		concatenatedSide1 = concatenateOpen2(resultGeneral.funnelSideA, hourglassSide1, tangent3);
		m1 = getMFunnel();
		m2 = getMHourglass();
		concatenatedSide2 = concatenateOpen1(resultGeneral.funnelSideB, hourglassSide2, tangent3);
		m3 = getMFunnel();
		m4 = getMHourglass();
	}

	rootInFunnel1 = false;
}


void TwoPointQuery::concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent4) {
	std::cout << "Open \n";
	concatenatedSide1 = concatenateClosed(resultGeneral.funnelSideA, hourglassSide1, tangent1);
	m1 = getMFunnel();
	m2 = getMHourglass();
	m2Index = getHourglassIndex();
	concatenatedSide2 = concatenateClosed(resultGeneral.funnelSideB, hourglassSide2, tangent4);
	m3 = getMFunnel();
	m4 = getMHourglass();
	m4Index = getHourglassIndex();

	rootInFunnel1 = true;
}


void TwoPointQuery::computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2) {
	std::cout << "root in funnel \n";
	QPointF mirrorM1 = mirrorPoint(m1, window1);
	QPointF mirrorM3 = mirrorPoint(m3, window1);
	bool searchFirstHalf1 = searchFirstHalf(m2, m2Index, concatenatedSide1, mirrorM1, window2);
	bool searchFirstHalf2 = searchFirstHalf(m4, m4Index, concatenatedSide2, mirrorM3, window2);



	// root start calculations
	size_t minLength = std::min(concatenatedSide1.size(), concatenatedSide2.size());
	for (size_t i = 0; i < minLength; ++i)
	{
		if (concatenatedSide1[i] == concatenatedSide2[i])
		{
			rootStar = concatenatedSide1[i];
			rootStarIndex = i;
		}
		else
		{
			break; // Paths diverge, stop the search
		}
	}

	for (size_t i = rootStarIndex; i < concatenatedSide1.size(); ++i) {
		pathRA2.append(concatenatedSide1[i]);
	}

	for (size_t i = rootStarIndex; i < concatenatedSide2.size(); ++i) {
		pathRB2.append(concatenatedSide2[i]);
	}

	for (size_t i = 0; i <= rootStarIndex; ++i) {
		optimalPath.append(concatenatedSide1[i]);
	}

	optimalPath.append(computeOptimalPathQ2(pathRA2, pathRB2, m1, m2, m3, m4, window1, searchFirstHalf1, searchFirstHalf2));
	c = optimalPath.rbegin()[0];
}


void TwoPointQuery::computeOptimalPathRootInHourglass(QLineF& window2) {
	std::cout << "root in hourglass \n";
	pathRA2.clear();
	pathRB2.clear();
	rootStarIndex = 0;
	rootStar = m4;

	for (int i = concatenatedSide1.size() - 1; i >= 0; --i) {
		if (concatenatedSide1[i] == rootStar) {
			rootStarIndex = i;
			break;
		}
	}
	for (int i = rootStarIndex; i < concatenatedSide1.size(); ++i) {
		pathRA2.append(concatenatedSide1[i]);
	}

	for (int i = concatenatedSide2.size() - 1; i >= 0; --i) {
		if (concatenatedSide2[i] == rootStar) {
			rootStarIndex = i;
			break;
		}
	}
	for (int i = rootStarIndex; i < concatenatedSide2.size(); ++i) {
		pathRB2.append(concatenatedSide2[i]);
	}

	c = m_onePointHandler.computeOptimalPoint(pathRA2, pathRB2, rootStar, window2);

	bool start = false;
	if (m_onePointHandler.getOnPathRootToA()) {
		for (int i = concatenatedSide1.size() - 1; i >= 0; --i) {
			if (concatenatedSide1[i] == m_onePointHandler.getVertexPerpendicularToC()) {
				start = true;
			}

			if (start) {
				optimalPath.append(concatenatedSide1[i]);
			}
		}
	}
	else {
		for (int i = concatenatedSide2.size() - 1; i >= 0; --i) {
			if (concatenatedSide2[i] == m_onePointHandler.getVertexPerpendicularToC()) {
				start = true;
			}

			if (start) {
				optimalPath.append(concatenatedSide2[i]);
			}
		}
	}

	if (c != optimalPath.rbegin()[0]) {
		optimalPath.append(c);
	}
}


void TwoPointQuery::constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon) {
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	hourglassSide1 = m_shortestPathHandler.findShortestPath(a1, a2, polygon);
	hourglassSide2 = m_shortestPathHandler.findShortestPath(b1, b2, polygon);

	QVector<QPointF> hourglassSide1Alt = m_shortestPathHandler.findShortestPath(a1, b2, polygon);
	QVector<QPointF> hourglassSide2Alt = m_shortestPathHandler.findShortestPath(b1, a2, polygon);

	bool sideOneDegen = std::adjacent_find(hourglassSide1.begin(), hourglassSide1.end(), std::not_equal_to<>()) == hourglassSide1.end();
	bool sideTwoDegen = std::adjacent_find(hourglassSide2.begin(), hourglassSide2.end(), std::not_equal_to<>()) == hourglassSide2.end();

	if (sideOneDegen || sideTwoDegen) {
		std::cout << "one side is degen \n";
		m_isHourglassOpen = true;
		return;
	}
	else if (hourglassSide1.size() == 1 || hourglassSide2.size() == 1) {
		std::cout << "one side is degen \n";
		m_isHourglassOpen = true;
		return;
	}

	m_isHourglassOpen = hourglassOpen(hourglassSide1, hourglassSide2);

	if (m_isHourglassOpen) {
		std::cout << "open hourglass \n";
	}
	else {
		std::cout << "closed hourglass \n";
		if (hourglassOpen(hourglassSide1Alt, hourglassSide2Alt)) {
			std::cout << "alt hourglass is open \n";
			hourglassSide1 = hourglassSide1Alt;
			hourglassSide2 = hourglassSide2Alt;
			m_isHourglassOpen = true;
		}
	}

	return;
}

TwoPointQuery::GeneralCaseResult TwoPointQuery::getGeneralCaseResult() {
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




