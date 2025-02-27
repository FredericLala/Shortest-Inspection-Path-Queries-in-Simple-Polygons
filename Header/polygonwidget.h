#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include "common.h"
#include "polygongen.h"
#include "onepointquery.h"
#include "twopointquery.h"
#include "shortestpath.h"
#include "approximatequery.h"
#include "generalcase.h"

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits_2.h>
#include <QTimer>

#include <list>
#include <CGAL/centroid.h>
#include <CGAL/Bbox_2.h>


class PolygonWidget : public QWidget
{
public:
    explicit PolygonWidget(QWidget *parent = nullptr);

    void setStartingPoint(const QPointF& point);

    void setQueryPoint1(const QPointF& point);

    void setQueryPoint2(const QPointF& point);

    void setScaledStartingPoint(const QPointF& point);

    void setScaledQueryPoint1(const QPointF& point);

    void setScaledQueryPoint2(const QPointF& point);

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
    void reset();
    void setStep(int step);

    void startOnePointQuery(int interval, bool stepMode);
    void startTwoPointQuery(int interval, bool stepMode);
    void startApproximateQuery(int interval, bool stepMode, double epsilon);
    void startNApproximateQuery(int interval, bool stepMode, double epsilon);
    bool withinBoundaryCheck();
    void increaseStep();
    void decreaseStep();
    void timedStepper(int targetStep, int interval);
    void setFixedPoints(int index);

    Polygon_2 scalePolygon(Polygon_2& polygon, int targetSize);

    const QPointF scalePoints(const QPointF& point);
    TwoPointQuery::QueryResult resultQ2;

    void resizeEvent(QResizeEvent* event);



protected:
    void drawPolygonPoints(QPainter& painter);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool startSelected;
    bool query1Selected;
    bool query2Selected;

    QVector<QPointF> clicks;
    Polygon_2 polygonC;
    QVector<QPointF> polygonQ;
    Surface_mesh m_mesh;

    PolygonGen m_polygonGenHandler;
    OnePointQuery m_onePointHandler;
    TwoPointQuery m_twoPointHandler;
    ShortestPath m_shortestPathHandler;
    ApproximateQuery m_approximateHandler;
    GeneralCase m_generalCaseHandler;

 
    QueryMode m_queryMode;
    int m_step;

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
    int step = 0;
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
    GeneralCase::GeneralCaseResult resultGeneral;
    TwoPointQuery::IntersectionResult resultIntersection;
    TwoPointQuery::DominationResult resultDomination;
    ApproximateQuery::ApproximateResult resultApprox;
    ApproximateQuery::NApproximateResult resultNApprox;

    bool newClickPoint = false;
    QString m_log;

    double centerX = 0;
    double centerY = 0;
    double scale = 0;
    QPolygonF shadowPolygon;
};

#endif // POLYGONWIDGET_H
