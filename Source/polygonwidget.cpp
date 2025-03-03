#include "polygonwidget.h"

PolygonWidget::PolygonWidget(QWidget* parent) : QWidget(parent)
{
    drawOwnPolygon = false;
    fixedPoints = false;
    step = 0;
}

////////////////////////////////////////////////////////////////

void PolygonWidget::setStartingPoint(const QPointF& point)
{
    startingPoint = point;
    startSelected = true;
}

void PolygonWidget::setQueryPoint1(const QPointF& point)
{
    queryPoint1 = point;
    query1Selected = true;
}

void PolygonWidget::setQueryPoint2(const QPointF& point)
{
    queryPoint2 = point;
    query2Selected = true;
}

void PolygonWidget::setScaledStartingPoint(const QPointF& point)
{
    startingPoint = scalePoints(point);
    startSelected = true;
}

void PolygonWidget::setScaledQueryPoint1(const QPointF& point)
{
    queryPoint1 = scalePoints(point);
    query1Selected = true;
}

void PolygonWidget::setScaledQueryPoint2(const QPointF& point)
{
    queryPoint2 = scalePoints(point);
    query2Selected = true;
}

bool PolygonWidget::isStartingPointSet() const
{
    return startSelected;
}

bool PolygonWidget::isQueryPoint1Set() const
{
    return query1Selected;
}

bool PolygonWidget::isQueryPoint2Set() const
{
    return query2Selected;
}

////////////////////////////////////////////////////////////////

// constructs a randomly generated polygon of a given vertex size
void PolygonWidget::constructRandomPolygon(int size) {
    clearCanvas();
    QSize widgetSize = this->size();
    int targetSize = std::min(widgetSize.width(), widgetSize.height()) - 20; // Slightly smaller
    polygonC = m_polygonGenHandler.generateRandomPolygon(size, targetSize / 2);
    m_mesh = m_shortestPathHandler.createMesh(polygonC);
    // Convert CGAL polygon vertices to Qt points
    std::cout << "###########################################\n";
    std::cout << "Vertices of the Randomly Generated Polygon: \n";
    for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
    {
        std::cout << "p.push_back(Point_2(" << it->x() << "," << it->y() << "));" << "\n";
        polygonQ.append(QPointF(it->x(), it->y()));
    }
    std::cout << "\n";

    update();
}

// wipes the canvas to enable new points to be drawn
void PolygonWidget::prepareDrawnPolygon() {
    clicks.clear();
    clearCanvas();
    drawOwnPolygon = true;
}

// generates a polygon from manually drawn points
void PolygonWidget::finishDrawnPolygon() {
    drawOwnPolygon = false;
    shadowPolygon.clear();
    polygonC.clear();
    std::cout << "###########################################\n";
    std::cout << "Vertices of the Manually Drawn Polygon: \n";
    for (QPointF vertex : polygonQ)
    {
        polygonC.push_back(Point_2(vertex.x(), vertex.y()));
        std::cout << "p.push_back(Point_2(" << vertex.x() << "," << vertex.y() << "));" << "\n";
    }
    std::cout << "\n";

    if (!polygonC.is_simple()) {
        std::cout << "Non-Simple-Polygon-Failure: The Polygon was not Simple \n";
        prepareDrawnPolygon();
        return;
    }

    if (polygonC.size() >= 3) {
        m_mesh = m_shortestPathHandler.createMesh(polygonC);
    } else {
        prepareDrawnPolygon();
    }

    update();
}

// depending on index, a predefined polygon is selected
void PolygonWidget::chooseExamplePolygon(int index) {
    clearCanvas();
    QSize widgetSize = this->size();
    int targetSize = std::min(widgetSize.width(), widgetSize.height()) - 20;

    switch (index)
    {
    case 0:
        polygonC = m_polygonGenHandler.qOneRoot();
        break;
    case 1:
        polygonC = m_polygonGenHandler.qOneA();
        break;
    case 2:
        polygonC = m_polygonGenHandler.qOneMiddle();
        break;
    case 3:
        polygonC = m_polygonGenHandler.qTwoIntersection();
        break;
    case 4:
        polygonC = m_polygonGenHandler.qTwoDomination();
        break;
    case 5:
        polygonC = m_polygonGenHandler.qTwoGeneralClosed();
        break;
    case 6:
        polygonC = m_polygonGenHandler.qTwoGeneralAltOpen();
        break;
    case 7:
        polygonC = m_polygonGenHandler.qTwoGeneralOpen();
        break;
    case 8:
        polygonC = m_polygonGenHandler.testPolygon();
        break;
    default:
        polygonC = m_polygonGenHandler.generateRandomPolygon(40, targetSize);
        break;
    }

    polygonC = scalePolygon(polygonC, targetSize);
    m_mesh = m_shortestPathHandler.createMesh(polygonC);
    for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
    {
        polygonQ.append(QPointF(it->x(), it->y()));
    }

    update();
}

