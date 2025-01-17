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
bool TwoPointQuery::isHourglassOpenCheck(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2) {
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

TwoPointQuery::TangentStruct TwoPointQuery::findTangent(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);
	QLineF line = QLineF(funnelPoint, mirroredPoint);
	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);
	TangentStruct tangent;

	tangent.funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	tangent.hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	if (!isTangent(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon) && !isTangent(QLineF(intersectionPoint, hourglassPoint), hourglassSide, polygon)) {
		tangent.failure = FUNNEL_HOURGLASS;
		return tangent;
	}

	// if the line is not tangent to the funnel, it can not be the tangent
	if (!isTangent(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon)) {
		tangent.failure = FUNNEL;
		return tangent;
	}

	if (!isTangent(QLineF(intersectionPoint, hourglassPoint), hourglassSide, polygon)) {
		tangent.failure = HOURGLASS;
		return tangent;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		tangent.failure = VISIBILITY;
		return tangent;
	}

	QVector<QPointF> tangentPath;
	tangentPath.append(funnelPoint);
	tangentPath.append(intersectionPoint);
	tangentPath.append(hourglassPoint);

	tangent.tangentPath = tangentPath;
	return tangent;
}

bool TwoPointQuery::isTangent(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon) {
	int intersectionCount = numberOfIntersections(line, side);
	//std::cout << "intersection count tangency check: " << intersectionCount << "\n";

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

// edge case for hourglass path endpoints
TwoPointQuery::TangentStruct TwoPointQuery::findTangentEdgeHourglass(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);
	QLineF line = QLineF(funnelPoint, mirroredPoint);
	TangentStruct tangent;


	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	tangent.funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	tangent.hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	// if the line is not tangent to the funnel, it can not be the tangent
	if (!isTangentEdgeHourglass(QLineF(funnelPoint, intersectionPoint), funnelSide, polygon)) {
		tangent.failure = FUNNEL;
		return tangent;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		tangent.failure = VISIBILITY;
		return tangent;
	}

	QVector<QPointF> tangentPath;
	tangentPath.append(funnelPoint);
	tangentPath.append(intersectionPoint);

	// case where the tangent ends on the very first point on the hourglass side
	if (intersectionPoint == hourglassPoint) {
		tangentPath.append(hourglassSide[1]);
	}
	else {
		tangentPath.append(hourglassPoint); // case where the tangent ends on the very last point on the hourglass side
	}

	tangent.tangentPath = tangentPath;
	return tangent;
}

bool TwoPointQuery::isTangentEdgeHourglass(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon) {
	int intersectionCount = numberOfIntersections(line, side);
	//std::cout << "intersection count tangency check: " << intersectionCount << "\n";

	if (intersectionCount == 0) {
		// Extend the line slightly
		QPointF direction = line.p2() - line.p1();
		double extendFactor = 0.01; // Small extension factor
		direction *= extendFactor / std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

		QPointF newStart = line.p1() - direction;
		QPointF end = line.p2();

		QLineF extendedLine(newStart, end);

		if (polygon.has_on_unbounded_side(Point_2(newStart.x(), newStart.y()))) {
			// If either of the extended endpoints are outside the polygon, it cant be tangent 
			return false;
		}

		return true;
	}

	return false;
}


// edge case for funnel path endpoints
TwoPointQuery::TangentStruct TwoPointQuery::findTangentEdgeFunnel(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	QPointF intersectionPoint;
	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);
	QLineF line = QLineF(funnelPoint, mirroredPoint);
	TangentStruct tangent;

	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);

	tangent.funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	tangent.hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);
	QLineF intersectionHourglassLine = QLineF(intersectionPoint, hourglassPoint);

	if (!isTangentEdgeFunnel(intersectionHourglassLine, hourglassSide, polygon)) {
		tangent.failure = HOURGLASS;
		return tangent;
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		tangent.failure = VISIBILITY;
		return tangent;
	}

	QVector<QPointF> tangentPath;
	tangentPath.append(funnelPoint);
	tangentPath.append(intersectionPoint);
	tangentPath.append(hourglassPoint);

	tangent.tangentPath = tangentPath;
	return tangent;
}

