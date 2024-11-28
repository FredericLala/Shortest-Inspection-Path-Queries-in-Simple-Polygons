#include "twopointquery.h"

TwoPointQuery::TwoPointQuery()
    : m_startSelected(false), m_query1Selected(false), m_query2Selected(false)
{
}

void TwoPointQuery::setStartingPoint(const QPointF &point)
{
    m_startingPoint = point;
    m_startSelected = true;
}

void TwoPointQuery::setQueryPoint1(const QPointF &point)
{
    m_queryPoint1 = point;
    m_query1Selected = true;
}

void TwoPointQuery::setQueryPoint2(const QPointF &point)
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