// sets examplary points corresponding for the predefined polygons
void PolygonWidget::setFixedPoints(int index) {
    switch (index)
    {
    case 0: // q1: c is perp. to root
        setScaledStartingPoint(QPointF(-211, 131));
        setScaledQueryPoint1(QPointF(21, 256));
        break;
    case 1: // q1: c = a
        setScaledStartingPoint(QPointF(121, 349));
        setScaledQueryPoint1(QPointF(-55, -137));
        break;
    case 2: // q1: c is perp to v1
        setScaledStartingPoint(QPointF(-184, 279));
        setScaledQueryPoint1(QPointF(58, 338));
        break;
    case 3: // q2: Intersection
        setScaledStartingPoint(QPointF(-151, 246));
        setScaledQueryPoint1(QPointF(-152, -216));
        setScaledQueryPoint2(QPointF(295, -39));
        break;
    case 4: // q2: Domination
        setScaledStartingPoint(QPointF(-348, 231));
        setScaledQueryPoint1(QPointF(299, -66));
        setScaledQueryPoint2(QPointF(-140, -299));

        break;
    case 5: // q2: general case - closed hourglass
        setScaledStartingPoint(QPointF(202, 158));
        setScaledQueryPoint1(QPointF(-151, 111));
        setScaledQueryPoint2(QPointF(-73, -1));
        break;
    case 6: // q2: general case - alt. is open
        setScaledStartingPoint(QPointF(-228, -27));
        setScaledQueryPoint1(QPointF(-322, 312));
        setScaledQueryPoint2(QPointF(-80, -131));
        break;
    case 7: // q2: general case - open hourglass
        setScaledStartingPoint(QPointF(-23, 135));
        setScaledQueryPoint1(QPointF(145, 64));
        setScaledQueryPoint2(QPointF(-204, 47));
        break;
    case 8: // test polygon
        setScaledStartingPoint(QPointF(-134, 384));
        setScaledQueryPoint1(QPointF(-242, -19));
        setScaledQueryPoint2(QPointF(-316, -260));
    default:
        break;
    }

    update();
}

// scales polygons to fit into the widget
Polygon_2 PolygonWidget::scalePolygon(Polygon_2& polygon, int targetSize) {
    if (polygon.is_empty()) {
        return polygon;
    }

    CGAL::Bbox_2 bbox = polygon.bbox();
    centerX = (bbox.xmin() + bbox.xmax()) / 2.0;
    centerY = (bbox.ymin() + bbox.ymax()) / 2.0;
    centerY = 0;
    double width = bbox.xmax() - bbox.xmin();
    double height = bbox.ymax() - bbox.ymin();
    scale = targetSize / std::max(width, height);

    Polygon_2 normalizedPolygon;
    for (auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
    {
        double newX = (CGAL::to_double(it->x()) - centerX) * scale;
        double newY = (CGAL::to_double(it->y()) - centerY) * scale;
        normalizedPolygon.push_back(Point_2(newX, newY));
    }

    return normalizedPolygon;
}

// scales points to fit into the scaled polygons
const QPointF PolygonWidget::scalePoints(const QPointF& point) {
    double newX = (point.x() - centerX) * scale;
    double newY = (point.y() - centerY) * scale;

    return QPointF(newX, newY);
}

// resizes the first polygon shown to a proper size
void PolygonWidget::resizeEvent(QResizeEvent* event)
{
    QSize widgetSize = this->size();
    int targetSize = std::min(widgetSize.width(), widgetSize.height()) - 20;

    if (targetSize > 200)
    {
        static bool firstResize = true;
        if (firstResize) {
            firstResize = false;
            polygonC = scalePolygon(polygonC, targetSize);
            m_mesh = m_shortestPathHandler.createMesh(polygonC);
            polygonQ.clear();
            for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
            {
                polygonQ.append(QPointF(it->x(), it->y()));
            }
        }

        update();
    }
}

////////////////////////////////////////////////////////////////

void PolygonWidget::setQueryMode(QueryMode queryMode)
{
    m_queryMode = queryMode;
    clearComputation();
    update();
}

void PolygonWidget::setStep(int step)
{
    m_step = step;
    update();
}

////////////////////////////////////////////////////////////////

void PolygonWidget::increaseStep()
{
    step++;
    update();
}

void PolygonWidget::decreaseStep()
{
    if (step - 1 >= 1)
    {
        step--;
        update();
    }
}

void PolygonWidget::timedStepper(int targetStep, int interval)
{
    step = 1;
    if (step >= targetStep) // Check if already at or above target
        return;

    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() {
        if (step < targetStep) {
            step++;
            update();
        }
        else {
            timer->stop();
            timer->deleteLater();
        }
    });

    timer->start(interval); // Start the timer with the given interval (milliseconds)
}