bool TwoPointQuery::isTangentEdgeFunnel(const QLineF& line, const QVector<QPointF>& side, const Polygon_2& polygon) {
	int intersectionCount = numberOfIntersections(line, side);
	//std::cout << "intersection count tangency check: " << intersectionCount << "\n";

	if (intersectionCount == 0) {
		// Extend the line slightly
		QPointF direction = line.p2() - line.p1();
		double extendFactor = 0.01; // Small extension factor
		direction *= extendFactor / std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

		QPointF start = line.p1();
		QPointF newEnd = line.p2() + direction;

		QLineF extendedLine(start, newEnd);

		if (polygon.has_on_unbounded_side(Point_2(newEnd.x(), newEnd.y()))) {
			// If either of the extended endpoints are outside the polygon, it cant be tangent 
			return false;
		}

		return true;
	}

	return false;
}


// edge case for hourglass and funnel path endpoints
TwoPointQuery::TangentStruct TwoPointQuery::findTangentEdgeBoth(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon)
{
	//std::cout << "edge both \n";
	QPointF intersectionPoint;
	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);
	QLineF line = QLineF(funnelPoint, mirroredPoint);
	//line = QLineF(funnelPoint, hourglassPoint);
	//std::cout << "hourglass point: " << hourglassPoint.x() << ", " << hourglassPoint.y() << "\n";
	TangentStruct tangent;

	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);
	//std::cout << "int point: " << intersectionPoint.x() << ", " << intersectionPoint.y() << "\n";

	tangent.funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	tangent.hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		std::cout << "vis failure \n";
		tangent.failure = VISIBILITY;
		return tangent;
	}

	QVector<QPointF> tangentPath;

	// case where the tangent ends on the very first point on the hourglass side
	if (hourglassSide.begin()[0] == hourglassPoint) {
		if (funnelSide.rbegin()[0] == hourglassPoint) { // funnel and hourglass are on the same side (outer tangent)
			tangentPath.append(funnelSide.rbegin()[0]);
			tangentPath.append(intersectionPoint);
			tangentPath.append(hourglassSide[1]);
		}
		else { // funnel and hourglass are not the same side (inner tangent)
			tangentPath.append(funnelPoint);
			tangentPath.append(intersectionPoint);
			tangentPath.append(hourglassSide[1]);
		}
	}
	else {  // case where the tangent ends on the very last point on the hourglass side
		tangentPath.append(funnelPoint);
		tangentPath.append(intersectionPoint);
		tangentPath.append(hourglassPoint);
	}

	tangent.tangentPath = tangentPath;
	return tangent;
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
	QVector<QPointF> funnelVec = funnelSide;
	QPointF mirroredPoint;
	QLineF line;
	QPointF reflectionPoint;
	QPointF intersectionPoint;
	bool funnelTangency = false;
	int funnelIntersections = 0;

	bool edgeFunnel = false;
	bool edgeHourglass = false;
	bool invalidEdgeCase = false;

	int funnelLeft = 0;
	int funnelRight = funnelVec.size() - 1;

	int hourglassLeft = 0;
	int hourglassRight = hourglassSide.size() - 1;

	TangentStruct tangent;
	funnelSideTest = funnelSide;
	funnelVecSideTest = funnelVec;

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
			// TODO: tangent that covers this case is still found (but it wont be counted as a valid funnelStar side) -> still but be better if it wouldnt
			if (funnelMid == funnelSide.size() && hourglassMid == 0) {
				continue;
			}

			int funnelIntersections = tangent.funnelIntersections;
			int hourglassIntersections = tangent.hourglassIntersections;

			if (edgeFunnel && edgeHourglass) {
				tangent = findTangentEdgeBoth(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				std::cout << "Edge Both: number of intersections: " << funnelIntersections << "\n";
			}
			else if (edgeHourglass) {
				tangent = findTangentEdgeHourglass(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				std::cout << "Edge Hourglass: number of intersections: " << funnelIntersections << "\n";
			}
			else if (edgeFunnel) {
				tangent = findTangentEdgeFunnel(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
				std::cout << "Edge Funnel: number of intersections: " << hourglassIntersections << "\n";
			}
			else {
				tangent = findTangent(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon);
			}

			switch (tangent.failure)
			{
			case TwoPointQuery::NONE: // case a.
				std::cout << "case a \n";
				return tangent.tangentPath;
				break;
			case TwoPointQuery::FUNNEL_HOURGLASS:
				if (funnelIntersections == 0 && hourglassIntersections == 0) { // case f.
					std::cout << "case f \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1 && hourglassIntersections == 0) { // case g.
					std::cout << "case g \n";
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 0 && hourglassIntersections == 1) { // case h.
					std::cout << "case h \n";
					funnelRight = funnelMid - 1;
				}

				if (funnelIntersections >= 1 && hourglassIntersections >= 1) { // case i.
					std::cout << "case i \n";
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
						std::cout << "case i2 \n";
						funnelLeft = funnelMid + 1;
					}
					else { // case i1.
						std::cout << "case i1 \n";
						hourglassRight = hourglassMid - 1;
					}
				}
				break;

			case TwoPointQuery::FUNNEL:
				if (funnelIntersections == 0) { // case d.
					std::cout << "case d \n";
					std::cout << funnelMid - 1 << "\n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1) { // case e.
					std::cout << "case e \n";
					funnelLeft = funnelMid + 1;
					hourglassLeft = hourglassMid + 1;
				}

				break;
			case TwoPointQuery::HOURGLASS:
				if (hourglassIntersections == 0) { // case b.
					std::cout << "case b \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (hourglassIntersections == 1) { // case c.
					std::cout << "case c \n";
					funnelRight = funnelMid - 1;
					hourglassRight = hourglassMid - 1;
				}

				break;
			case TwoPointQuery::VISIBILITY:
				std::cout << "vis \n";
				hourglassRight = hourglassMid - 1;
				if (hourglassRight <= hourglassLeft) {
					funnelVec.removeAt(funnelMid);
				}

				if (funnelVec.isEmpty()) {
					return tangent.tangentPath;
				}
				break;
			default:
				std::cout << "default \n";

				break;
			}
		}
	}
	return tangent.tangentPath;
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

