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

void PolygonWidget::constructRandomPolygon(int size) {
	clearCanvas();
	QSize widgetSize = this->size();
	int targetSize = std::min(widgetSize.width(), widgetSize.height()) - 20; // Slightly smaller
	polygonC = m_polygonGenHandler.generateRandomPolygon(size, targetSize / 2);
	m_mesh = m_shortestPathHandler.createMesh(polygonC);
	// Convert CGAL polygon vertices to Qt points
	for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
	{
		std::cout << "p.push_back(Point_2(" << it->x() << "," << it->y() << "));" << "\n";
		polygonQ.append(QPointF(it->x(), it->y()));
	}
	//std::cout << "" << "\n";

	update();
}

void PolygonWidget::prepareDrawnPolygon() {
	clicks.clear();
	clearCanvas();
	drawOwnPolygon = true;
}

void PolygonWidget::finishDrawnPolygon() {
	drawOwnPolygon = false;
	shadowPolygon.clear();
	polygonC.clear();
	for (QPointF vertex : polygonQ)
	{
		polygonC.push_back(Point_2(vertex.x(), vertex.y()));
	}

	if (!polygonC.is_simple()) {
		clearCanvas();
		return;
	}

	if (polygonC.size() >= 3) {
		m_mesh = m_shortestPathHandler.createMesh(polygonC);
	}
	update();
}

void PolygonWidget::chooseExamplePolygon(int index) {
	clearCanvas();
	QSize widgetSize = this->size();
	int targetSize = std::min(widgetSize.width(), widgetSize.height()) - 20; // Slightly smaller

	switch (index)
	{
	case 0:
		polygonC = m_polygonGenHandler.exampleOne();
		break;
	case 1:
		polygonC = m_polygonGenHandler.exampleTwo();
		break;
	case 2:
		polygonC = m_polygonGenHandler.exampleThree();
		break;
	case 3:
		polygonC = m_polygonGenHandler.exampleFour();
		break;
	case 4:
		polygonC = m_polygonGenHandler.altIsOpen();
		break;
	case 5:
		polygonC = m_polygonGenHandler.openHourglass();
		break;
	case 6:
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

void PolygonWidget::setFixedPoints(int index) {
	switch (index)
	{
	case 0:
		break;
	case 1:
		setScaledStartingPoint(QPointF(-31, -104));
		setScaledQueryPoint1(QPointF(-146, -20));
		setScaledQueryPoint2(QPointF(99, -48));
		break;
	case 2:
		break;
	case 3:
		//closed hourglass
		setScaledStartingPoint(QPointF(202, 158));
		setScaledQueryPoint1(QPointF(-151, 111));
		setScaledQueryPoint2(QPointF(-73, -1));

		// opern hourglass
		/*
		startingPoint = QPointF(204, 162);
		queryPoint1 = QPointF(-111, 10);
		queryPoint2 = QPointF(-154, 114);
		update();
		*/
		break;
	case 4:
		setScaledStartingPoint(QPointF(-228, -27));
		setScaledQueryPoint1(QPointF(-322, 312));
		setScaledQueryPoint2(QPointF(-80, -131));
		break;

	case 5:
		setScaledStartingPoint(QPointF(-23, 135));
		setScaledQueryPoint1(QPointF(145, 64));
		setScaledQueryPoint2(QPointF(-204, 47));
		break;
	case 6:
		setScaledStartingPoint(QPointF(307, -108));
		setScaledQueryPoint1(QPointF(291, 153));
		setScaledQueryPoint2(QPointF(-111, -291));
		break;
	default:
		break;
	}

	update();
}

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

	// Apply scaling and translation to center it
	Polygon_2 normalizedPolygon;
	for (auto it = polygon.vertices_begin(); it != polygon.vertices_end(); ++it)
	{
		double newX = (CGAL::to_double(it->x()) - centerX) * scale;
		double newY = (CGAL::to_double(it->y()) - centerY) * scale;
		normalizedPolygon.push_back(Point_2(newX, newY));
	}

	return normalizedPolygon;
}

const QPointF PolygonWidget::scalePoints(const QPointF& point) {
	double newX = (point.x() - centerX) * scale;
	double newY = (point.y() - centerY) * scale;

	return QPointF(newX, newY);
}

/*
void PolygonWidget::resizeEvent(QResizeEvent* event)
{
	QSize newSize = event->size();
	int targetSize = std::min(newSize.width(), newSize.height()) - 20; // Keep margin

	// Re-center and normalize the polygon
	scalePolygon(polygonC, targetSize);
	m_mesh = m_shortestPathHandler.createMesh(polygonC);

	// Update Qt polygon data
	polygonQ.clear();
	for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
	{
		polygonQ.append(QPointF(it->x(), it->y()));
	}

	update();  // Trigger a repaint
}
*/

////////////////////////////////////////////////////////////////

void PolygonWidget::setQueryMode(QueryMode queryMode)
{
	// Set the mode
	m_queryMode = queryMode;
	clearComputation();
	update(); // Force a repaint to update the GUI
}

void PolygonWidget::setStep(int step)
{
	// Set the mode
	m_step = step;
	update(); // Force a repaint to update the GUI
}

////////////////////////////////////////////////////////////////

QString PolygonWidget::updateLog()
{
	return errorMessage + m_onePointHandler.getLog();
}

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

	QTimer* timer = new QTimer(this); // Create a timer

	connect(timer, &QTimer::timeout, this, [=]() {
		if (step < targetStep) {
			step++;
			update();
		}
		else {
			timer->stop(); // Stop the timer once the target is reached
			timer->deleteLater(); // Clean up the timer
		}
		});

	timer->start(interval); // Start the timer with the given interval (milliseconds)
}