////////////////////////////////////////////////////////////////

// clears everything
void PolygonWidget::clearCanvas() {
    polygonC.clear();
    polygonQ.clear();
    shadowPolygon.clear();
    drawOwnPolygon = false;
    m_mesh.clear();
    clearComputation();
    update();
}

// clears computations and all points, but not the polygon
void PolygonWidget::clearComputation() {
    clearPoints();
    reset();
    update();
}

// clears all points
void PolygonWidget::clearPoints() {
    startSelected = false;
    query1Selected = false;
    query2Selected = false;
    approxQueryPoints.clear();
}

// clears computations, but not the points or the polygon
void PolygonWidget::reset() {
    step = 0;
    resultQ1 = OnePointQuery::QueryResult();
    resultQ2 = TwoPointQuery::QueryResult();
    resultGeneral = GeneralCase::GeneralCaseResult();
    resultIntersection = TwoPointQuery::IntersectionResult();
    resultDomination = TwoPointQuery::DominationResult();
    resultNApprox = ApproximateQuery::NApproximateResult();
    m_onePointHandler.clearTree();
    m_shortestPathHandler.clearTree();
    update();
}

////////////////////////////////////////////////////////////////

// tests whether all points are within the polygon's boundary
bool PolygonWidget::withinBoundaryCheck()
{
    if (query2Selected && !polygonC.has_on_bounded_side(Point_2(queryPoint2.x(), queryPoint2.y())))
    {
        std::cout << "Within-Boundary-Failure: Query Point 2 needs to be Inside of the Polygon \n";
        clearPoints();
        update();
        return false;
    }

    if (startSelected && !polygonC.has_on_bounded_side(Point_2(startingPoint.x(), startingPoint.y())))
    {
        std::cout << "Within-Boundary-Failure: The Source Point needs to be Inside of the Polygon \n";
        clearPoints();
        update();
        return false;
    }

    if (query1Selected && !polygonC.has_on_bounded_side(Point_2(queryPoint1.x(), queryPoint1.y())))
    {
        std::cout << "Within-Boundary-Failure: Query Point 1 needs to be Inside of the Polygon \n";
        clearPoints();
        update();
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////

// initializes the query for one query point
void PolygonWidget::startOnePointQuery(int interval, bool stepMode) {
    if (withinBoundaryCheck())
    {
        reset();
        stepmode = stepMode;
        step = 1;
        std::cout << "###########################################\n";
        std::cout << "Start of the Shortest Inspection-Path Query for a Single Points: \n";
        m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint1, polygonC, m_mesh);
        resultQ1 = m_onePointHandler.getResult();

        if (!stepMode) {
            timedStepper(8, interval);
        }

        update();
    }
}

// initializes the query for two query points
void PolygonWidget::startTwoPointQuery(int interval, bool stepMode) {
    int maxSteps = 0;
    if (withinBoundaryCheck()) {
        reset();
        stepmode = stepMode;
        step = 1;
        m_twoPointHandler.executeTwoPointQuery(startingPoint, queryPoint1, queryPoint2, polygonC, m_mesh);
        resultQ2 = m_twoPointHandler.getQ2Result();

        switch (resultQ2.currentCase) {
        case TwoPointQuery::QNONE:
            maxSteps = 0;
            break;
        case TwoPointQuery::INTERSECTION:
            maxSteps = 7;
            break;
        case TwoPointQuery::DOMINATION:
            maxSteps = 5;
            break;
        case TwoPointQuery::GENERAL:
            maxSteps = 9;
            break;
        default:
            maxSteps = 0;
            break;
        }

        if (!stepMode) {
            timedStepper(maxSteps, interval);
        }

        update();
    }

}

// initializes the approximate-query
void PolygonWidget::startNApproximateQuery(int interval, bool stepMode, double epsilon) {
    int maxSteps = 6;
    if (approxQueryPoints.size() == 2) {
        maxSteps = 7;
    }

    if (withinBoundaryCheck()) {
        stepmode = stepMode;
        step = 1;
        m_approximateHandler.nEpsilonApproximateQuery(epsilon, startingPoint, approxQueryPoints, polygonC, m_mesh);

        if (!stepMode) {
            timedStepper(maxSteps, interval);
        }

        update();
    }
}

////////////////////////////////////////////////////////////////

// converts mouse clicks into points
void PolygonWidget::mousePressEvent(QMouseEvent* event)
{
    // convert the clicks to the cartesian plane system
    QPointF clickPoint = QPointF(event->pos().x() - width() / 2, height() / 2 - event->pos().y());

    // clicks are converted into polygon vertices instead
    if (drawOwnPolygon) {
        clicks.append(clickPoint);
        newClickPoint = true;
        return;
    }

    if (m_queryMode == EXACT) {
        if (!startSelected) {
            setStartingPoint(clickPoint);
            std::cout << "setScaledStartingPoint(QPointF(" << startingPoint.x() << ", " << startingPoint.y() << ")); \n";
        }
        else if (!query1Selected) {
            setQueryPoint1(clickPoint);
            std::cout << "setScaledQueryPoint1(QPointF(" << queryPoint1.x() << ", " << queryPoint1.y() << ")); \n";
        }
        else if (!query2Selected) {
            if (step > 0) {
                QPointF tempStart = startingPoint;
                QPointF tempQuery1 = queryPoint1;
                clearComputation();
                setStartingPoint(tempStart);
                setQueryPoint1(tempQuery1);
            }
            setQueryPoint2(clickPoint);
            std::cout << "setScaledQueryPoint2(QPointF(" << queryPoint2.x() << ", " << queryPoint2.y() << ")); \n";
        }
    }
    else if (m_queryMode == APPROX) {
        if (!startSelected) {
            setStartingPoint(clickPoint);
        }
        else {
            approxQueryPoints.append(clickPoint);
        }
    }
    update();
}

////////////////////////////////////////////////////////////////

// draws the progression of the manually drawn polygon
void PolygonWidget::drawPolygonPoints(QPainter& painter) {
    QColor customColor(217, 217, 217);
    if (!clicks.isEmpty() && newClickPoint) {
        std::cout << "p.push_back(Point_2(" << clicks.last().x() << "," << clicks.last().y() << "));" << "\n";
        polygonQ.append(clicks.last());
        shadowPolygon.push_back(clicks.last());
        newClickPoint = false;
    }

    if (clicks.size() > 0) {
        painter.setBrush(customColor);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(shadowPolygon);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::black, 3));
        painter.drawEllipse(clicks[0], 3, 3);
        for (qsizetype i = 1; i < clicks.size(); i++) {
            painter.drawEllipse(clicks[i], 3, 3);
            painter.drawLine(clicks[i - 1], clicks[i]);
        }
    }
}

