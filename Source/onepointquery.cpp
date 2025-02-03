#include "onepointquery.h"

OnePointQuery::OnePointQuery()
	: m_startSelected(false), m_querySelected(false)
{
	logQ1 = "";
}

Point_2 OnePointQuery::convertToCGALPoint(const QPointF& qtPoint)
{
	return Point_2(qtPoint.x(), qtPoint.y());
}

QPointF OnePointQuery::convertToQTPoint(const Point_2& cgalPoint)
{
	return QPointF(cgalPoint.x(), cgalPoint.y());
}

// TDOD: dont recalculate tree and edges if not needed (if tree.isempty() does not work)
bool OnePointQuery::checkVisibilty(const QPointF& point1, const QPointF& point2, Polygon_2& polygon)
{
	if (point1 == point2) {
		return true;
	}

	//std::cout << "source of vis: " << point1.x() << ", " << point1.y() << "\n";
	//std::cout << "end of vis: " << point2.x() << ", " << point2.y() << "\n";
	// Convert points and create segment query
	Point_2 a = convertToCGALPoint(point1);
	Point_2 b = convertToCGALPoint(point2);
	Segment_2 segment_query(a, b);

	// Build AABB tree if not already initialized
	//if (tree.empty())
	//{
	tree.clear();
	edges.clear();
	for (auto edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge)
	{
		edges.push_back(*edge);
	}
	tree.rebuild(edges.begin(), edges.end());
	//}
	tree.accelerate_distance_queries();

	// Check for intersections
	std::list<Segment_intersection> intersections;
	tree.all_intersections(segment_query, std::back_inserter(intersections));

	for (const auto& intersection : intersections)
	{
		if (!intersection) {
			// Skip invalid intersection objects
			continue;
		}

		const Point_2* p = std::get_if<Point_2>(&(intersection->first));
		if (!p) {
			// If std::get_if failed, it means `intersection->first` is not a Point_2.
			continue;
		}

		if (areEqual(a, *p) || areEqual(b, *p)) {
			// Intersection is at one of the segment endpoints; skip it.
			continue;
		}

		//std::cout << "obstacle of vis: " << p->x() << ", " << p->y() << "\n";
		return false; // Non-endpoint intersection found; visibility check fails.
	}

	Point_2 midpoint = Point_2((a.x() + b.x()) / 2.0, (a.y() + b.y()) / 2.0);
	if (polygon.has_on_unbounded_side(midpoint)) {
		return false;
	}

	return true;
}

bool OnePointQuery::areEqual(Point_2 a, Point_2 b) {
	const double EPSILON = 1e-9; // A small tolerance value
	bool equalX = std::abs(a.x() - b.x()) < EPSILON;
	bool equalY = std::abs(a.y() - b.y()) < EPSILON;

	return (equalX && equalY);
}

void OnePointQuery::clearTree() {
	tree.clear();
	edges.clear();
}

// Skip as an unfinished alternative
struct Skip
{
	Point_2 target;

	Skip(const Point_2 target)
		: target(target)
	{
	}

	bool operator()(const Point_2& t) const
	{
		if (t == target)
		{
			std::cerr << "ignore " << t << std::endl;
		};
		return (t == target);
	}
};

// TDOD: dont recalculate tree and edges if not needed (if tree.isempty() does not work)
QPointF OnePointQuery::shootRayExtended(const QPointF& point1, const QPointF& point2, Polygon_2& polygon)
{
	QPointF intersectionPoint;
	// Convert points to CGAL types
	Point_2 source = convertToCGALPoint(point1);
	Point_2 target = convertToCGALPoint(point2);

	// Define the ray from `source` through `target`
	Ray ray_query(source, target);

	// Extract edges of the polygon into a segment vector
	tree.clear();
	edges.clear();
	//if (tree.empty())
	//{
	for (Polygon_2::Edge_const_iterator edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge)
	{
		edges.push_back(*edge);
	}

	// Create AABB tree
	tree.rebuild(edges.begin(), edges.end());
	//}
	tree.accelerate_distance_queries();

	// Collect all intersections along the ray
	std::vector<Ray_intersection> intersections;
	tree.all_intersections(ray_query, std::back_inserter(intersections));

	// Initialize the closest intersection point
	std::optional<Point_2> closest_intersection;
	K::FT closest_distance = std::numeric_limits<double>::max();

	// Iterate through intersections to find the first valid one that isn't equal to target
	for (const auto& intersection : intersections)
	{
		// Check if intersection is a Point_2
		if (const Point_2* p = std::get_if<Point_2>(&(intersection->first)))
		{
			if (*p == target)
			{
				// Skip the intersection if it matches the target point
				continue;
			}

			// Compute the distance from the source to this intersection point
			K::FT distance = CGAL::squared_distance(source, *p);

			// Check if this intersection is the closest one found so far
			if (distance < closest_distance)
			{
				closest_distance = distance;
				closest_intersection = *p;
			}
		}
	}

	Point_2 closestPoint;
	// Set `b` to the closest intersection point found
	if (closest_intersection)
	{
		closestPoint = *closest_intersection;
		intersectionPoint = snapPointInPolygon(closestPoint, source, polygon);
	}

	return intersectionPoint;
}