void TwoPointQuery::executeTwoPointQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon, Surface_mesh& mesh)
{
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

			computeGeneralCase(startingPoint, window1, window2, polygon, mesh);
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
	return;
}


void TwoPointQuery::computeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh) {
	/*
	generalCase(startingPoint, window1, window2, polygon, mesh);
	GeneralCaseResult firstWindow1 = resultGeneral;
	double tempOptimalPathLength1 = calculatePathLength(firstWindow1.optimalPath);
	*/

	generalCase(startingPoint, window2, window1, polygon, mesh);
	GeneralCaseResult firstWindow2 = resultGeneral;
	double tempOptimalPathLength2 = calculatePathLength(firstWindow2.optimalPath);

	/*
	if (tempOptimalPathLength2 >= tempOptimalPathLength1) {
		resultGeneral = firstWindow1;
		return;
	}
	*/

	//resultGeneral = firstWindow1;
	resultGeneral = firstWindow2;
}


void TwoPointQuery::generalCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh)
{
	std::cout << "General case: " << "\n";
	currentCase = GENERAL;
	resultQ2.currentCase = GENERAL;

	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> pathStartToA1 = m_shortestPathHandler.findShortestPath(startingPoint, a1, polygon, mesh);
	QVector<QPointF> pathStartToB1 = m_shortestPathHandler.findShortestPath(startingPoint, b1, polygon, mesh);

	QPointF penultimatePointSToA1 = pathStartToA1.begin()[1];
	QPointF penultimatePointSToB1 = pathStartToB1.begin()[1];
	QPointF funnelRoot;

	if (penultimatePointSToA1 != penultimatePointSToB1)
	{
		funnelRoot = startingPoint;
	}
	else
	{
		funnelRoot = m_shortestPathHandler.getLCA(pathStartToA1, pathStartToB1);
	}
	resultGeneral.funnelRoot = funnelRoot;

	FunnelStruct funnel;
	QVector<QPointF> funnelSideA = m_shortestPathHandler.findShortestPath(funnelRoot, a1, polygon, mesh);
	funnel.funnelSideA = funnelSideA;
	resultGeneral.funnelSideA = funnelSideA;
	QVector<QPointF> funnelSideB = m_shortestPathHandler.findShortestPath(funnelRoot, b1, polygon, mesh);
	funnel.funnelSideB = funnelSideB;
	resultGeneral.funnelSideB = funnelSideB;

	HourglassStruct hourglass = constructHourglass(window1, window2, polygon, mesh);
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	resultGeneral.hourglassSide1 = hourglassSide1;
	resultGeneral.hourglassSide2 = hourglassSide2;

	////
	// TEST
	firstWindow = window1;
	/////

	QVector<QPointF> tangent1 = tangentBinary(funnelSideA, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent2 = tangentBinary(funnelSideA, hourglassSide2, window1, polygon);
	QVector<QPointF> tangent3 = tangentBinary(funnelSideB, hourglassSide1, window1, polygon);
	QVector<QPointF> tangent4 = tangentBinary(funnelSideB, hourglassSide2, window1, polygon);
	resultGeneral.tangent1 = tangent1;
	resultGeneral.tangent2 = tangent2;
	resultGeneral.tangent3 = tangent3;
	resultGeneral.tangent4 = tangent4;

	bool areOuterTangentsBlocked = (tangent1.isEmpty() || tangent4.isEmpty());


	FunnelStar funnelStar;
	if (!hourglass.isOpen) {
		funnelStar = concatenateClosedHourglass(tangent1, tangent2, tangent3, tangent4, funnel, hourglass);
	}
	else {
		if (areOuterTangentsBlocked) {
			funnelStar = concatenateBlockedOpenHourglass(tangent2, tangent3, funnel, hourglass);
		}
		else {
			funnelStar = concatenateOpenHourglass(tangent1, tangent4, funnel, hourglass);

		}
	}

	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	resultGeneral.concatenatedSide1 = funnelStarSide1;
	resultGeneral.concatenatedSide2 = funnelStarSide2;
	resultGeneral.m1 = funnelStar.m1;
	resultGeneral.m2 = funnelStar.m2;
	resultGeneral.m3 = funnelStar.m3;
	resultGeneral.m4 = funnelStar.m4;

	QVector<QPointF> optimalPath;
	QVector<QPointF> pathStartToFunnelRoot = m_shortestPathHandler.findShortestPath(startingPoint, funnelRoot, polygon, mesh);
	if (funnelStar.isRootInFunnel) {
		optimalPath = computeOptimalPathRootInFunnel(window1, window2, funnelStar, pathStartToFunnelRoot);
	}
	else {
		optimalPath = computeOptimalPathRootInHourglass(window2, funnelStar, pathStartToFunnelRoot);
	}

	QPointF c = optimalPath.rbegin()[0];

	resultGeneral.optimalPath = optimalPath;
	resultGeneral.optimalPoint = c;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hourglass Funnel Concatenation

TwoPointQuery::ConcatenatedSideStruct TwoPointQuery::concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;
	int indexCount = 0;

	for (QPointF funnelPoint : funnelSide) {
		if (funnelPoint != tangent[0]) {
			concatenatedSide.append(funnelPoint);
			++indexCount;
		}
		else {
			concatenatedSideStruct.mFunnelIndex = indexCount;
			concatenatedSideStruct.mFunnelPoint = funnelPoint;
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
			concatenatedSideStruct.mHourglassIndex = indexCount;
			concatenatedSideStruct.mHourglassPoint = hourglassPoint;
			start = true;
		}
	}


	concatenatedSideStruct.concatenatedSide = concatenatedSide;

	return concatenatedSideStruct;
}


TwoPointQuery::ConcatenatedSideStruct TwoPointQuery::concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
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

	concatenatedSideStruct.mFunnelPoint = tangent[0];
	concatenatedSideStruct.mHourglassPoint = tangent.rbegin()[0];

	concatenatedSide.append(hourglassSide.rbegin()[0]);

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}