// draws labels for points
void PolygonWidget::drawLabel(double x, double y, QString label, QPainter& painter)
{
    QPen originalPen = painter.pen();
    const int RADIUS = 3;
    painter.resetTransform();
    painter.translate(width() / 2, height() / 2);
    painter.setPen(Qt::black);

    // Set text size
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    // Draw text outline manually by offsetting in multiple directions
    painter.setPen(Qt::white);
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 || dy != 0) { // Skip the center
                painter.drawText(x + RADIUS + dx, -y - RADIUS + dy, label);
            }
        }
    }

    // Draw text in black over the outline
    painter.setPen(Qt::black);
    painter.drawText(x + RADIUS, -y - RADIUS, label);

    painter.scale(1, -1);
    painter.setPen(originalPen);
}

// responsible for overall visualization
void PolygonWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // Translate and scale to center the view
    painter.translate(width() / 2, height() / 2);
    painter.scale(1, -1); // Flip y-axis for Cartesian coordinates

    QColor customColor(217, 217, 217);
    QPen pen(customColor, 3);
    pen.setBrush(customColor);
    pen.setColor(Qt::black);
    painter.setBrush(customColor);
    painter.setPen(pen);

    if (drawOwnPolygon)
    {
        drawPolygonPoints(painter);
        update();
    }
    else if (polygonQ.size() > 2)
    {
        painter.drawPolygon(polygonQ);
    }


    // Draw the polygon and its Delaunay triangulation
    painter.setPen(Qt::darkGray);
    for (auto edge : m_mesh.edges())
    {
        auto source = m_mesh.point(m_mesh.vertex(edge, 0));
        auto target = m_mesh.point(m_mesh.vertex(edge, 1));

        painter.drawLine(QPointF(source.x(), source.y()), QPointF(target.x(), target.y()));
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    if (startSelected) {
        painter.drawEllipse(startingPoint, 3, 3);
        drawLabel(startingPoint.x(), startingPoint.y(), QString("s"), painter);
    }

    if (query1Selected) {
        painter.drawEllipse(queryPoint1, 3, 3);
        drawLabel(queryPoint1.x(), queryPoint1.y(), QString("q1"), painter);
    }

    if (query2Selected) {
        painter.drawEllipse(queryPoint2, 3, 3);
        drawLabel(queryPoint2.x(), queryPoint2.y(), QString("q2"), painter);
    }

    int index = 1;
    if (m_queryMode == APPROX) {
        for (QPointF point : approxQueryPoints) {
            painter.drawEllipse(point, 3, 3);
            drawLabel(point.x(), point.y(), QString("q%1").arg(index), painter);
            index++;
        }
    }

    if (step >= 1) {
        switch (m_queryMode)
        {
        case EXACT:
            if (query1Selected && !query2Selected) {
                visualizeOnePointQuery(painter);
            }
            else if (query2Selected) {
                visualizeTwoPointQuery(painter);
            }
            break;

        case APPROX:
            visualizeNApprox(painter);
            break;

        default:
            break;
        }
    }
}


