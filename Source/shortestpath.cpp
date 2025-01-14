#include "shortestpath.h"

ShortestPath::ShortestPath()
	: pointsSet(false)
{
}

void ShortestPath::createMesh(const Polygon_2& polygon)
{
	mesh.clear();
	cdt.clear();
	cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);
	assert(cdt.is_valid());

	std::unordered_map<CDT::Face_handle, bool> in_domain_map;
	boost::associative_property_map<std::unordered_map<CDT::Face_handle, bool>>
		in_domain(in_domain_map);

	// Mark facets that are inside the domain bounded by the polygon
	CGAL::mark_domain_in_triangulation(cdt, in_domain);

	std::map<CDT::Vertex_handle, Surface_mesh::Vertex_index> vertexMap;

	// Add vertices to the Surface_mesh
	for (CDT::Finite_vertices_iterator vit = cdt.finite_vertices_begin(); vit != cdt.finite_vertices_end(); ++vit)
	{
		Point_3 p3(vit->point().x(), vit->point().y(), 0);
		vertexMap[vit] = mesh.add_vertex(p3);
	}

	// Add faces (triangles) to the Surface_mesh only if they are inside the polygon
	for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit)
	{
		// Skip infinite faces and faces outside the domain
		if (cdt.is_infinite(fit) || !in_domain[fit])
			continue;

		// Get vertices of the triangle
		Surface_mesh::Vertex_index v0 = vertexMap[fit->vertex(0)];
		Surface_mesh::Vertex_index v1 = vertexMap[fit->vertex(1)];
		Surface_mesh::Vertex_index v2 = vertexMap[fit->vertex(2)];

		// Add the triangular face to the mesh
		mesh.add_face(v0, v1, v2);
	}
}

const Surface_mesh& ShortestPath::getMesh() const
{
	return mesh;
}

void ShortestPath::clearTree() {
	tree.clear();
}

void ShortestPath::clearMesh() {
	mesh.clear();
}

bool ShortestPath::is_point_on_polygon_edge(const Polygon_2& polygon, const Point_2& point) {
	for (auto edge = polygon.edges_begin(); edge != polygon.edges_end(); ++edge) {
		if (CGAL::collinear_are_ordered_along_line(edge->source(), point, edge->target())) {
			return true;
		}
	}
	return false;
}

QVector<QPointF> ShortestPath::findShortestPath(QPointF source2D, QPointF query2D, const Polygon_2& polygon)
{
	points.clear();
	shortestPath.clear();

	if (mesh.is_empty()) {
		createMesh(polygon);
	}

	Point_2 query2 = snapQueryInsidePolygon(query2D, polygon);

	// construct a shortest path query object and add a source point
	Surface_mesh_shortest_path shortest_paths(mesh);

	// Convert source and query points to 3D points
	const Point_3 source = Point_3(source2D.x(), source2D.y(), 0);
	const Point_3 query = Point_3(query2.x(), query2.y(), 0);
	// Build the AABB tree for the mesh
	Face_location source_loc = shortest_paths.locate<AABB_face_graph_traits>(source);

	// Add the source point
	shortest_paths.add_source_point(source_loc.first, source_loc.second);

	Face_location query_loc = shortest_paths.locate<AABB_face_graph_traits>(query);

	// Collect shortest path points
	shortest_paths.shortest_path_points_to_source_points(query_loc.first, query_loc.second, std::back_inserter(points));

	std::set<Point_2> polygonVertices;
	for (auto vertex = polygon.vertices_begin(); vertex != polygon.vertices_end(); ++vertex) {
		polygonVertices.insert(*vertex);
	}

	for (int i = 0; i < points.size(); i++) {
		Point_2 currentPoint(points[i].x(), points[i].y());
		if (polygonVertices.count(currentPoint) > 0 || i == 0 || i == points.size() - 1) {
			shortestPath.push_back(points[i]);
		}
	}

	QVector<QPointF> shortestPathQt = point3VectorToQtVector(shortestPath);
	return reversePath(shortestPathQt);
}

