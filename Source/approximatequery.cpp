#include "approximatequery.h"

ApproximateQuery::ApproximateQuery()
{
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

QVector<QPointF> ApproximateQuery::nApproximateQuery(QPointF& startingPoint, QVector<QPointF>& queryPoints, Polygon_2& polygon) {
	windowsVector.clear();
	QVector<QPointF> nApproximatePath;

	if (queryPoints.size() < 1) {
		return nApproximatePath;
	}

	OnePointQuery::QueryResult tempQueryResult;

	for (QPointF point : queryPoints) {
		m_onePointHandler.executeOnePointQuery(startingPoint, point, polygon);
		tempQueryResult = m_onePointHandler.getResult();
		windowsVector.append(tempQueryResult.window);
		nApproximatePath.append(tempQueryResult.optimalPath);
		if (point != queryPoints.rbegin()[0]) {
			nApproximatePath.append(m_shortestPathHandler.reversePath(tempQueryResult.optimalPath));
		}
	}

	return nApproximatePath;
}

void ApproximateQuery::nEpsilonApproximateQuery(double epsilon, QPointF& startingPoint, QVector<QPointF>& queryPoints, Polygon_2& polygon) {
	if (queryPoints.size() < 1) {
		return;
	}

	QElapsedTimer timer;
	timer.start();

	// 1.
	QVector<QPointF> nApproximatePath = nApproximateQuery(startingPoint, queryPoints, polygon);
	nApproximateResult.nApproxPath = nApproximatePath;
	double nApproximatePathLength = m_twoPointHandler.calculatePathLength(nApproximatePath);
	const int sizeOfN = queryPoints.size() * 2 - 1;
	std::cout << "size n: " << sizeOfN << "\n";

	// 2.
	nApproximateResult.windows = windowsVector;

	// 3.
	Point startingPointC = Point(startingPoint.x(), startingPoint.y());
	double radius = 2 * nApproximatePathLength;
	nApproximateResult.discRadius = radius;
	double squaredRadius = radius * radius;
	Circle_2 disc = Circle_2(startingPointC, squaredRadius, CGAL::COUNTERCLOCKWISE);
	QVector<QLineF> intersectionWindowsVector;
	QVector<QPointF> discWindowIntersections;
	for (QLineF window : windowsVector) {
		discWindowIntersections = computeDiscIntersection(startingPoint, disc, window);
		if (discWindowIntersections.size() == 2) {
			intersectionWindowsVector.append(QLineF(discWindowIntersections[0], discWindowIntersections[1]));
		}
	}
	nApproximateResult.intersectionWindows = intersectionWindowsVector;

	// 4.
	double const delta = epsilon * nApproximatePathLength / sizeOfN;
	double const spacedDistance = delta / sizeOfN;
	QVector<QVector<QPointF>> spacedPointsVectorGroup;
	for (QLineF intersectionWindow : intersectionWindowsVector) {
		spacedPointsVectorGroup.append(generateEquallySpacedPoints(intersectionWindow, spacedDistance));

	}

	nApproximateResult.equallySpacedPointsGroup = spacedPointsVectorGroup;

	// 5.
	QVector<QPointF> shortestPath = findShortestPathAmongVectorOfVectors(spacedPointsVectorGroup, startingPoint, polygon);
	nApproximateResult.shortestPath = shortestPath;

	qint64 elapsedTime = timer.elapsed();
	std::cout << "epsilonApproximateQuery completed in" << elapsedTime << " ms \n";
}

ApproximateQuery::NApproximateResult ApproximateQuery::getNApproximateResult() { return nApproximateResult; }

void ApproximateQuery::epsilonApproximateQuery(double epsilon, QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2, Polygon_2& polygon) {
	QElapsedTimer timer;
	timer.start();

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
	QVector<QPointF> intersectionsVector1 = computeDiscIntersection(startingPoint, disc, window1);
	QVector<QPointF> intersectionsVector2 = computeDiscIntersection(startingPoint, disc, window2);
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

	qint64 elapsedTime = timer.elapsed();
	std::cout << "epsilonApproximateQuery completed in" << elapsedTime << " ms \n";
}

ApproximateQuery::ApproximateResult ApproximateQuery::getApproximateResult() { return approximateResult; }

Line ApproximateQuery::convertToCLine(QLineF& line) {
	QPointF firstEndpoint = line.p1();
	QPointF secondEndpoint = line.p2();

	Point firstEndpointC = Point(firstEndpoint.x(), firstEndpoint.y());
	Point secondEndpointC = Point(secondEndpoint.x(), secondEndpoint.y());

	return Line(firstEndpointC, secondEndpointC);
}

QVector<QPointF> ApproximateQuery::computeDiscIntersection(QPointF& circleCenter, Circle_2& disc, QLineF& window) {
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
		if (calculatePointToPointDistance(circleCenter, window.p1()) <= calculatePointToPointDistance(circleCenter, window.p2())) {
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

QVector<QPointF> ApproximateQuery::findShortestPathAmongVectorOfVectors(
	QVector<QVector<QPointF>>& spacedPointsGroups,
	QPointF& startingPoint,
	Polygon_2& polygon)
{
	auto computeShortestPathLength = [&](const QPointF& p1, const QPointF& p2) -> double {
		return m_shortestPathHandler.findShortestPathLength(p1, p2, polygon);
		};

	int numGroups = spacedPointsGroups.size();
	QVector<int> bestWindowOrder;
	double minTotalDistance = std::numeric_limits<double>::max();

	QVector<int> indices(numGroups, 0);  // To track indices of selected points in each group.
	bool done = false;

	while (!done) {
		QVector<QPointF> currentPath;
		QVector<int> currentWindowOrder;  // To track the current window order.
		currentPath.append(startingPoint);

		// Append selected points and their window indices.
		for (int i = 0; i < numGroups; ++i) {
			currentPath.append(spacedPointsGroups[i][indices[i]]);
			currentWindowOrder.append(i);  // Record the window index.
		}

		// Compute the total path length for the current combination.
		double totalDistance = 0.0;
		for (int i = 0; i < currentPath.size() - 1; ++i) {
			totalDistance += computeShortestPathLength(currentPath[i], currentPath[i + 1]);
		}

		// Update the best path and window order if a shorter one is found.
		if (totalDistance < minTotalDistance) {
			minTotalDistance = totalDistance;
			bestWindowOrder = currentWindowOrder;
		}

		// Increment indices to generate the next combination.
		for (int i = 0; i < numGroups; ++i) {
			indices[i]++;
			if (indices[i] < spacedPointsGroups[i].size()) {
				break;
			}
			indices[i] = 0;
			if (i == numGroups - 1) {
				done = true;
			}
		}
	}

	// Compute the actual shortest path using the best window order.
	QVector<QPointF> bestPath;
	bestPath.append(startingPoint);

	QPointF currentPoint = startingPoint;
	for (int windowIndex : bestWindowOrder) {
		std::cout << windowIndex << ", ";
		// Find the closest point from the current window.
		QPointF nextPoint = spacedPointsGroups[windowIndex][indices[windowIndex]];

		// Compute the actual shortest path between currentPoint and nextPoint.
		QVector<QPointF> segmentPath = m_shortestPathHandler.findShortestPath(currentPoint, nextPoint, polygon);

		// Append the segment path to the best path, excluding the first point to avoid duplication.
		if (!bestPath.isEmpty()) {
			segmentPath.pop_front();
		}
		bestPath.append(segmentPath);

		// Update currentPoint for the next iteration.
		currentPoint = nextPoint;
	}
	std::cout << "\n";

	return bestPath;
}

/*
QVector<QPointF> ApproximateQuery::findShortestPathAmongVectorOfVectors(
	QVector<QVector<QPointF>>& spacedPointsGroups,
	QPointF& startingPoint,
	Polygon_2& polygon)
{
	QVector<QPointF> bestPath;  // Store the shortest path
	std::vector<int> visitedIndices;

	double minPathLength = std::numeric_limits<double>::max();  // Initialize to infinity

	// Iterate over all unique pairs of QVector<QPointF>
	int numGroups = spacedPointsGroups.size();
	for (int i = 0; i < numGroups; ++i) {
		for (int j = i + 1; j < numGroups; ++j) {
			QVector<QPointF>& spacedPoints1 = spacedPointsGroups[i];
			QVector<QPointF>& spacedPoints2 = spacedPointsGroups[j];

			// Iterate over all pairs of points (u, v) from the two groups
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
		}
	}

	return bestPath;
}
*/