void PolygonWidget::visualizeOnePointQuery(QPainter& painter)
{
    if (step >= 1) {
        if (resultQ1.visibility)
        {
            drawLabel(startingPoint.x() + 5, startingPoint.y(), QString(" = c"), painter);

            return;
        }
    }

    // Draw the Shortest Path
    if (step >= 2) {
        painter.setPen(QPen(Qt::red, 3));
        QVector<QPointF> pathStartToQuery = resultQ1.pathStartToQuery;
        for (size_t i = 1; i < pathStartToQuery.size(); ++i)
        {
            painter.drawLine(pathStartToQuery[i - 1], pathStartToQuery[i]);
        }
    }

    // Draw Point A
    QLineF window = resultQ1.window;
    if (step >= 3) {
        QPointF a = window.p1();
        painter.setPen(QPen(Qt::green, 3));
        painter.setBrush(Qt::green);
        painter.drawEllipse(a, 3, 3);
        drawLabel(a.x(), a.y(), QString("a"), painter);
    }

    // Draw Point B and Window
    if (step >= 4) {
        QPointF b = window.p2();
        painter.drawEllipse(b, 3, 3);
        drawLabel(b.x(), b.y(), QString("b"), painter);
        painter.drawLine(window);
    }

    // Draw LCA
    if (step >= 5) {
        QPointF lca = resultQ1.lca;
        painter.setPen(QPen(Qt::blue, 3));
        painter.setBrush(Qt::blue);
        painter.drawEllipse(lca, 3, 3);
        if (m_generalCaseHandler.areEqual(a, lca) || m_generalCaseHandler.areEqual(b, lca) || m_generalCaseHandler.areEqual(startingPoint, lca)) {
            drawLabel(lca.x() + 5, lca.y(), QString(" = r"), painter);
        }
        else {
            drawLabel(lca.x(), lca.y(), QString("r"), painter);
        }
    }

    // Draw the Funnel
    if (step >= 6) {
        QVector<QPointF> pathRA = resultQ1.pathRootToA;
        for (size_t i = 1; i < pathRA.size(); ++i)
        {
            painter.drawLine(pathRA[i - 1], pathRA[i]);
            painter.drawEllipse(pathRA[i], 3, 3);
        }

        QVector<QPointF> pathRB = resultQ1.pathRootToB;
        for (size_t i = 1; i < pathRB.size(); ++i)
        {
            painter.drawLine(pathRB[i - 1], pathRB[i]);
            painter.drawEllipse(pathRB[i], 3, 3);
        }
    }

    // Draw the Optimal Point C
    if (step >= 7) {
        QPointF c = resultQ1.optimalPoint;
        painter.setPen(QPen(Qt::magenta, 3));
        painter.setBrush(Qt::magenta);
        painter.drawEllipse(c, 3, 3);
        if (m_generalCaseHandler.areEqual(a, c)|| m_generalCaseHandler.areEqual(b, c)) {
            drawLabel(c.x() + 5, c.y(), QString(" = c"), painter);
        }
        else {
            drawLabel(c.x(), c.y(), QString("c"), painter);
        }
    }

    // Draw the Optimal Path
    if (step >= 8) {
        QVector<QPointF> optimalPath = resultQ1.optimalPath;
        for (size_t i = 1; i < optimalPath.size(); ++i)
        {
            painter.drawLine(optimalPath[i - 1], optimalPath[i]);
            painter.drawEllipse(optimalPath[i], 3, 3);
        }
    }
}