QPointF OnePointQuery::snapPointInPolygon(const Point_2& query, const Point_2& source, const Polygon_2& polygon) {
	const float epsilon = 0.000001;

	// Compute the direction vector from source to target
	K::FT dx = query.x() - source.x();
	K::FT dy = query.y() - source.y();

	// Compute the length of the direction vector
	K::FT length = std::sqrt(dx * dx + dy * dy);

	// Normalize the direction vector
	K::FT dirX = dx / length;
	K::FT dirY = dy / length;

	// Adjust the query point slightly along the direction of the ray
	Point_2 adjustedPoint(query.x() - epsilon * dirX, query.y() - epsilon * dirY);

	// Check if the adjusted point is inside the polygon
	if (polygon.has_on_bounded_side(adjustedPoint)) {
		return QPointF(adjustedPoint.x(), adjustedPoint.y());
	}

	// If the adjusted point is not inside, return the original query point
	return QPointF(query.x(), query.y());
}

QPointF OnePointQuery::unsnapPointInPolygon(const QPointF& adjustedPoint, const QPointF& source) {
	const float epsilon = 0.000001;

	// Compute the direction vector from source to adjustedPoint
	double dx = adjustedPoint.x() - source.x();
	double dy = adjustedPoint.y() - source.y();

	// Compute the length of the direction vector
	double length = std::sqrt(dx * dx + dy * dy);

	// Avoid division by zero
	if (length < epsilon) {
		return adjustedPoint; // No significant movement
	}

	// Normalize the direction vector
	double dirX = dx / length;
	double dirY = dy / length;

	// Move the adjusted point back along the direction vector
	QPointF originalPoint(adjustedPoint.x() + epsilon * dirX, adjustedPoint.y() + epsilon * dirY);

	return originalPoint;
}

// Helper function to calculate angle between two vectors in radians
double OnePointQuery::calculateAngle(const K::Vector_2& v1, const K::Vector_2& v2)
{
	double dot_product = v1 * v2;
	double magnitude1 = std::sqrt(v1.squared_length());
	double magnitude2 = std::sqrt(v2.squared_length());
	double radian = std::acos(dot_product / (magnitude1 * magnitude2));
	double degree = (radian * (180 / CGAL_PI));
	return degree;
}

// Main function to calculate angles in funnel
double OnePointQuery::calculateFunnelAngle(const QPointF& point1, const QPointF& point2, const QPointF& a, const QPointF& b)
{
	Point_2 source = convertToCGALPoint(point1);
	Point_2 target = convertToCGALPoint(point2);
	Point_2 endpoint1 = convertToCGALPoint(a);
	Point_2 endpoint2 = convertToCGALPoint(b);

	K::Vector_2 w_vector(endpoint1, endpoint2); // window vector
	K::Vector_2 edge_vector(source, target);
	K::Vector_2 extended_vector = edge_vector.direction().to_vector(); // Extend this vector

	// Calculate angle with window
	double angle = calculateAngle(extended_vector, w_vector);

	Orientation orientation = CGAL::orientation(endpoint1, endpoint2, source);

	angle = 180 - angle;

	return angle;
}