////////////////////////////////////////////////////////////////

void PolygonWidget::clearCanvas() {
	polygonC.clear();
	polygonQ.clear();
	drawOwnPolygon = false;
	m_mesh.clear();
	clearComputation();
	update();
}

void PolygonWidget::clearComputation() {
	m_onePointHandler.clearTree();
	m_shortestPathHandler.clearTree();
	clearPoints();
	step = 0;
	resultQ1 = OnePointQuery::QueryResult();
	resultQ2 = TwoPointQuery::QueryResult();
	resultGeneral = GeneralCase::GeneralCaseResult();
	resultIntersection = TwoPointQuery::IntersectionResult();
	resultDomination = TwoPointQuery::DominationResult();
	//resultApprox = ApproximateQuery::ApproximateResult();
	resultNApprox = ApproximateQuery::NApproximateResult();
	errorMessage = "";
	m_onePointHandler.resetLog();
	update();
}

void PolygonWidget::clearPoints() {
	startSelected = false;
	query1Selected = false;
	query2Selected = false;
	approxQueryPoints.clear();
}

////////////////////////////////////////////////////////////////

bool PolygonWidget::withinBoundaryCheck()
{
	if (query2Selected && !polygonC.has_on_bounded_side(Point_2(queryPoint2.x(), queryPoint2.y())))
	{
		std::cout << "Query Point 2 needs to be Inside of the Polygon \n";
		clearPoints();
		update();
		return false;
	}

	if (startSelected && !polygonC.has_on_bounded_side(Point_2(startingPoint.x(), startingPoint.y())))
	{
		std::cout << "Starting Point needs to be Inside of the Polygon \n";
		clearPoints();
		update();
		return false;
	}

	if (query1Selected && !polygonC.has_on_bounded_side(Point_2(queryPoint1.x(), queryPoint1.y())))
	{
		std::cout << "Query Point 1 needs to be Inside of the Polygon \n";
		clearPoints();
		update();
		return false;
	}

	// no error
	errorMessage = "";
	return true;
}

////////////////////////////////////////////////////////////////

void PolygonWidget::startOnePointQuery(int interval, bool stepMode) {
	if (withinBoundaryCheck())
	{
		stepmode = stepMode;
		step = 1;
		m_onePointHandler.executeOnePointQuery(startingPoint, queryPoint1, polygonC, m_mesh);
		resultQ1 = m_onePointHandler.getResult();

		if (!stepMode) {
			timedStepper(8, interval);
		}

		update();
	}
}