void PolygonWidget::visualizeTwoPointQuery(QPainter& painter) {
    // Visibility Check
    if (step >= 1) {
        if (resultQ2.visibilityQ1 && resultQ2.visibilityQ2) {
            drawLabel(startingPoint.x() + 5, startingPoint.y(), QString("c"), painter);
            return;
        }
        else if (resultQ2.visibilityQ1) {
            resultQ1 = resultQ2.resultQ1;
            step = 10;
            visualizeOnePointQuery(painter);
        }
        else if (resultQ2.visibilityQ2) {
            resultQ1 = resultQ2.resultQ1;
            step = 10;
            visualizeOnePointQuery(painter);
        }
    }

    // Draw Window 1
    if (step >= 2) {
        a1 = resultQ2.window1.p1();
        b1 = resultQ2.window1.p2();
        painter.setPen(QPen(Qt::green, 3));
        painter.setBrush(Qt::green);
        painter.drawEllipse(a1, 3, 3);
        drawLabel(a1.x(), a1.y(), QString("a1"), painter);

        painter.drawEllipse(b1, 3, 3);
        drawLabel(b1.x(), b1.y(), QString("b1"), painter);

        painter.drawLine(a1, b1);
    }

    // Draw Window 2
    if (step >= 3) {
        a2 = resultQ2.window2.p1();
        b2 = resultQ2.window2.p2();
        painter.setPen(QPen(Qt::darkGreen, 3));
        painter.setBrush(Qt::darkGreen);
        painter.drawEllipse(a2, 3, 3);
        drawLabel(a2.x(), a2.y(), QString("a2"), painter);

        painter.drawEllipse(b2, 3, 3);
        drawLabel(b2.x(), b2.y(), QString("b2"), painter);

        painter.drawLine(a2, b2);
    }

    switch (resultQ2.currentCase)
    {
    case TwoPointQuery::Q2CASE::QNONE:
        break;
    case TwoPointQuery::Q2CASE::INTERSECTION:
        visualizeIntersection(painter);
        break;
    case TwoPointQuery::Q2CASE::DOMINATION:
        visualizeDomination(painter);
        break;
    case TwoPointQuery::Q2CASE::GENERAL:
        visualizeGeneralCase(painter);
        break;
    default:
        break;
    }
}

void PolygonWidget::visualizeIntersection(QPainter& painter) {
    TwoPointQuery::IntersectionResult intersectionResult = m_twoPointHandler.getIntersectionResult();
    QVector<QPointF> intersectionPath1 = intersectionResult.intersectionPath1;
    QVector<QPointF> intersectionPath2 = intersectionResult.intersectionPath2;
    QVector<QPointF> intersectionPath3 = intersectionResult.intersectionPath3;
    QVector<QPointF> optimalPath = intersectionResult.optimalPath;

    if (step >= 4) {
        painter.setPen(QPen(Qt::darkRed, 3));
        for (size_t i = 1; i < intersectionPath1.size(); ++i)
        {
            painter.drawLine(intersectionPath1[i - 1], intersectionPath1[i]);
        }
    }

    if (step >= 5) {
        painter.setPen(QPen(Qt::red, 3));
        for (size_t i = 1; i < intersectionPath2.size(); ++i)
        {
            painter.drawLine(intersectionPath2[i - 1], intersectionPath2[i]);
        }
    }

    if (step >= 6) {
        painter.setPen(QPen(Qt::yellow, 3));
        for (size_t i = 1; i < intersectionPath3.size(); ++i)
        {
            painter.drawLine(intersectionPath3[i - 1], intersectionPath3[i]);
        }
    }

    if (step == 7) {
        painter.setPen(QPen(Qt::magenta, 3));
        for (size_t i = 1; i < optimalPath.size(); ++i)
        {
            painter.drawLine(optimalPath[i - 1], optimalPath[i]);
        }
    }
}

