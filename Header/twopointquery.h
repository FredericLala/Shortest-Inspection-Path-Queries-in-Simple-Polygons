#ifndef TwoPointQuery_H
#define TwoPointQuery_H

#include <QPoint>

class TwoPointQuery
{
public:
    TwoPointQuery();

    void setStartingPoint(const QPointF &point);
    void setQueryPoint1(const QPointF &point);
    void setQueryPoint2(const QPointF &point);

    void clearPoints();

    QPointF getStartingPoint() const;
    QPointF getQueryPoint1() const;
    QPointF getQueryPoint2() const;

    bool isStartingPointSet() const;
    bool isQueryPoint1Set() const;
    bool isQueryPoint2Set() const;

private:
    QPointF m_startingPoint;
    QPointF m_queryPoint1;
    QPointF m_queryPoint2;
    bool m_startSelected;
    bool m_query1Selected;
    bool m_query2Selected;
};

#endif // TwoPointQuery_H