void PolygonWidget::startTwoPointQuery(int interval, bool stepMode) {
	int maxSteps = 0;
	if (withinBoundaryCheck()) {
		stepmode = stepMode;
		step = 1;
		m_twoPointHandler.executeTwoPointQuery(startingPoint, queryPoint1, queryPoint2, polygonC, m_mesh);
		resultQ2 = m_twoPointHandler.getQ2Result();

		switch (resultQ2.currentCase) {
		case TwoPointQuery::QNONE:
			maxSteps = 0;
			break;
		case TwoPointQuery::INTERSECTION:
			maxSteps = 9;
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

void PolygonWidget::startApproximateQuery(int interval, bool stepMode, double epsilon) {
	const int maxSteps = 6;
	if (withinBoundaryCheck()) {
		stepmode = stepMode;
		step = 1;
		m_approximateHandler.epsilonApproximateQuery(epsilon, startingPoint, queryPoint1, queryPoint2, polygonC, m_mesh);

		if (!stepMode) {
			timedStepper(maxSteps, interval);
		}

		update();
	}
}

void PolygonWidget::startNApproximateQuery(int interval, bool stepMode, double epsilon) {
	const int maxSteps = 6;
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

void PolygonWidget::mousePressEvent(QMouseEvent* event)
{
	// Convert the click point to the Cartesian plane system
	QPointF clickPoint = QPointF(event->pos().x() - width() / 2, height() / 2 - event->pos().y());

	if (drawOwnPolygon) {
		clicks.append(clickPoint);
		newClickPoint = true;
		return;
	}

	if (m_queryMode == EXACT) {
		if (!startSelected) {
			setStartingPoint(clickPoint);
			std::cout << "setStartingPoint(QPointF(" << startingPoint.x() << ", " << startingPoint.y() << ")); \n";
		}
		else if (!query1Selected) {
			setQueryPoint1(clickPoint);
			std::cout << "setQueryPoint1(QPointF(" << queryPoint1.x() << ", " << queryPoint1.y() << ")); \n";
		}
		else if (!query2Selected) {
			setQueryPoint2(clickPoint);
			std::cout << "setQueryPoint2(QPointF(" << queryPoint2.x() << ", " << queryPoint2.y() << ")); \n";
		}
	}
	else if (m_queryMode == APPROX) {
		if (!startSelected) {
			setStartingPoint(clickPoint);
			std::cout << "setStartingPoint(QPointF(" << startingPoint.x() << ", " << startingPoint.y() << ")); \n";
		}
		else {
			approxQueryPoints.append(clickPoint);
			if (approxQueryPoints.size() == 2) {
				std::cout << "setQueryPoint2(QPointF(" << approxQueryPoints[1].x() << ", " << approxQueryPoints[1].y() << ")); \n";
			}
			else if (approxQueryPoints.size() == 1) {
				std::cout << "setQueryPoint1(QPointF(" << approxQueryPoints[0].x() << ", " << approxQueryPoints[0].y() << ")); \n";
			}
		}
	}

	// After a point is selected, trigger a repaint to show it visually
	update();
}

////////////////////////////////////////////////////////////////

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

void PolygonWidget::drawLabel(double x, double y, QString label, QPainter& painter)
{
	const int RADIUS = 3;
	// painter.restore();
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
}

void PolygonWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing, true); // Smooth edges

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

	pen.setBrush(Qt::black);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);

	//fixedPoints = true;
	if (fixedPoints) {
		//startingPoint = QPointF(47, -119);
		//queryPoint1 = QPointF(275, -98);

		// one point query test
		//startingPoint = QPointF(-218, 88);
		//queryPoint1 = QPointF(307, -80);

		// one point query not working
		/*
		startingPoint = QPointF(-130, 166);
		queryPoint1 = QPointF(-184, -121);
		*/





		//setFixedPoints(startingPoint = QPointF(205, 169), queryPoint1 = QPointF(-105, -5), queryPoint2 = QPointF(-157, 108));
	}



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

	int size = 0;
	if (m_queryMode == APPROX) {
		for (QPointF point : approxQueryPoints) {
			painter.drawEllipse(point, 3, 3);
			drawLabel(point.x(), point.y(), QString("q"), painter);
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
				if (resultQ2.currentCase == TwoPointQuery::Q2CASE::QNONE)
				{
					resultQ1 = resultQ2.resultQ1;
					visualizeOnePointQuery(painter);
				}
				visualizeTwoPointQuery(painter);
			}
			break;

		case APPROX:
			//visualizeApprox(painter);
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
			errorMessage = "The Query Point is Visible from the Starting Point";
			drawLabel(startingPoint.x() + 5, startingPoint.y(), QString(" = c"), painter);
			
			return;
		}
	}

	// Draw the Shortest Path
	if (step >= 2) {
		painter.setPen(QPen(Qt::red, 1));
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
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
		painter.drawEllipse(a, 3, 3);
		drawLabel(a.x(), a.y(), QString("a"), painter);
	}

	// Draw Point B and Window
	if (step >= 4) {
		QPointF b = window.p2();
		painter.drawEllipse(b, 3, 3);
		drawLabel(b.x(), b.y(), QString("b"), painter);
		painter.setPen(Qt::green);
		painter.setBrush(Qt::green);

		painter.drawLine(window);
	}

	// Draw LCA
	if (step >= 5) {
		QPointF lca = resultQ1.lca;
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
		painter.drawEllipse(lca, 3, 3);
		if (lca == a || lca == b || lca == startingPoint) {
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
			painter.setPen(QPen(Qt::blue, 2));
			painter.drawLine(pathRA[i - 1], pathRA[i]);
			//painter.setPen(Qt::gray);
			//painter.drawEllipse(pathRA[i - 1], 5, 5);
		}

		QVector<QPointF> pathRB = resultQ1.pathRootToB;
		for (size_t i = 1; i < pathRB.size(); ++i)
		{
			painter.setPen(QPen(Qt::blue, 2));
			painter.drawLine(pathRB[i - 1], pathRB[i]);
			//painter.setPen(Qt::green);
			//painter.drawEllipse(pathRB[i - 1], 5, 5);
		}
	}

	// Draw the Optimal Point C
	if (step >= 7) {
		QPointF c = resultQ1.optimalPoint;
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
		painter.drawEllipse(c, 3, 3);
		if (c == a || c == b) {
			drawLabel(c.x() + 5, c.y(), QString(" = c"), painter);
		}
		else {
			drawLabel(c.x(), c.y(), QString("c"), painter);
		}
	}

	// Draw the Optimal Path from Start to C
	if (step == 8) {
		QVector<QPointF> optimalPath = resultQ1.optimalPath;
		painter.setPen(QPen(Qt::magenta, 2));
		for (size_t i = 1; i < optimalPath.size(); ++i)
		{
			painter.drawLine(optimalPath[i - 1], optimalPath[i]);
		}
	}
}

