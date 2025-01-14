#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include "common.h"
#include "polygongen.h"
#include "OnePointQuery.h"
#include "TwoPointQuery.h"
#include "ShortestPath.h"
#include "approximatequery.h"

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits_2.h>
#include <QTimer>

#include <list>

class PolygonWidget : public QWidget
{
public:
    explicit PolygonWidget(QWidget *parent = nullptr);

    void setStartingPoint(const QPointF& point);

    void setQueryPoint1(const QPointF& point);

    void setQueryPoint2(const QPointF& point);

    enum QueryMode { EXACT, APPROX };

    void setQueryMode(QueryMode queryMode);
    void clearPoints();     // Clear all selected points
    bool isStartingPointSet() const;
    bool isQueryPoint1Set() const;
    bool isQueryPoint2Set() const;
    void constructRandomPolygon(int size);
    void prepareDrawnPolygon();
    void finishDrawnPolygon();
    void chooseExamplePolygon(int index);
    void clearCanvas();
    void clearComputation();
    void setStep(int step);

    void startOnePointQuery(int interval, bool stepMode);
    void startTwoPointQuery(int interval, bool stepMode);
    void startApproximateQuery(int interval, bool stepMode, double epsilon);
    void startNApproximateQuery(int interval, bool stepMode, double epsilon);
    bool withinBoundaryCheck();
    void increaseStep();
    void decreaseStep();
    void timedStepper(int targetStep, int interval);
    QString updateLog();
    

protected:
    void drawPolygonPoints(QPainter& painter);
    void setFixedPoints(QPointF& startingPoint, QPointF& queryPoint1, QPointF& queryPoint2);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool startSelected;
    bool query1Selected;
    bool query2Selected;

    QVector<QPointF> clicks;
    Polygon_2 polygonC;
    QVector<QPointF> polygonQ;

    PolygonGen m_polygonGenHandler;
    OnePointQuery m_onePointHandler;
    TwoPointQuery m_twoPointHandler;
    ShortestPath m_shortestPathHandler;
    ApproximateQuery m_approximateHandler;

 
    QueryMode m_queryMode;
    int m_step;
    enum Step;

    QPointF startingPoint;
    QPointF queryPoint1;
    QPointF queryPoint2;

    QPointF a;
    QPointF b;
    QPointF a1;
    QPointF b1;
    QPointF a2;
    QPointF b2;
    QVector<QPointF> shortestPathSQ;
    QVector<QPointF> shortestPathSQ1;
    QVector<QPointF> shortestPathSQ2;

    Point_2 lcaC;
    QPointF lca;
    QVector<QPointF> pathRA;
    QVector<QPointF> pathRB;
    void drawLabel(double x, double y, QString label, QPainter &painter);

    QPointF c;

    bool visibilitySQ;
    bool visibilitySQ1;
    bool visibilitySQ2;
    int step;
    bool stepmode;

    bool drawOwnPolygon;
    bool fixedPoints;

    QVector<QPointF> approxQueryPoints;

    void visualizeOnePointQuery(QPainter& painter);
    void visualizeTwoPointQuery(QPainter& painter);
    void visualizeIntersection(QPainter& painter);
    void visualizeDomination(QPainter& painter);
    void visualizeGeneralCase(QPainter& painter);
    void visualizeApprox(QPainter& painter);
    void visualizeNApprox(QPainter& painter);
    // Q_SLOT void startSingleQuery();
    QString errorMessage;

    QVector<QPointF> optimalPath;

    OnePointQuery::QueryResult resultQ1;
    TwoPointQuery::QueryResult resultQ2;
    TwoPointQuery::GeneralCaseResult resultGeneral;
    TwoPointQuery::IntersectionResult resultIntersection;
    TwoPointQuery::DominationResult resultDomination;
    ApproximateQuery::ApproximateResult resultApprox;
    ApproximateQuery::NApproximateResult resultNApprox;
};

#endif // POLYGONWIDGET_H