void PolygonWidget::visualizeDomination(QPainter& painter) {
    TwoPointQuery::DominationResult dominationResult = m_twoPointHandler.getDominationResult();
    QVector<QPointF> optimalPath = dominationResult.optimalPath;

    if (step >= 4) {
        painter.setPen(QPen(Qt::magenta, 3));
        painter.setBrush(Qt::magenta);
        QPointF c = optimalPath.rbegin()[0];
        painter.drawEllipse(c, 3, 3);
        if (m_generalCaseHandler.areEqual(a1, c)|| m_generalCaseHandler.areEqual(b1, c) || m_generalCaseHandler.areEqual(a2, c) || m_generalCaseHandler.areEqual(b2, c)) {
            drawLabel(c.x() + 5, c.y(), QString(" = c"), painter);
        }
        else {
            drawLabel(c.x(), c.y(), QString("c"), painter);
        }
    }

    if (step == 5) {
        for (size_t i = 1; i < optimalPath.size(); ++i)
        {
            painter.drawLine(optimalPath[i - 1], optimalPath[i]);
        }
    }
}

void PolygonWidget::visualizeGeneralCase(QPainter& painter) {
    GeneralCase::GeneralCaseResult generalResult = m_twoPointHandler.getGeneralCaseResult();

    QVector<QPointF> funnelSideA = generalResult.funnelSideA;
    QVector<QPointF> funnelSideB = generalResult.funnelSideB;
    QVector<QPointF> hourglassSide1 = generalResult.hourglassSide1;
    QVector<QPointF> hourglassSide2 = generalResult.hourglassSide2;
    QVector<QPointF> concatenatedSide1 = generalResult.concatenatedSide1;
    QVector<QPointF> concatenatedSide2 = generalResult.concatenatedSide2;
    QVector<QPointF> optimalPath = generalResult.optimalPath;
    QPointF optimalPoint = generalResult.optimalPoint;


    if (step >= 4) {
        painter.setPen(QPen(Qt::blue, 3));
        painter.setBrush(Qt::blue);
        painter.drawEllipse(generalResult.funnelRoot, 3, 3);

        for (size_t i = 1; i < funnelSideA.size(); ++i)
        {
            painter.drawLine(funnelSideA[i - 1], funnelSideA[i]);
            painter.drawEllipse(funnelSideA[i], 3, 3);
        }


        for (size_t i = 1; i < funnelSideB.size(); ++i)
        {
            painter.drawLine(funnelSideB[i - 1], funnelSideB[i]);
            painter.drawEllipse(funnelSideB[i], 3, 3);
        }
    }

    if (step >= 5) {
        painter.setPen(QPen(Qt::red, 3));
        painter.setBrush(Qt::red);

        for (size_t i = 1; i < hourglassSide1.size(); ++i)
        {
            painter.drawLine(hourglassSide1[i - 1], hourglassSide1[i]);
            painter.drawEllipse(hourglassSide1[i], 3, 3);
        }

        for (size_t i = 1; i < hourglassSide2.size(); ++i)
        {
            painter.drawLine(hourglassSide2[i - 1], hourglassSide2[i]);
            painter.drawEllipse(hourglassSide2[i], 3, 3);
        }
    }

    if (step >= 6) {
        QVector<QPointF> tangent1 = generalResult.tangent1;
        QVector<QPointF> tangent2 = generalResult.tangent2;
        QVector<QPointF> tangent3 = generalResult.tangent3;
        QVector<QPointF> tangent4 = generalResult.tangent4;
        painter.setPen(QPen(Qt::darkMagenta, 3));
        painter.setBrush(Qt::darkMagenta);

        for (size_t i = 1; i < tangent1.size(); ++i)
        {
            painter.drawLine(tangent1[i - 1], tangent1[i]);
        }

        for (size_t i = 1; i < tangent2.size(); ++i)
        {
            painter.drawLine(tangent2[i - 1], tangent2[i]);
        }

        for (size_t i = 1; i < tangent3.size(); ++i)
        {
            painter.drawLine(tangent3[i - 1], tangent3[i]);
        }

        for (size_t i = 1; i < tangent4.size(); ++i)
        {
            painter.drawLine(tangent4[i - 1], tangent4[i]);
        }
    }

    if (step >= 7) {
        painter.setPen(QPen(Qt::darkCyan, 3));
        painter.setBrush(Qt::darkCyan);
        for (size_t i = 1; i < concatenatedSide1.size(); ++i)
        {
            painter.drawLine(concatenatedSide1[i - 1], concatenatedSide1[i]);
            painter.drawEllipse(concatenatedSide1[i], 3, 3);
        }

        for (size_t i = 1; i < concatenatedSide2.size(); ++i)
        {
            painter.drawLine(concatenatedSide2[i - 1], concatenatedSide2[i]);
            painter.drawEllipse(concatenatedSide2[i], 3, 3);
        }

        QPointF m1 = generalResult.m1;
        QPointF m2 = generalResult.m2;
        QPointF m3 = generalResult.m3;
        QPointF m4 = generalResult.m4;
        painter.drawEllipse(m1, 3, 3);
        drawLabel(m1.x(), m1.y(), QString("m1"), painter);

        painter.drawEllipse(m2, 3, 3);
        drawLabel(m2.x(), m2.y(), QString("m2"), painter);

        painter.drawEllipse(m3, 3, 3);
        drawLabel(m3.x(), m3.y(), QString("m3"), painter);

        painter.drawEllipse(m4, 3, 3);
        drawLabel(m4.x(), m4.y(), QString("m4"), painter);
    }

    if (step >= 8) {
        painter.setPen(QPen(Qt::magenta, 3));
        painter.setBrush(Qt::magenta);

        painter.drawEllipse(optimalPoint, 3, 3);

        if (m_generalCaseHandler.areEqual(a1, optimalPoint)|| m_generalCaseHandler.areEqual(b1, optimalPoint) || m_generalCaseHandler.areEqual(a2, optimalPoint) || m_generalCaseHandler.areEqual(b2, optimalPoint)) {
            drawLabel(optimalPoint.x() + 5, optimalPoint.y(), QString(" = c"), painter);
        }
        else {
            drawLabel(optimalPoint.x(), optimalPoint.y(), QString("c"), painter);
        }
    }

    if (step == 9) {
        for (size_t i = 1; i < optimalPath.size(); ++i)
        {
            painter.drawLine(optimalPath[i - 1], optimalPath[i]);
            painter.drawEllipse(optimalPath[i], 3, 3);
        }
    }
}