TwoPointQuery::ConcatenatedSideStruct TwoPointQuery::concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;

	concatenatedSide.append(funnelSide[0]);

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}

	concatenatedSideStruct.mFunnelPoint = tangent[0];
	concatenatedSideStruct.mHourglassPoint = tangent.rbegin()[0];

	bool start = false;
	for (QPointF hourglassPoint : hourglassSide) {
		if (start) {
			concatenatedSide.append(hourglassPoint);
		}

		if (hourglassPoint == tangent.rbegin()[0]) {
			start = true;
		}
	}

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}

TwoPointQuery::FunnelStar TwoPointQuery::concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate closed hourglass \n";

	if (!tangent1.isEmpty()) {
		std::cout << "1 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide1, tangent1);

	}
	else if (!tangent2.isEmpty()) {
		std::cout << "2 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide2, tangent2);
	}
	funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;

	funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

	if (!tangent4.isEmpty()) {
		std::cout << "4 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide2, tangent4);
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "3 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide1, tangent3);
	}

	funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;

	funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;

	funnelStar.isRootInFunnel = false;
	return funnelStar;
}


TwoPointQuery::FunnelStar TwoPointQuery::concatenateBlockedOpenHourglass(QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate blocked open hourglass \n";

	if (!tangent2.isEmpty()) {
		std::cout << "Open: 2 not empty \n";
		concatenatedSideStruct = concatenateOpen1(funnelSideA, hourglassSide1, tangent2);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateOpen2(funnelSideB, hourglassSide2, tangent2);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "Open: 3 not empty \n";
		concatenatedSideStruct = concatenateOpen2(funnelSideA, hourglassSide1, tangent3);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateOpen1(funnelSideB, hourglassSide2, tangent3);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}

	funnelStar.isRootInFunnel = false;

	return funnelStar;
}