void PolygonWidget::visualizeTwoPointQuery(QPainter& painter) {
	// Visibility Check
	if (step >= 1) {
		if (resultQ2.visibilityQ1 && resultQ2.visibilityQ2) {
			errorMessage = "Both Query Points arre Visible from the Starting Point";
			drawLabel(startingPoint.x() + 1, startingPoint.y() - 1, QString("c"), painter);
			return;
		}
		// TODO: This does not work
		else if (resultQ2.visibilityQ1 || resultQ2.visibilityQ2) {
			resultQ1 = resultQ2.resultQ1;
			visualizeOnePointQuery(painter);
		}
	}

	QPen thinPen = QPen((Qt::blue));

	// Draw Window 1
	if (step >= 2) {
		a1 = resultQ2.window1.p1();
		b1 = resultQ2.window1.p2();
		painter.drawEllipse(a1, 3, 3);
		drawLabel(a1.x(), a1.y(), QString("a1"), painter);

		painter.drawEllipse(b1, 3, 3);
		drawLabel(b1.x(), b1.y(), QString("b1"), painter);
		painter.setPen(Qt::green);
		painter.setBrush(Qt::green);

		painter.drawLine(a1, b1);
	}

	// Draw Window 2
	if (step >= 3) {
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
		a2 = resultQ2.window2.p1();
		b2 = resultQ2.window2.p2();
		painter.drawEllipse(a2, 3, 3);
		drawLabel(a2.x(), a2.y(), QString("a2"), painter);

		painter.drawEllipse(b2, 3, 3);
		drawLabel(b2.x(), b2.y(), QString("b2"), painter);
		painter.setPen(Qt::darkGreen);
		painter.setBrush(Qt::darkGreen);

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
	painter.setPen(Qt::blue);
	QVector<QPointF> intersectionPath1 = intersectionResult.intersectionPath1;
	QVector<QPointF> intersectionPath2 = intersectionResult.intersectionPath2;
	QVector<QPointF> intersectionPath3 = intersectionResult.intersectionPath3;
	QVector<QPointF> optimalPath = intersectionResult.optimalPath;

	if (step >= 4) {
		for (size_t i = 1; i < intersectionPath1.size(); ++i)
		{
			painter.drawLine(intersectionPath1[i - 1], intersectionPath1[i]);
		}
	}

	if (step >= 5) {
		painter.setPen(Qt::red);
		for (size_t i = 1; i < intersectionPath2.size(); ++i)
		{
			painter.drawLine(intersectionPath2[i - 1], intersectionPath2[i]);
		}
	}

	if (step >= 6) {
		painter.setPen(Qt::yellow);
		for (size_t i = 1; i < intersectionPath3.size(); ++i)
		{
			painter.drawLine(intersectionPath3[i - 1], intersectionPath3[i]);
		}
	}

	if (step == 7) {
		painter.setPen(QPen(Qt::magenta, 2));
		for (size_t i = 1; i < optimalPath.size(); ++i)
		{
			painter.drawLine(optimalPath[i - 1], optimalPath[i]);
		}
	}

	visualizeGeneralCase(painter);
}

void PolygonWidget::visualizeDomination(QPainter& painter) {
	TwoPointQuery::DominationResult dominationResult = m_twoPointHandler.getDominationResult();
	QVector<QPointF> optimalPath = dominationResult.optimalPath;

	if (step >= 4) {
		for (size_t i = 1; i < optimalPath.size(); ++i)
		{
			painter.drawLine(optimalPath[i - 1], optimalPath[i]);
		}
	}

	QPointF c = optimalPath.rbegin()[0];

	if (step == 5) {
		painter.drawEllipse(c, 3, 3);
		if (c == a2 || c == b2 || c == a1 || c == b1) {
			drawLabel(c.x() + 5, c.y(), QString(" = c"), painter);
		}
		else {
			drawLabel(c.x(), c.y(), QString("c"), painter);
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

	QPen thinPen = QPen((Qt::blue));


	if (step >= 4) {
		painter.setPen(QPen(Qt::darkRed, 1));
		painter.drawEllipse(generalResult.funnelRoot, 3, 3);

		for (size_t i = 1; i < funnelSideA.size(); ++i)
		{
			painter.drawLine(funnelSideA[i - 1], funnelSideA[i]);
			painter.drawEllipse(funnelSideA[i - 1], 3, 3);
		}


		for (size_t i = 1; i < funnelSideB.size(); ++i)
		{
			painter.drawLine(funnelSideB[i - 1], funnelSideB[i]);
			painter.drawEllipse(funnelSideB[i - 1], 3, 3);
		}
	}

	if (step >= 5) {
		painter.setPen(QPen(Qt::blue, 2));

		for (size_t i = 1; i < hourglassSide1.size(); ++i)
		{
			painter.setPen(QPen(Qt::blue, 2));
			painter.drawLine(hourglassSide1[i - 1], hourglassSide1[i]);
			painter.setPen(QPen(Qt::yellow, 2));
			painter.drawLine(m_generalCaseHandler.mirrorPoint(hourglassSide1[i - 1], QLineF(a2, b2)), m_generalCaseHandler.mirrorPoint(hourglassSide1[i], QLineF(a2, b2)));
			painter.drawLine(m_generalCaseHandler.mirrorPoint(hourglassSide1[i - 1], QLineF(a1, b1)), m_generalCaseHandler.mirrorPoint(hourglassSide1[i], QLineF(a1, b1)));
			painter.drawEllipse(m_generalCaseHandler.mirrorPoint(hourglassSide1[i - 1], QLineF(a2, b2)), 3, 3);
		}

		for (size_t i = 1; i < hourglassSide2.size(); ++i)
		{
			painter.setPen(QPen(Qt::blue, 2));
			painter.drawLine(hourglassSide2[i - 1], hourglassSide2[i]);
			painter.setPen(QPen(Qt::red, 2));
			painter.drawLine(m_generalCaseHandler.mirrorPoint(hourglassSide2[i - 1], QLineF(a2, b2)), m_generalCaseHandler.mirrorPoint(hourglassSide2[i], QLineF(a2, b2)));
			painter.drawLine(m_generalCaseHandler.mirrorPoint(hourglassSide2[i - 1], QLineF(a1, b1)), m_generalCaseHandler.mirrorPoint(hourglassSide2[i], QLineF(a1, b1)));
			painter.drawEllipse(m_generalCaseHandler.mirrorPoint(hourglassSide2[i - 1], QLineF(a1, b2)), 3, 3);
		}
	}

	if (step >= 6) {
		QVector<QPointF> tangent1 = generalResult.tangent1;
		QVector<QPointF> tangent2 = generalResult.tangent2;
		QVector<QPointF> tangent3 = generalResult.tangent3;
		QVector<QPointF> tangent4 = generalResult.tangent4;
		painter.setPen(QPen(Qt::green, 2));

		for (size_t i = 1; i < tangent1.size(); ++i)
		{
			painter.drawLine(tangent1[i - 1], tangent1[i]);
		}

		painter.setPen(QPen(Qt::gray, 2));

		for (size_t i = 1; i < tangent2.size(); ++i)
		{
			painter.drawLine(tangent2[i - 1], tangent2[i]);
		}

		painter.setPen(QPen(Qt::darkCyan, 2));

		for (size_t i = 1; i < tangent3.size(); ++i)
		{
			painter.drawLine(tangent3[i - 1], tangent3[i]);
		}

		painter.setPen(QPen(Qt::cyan, 2));

		for (size_t i = 1; i < tangent4.size(); ++i)
		{
			painter.drawLine(tangent4[i - 1], tangent4[i]);
		}
	}

	if (step >= 7) {
		for (size_t i = 1; i < concatenatedSide1.size(); ++i)
		{
			painter.setPen(QPen(Qt::darkMagenta, 2));
			painter.drawLine(concatenatedSide1[i - 1], concatenatedSide1[i]);
		}

		for (size_t i = 1; i < concatenatedSide2.size(); ++i)
		{
			painter.setPen(QPen(Qt::magenta, 2));
			painter.drawLine(concatenatedSide2[i - 1], concatenatedSide2[i]);
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
		for (size_t i = 1; i < optimalPath.size(); ++i)
		{
			painter.setPen(QPen(Qt::darkYellow, 2));
			painter.drawLine(optimalPath[i - 1], optimalPath[i]);
		}
	}

	if (step == 9) {
		painter.drawEllipse(optimalPoint, 3, 3);

		if (optimalPoint == a2 || optimalPoint == b2) {
			drawLabel(optimalPoint.x() + 5, optimalPoint.y(), QString(" = c"), painter);
		}
		else {
			drawLabel(optimalPoint.x(), optimalPoint.y(), QString("c"), painter);
		}
	}

	painter.setPen(QPen(Qt::green, 2));
	painter.drawLine(QPointF(-110.59, 149.287), QPointF(-145.384, 111.7));
	painter.drawLine(QPointF(-145.384, 111.7), QPointF(-268.896, -61.0969));
	QPointF(-110.59, 149.287);
	QPointF(-145.384, 111.7);
	QPointF(-268.896, -61.0969);
	QPointF tangentPoint1 = QPointF(-110.59, 149.287);
	QPointF tangentPoint2 = QPointF(-145.384, 111.7);
	painter.setPen(QPen(Qt::magenta, 2));
	painter.drawLine(tangentPoint1, tangentPoint2);
}


void PolygonWidget::visualizeApprox(QPainter& painter) {
	resultApprox = m_approximateHandler.getApproximateResult();

	QVector<QPointF> threeApproxPath = resultApprox.threeApproxPath;
	if (step >= 1) {
		painter.setPen(QPen(Qt::yellow, 2));
		for (size_t i = 1; i < threeApproxPath.size(); ++i)
		{
			painter.drawLine(threeApproxPath[i - 1], threeApproxPath[i]);
		}
	}

	QLineF window1 = resultApprox.window1;
	QLineF window2 = resultApprox.window2;
	if (step >= 2) {
		painter.setPen(QPen(Qt::green));
		painter.drawLine(window1);
		painter.drawLine(window2);
	}

	double radius = resultApprox.discRadius;
	if (step >= 3) {
		painter.setBrush(Qt::NoBrush); // No filling for the ellipse
		painter.drawEllipse(startingPoint, radius, radius);
	}

	QLineF intersectionWindow1 = resultApprox.intersectionWindow1;
	QLineF intersectionWindow2 = resultApprox.intersectionWindow2;
	if (step >= 4) {
		painter.setPen(QPen(Qt::darkGreen));
		painter.drawLine(intersectionWindow1);
		painter.drawLine(intersectionWindow2);
	}

	QVector<QPointF> spacedPoints1 = resultApprox.equallySpacedPoints1;
	QVector<QPointF> spacedPoints2 = resultApprox.equallySpacedPoints2;
	if (step >= 5) {
		painter.setPen(QPen(Qt::magenta));
		for (size_t i = 0; i < spacedPoints1.size(); ++i)
		{
			painter.drawEllipse(spacedPoints1[i], 1, 1);
		}
		for (size_t i = 0; i < spacedPoints2.size(); ++i)
		{
			painter.drawEllipse(spacedPoints2[i], 1, 1);
		}
	}

	if (step == 6) {
		QVector<QPointF> shortestPath = resultApprox.shortestPath;
		painter.setPen(QPen(Qt::darkRed, 2));
		for (size_t i = 1; i < shortestPath.size(); ++i)
		{
			painter.drawLine(shortestPath[i - 1], shortestPath[i]);
		}
	}
}


void PolygonWidget::visualizeNApprox(QPainter& painter) {
	resultNApprox = m_approximateHandler.getNApproximateResult();

	QVector<QPointF> nApproxPath = resultNApprox.nApproxPath;
	if (step >= 1) {
		painter.setPen(QPen(Qt::yellow, 2));
		for (size_t i = 1; i < nApproxPath.size(); ++i)
		{
			painter.drawLine(nApproxPath[i - 1], nApproxPath[i]);
		}
	}

	if (step >= 2) {
		painter.setPen(QPen(Qt::green));
		for (const QLineF window : resultNApprox.windows) {
			painter.drawLine(window);
		}
	}

	double radius = resultNApprox.discRadius;
	if (step >= 3) {
		painter.setBrush(Qt::NoBrush); // No filling for the ellipse
		painter.drawEllipse(startingPoint, radius, radius);
	}

	if (step >= 4) {
		painter.setPen(QPen(Qt::darkGreen));
		for (const QLineF intersectionWindow : resultNApprox.intersectionWindows) {
			painter.drawLine(intersectionWindow);
		}
	}

	if (step >= 5) {
		painter.setPen(QPen(Qt::magenta));

		for (size_t i = 0; i < resultNApprox.equallySpacedPointsGroup.size(); ++i) {
			for (size_t j = 0; j < resultNApprox.equallySpacedPointsGroup[i].size(); ++j) {
				painter.drawEllipse(resultNApprox.equallySpacedPointsGroup[i][j], 1, 1);
			}
		}
	}

	if (step == 6) {
		QVector<QPointF> shortestPath = resultNApprox.shortestPath;
		painter.setPen(QPen(Qt::darkRed, 2));
		for (size_t i = 1; i < shortestPath.size(); ++i)
		{
			painter.drawLine(shortestPath[i - 1], shortestPath[i]);
		}
	}
}