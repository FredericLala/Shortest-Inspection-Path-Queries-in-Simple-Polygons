#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include "common.h"
#include "polygongen.h"
#include "OnePointQuery.h"
#include "TwoPointQuery.h"
#include "ShortestPath.h"

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits_2.h>
#include <QTimer>

#include <list>

class PolygonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PolygonWidget(QWidget *parent = nullptr);

    void setMode(int mode); // Set the mode (1 or 2)
    void clearPoints();     // Clear all selected points
    void constructPolygon(int size);
    void clearCanvas();
    void onePointQuery(QPointF queryPoint);
    void setStep(int step);
    void computeOptimalPoint();

    void startStepperQ1();
    void startAutoQ1(int interval);
    void startAutoQ2(int interval);
    void increaseStep();
    void decreaseStep();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QVector<QPointF> convertToQT(std::vector<Point_3> points);

private:
    Polygon_2 polygonC;
    QVector<QPointF> polygonQ;

    PolygonGen m_polygonGenHandler;
    OnePointQuery m_onePointHandler;
    TwoPointQuery m_twoPointHandler;
    ShortestPath m_shortestPathHandler;

    int m_mode; // Mode 1 or 2
    int m_step;
    enum Step;

    QPointF startingPoint;
    QPointF queryPoint1;
    QPointF queryPoint2;

    QVector<QPointF> calculateWindow(std::vector<Point_3> &path, QPointF queryPoint);
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
    int binarySearchByAngle(QVector<QPointF> &path);

    QPointF c;

    void twoPointQuery();
    bool visibilitySQ;
    bool visibilitySQ1;
    bool visibilitySQ2;
    int step;
    bool stepmode;

    void visualizeAuto(QPainter &painter);
    void visualizeStep(QPainter &painter);
    void visualizeAuto2(QPainter &painter);
    // Q_SLOT void startSingleQuery();
    void pointCheck();
    bool valid;
    std::string errorMessage;
    bool hideQuery;
};

#endif // POLYGONWIDGET_H
