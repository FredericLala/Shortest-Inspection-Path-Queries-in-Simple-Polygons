#include "onepointquery.h"

OnePointQuery::OnePointQuery()
	: m_startSelected(false), m_querySelected(false)
{
}

void OnePointQuery::setStartingPoint(const QPointF& point)
{
	m_startingPoint = point;
	m_startSelected = true;
}

void OnePointQuery::setQueryPoint(const QPointF& point)
{
	m_queryPoint = point;
	m_querySelected = true;
}

void OnePointQuery::clearPoints()
{
	m_startSelected = false;
	m_querySelected = false;
}

QPointF OnePointQuery::getStartingPoint() const
{
	return m_startingPoint;
}

QPointF OnePointQuery::getQueryPoint() const
{
	return m_queryPoint;
}

bool OnePointQuery::isStartingPointSet() const
{
	return m_startSelected;
}

bool OnePointQuery::isQueryPointSet() const
{
	return m_querySelected;
}

Point_2 OnePointQuery::convertToCGALPoint(const QPointF& qtPoint)
{
	return Point_2(qtPoint.x(), qtPoint.y());
}

QPointF OnePointQuery::convertToQTPoint(const Point_2& cgalPoint)
{
	return QPointF(cgalPoint.x(), cgalPoint.y());
}

bool OnePointQuery::checkVisibilty(const QPointF& point1, const QPointF& point2, Polygon_2& polygon)
{
	// Convert points and create segment query
	Point_2 a = convertToCGALPoint(point1);
	Point_2 b = convertToCGALPoint(point2);
	Segment_2 segment_query(a, b);

	// Build AABB tree if not already initialized
	if (tree.empty())
	{
		for (auto edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge)
		{
			edges.push_back(*edge);
		}
		tree.rebuild(edges.begin(), edges.end());
	}
	tree.accelerate_distance_queries();

	// Return visibility
	return !tree.do_intersect(segment_query);
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

void OnePointQuery::shootRayExtended(const QPointF& point1, const QPointF& point2, Polygon_2& polygon)
{
	// Convert points to CGAL types
	Point_2 source = convertToCGALPoint(point1);
	Point_2 target = convertToCGALPoint(point2);

	// Define the ray from `source` through `target`
	Ray ray_query(source, target);

	// Extract edges of the polygon into a segment vector
	if (tree.empty())
	{
		for (Polygon_2::Edge_const_iterator edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge)
		{
			edges.push_back(*edge);
		}

		// Create AABB tree
		tree.rebuild(edges.begin(), edges.end());
	}
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

	// Set `b` to the closest intersection point found
	if (closest_intersection)
	{
		b = *closest_intersection;
	}
}

Point_2 OnePointQuery::getIntersection()
{
	return b;
}

/* AABB_tree OnePointQuery::constructTree(Polygon_2 &polygon)
{
	// Extract edges of the polygon into a segment vector
	std::vector<Segment_2> edges;
	for (Polygon_2::Edge_const_iterator edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge)
	{
		edges.push_back(*edge);
	}

	// Create AABB tree
	AABB_tree tree(edges.begin(), edges.end());
	return tree;
} */

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

	/*     if (orientation == CGAL::LEFT_TURN) {
			angle = 180 - angle;
		} */

	return angle;
}

QPointF OnePointQuery::calculateWindowIntersection(const QPointF& pathPoint, const QPointF& windowStart, const QPointF& windowEnd) {
	// Convert QPointF to CGAL Point_2 for calculations
	Point_2 p = convertToCGALPoint(pathPoint);
	Point_2 w1 = convertToCGALPoint(windowStart);
	Point_2 w2 = convertToCGALPoint(windowEnd);

	// Create the window line using the two points
	Line_2 windowLine(w1, w2);

	// Calculate the perpendicular line passing through `pathPoint`
	Line_2 perpendicularLine = windowLine.perpendicular(p);

	// Find the intersection point of the perpendicular line with the window line
	auto intersectionResult = CGAL::intersection(windowLine, perpendicularLine);

	//if (const Point_2* p = std::get_if<Point_2>(&(intersection->first)))

	if (intersectionResult) {
		if (const Point_2* intersectionPoint = std::get_if<Point_2>(&*intersectionResult))
		{
			// If the intersection is a point, convert it to QPointF and return
			return convertToQTPoint(*intersectionPoint);
		}
	}
	else
	{
		// No intersection found, return an invalid point
		return QPointF(); // Empty QPointF indicates no intersection
	}
}