void PolygonWidget::visualizeNApprox(QPainter& painter) {
    resultNApprox = m_approximateHandler.getNApproximateResult();

    QVector<QPointF> nApproxPath = resultNApprox.nApproxPath;
    if (step >= 1) {
        painter.setPen(QPen(Qt::yellow, 3));
        painter.setBrush(Qt::yellow);
        for (size_t i = 1; i < nApproxPath.size(); ++i)
        {
            painter.drawLine(nApproxPath[i - 1], nApproxPath[i]);
            painter.drawEllipse(nApproxPath[i], 3, 3);
        }
    }

    if (step >= 2) {
        painter.setPen(QPen(Qt::green, 3));
        painter.setBrush(Qt::green);
        for (const QLineF window : resultNApprox.windows) {
            painter.drawLine(window);
        }
    }

    double radius = resultNApprox.discRadius;
    if (step >= 3) {
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(Qt::NoBrush); // No filling for the ellipse
        painter.drawEllipse(startingPoint, radius, radius);
    }

    if (step >= 4) {
        painter.setPen(QPen(Qt::darkCyan, 3));
        painter.setBrush(Qt::darkCyan);
        for (const QLineF intersectionWindow : resultNApprox.intersectionWindows) {
            painter.drawLine(intersectionWindow);
        }
    }

    if (step >= 5) {
        painter.setPen(QPen(Qt::red, 3));
        painter.setBrush(Qt::red);

        for (size_t i = 0; i < resultNApprox.equallySpacedPointsGroup.size(); ++i) {
            for (size_t j = 0; j < resultNApprox.equallySpacedPointsGroup[i].size(); ++j) {
                painter.drawEllipse(resultNApprox.equallySpacedPointsGroup[i][j], 1, 1);
            }
        }
    }

    if (step >= 6) {
        painter.setPen(QPen(Qt::darkRed, 3));
        painter.setBrush(Qt::darkRed);
        QVector<QPointF> shortestPath = resultNApprox.shortestPath;
        for (size_t i = 1; i < shortestPath.size(); ++i)
        {
            painter.drawLine(shortestPath[i - 1], shortestPath[i]);
        }
    }

    if (step == 7) {
        QPen pen(Qt::magenta, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);

        QVector<QPointF> exactShortestPath = resultNApprox.exactShortestPath;
        for (size_t i = 1; i < exactShortestPath.size(); ++i)
        {
            painter.drawLine(exactShortestPath[i - 1], exactShortestPath[i]);
        }
    }
}
