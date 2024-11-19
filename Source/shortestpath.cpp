#include "shortestpath.h"

ShortestPath::ShortestPath()
    : pointsSet(false)
{
}

void ShortestPath::createMesh(const Polygon_2 &polygon)
{
    mesh.clear();
    cdt.clear();
    cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);

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

const Surface_mesh &ShortestPath::getMesh() const
{
    return mesh;
}

Point_3 convertToCGALPoint3D(const QPointF &qtPoint)
{
    return Point_3(qtPoint.x(), qtPoint.y(), 0);
}

std::vector<Point_3> ShortestPath::findShortestPath(QPointF source_2D, QPointF query_2D, const Polygon_2 &polygon)
{
    points.clear();
    shortestPath.clear();
    Surface_mesh_shortest_path shortest_path(mesh);

    // Convert to 3D points
    Point_3 source = convertToCGALPoint3D(source_2D);
    Point_3 query = convertToCGALPoint3D(query_2D);

    Face_location source_loc = shortest_path.locate<AABB_face_graph_traits>(source);
    // Add source point
    shortest_path.add_source_point(source_loc.first, source_loc.second);

    if (tree.empty()) {
        shortest_path.build_aabb_tree(tree);
    }
    
    Face_location query_loc = shortest_path.locate<AABB_face_graph_traits>(query);

    // Collect shortest path points
    shortest_path.shortest_path_points_to_source_points(query_loc.first, query_loc.second, std::back_inserter(points));

    for (int i = 0; i < points.size(); i++)
    {
        if (polygon.has_on_boundary(Point_2(points[i].x(), points[i].y())) || i == 0 || i == points.size() - 1)
        {
            shortestPath.push_back(points[i]);
        }
    }

    shortestPath = reversePath(shortestPath); // Path is now from source to query
    return shortestPath;
}

const Point_2 &ShortestPath::getPenultimate(const std::vector<Point_3> &path, const Polygon_2 &polygon) const
{
    // Iterate over the points vector, starting from the second element (index 1)
    for (int i = path.size() - 1; i >= 0 ; i--)
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
    throw std::runtime_error("No point on the polygon boundary was found in the path.");
}

std::vector<Point_3> ShortestPath::reversePath(std::vector<Point_3> path)
{
    std::reverse(path.begin(), path.end());
    return path;
}

Point_2 ShortestPath::getLCA(QPointF start, QPointF a, QPointF b, const Polygon_2 &polygon)
{
    std::vector<Point_3> path1 = findShortestPath(start, a, polygon);
    std::vector<Point_3> path2 = findShortestPath(start, b, polygon);

    // Initialize a variable to store the Last Common Ancestor
    Point_3 lca;

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
    return Point_2(lca.x(), lca.y());
}