TwoPointQuery::FunnelStar TwoPointQuery::concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate open hourglass \n";

	concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide1, tangent1);
	funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
	funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;
	funnelStar.m2Index = concatenatedSideStruct.mHourglassIndex;

	concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide2, tangent4);
	funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
	funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	funnelStar.m4Index = concatenatedSideStruct.mHourglassIndex;

	funnelStar.isRootInFunnel = true;

	return funnelStar;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

QVector<QPointF> TwoPointQuery::computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot) {
	std::cout << "root in funnel \n";
	QVector<QPointF> pathRootToA2;
	QVector<QPointF> pathRootToB2;
	QVector<QPointF> optimalPath;
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	QPointF funnelStarRoot;
	int funnelStarRootIndex = 0;

	QPointF mirrorM1 = mirrorPoint(funnelStar.m1, window1);
	QPointF mirrorM3 = mirrorPoint(funnelStar.m3, window1);
	bool searchFirstHalf1 = searchFirstHalf(funnelStar.m2, funnelStar.m2Index, funnelStarSide1, mirrorM1, window2);
	bool searchFirstHalf2 = searchFirstHalf(funnelStar.m4, funnelStar.m4Index, funnelStarSide2, mirrorM3, window2);


	// root start calculations
	size_t minLength = std::min(funnelStarSide1.size(), funnelStarSide2.size());
	for (size_t i = 0; i < minLength; ++i)
	{
		if (funnelStarSide1[i] == funnelStarSide2[i])
		{
			funnelStarRoot = funnelStarSide1[i];
			funnelStarRootIndex = i;
		}
		else
		{
			break; // Paths diverge, stop the search
		}
	}

	for (size_t i = funnelStarRootIndex; i < funnelStarSide1.size(); ++i) {
		pathRootToA2.append(funnelStarSide1[i]);
	}

	for (size_t i = funnelStarRootIndex; i < funnelStarSide2.size(); ++i) {
		pathRootToB2.append(funnelStarSide2[i]);
	}

	for (size_t i = 0; i < pathStartToFunnelRoot.size(); ++i) {
		optimalPath.append(pathStartToFunnelRoot[i]);
	}
	for (size_t i = 0; i <= funnelStarRootIndex; ++i) {
		optimalPath.append(funnelStarSide1[i]); // arbitrary funnelStarSide
	}

	optimalPath.append(computeOptimalPathQ2(pathRootToA2, pathRootToB2, funnelStar.m1, funnelStar.m2, funnelStar.m3, funnelStar.m4, window1, searchFirstHalf1, searchFirstHalf2));
	QPointF c = optimalPath.rbegin()[0];

	return optimalPath;
}


