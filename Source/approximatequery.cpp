#include "approximatequery.h"

ApproximateQuery::ApproximateQuery()
{
}

void ApproximateQuery::setStartingPoint(const QPointF& point)
{
	m_startingPoint = point;
	m_startSelected = true;
}

void ApproximateQuery::setQueryPoint1(const QPointF& point)
{
	m_queryPoint1 = point;
	m_query1Selected = true;
}

void ApproximateQuery::setQueryPoint2(const QPointF& point)
{
	m_queryPoint2 = point;
	m_query2Selected = true;
}

void ApproximateQuery::clearPoints()
{
	m_startSelected = false;
	m_query1Selected = false;
	m_query2Selected = false;
}

QPointF ApproximateQuery::getStartingPoint() const
{
	return m_startingPoint;
}

QPointF ApproximateQuery::getQueryPoint1() const
{
	return m_queryPoint1;
}

QPointF ApproximateQuery::getQueryPoint2() const
{
	return m_queryPoint2;
}

bool ApproximateQuery::isStartingPointSet() const
{
	return m_startSelected;
}

bool ApproximateQuery::isQueryPoint1Set() const
{
	return m_query1Selected;
}

bool ApproximateQuery::isQueryPoint2Set() const
{
	return m_query2Selected;
}

void ApproximateQuery::threeApproximateQuery(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon) {
	threeApproximatePath.clear();
	m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint1, polygon);
	resultQ1 = m_onePointHandler.getResult();
	threeApproximatePath.append(resultQ1.optimalPath);
	threeApproximatePath.append(m_shortestPathHandler.reversePath(resultQ1.optimalPath));

	m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint2, polygon);
	resultQ2 = m_onePointHandler.getResult();
	threeApproximatePath.append(resultQ2.optimalPath);
}

QVector<QPointF> ApproximateQuery::getThreeApproximatePath() { return threeApproximatePath; }

void ApproximateQuery::epsilonApproximateQuery(double epsilon, QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon) {
	// 1.
	threeApproximateQuery(startingPoint, queryPoint1, queryPoint2, polygon);
	QVector<QPointF> threeApproximatePath = getThreeApproximatePath();
	approximateResult.threeApproxPath = threeApproximatePath;
	double threeApproximatePathLength = m_twoPointHandler.calculatePathLength(threeApproximatePath);

	// 2.
	QLineF window1 = resultQ1.window;
	QLineF window2 = resultQ2.window;
	approximateResult.window1 = window1;
	approximateResult.window2 = window2;


	// 3.
	Point startingPointC = Point(startingPoint.x(), startingPoint.y());
	double radius = 2 * threeApproximatePathLength;
	approximateResult.discRadius = radius;
	double squaredRadius = radius * radius;
	Circle_2 disc = Circle_2(startingPointC, squaredRadius, CGAL::COUNTERCLOCKWISE);
	QVector<QPointF> intersectionsVector1 = computeDiscIntersection(disc, window1);
	QVector<QPointF> intersectionsVector2 = computeDiscIntersection(disc, window2);
	QLineF intersectionWindow1;
	QLineF intersectionWindow2;
	if (intersectionsVector1.size() == 2) {
		intersectionWindow1 = QLineF(intersectionsVector1[0], intersectionsVector1[1]);
	}
	if (intersectionsVector2.size() == 2) {
		intersectionWindow2 = QLineF(intersectionsVector2[0], intersectionsVector2[1]);
	}
	approximateResult.intersectionWindow1 = intersectionWindow1;
	approximateResult.intersectionWindow2 = intersectionWindow2;

	// 4.
	double const delta = epsilon * threeApproximatePathLength / 3;
	double const spacedDistance = delta / 3;
	QVector<QPointF> spacedPoints1 = generateEquallySpacedPoints(intersectionWindow1, spacedDistance);
	QVector<QPointF> spacedPoints2 = generateEquallySpacedPoints(intersectionWindow2, spacedDistance);
	approximateResult.equallySpacedPoints1 = spacedPoints1;
	approximateResult.equallySpacedPoints2 = spacedPoints2;

	// 5.
	QVector<QPointF> shortestPath = findShortestPathAmongPairs(spacedPoints1, spacedPoints2, startingPoint, polygon);
	approximateResult.shortestPath = shortestPath;
}

ApproximateQuery::ApproximateResult ApproximateQuery::getApproximateResult() { return approximateResult; }

Line ApproximateQuery::convertToCLine(QLineF& line) {
	QPointF firstEndpoint = line.p1();
	QPointF secondEndpoint = line.p2();

	Point firstEndpointC = Point(firstEndpoint.x(), firstEndpoint.y());
	Point secondEndpointC = Point(secondEndpoint.x(), secondEndpoint.y());

	return Line(firstEndpointC, secondEndpointC);
}

