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
#include <QObject>

#include <list>

class PolygonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PolygonWidget(QWidget *parent = nullptr);

    void setMode(int mode); // Set the mode (1 or 2)
    void clearPoints();     // Clear all selected points
    void constructPolygon(int size);
    void setPolygonMode(int index);
    void drawGivenPolygon(int index);
    void clearCanvas();
    void setStep(int step);

    void startStepperQ1();
    void startAutoQ1(int interval);
    void startAutoQ2(int interval);
    void startAutoApproximate(int interval);
    void increaseStep();
    void decreaseStep();
    QVector<QPointF> convertToQT(std::vector<Point_3> points);
    QString updateLog();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Polygon_2 polygonC;
    QVector<QPointF> polygonQ;

    PolygonGen m_polygonGenHandler;
    OnePointQuery m_onePointHandler;
    TwoPointQuery m_twoPointHandler;
    ShortestPath m_shortestPathHandler;
    ApproximateQuery m_approximateHandler;

    int m_mode; // Mode 1 or 2
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

    void twoPointQuery();
    void intersectionCase(QLineF& window1, QLineF& window2);
    void dominationCase(QLineF& window1, QLineF& window2);
    void computeGeneralCase(QLineF& window1, QLineF& window2, Polygon_2& polygon);
    void generalCase(QLineF& window1, QLineF& window2, Polygon_2& polygon);
    bool constructHourglass(QPointF& a1, QPointF& a2, QPointF& b1, QPointF& b2, Polygon_2& polygon);
    bool visibilitySQ;
    bool visibilitySQ1;
    bool visibilitySQ2;
    int step;
    bool stepmode;

    void visualizeAuto(QPainter &painter);
    void visualizeStep(QPainter &painter);
    void visualizeAuto2(QPainter &painter);
    void visualizeGeneralCase(QPainter& painter);
    void visualizeApprox(QPainter& painter);
    // Q_SLOT void startSingleQuery();
    void pointCheck();
    bool valid;
    QString errorMessage;
    bool hideQuery;

    int polygonMode;
    QVector<QPointF> clicks;
    QVector<QPointF> optimalPath;

    enum Q2CASE { NONE, INTERSECTION, DOMINATION, GENERAL };
    Q2CASE currentCase;

    OnePointQuery::QueryResult resultQ1;
    ApproximateQuery::ApproximateResult resultApprox;


    ////////////////////
// FOR TESTING
    QVector<QPointF> pathSA1;
    QVector<QPointF> pathSB1;
    QVector<QPointF> tangent1;
    QVector<QPointF> tangent2;
    QVector<QPointF> tangent3;
    QVector<QPointF> tangent4;

    QVector<QPointF> funnelSide1;
    QVector<QPointF> funnelSide2;

    QVector<QPointF> hourglassSide1;
    QVector<QPointF> hourglassSide2;
    QPointF root;

    QVector<QPointF> concatenatedSide1;
    QVector<QPointF> concatenatedSide2;

    QVector<QPointF> pathRA2;
    QVector<QPointF> pathRB2;

    QPointF m1;
    QPointF m2;
    QPointF m3;
    QPointF m4;
    /////////////////

    QVector<QPointF> intersectionPath1;
    QVector<QPointF> intersectionPath2;
    QVector<QPointF> intersectionPath3;
};

#endif // POLYGONWIDGET_H