double ShortestPath::findShortestPathLength(QPointF source2D, QPointF query2D, const Polygon_2& polygon)
{
	if (mesh.is_empty()) {
		createMesh(polygon);
	}

	Point_2 query2 = snapQueryInsidePolygon(query2D, polygon);

	// Create shortest path query object and add a source point
	Surface_mesh_shortest_path shortest_paths(mesh);

	// Convert source and query points to 3D points
	const Point_3 source = Point_3(source2D.x(), source2D.y(), 0);
	const Point_3 query = Point_3(query2.x(), query2.y(), 0);

	// Locate the source on the mesh
	Face_location source_loc = shortest_paths.locate<AABB_face_graph_traits>(source);
	shortest_paths.add_source_point(source_loc.first, source_loc.second);

	// Locate the query on the mesh
	Face_location query_loc = shortest_paths.locate<AABB_face_graph_traits>(query);

	// Return the shortest distance directly
	return shortest_paths.shortest_distance_to_source_points(query_loc.first, query_loc.second).first;
}

Point_2 ShortestPath::snapQueryInsidePolygon(QPointF& queryPoint, const Polygon_2& polygon) {
	Point_2 query = Point_2(queryPoint.x(), queryPoint.y());

	const float epsilon = 0.000001;
	// TODO: Add more finer directions (i.e. diagonals)
	//if (is_point_on_polygon_edge(polygon, query)) {
	if (polygon.has_on_bounded_side(Point_2(query.x() + epsilon, query.y()))) {
		query = Point_2(query.x() + epsilon, query.y());
	}
	else if (polygon.has_on_bounded_side(Point_2(query.x() - epsilon, query.y()))) {
		query = Point_2(query.x() - epsilon, query.y());
	}
	else if (polygon.has_on_bounded_side(Point_2(query.x(), query.y() + epsilon))) {
		query = Point_2(query.x(), query.y() + epsilon);
	}
	else if (polygon.has_on_bounded_side(Point_2(query.x(), query.y() - epsilon))) {
		query = Point_2(query.x(), query.y() - epsilon);
	}
	//}

	return query;
}

QVector<QPointF> ShortestPath::point3VectorToQtVector(std::vector<Point_3>& points)
{
	QVector<QPointF> qtPoints;
	qtPoints.reserve(points.size());

	for (const auto& point : points)
	{
		qtPoints.append(QPointF(point.x(), point.y()));
	}

	return qtPoints;
}

const Point_2& ShortestPath::getPenultimate(const std::vector<Point_3>& path, const Polygon_2& polygon) const
{
	// Iterate over the points vector, starting from the second element (index 1)
	for (int i = path.size() - 1; i >= 0; i--)
	{
		// Convert each point to a Point_2
		Point_2 penultimate(path[i].x(), path[i].y());

		// Check if this point lies on the boundary of the polygon
		if (polygon.has_on_boundary(penultimate))
		{
			return penultimate; // Return the first point found on the boundary
		}
	}

	// Handle case where no point on the boundary was found
	//throw std::runtime_error("No point on the polygon boundary was found in the path.");
	Point_3 lastPoint_3 = path.back();
	return Point_2(lastPoint_3.x(), lastPoint_3.y());
}

QVector<QPointF> ShortestPath::reversePath(QVector<QPointF>& path)
{
	std::reverse(path.begin(), path.end());
	return path;
}

QPointF ShortestPath::getLCA(QPointF& start, QPointF& a, QPointF& b, Polygon_2& polygon)
{
	QVector<QPointF> path1 = findShortestPath(start, a, polygon);
	QVector<QPointF> path2 = findShortestPath(start, b, polygon);

	QPointF lca;

	// Iterate over both paths until they diverge
	size_t minLength = std::min(path1.size(), path2.size());
	for (size_t i = 0; i < minLength; ++i)
	{
		if (path1[i] == path2[i])
		{
			lca = path1[i]; // Update LCA to the current common point
		}
		else
		{
			break; // Paths diverge, stop the search
		}
	}

	return lca;
}