QVector<QPointF> ApproximateQuery::computeDiscIntersection(Circle_2& disc, QLineF& window) {
	Line windowC = convertToCLine(window);
	QVector<QPointF> intersectionsQ;
	std::vector<var> intersections;
	std::vector<Circular_arc_point_2> pairResult;

	// Calculate the intersections
	CGAL::intersection(disc, windowC, std::back_inserter(intersections));

	for (const auto& intersection : intersections)
	{
		// Case 1: Intersection is a pair of Circular_arc_point_2 and multiplicity
		if (const auto* pairResult = std::get_if<std::pair<Circular_arc_point_2, unsigned>>(&intersection)) {
			std::cout << "Circular arc  Points detected.\n";
			const Circular_arc_point_2& arcPoint = pairResult->first;
			double x = CGAL::to_double(arcPoint.x());
			double y = CGAL::to_double(arcPoint.y());

			// Check if the intersection is within the bounding box of the line segment
			QPointF point = QPointF(x, y);
			if (isPointOnLine(window, point)) {
				intersectionsQ.append(point);
			}
			

		}
		// Case 2: Intersection is a Circular_arc_2 (overlap of circular arcs)
		else if (std::holds_alternative<Circular_arc_2>(intersection)) {
			std::cout << "Circular arc overlap detected.\n";
			// Handle overlapping arc as needed (e.g., extract endpoints)
		}
		// Case 3: Intersection is a Line_arc_2 (overlap of line segments)
		else if (std::holds_alternative<Line_arc_2>(intersection)) {
			std::cout << "Line arc overlap detected.\n";
			// Handle overlapping line segment as needed
		}
		// Case 4: Intersection is a full Line_2 or Circle_2 (coincident objects)
		else if (std::holds_alternative<Line>(intersection)) {
			std::cout << "Full line overlap detected.\n";
		}
		else if (std::holds_alternative<Circle_2>(intersection)) {
			std::cout << "Full circle overlap detected.\n";
		}
	}

	std::cout << "circle window intersections: " << intersectionsQ.size() << "\n";

	if (intersectionsQ.isEmpty()) {
		intersectionsQ.append(window.p1());
		intersectionsQ.append(window.p2());
	}
	else if (intersectionsQ.size() == 1) {
		if (calculatePointToPointDistance(getStartingPoint(), window.p1()) <= calculatePointToPointDistance(getStartingPoint(), window.p2())) {
			intersectionsQ.append(window.p1());
		}
		else {
			intersectionsQ.append(window.p2());
		}

	}

	return intersectionsQ;
}

bool ApproximateQuery::isPointOnLine(const QLineF& line, const QPointF& point) {
	// Get the endpoints of the line
	QPointF p1 = line.p1();
	QPointF p2 = line.p2();

	// Check if the cross product is zero (collinearity)
	double crossProduct = (point.y() - p1.y()) * (p2.x() - p1.x()) -
		(point.x() - p1.x()) * (p2.y() - p1.y());

	if (qFuzzyCompare(crossProduct, 0.0)) {
		// Check if the point is within the bounding box of the segment
		double minX = qMin(p1.x(), p2.x());
		double maxX = qMax(p1.x(), p2.x());
		double minY = qMin(p1.y(), p2.y());
		double maxY = qMax(p1.y(), p2.y());

		if (point.x() >= minX && point.x() <= maxX &&
			point.y() >= minY && point.y() <= maxY) {
			return true;
		}
	}

	return false;
}

double ApproximateQuery::calculatePointToPointDistance(const QPointF& a, const QPointF& b) {
	const double x1 = a.x();
	const double y1 = a.y();
	const double x2 = b.x();
	const double y2 = b.y();

	return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2));
}

QVector<QPointF> ApproximateQuery::generateEquallySpacedPoints(QLineF& line, double spacedDistance) {
	QVector<QPointF> points;

	if (spacedDistance <= 0) {
		return points; // Return empty if invalid spacing is provided
	}

	// Start with the starting point of the line
	QPointF startPoint = line.p1();
	QPointF endPoint = line.p2();
	points.append(startPoint);

	double totalLength = line.length();
	double remainingLength = totalLength;

	// Direction vector normalized to unit length
	QPointF direction = (endPoint - startPoint) / totalLength;

	// Generate equally spaced points
	while (remainingLength > spacedDistance) {
		startPoint += direction * spacedDistance;
		points.append(startPoint);
		remainingLength -= spacedDistance;
	}

	// Ensure the last point is included
	if (points.last() != endPoint) {
		points.append(endPoint);
	}

	return points;
}

QVector<QPointF> ApproximateQuery::findShortestPathAmongPairs(
	QVector<QPointF>& spacedPoints1,
	QVector<QPointF>& spacedPoints2,
	QPointF& startingPoint,
	Polygon_2& polygon)
{
	QVector<QPointF> bestPath;  // Store the shortest path
	double minPathLength = std::numeric_limits<double>::max();  // Initialize to infinity
	m_shortestPathHandler.createMesh(polygon);

	// Iterate over all pairs of points (u, v)
	for (QPointF& u : spacedPoints1) {
		for (QPointF& v : spacedPoints2) {
			// Path visiting u first, then v
			QVector<QPointF> path1 = m_shortestPathHandler.findShortestPath(startingPoint, u, polygon);
			QVector<QPointF> pathToV1 = m_shortestPathHandler.findShortestPath(u, v, polygon);
			path1.append(pathToV1);
			double pathLength1 = m_twoPointHandler.calculatePathLength(path1);

			// Path visiting v first, then u
			QVector<QPointF> path2 = m_shortestPathHandler.findShortestPath(startingPoint, v, polygon);
			QVector<QPointF> pathToU2 = m_shortestPathHandler.findShortestPath(v, u, polygon);
			path2.append(pathToU2);
			double pathLength2 = m_twoPointHandler.calculatePathLength(path2);

			// Choose the shorter path
			if (pathLength1 < minPathLength) {
				minPathLength = pathLength1;
				bestPath = path1;
			}
			if (pathLength2 < minPathLength) {
				minPathLength = pathLength2;
				bestPath = path2;
			}
		}
	}

	return bestPath;
}