QVector<QPointF> TwoPointQuery::computeOptimalPathRootInHourglass(QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot) {
	std::cout << "root in hourglass \n";
	QVector<QPointF> pathA2ToRoot;
	QVector<QPointF> pathB2ToRoot;
	QVector<QPointF> pathRootToA2;
	QVector<QPointF> pathRootToB2;
	QVector<QPointF> optimalPath;
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	QPointF funnelStarRoot = funnelStar.m4;
	starRoot = funnelStarRoot;
	int funnelStarRootIndex = 0;

	for (int i = funnelStarSide1.size() - 1; i >= 0; --i) {
		if (funnelStarSide1[i] != funnelStarRoot) {
			pathA2ToRoot.append(funnelStarSide1[i]);
		}
		else {
			pathA2ToRoot.append(funnelStarSide1[i]);
			break;
		}
	}
	pathRootToA2 = m_shortestPathHandler.reversePath(pathA2ToRoot);

	for (int i = funnelStarSide2.size() - 1; i >= 0; --i) {
		if (funnelStarSide2[i] != funnelStarRoot) {
			pathB2ToRoot.append(funnelStarSide2[i]);
		}
		else {
			pathB2ToRoot.append(funnelStarSide2[i]);
			break;
		}
	}
	pathRootToB2 = m_shortestPathHandler.reversePath(pathB2ToRoot);

	////
	// TEST
	PRAT = pathRootToA2;
	PRBT = pathRootToB2;
	//

	QPointF c = m_onePointHandler.computeOptimalPoint(pathRootToA2, pathRootToB2, funnelStarRoot, window2);
	QPointF vertexPerpendicularToC = m_onePointHandler.getVertexPerpendicularToC();
	if (c != vertexPerpendicularToC) {
		optimalPath.append(c);
	}

	bool start = false;
	if (m_onePointHandler.getOnPathRootToA()) {
		for (int i = funnelStarSide1.size() - 1; i >= 0; --i) {
			if (funnelStarSide1[i] == vertexPerpendicularToC) {
				start = true;
			}

			if (start) {
				optimalPath.append(funnelStarSide1[i]);
			}
		}
	}
	else {
		for (int i = funnelStarSide2.size() - 1; i >= 0; --i) {
			if (funnelStarSide2[i] == vertexPerpendicularToC) {
				start = true;
			}

			if (start) {
				optimalPath.append(funnelStarSide2[i]);
			}
		}
	}

	for (int i = pathStartToFunnelRoot.size() - 1; i >= 0; --i) {
		optimalPath.append(pathStartToFunnelRoot[i]);
	}


	return m_shortestPathHandler.reversePath(optimalPath);
}


TwoPointQuery::HourglassStruct TwoPointQuery::constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh) {
	TwoPointQuery::HourglassStruct hourglass;
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> hourglassSide1 = m_shortestPathHandler.findShortestPath(a1, a2, polygon, mesh);
	QVector<QPointF> hourglassSide2 = m_shortestPathHandler.findShortestPath(b1, b2, polygon, mesh);

	QVector<QPointF> hourglassSide1Alt = m_shortestPathHandler.findShortestPath(a1, b2, polygon, mesh);
	QVector<QPointF> hourglassSide2Alt = m_shortestPathHandler.findShortestPath(b1, a2, polygon, mesh);

	bool sideOneDegen = std::adjacent_find(hourglassSide1.begin(), hourglassSide1.end(), std::not_equal_to<>()) == hourglassSide1.end();
	bool sideTwoDegen = std::adjacent_find(hourglassSide2.begin(), hourglassSide2.end(), std::not_equal_to<>()) == hourglassSide2.end();

	if (sideOneDegen || sideTwoDegen) {
		std::cout << "one side is degen \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
		return hourglass;
	}
	else if (hourglassSide1.size() == 1 || hourglassSide2.size() == 1) {
		std::cout << "one side is degen \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
		return hourglass;
	}

	if (isHourglassOpenCheck(hourglassSide1, hourglassSide2)) {
		std::cout << "open hourglass \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
	}
	else {
		std::cout << "closed hourglass \n";
		hourglass.isOpen = false;
		if (isHourglassOpenCheck(hourglassSide1Alt, hourglassSide2Alt)) {
			std::cout << "alt hourglass is open \n";
			hourglassSide1 = hourglassSide1Alt;
			hourglassSide2 = hourglassSide2Alt;
			hourglass.isOpen = true;
		}
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
	}

	return hourglass;
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




