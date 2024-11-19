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