QPointF OnePointQuery::calculateWindowIntersection(const QPointF& pathPoint, const QPointF& windowStart, const QPointF& windowEnd) {
	// Convert QPointF to CGAL Point_2 for calculations
	Point_2 p = convertToCGALPoint(pathPoint);
	Point_2 w1 = convertToCGALPoint(windowStart);
	Point_2 w2 = convertToCGALPoint(windowEnd);
	QPointF intersection;

	// Create the window line using the two points
	Line_2 windowLine(w1, w2);

	// Calculate the perpendicular line passing through `pathPoint`
	Line_2 perpendicularLine = windowLine.perpendicular(p);

	// Find the intersection point of the perpendicular line with the window line
	auto intersectionResult = CGAL::intersection(windowLine, perpendicularLine);

	if (intersectionResult) {
		if (const Point_2* intersectionPoint = std::get_if<Point_2>(&*intersectionResult))
		{
			// If the intersection is a point, convert it to QPointF and return
			intersection = convertToQTPoint(*intersectionPoint);
		}
	}

	return intersection; // Empty QPointF indicates no intersection
}

QPointF OnePointQuery::computeOptimalPoint(QVector<QPointF>& pathRA, QVector<QPointF>& pathRB, QPointF& lca, QLineF& window)
{
	onPathRootToA = false;
	QPointF a = window.p1();
	QPointF b = window.p2();
	const double angle0 = calculateFunnelAngle(pathRA.rbegin()[1], pathRA.rbegin()[0], a, b); // theta_0
	if (angle0 > 90)
	{
		logQ1 = "c = a";
		std::cout << "c = a \n";
		onPathRootToA = true;
		vertexPerpendicularToC = a;
		return a;
	}

	const double anglek = calculateFunnelAngle(pathRB.rbegin()[1], pathRB.rbegin()[0], a, b); // theta_k
	if (anglek < 90)
	{
		logQ1 = "c = b";
		std::cout << "c = b \n";
		onPathRootToA = false;
		vertexPerpendicularToC = b;
		return b;
	}
	//
	//
	const double anglem1 = calculateFunnelAngle(pathRA.begin()[0], pathRA.begin()[1], a, b); // theta_m-1
	const double anglem = calculateFunnelAngle(pathRB.begin()[0], pathRB.begin()[1], a, b);  // theta_m
	if (anglem1 == 90 || anglem == 90 || (anglem1 <= 90 && 90 < anglem))
	{
		logQ1 = "c is at the foot of the perpendicular from root";
		std::cout << "foot of root \n";
		vertexPerpendicularToC = lca;
		return calculateWindowIntersection(lca, a, b);
	}

	if (anglem1 > 90)
	{
		int vertex = binarySearchByAngleASide(pathRA, a, b);
		int v = (pathRA.size() - 1) - vertex; // root would be lowest, but needs to be highest

		//logQ1 = "c is at the foot of the perpendicular from v" + v;
		std::cout << "c is foot from v" << v << "\n";
		onPathRootToA = true;
		vertexPerpendicularToC = pathRA[vertex];
		return calculateWindowIntersection(pathRA[vertex], a, b);
	}
	else
	{
		int vertex = binarySearchByAngleBSide(pathRB, a, b);
		int v = vertex + (pathRA.size() - 1);
		logQ1 = "c is at the foot of the perpendicular from v" + v;
		std::cout << "c is foot from v" << v << "\n";
		onPathRootToA = false;
		vertexPerpendicularToC = pathRB[vertex];
		return calculateWindowIntersection(pathRB[vertex], a, b);
	}
}

bool OnePointQuery::getOnPathRootToA() {
	return onPathRootToA;
}

QPointF OnePointQuery::getVertexPerpendicularToC() {
	return vertexPerpendicularToC;
}

int OnePointQuery::binarySearchByAngleASide(QVector<QPointF>& path, QPointF& a, QPointF& b)
{
	int left = 0;
	int right = path.size() - 1;
	int mid = 0;
	double theta_mid = 0;
	// right - left > 1

	while (right - left > 1)
	{
		mid = (right + left + 1) / 2;
		double theta_mid = calculateFunnelAngle(path[mid - 1], path[mid], a, b);

		if (theta_mid > 90)
		{
			left = mid;
		}
		else if (theta_mid < 90)
		{
			right = mid;
		}
		else {
			return mid;
		}
	}
	return left;
}

