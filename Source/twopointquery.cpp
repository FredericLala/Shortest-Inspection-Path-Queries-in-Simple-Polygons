#include "twopointquery.h"

TwoPointQuery::TwoPointQuery()
	: m_startSelected(false), m_query1Selected(false), m_query2Selected(false)
{
}

void TwoPointQuery::setStartingPoint(const QPointF& point)
{
	m_startingPoint = point;
	m_startSelected = true;
}

void TwoPointQuery::setQueryPoint1(const QPointF& point)
{
	m_queryPoint1 = point;
	m_query1Selected = true;
}

void TwoPointQuery::setQueryPoint2(const QPointF& point)
{
	m_queryPoint2 = point;
	m_query2Selected = true;
}

void TwoPointQuery::clearPoints()
{
	m_startSelected = false;
	m_query1Selected = false;
	m_query2Selected = false;
}

QPointF TwoPointQuery::getStartingPoint() const
{
	return m_startingPoint;
}

QPointF TwoPointQuery::getQueryPoint1() const
{
	return m_queryPoint1;
}

QPointF TwoPointQuery::getQueryPoint2() const
{
	return m_queryPoint2;
}

bool TwoPointQuery::isStartingPointSet() const
{
	return m_startSelected;
}

bool TwoPointQuery::isQueryPoint1Set() const
{
	return m_query1Selected;
}

bool TwoPointQuery::isQueryPoint2Set() const
{
	return m_query2Selected;
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
	const QPointF a = segment.p1();
	const QPointF b = segment.p2();

	shortestPathHandler.createMesh(polygon); // TODO: give mesh and not recalculate it

	const Point_2 lcaC = shortestPathHandler.getLCA(start, a, b, polygon);
	const QPointF lca = QPointF(lcaC.x(), lcaC.y());

	const std::vector<Point_3> pathCRA = shortestPathHandler.findShortestPath(lca, a, polygon);
	std::vector<Point_3> pathCRB = shortestPathHandler.findShortestPath(b, lca, polygon); // makes the calculations more consistent
	pathCRB = shortestPathHandler.reversePath(pathCRB); // need to reverse to get the path from b to lca
	QVector<QPointF> pathRA = convertToQT(pathCRA);
	QVector<QPointF> pathRB = convertToQT(pathCRB);

	const QPointF c = onePointHandler.computeOptimalPoint(pathRA, pathRB, segment);

	const std::vector<Point_3> pathCSC = shortestPathHandler.findShortestPath(start, c, polygon);
	const QVector<QPointF> pathSC = convertToQT(pathCSC);
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

/*
void TwoPointQuery::generalCase(QLineF& window1, QLineF& window2, Polygon_2& polygon)
{
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();
	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();
	shortestPathHandler.createMesh(polygon); // TODO: give mesh and not recalculate it


	std::vector<Point_3> PathSA1 = shortestPathHandler.findShortestPath(m_startingPoint, a1, polygon);
	std::vector<Point_3> PathSB1 = shortestPathHandler.findShortestPath(b1, m_startingPoint, polygon);
	PathSB1 = shortestPathHandler.reversePath(PathSB1);

	pathSA1 = convertToQT(PathSA1);
	pathSB1 = convertToQT(PathSB1);
	QPointF hatS1 = pathSA1.begin()[1];
	QPointF hatS2 = pathSB1.begin()[1];
	QPointF root;

	if (hatS1 != hatS2)
	{
		root = m_startingPoint;
	}
	else
	{
		Point_2 rootP = shortestPathHandler.getLCA(m_startingPoint, a1, b1, polygon);
		root = QPointF(rootP.x(), rootP.y());
	}


	QVector<QPointF> funnelSide1 = convertToQT(shortestPathHandler.findShortestPath(root, a1, polygon));
	QVector<QPointF> funnelSide2 = convertToQT(shortestPathHandler.findShortestPath(root, b1, polygon));

	QVector<QPointF> hourglassSide1 = convertToQT(shortestPathHandler.findShortestPath(a1, a2, polygon));
	QVector<QPointF> hourglassSide2 = convertToQT(shortestPathHandler.findShortestPath(b1, b2, polygon));

	QVector<QPointF> tangent1 = findTangent(funnelSide1, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent2 = findTangent(funnelSide1, hourglassSide2, window1, polygon);
	QVector<QPointF> tangent3 = findTangent(funnelSide2, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent4 = findTangent(funnelSide2, hourglassSide2, window1, polygon);


	funnelSide1 = convertToQT(shortestPathHandler.findShortestPath(root, a1, polygon));
	std::cout << "Funnel size 1: " << funnelSide1.size() << "\n";
	funnelSide2 = convertToQT(shortestPathHandler.reversePath(shortestPathHandler.findShortestPath(b1, root, polygon)));
	std::cout << "Funnel size 2: " << funnelSide2.size() << "\n";

	hourglassSide1 = convertToQT(shortestPathHandler.findShortestPath(a1, a2, polygon));
	hourglassSide2 = convertToQT(shortestPathHandler.findShortestPath(b1, b2, polygon));

	tangent1 = findTangent(funnelSide1, hourglassSide1, window1, polygon);
	tangent2 = findTangent(funnelSide1, hourglassSide2, window1, polygon);
	tangent3 = findTangent(funnelSide2, hourglassSide1, window1, polygon);
	tangent4 = findTangent(funnelSide2, hourglassSide2, window1, polygon);
}
*/

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

	if (!(onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
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

	if (!(onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
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

	if (!(onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
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

	if (!(onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
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
		angle1 = onePointHandler.calculateFunnelAngle(m, concatenatedSide[mIndex + 1], a2, b2);
		angle2 = onePointHandler.calculateFunnelAngle(mirrorPoint, m, a2, b2);

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
	const double angle0 = onePointHandler.calculateFunnelAngle(pathRA2.rbegin()[1], pathRA2.rbegin()[0], a2, b2); // theta_0
	if (angle0 > 90)
	{
		std::cout << "c = a2 \n";
		optimalPath = pathRA2;
		return optimalPath;
	}

	const double anglek = onePointHandler.calculateFunnelAngle(pathRB2.rbegin()[1], pathRB2.rbegin()[0], a2, b2); // theta_k
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

	const double anglem1 = onePointHandler.calculateFunnelAngle(mirrorPoint1, mirrorPoint2, a2, b2); // theta_m-1

	mirrorPoint1 = mirrorPoint(pathRB2.begin()[0], window1);
	mirrorPoint2 = mirrorPoint(pathRB2.begin()[1], window1);

	const double anglem = onePointHandler.calculateFunnelAngle(mirrorPoint1, mirrorPoint2, a2, b2);  // theta_m
	if (anglem1 == 90 || anglem == 90 || (anglem1 <= 90 && 90 < anglem))
	{
		std::cout << "c is at the foot of the reflected perpendicular from root*";
		line = QLineF(rootStar, mirrorPoint(rootStar, window1));
		QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
		intersectionPointW2 = onePointHandler.calculateWindowIntersection(mirrorPoint(rootStar, window1), a2, b2);

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

			vertex = onePointHandler.binarySearchByAngle(mirrorPath, a2, b2);
			v = (pathRA2.size() - 1) - vertex; // root would be lowest, but needs to be highest

			std::cout << "c is at the foot of the perpendicular from reflected v" + v;

			line = QLineF(firstHalfPath[vertex], mirrorPath[vertex]);
			QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
			intersectionPointW2 = onePointHandler.calculateWindowIntersection(intersectionPointW1, a2, b2);

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

			vertex = onePointHandler.binarySearchByAngle(secondHalfPath, a2, b2);
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

			optimalPath.append(onePointHandler.calculateWindowIntersection(secondHalfPath[vertex], a2, b2));

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

			vertex = onePointHandler.binarySearchByAngle(mirrorPath, a2, b2);
			v = vertex + (pathRB2.size() - 1);
			std::cout << "c is at the foot of the perpendicular from reflected v" + v;

			line = QLineF(firstHalfPath[vertex], mirrorPath[vertex]);
			QLineF::IntersectType type = line.intersects(window1, &intersectionPointW1);
			intersectionPointW2 = onePointHandler.calculateWindowIntersection(intersectionPointW1, a2, b2);

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

			vertex = onePointHandler.binarySearchByAngle(secondHalfPath, a2, b2);
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

			optimalPath.append(onePointHandler.calculateWindowIntersection(secondHalfPath[vertex], a2, b2));

			return optimalPath;
		}
	}
}