int OnePointQuery::binarySearchByAngleBSide(QVector<QPointF>& path, QPointF& a, QPointF& b)
{
	int left = 0;
	int right = path.size() - 1;
	int mid = 0;
	double theta_mid = 0;
	// right - left > 1

	while (right - left > 1)
	{
		mid = (right + left + 1) / 2;
		double theta_mid = calculateFunnelAngle(path[mid - 1], path[mid], a, b);

		if (theta_mid > 90)
		{
			right = mid;
		}
		else if (theta_mid < 90)
		{
			left = mid;
		}
		else {
			return mid;
		}
	}
	return left;
}

QString OnePointQuery::getLog()
{
	return logQ1;
}

void OnePointQuery::resetLog()
{
	logQ1 = "";
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnePointQuery::executeOnePointQuery(QPointF& startingPoint, QPointF& queryPoint, Polygon_2& polygon, const Surface_mesh& mesh)
{
	// FOR TESTING
	m_shortestPathHandler.createMesh(polygon);


	// visibility check
	bool visibility = checkVisibilty(startingPoint, queryPoint, polygon);
	result.visibility = visibility;

	if (visibility)
	{
		return;
	}

	// Find shortest path from s to q
	QVector<QPointF> pathStartToQuery = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint, polygon, mesh);
	result.pathStartToQuery = pathStartToQuery;

	// calculate the window
	QLineF window = calculateWindow(pathStartToQuery, queryPoint, polygon);
	result.window = window;
	QPointF a = window.p1();
	QPointF b = window.p2();

	// calculate the LCA / root of the funnel
	QVector<QPointF> pathStartToA = m_shortestPathHandler.findShortestPath(startingPoint, a, polygon, mesh);
	QVector<QPointF> pathStartToB = m_shortestPathHandler.findShortestPath(startingPoint, b, polygon, mesh);
	QPointF lca = m_shortestPathHandler.getLCA(pathStartToA, pathStartToB);
	result.lca = lca;

	// construct the funnel through its sides
	QVector<QPointF> pathRootToA = m_shortestPathHandler.findShortestPath(lca, a, polygon, mesh);
	result.pathRootToA = pathRootToA;
	QVector<QPointF> pathRootToB = m_shortestPathHandler.findShortestPath(lca, b, polygon, mesh);
	result.pathRootToB = pathRootToB;

	QPointF c = computeOptimalPoint(pathRootToA, pathRootToB, lca, window);
	result.optimalPoint = c;

	QVector<QPointF> optimalPath;
	if (onPathRootToA) {
		optimalPath = computeOptimalPath(pathStartToQuery, pathRootToA, lca, c);
	}
	else {
		optimalPath = computeOptimalPath(pathStartToQuery, pathRootToB, lca, c);
	}
	result.optimalPath = optimalPath;
}

const OnePointQuery::QueryResult OnePointQuery::getResult() const { return result; }

QLineF OnePointQuery::calculateWindow(QVector<QPointF>& path, QPointF& queryPoint, Polygon_2& polygon)
{
	//const Point_2 aC = m_shortestPathHandler.getPenultimate(path, polygon);
	// a is the first point before q on the shortest path sq
	const QPointF a = path.rbegin()[1];
	const QPointF b = shootRayExtended(queryPoint, a, polygon);

	return QLineF(a, b);
}

QVector<QPointF> OnePointQuery::computeOptimalPath(QVector<QPointF>& pathStartToQuery, QVector<QPointF>& pathRootToWindowEndpoint, QPointF& lca, QPointF& c) {
	bool start = false;
	QVector<QPointF> optimalPath;

	optimalPath.append(c);

	for (int i = pathRootToWindowEndpoint.size() - 1; i >= 0; --i) {
		if (pathRootToWindowEndpoint[i] == vertexPerpendicularToC) {
			start = true;
		}

		if (start) {
			optimalPath.append(pathRootToWindowEndpoint[i]);
		}
	}

	start = false;
	for (int i = pathStartToQuery.size() - 1; i >= 0; --i) {
		if (pathStartToQuery[i] == lca) {
			start = true;
		}

		if (start) {

			optimalPath.append(pathStartToQuery[i]);
		}
	}

	return m_shortestPathHandler.reversePath(optimalPath);
}
