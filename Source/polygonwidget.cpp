#include "polygonwidget.h"

PolygonWidget::PolygonWidget(QWidget* parent) : QWidget(parent)
{
	polygonMode = 0;
	step = 0;
	hideQuery = false;
}

void PolygonWidget::constructPolygon(int size)
{
	switch (polygonMode)
	{
	case 0:
		clearCanvas();
		polygonC = m_polygonGenHandler.generateRandomPolygon(size);
		// Convert CGAL polygon vertices to Qt points
		for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
		{
			//std::cout << "p.push_back(Point_2(" << it->x() << "," << it->y() << "));" << "\n";
			polygonQ.append(QPointF(it->x(), it->y()));
		}
		//std::cout << "" << "\n";

		update();
		break;

	case 1:
		clicks.clear();
		clearCanvas();
		break;

	case 2:
		clearCanvas();
		polygonC = m_polygonGenHandler.exampleOne();
		for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
		{
			polygonQ.append(QPointF(it->x(), it->y()));
		}
		update();
		break;

	case 3:
		polygonC.clear();
		for (QPointF vertex : polygonQ)
		{
			polygonC.push_back(Point_2(vertex.x(), vertex.y()));
		}

		if (!polygonC.is_simple()) {
			clearCanvas();
			setPolygonMode(1);
		}
		update();
		break;
	}
}

void PolygonWidget::setPolygonMode(int index) {
	polygonMode = index;
	constructPolygon(40);
}

void PolygonWidget::drawGivenPolygon(int index)
{
	clearCanvas();

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
	default:
		polygonC = m_polygonGenHandler.generateRandomPolygon(40);
		break;
	}

	for (auto it = polygonC.vertices_begin(); it != polygonC.vertices_end(); ++it)
	{
		polygonQ.append(QPointF(it->x(), it->y()));
	}

	update();
}

void PolygonWidget::clearCanvas() {
	polygonC.clear();
	polygonQ.clear();
	m_onePointHandler.clearTree();
	m_shortestPathHandler.clearTree();
	clearPoints();
	step = 0;
	hideQuery = true;
	errorMessage = "";
	m_onePointHandler.resetLog();
	update();
}

QVector<QPointF> PolygonWidget::convertToQT(std::vector<Point_3> points)
{
	QVector<QPointF> qtPoints;
	qtPoints.reserve(points.size());

	for (const auto& point : points)
	{
		qtPoints.append(QPointF(point.x(), point.y()));
	}

	return qtPoints;
}

void PolygonWidget::setMode(int mode)
{
	// Set the mode
	m_mode = mode;
	clearPoints();
	update(); // Force a repaint to update the GUI
}

void PolygonWidget::setStep(int step)
{
	// Set the mode
	m_step = step;
	update(); // Force a repaint to update the GUI
}

void PolygonWidget::clearPoints()
{
	switch (m_mode)
	{
	case 1:
		m_onePointHandler.clearPoints();
		break;

	case 2:
		m_twoPointHandler.clearPoints();
		break;

	default:
		break;
	}
}

void PolygonWidget::mousePressEvent(QMouseEvent* event)
{
	// Convert the click point to the Cartesian plane system
	QPointF clickPoint = QPointF(event->pos().x() - width() / 2, height() / 2 - event->pos().y());

	if (polygonMode == 1) {
		clicks.append(clickPoint);
		update();
		return;
	}

	// Based on the mode, decide what to do with the point selection
	switch (m_mode)
	{
	case 1:
		if (!m_onePointHandler.isStartingPointSet())
		{
			m_onePointHandler.setStartingPoint(clickPoint);
			startingPoint = m_onePointHandler.getStartingPoint();
			std::cout << "QPointF(" << startingPoint.x() << "," << startingPoint.y() << ")" << "\n";
		}
		else if (!m_onePointHandler.isQueryPointSet())
		{
			m_onePointHandler.setQueryPoint(clickPoint);
			queryPoint1 = m_onePointHandler.getQueryPoint();
			std::cout << "QPointF(" << queryPoint1.x() << "," << queryPoint1.y() << ")" << "\n";
		}

		break;

	case 2:
		if (!m_twoPointHandler.isStartingPointSet())
		{
			m_twoPointHandler.setStartingPoint(clickPoint);
			startingPoint = m_twoPointHandler.getStartingPoint();
		}
		else if (!m_twoPointHandler.isQueryPoint1Set())
		{
			m_twoPointHandler.setQueryPoint1(clickPoint);
			queryPoint1 = m_twoPointHandler.getQueryPoint1();
		}
		else if (!m_twoPointHandler.isQueryPoint2Set())
		{
			m_twoPointHandler.setQueryPoint2(clickPoint);
			queryPoint2 = m_twoPointHandler.getQueryPoint2();
		}

		break;

	default:
		break;
	}

	// After a point is selected, trigger a repaint to show it visually
	update();
}

void PolygonWidget::startStepperQ1()
{
	pointCheck();
	valid = true;
	if (valid)
	{
		stepmode = true;
		step = 1;
		onePointQuery(queryPoint1);
		update();
	}
}

void PolygonWidget::startAutoQ1(int interval)
{
	pointCheck();
	if (valid)
	{
		stepmode = false;
		onePointQuery(queryPoint1);
		update();
	}
}

void PolygonWidget::startAutoQ2(int interval)
{
	pointCheck();
	if (valid)
	{
		stepmode = false;
		twoPointQuery();
		update();
	}
}

void PolygonWidget::pointCheck()
{
	valid = false;

	switch (m_mode)
	{
	case 1:
		if (!m_onePointHandler.isStartingPointSet())
		{
			errorMessage = "Starting Point needs to be set";
			return;
		}
		else if (!m_onePointHandler.isQueryPointSet())
		{
			errorMessage = "Query Point needs to be set";
			return;
		}

		break;

	case 2:
		if (!m_twoPointHandler.isStartingPointSet())
		{
			errorMessage = "Starting Point needs to be set";
			return;
		}
		else if (!m_twoPointHandler.isQueryPoint1Set())
		{
			errorMessage = "Query Point 1 needs to be set";
			return;
		}
		else if (!m_twoPointHandler.isQueryPoint2Set())
		{
			errorMessage = "Query Point 2 needs to be set";
			return;
		}

		if (!polygonC.has_on_bounded_side(Point_2(queryPoint2.x(), queryPoint2.y())))
		{
			errorMessage = "Query Point 2 needs to be Inside of the Polygon";
			clearPoints();
			update();
			return;
		}

		break;

	default:
		break;
	}

	if (!polygonC.has_on_bounded_side(Point_2(startingPoint.x(), startingPoint.y())))
	{
		errorMessage = "Starting Point needs to be Inside of the Polygon";
		clearPoints();
		update();
		return;
	}
	else if (!polygonC.has_on_bounded_side(Point_2(queryPoint1.x(), queryPoint1.y())))
	{
		errorMessage = "Query Point 1 needs to be Inside of the Polygon";
		clearPoints();
		update();
		return;
	}

	// no error
	errorMessage = "";
	valid = true;
}

QString PolygonWidget::updateLog()
{
	return errorMessage + m_onePointHandler.getLog();
}

/* void PolygonWidget::startAutoQ1(int interval)
{
	step = 1;
	stepmode = true;
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &PolygonWidget::startSingleQuery);
	timer->start(interval);
}

void PolygonWidget::startSingleQuery()
{
	std::vector<Point_3> pathCRA;
	std::vector<Point_3> pathCRB;

	switch (step)
	{
	case 1:
		std::cout << "1";
		visibility = m_onePointHandler.checkVisibilty(startingPoint, queryPoint1, polygonC);
		if (visibility)
		{
			return;
		}

		m_shortestPathHandler.createMesh(polygonC);
		break;
	case 2:
		std::cout << "2";
		shortestPathSQ = convertToQT(m_shortestPathHandler.findShortestPath(startingPoint, queryPoint1, polygonC));
		break;
	case 3:
		setPointA();
		m_onePointHandler.shootRayExtended(queryPoint1, a, polygonC);
		break;
	case 4:
		setPointB();
		break;
	case 5:
		lcaC = m_shortestPathHandler.getLCA(startingPoint, a, b, polygonC);
		lca = QPointF(lcaC.x(), lcaC.y());
		break;
	case 6:
		pathCRA = m_shortestPathHandler.findShortestPath(lca, a, polygonC);
		pathCRB = m_shortestPathHandler.findShortestPath(lca, b, polygonC);
		pathRA = convertToQT(pathCRA);
		pathRB = convertToQT(pathCRB);
		break;
	case 7:
		computeOptimalPoint();
		break;
	}
	update();
	step++;
} */

void PolygonWidget::onePointQuery(QPointF queryPoint)
{
	hideQuery = false;
	visibilitySQ = m_onePointHandler.checkVisibilty(startingPoint, queryPoint, polygonC);
	if (visibilitySQ)
	{
		return;
	}

	m_shortestPathHandler.createMesh(polygonC);

	// Find shortest path from s to q
	std::vector shortestPathSQC = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint, polygonC);
	shortestPathSQ = convertToQT(shortestPathSQC);

	// calculate the window
	QLineF window = calculateWindow(shortestPathSQC, queryPoint);
	a = window.p1();
	b = window.p2();

	lcaC = m_shortestPathHandler.getLCA(startingPoint, a, b, polygonC);
	lca = QPointF(lcaC.x(), lcaC.y());

	std::vector<Point_3> pathCRA = m_shortestPathHandler.findShortestPath(lca, a, polygonC);
	std::vector<Point_3> pathCRB = m_shortestPathHandler.findShortestPath(b, lca, polygonC); // makes the calculations more consistent
	pathCRB = m_shortestPathHandler.reversePath(pathCRB); // need to reverse to get the path from b to lca
	pathRA = convertToQT(pathCRA);
	pathRB = convertToQT(pathCRB);

	c = m_onePointHandler.computeOptimalPoint(pathRA, pathRB, window);
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

void PolygonWidget::twoPointQuery()
{
	hideQuery = false;
	// visibility check
	visibilitySQ1 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint1, polygonC);
	visibilitySQ2 = m_onePointHandler.checkVisibilty(startingPoint, queryPoint2, polygonC);
	if (visibilitySQ1 && visibilitySQ2)
	{
		errorMessage = "Both Query Points are Visible from the Starting Point";
		return;
	}
	else if (visibilitySQ1)
	{
		errorMessage = "Q1 is visible from the Starting Point";
		onePointQuery(queryPoint2);
		return;
	}
	else if (visibilitySQ2)
	{
		onePointQuery(queryPoint1);
		return;
	}

	m_shortestPathHandler.createMesh(polygonC);

	// AABB_tree polygonTree = m_onePointHandler.constructTree(polygonC);
	std::vector shortestPathSQ1C = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint1, polygonC);
	std::vector shortestPathSQ2C = m_shortestPathHandler.findShortestPath(startingPoint, queryPoint2, polygonC);

	shortestPathSQ1 = convertToQT(shortestPathSQ1C);
	shortestPathSQ2 = convertToQT(shortestPathSQ2C);

	QLineF window1 = calculateWindow(shortestPathSQ1C, queryPoint1);
	a1 = window1.p1();
	b1 = window1.p2();
	QLineF window2 = calculateWindow(shortestPathSQ2C, queryPoint2);
	a2 = window2.p1();
	b2 = window2.p2();

	QPointF intersectionPoint;
	QLineF::IntersectionType intersection = window1.intersects(window2, &intersectionPoint);

	if (intersection == QLineF::BoundedIntersection)
	{
		// Check if the intersection point is not a shared endpoint
		if (intersectionPoint != window1.p1() && intersectionPoint != window1.p2() &&
			intersectionPoint != window2.p1() && intersectionPoint != window2.p2())
		{
			std::cout << "intersect";
			errorMessage = "The Windows Intersect eachother";
			QPointF visibleEndpoint1;
			QPointF visibleEndpoint2;

			if (m_onePointHandler.checkVisibilty(a1, queryPoint2, polygonC))
			{
				visibleEndpoint1 = a1;
			}
			else
			{
				visibleEndpoint1 = b1;
			}

			if (m_onePointHandler.checkVisibilty(a2, queryPoint1, polygonC))
			{
				visibleEndpoint2 = a2;
			}
			else
			{
				visibleEndpoint2 = b2;
			}
			
			QVector<QPointF> path1 = m_twoPointHandler.shortestPathToSegment(startingPoint, QLineF(visibleEndpoint1, intersectionPoint), polygonC);
			double sizePath1 = calculatePathLength(path1);
			QVector<QPointF> path2 = m_twoPointHandler.shortestPathToSegment(startingPoint, QLineF(visibleEndpoint2, intersectionPoint), polygonC);
			double sizePath2 = calculatePathLength(path2);
			std::cout << "Size of Path 1: " << sizePath1 << "\n";
			std::cout << "Size of Path 2: " << sizePath2 << "\n";
		}
	}

	QVector<QPointF> shortestPathSA1 = convertToQT(m_shortestPathHandler.findShortestPath(startingPoint, a1, polygonC));
	QVector<QPointF> shortestPathSB1 = convertToQT(m_shortestPathHandler.findShortestPath(b1, startingPoint, polygonC));
	QVector<QPointF> shortestPathSA2 = convertToQT(m_shortestPathHandler.findShortestPath(startingPoint, a2, polygonC));
	QVector<QPointF> shortestPathSB2 = convertToQT(m_shortestPathHandler.findShortestPath(b2, startingPoint, polygonC));

	if (dominateWindowCheck(window2, shortestPathSA1) && dominateWindowCheck(window2, shortestPathSB1))
	{
		errorMessage = "Window1 lies behind Window2";
	}
	else if ((dominateWindowCheck(window1, shortestPathSA2) && dominateWindowCheck(window1, shortestPathSB2)))
	{
		errorMessage = "Window2 lies behind Window1";
	}
}

bool PolygonWidget::dominateWindowCheck(QLineF window, QVector<QPointF> shortestPath)
{
	for (size_t i = 0; i < shortestPath.size() - 1; ++i) {
		QLineF segment(shortestPath[i], shortestPath[i + 1]);
		QLineF::IntersectionType intersection = segment.intersects(window, nullptr);
		if (intersection == QLineF::BoundedIntersection)
		{
			return true; // Intersection found
		}
	}
	return false;
}

double PolygonWidget::calculatePathLength(const QVector<QPointF>& path) {
	double length = 0.0;
	for (int i = 1; i < path.size(); ++i) {
		// Distance between consecutive points
		double dx = path[i].x() - path[i - 1].x();
		double dy = path[i].y() - path[i - 1].y();
		length += std::sqrt(dx * dx + dy * dy);
	}
	return length;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PolygonWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// Translate and scale to center the view
	painter.translate(width() / 2, height() / 2);
	painter.scale(1, -1); // Flip y-axis for Cartesian coordinates


	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	QPointF tempClick;
	if (polygonMode == 1)
	{
		if (!clicks.isEmpty() && clicks.last() != tempClick) {
			painter.drawEllipse(clicks[0], 3, 3);
			for (qsizetype i = 1; i < clicks.size(); i++) {
				painter.drawEllipse(clicks[i], 3, 3);
				painter.drawLine(clicks[i - 1], clicks[i]);
			}
			tempClick = clicks.last();
			std::cout << "p.push_back(Point_2(" << tempClick.x() << "," << tempClick.y() << "));" << "\n";
			polygonQ.append(tempClick);
		}
		return;
	}
	/*else if (polygonMode == 2)
	{
		startingPoint = QPointF(40, -102);
		queryPoint1 = QPointF(255, -105);
		update();
	}*/

	if (polygonQ.size() > 2)
	{
		painter.setPen(Qt::black);
		painter.setBrush(Qt::white);
		painter.drawPolygon(polygonQ);
	}

	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);

	switch (m_mode)
	{
	case 1:
		if (m_onePointHandler.isStartingPointSet())
		{
			painter.drawEllipse(startingPoint, 3, 3);
			drawLabel(startingPoint.x(), startingPoint.y(), QString("s"), painter);
		}

		if (m_onePointHandler.isQueryPointSet())
		{
			painter.drawEllipse(queryPoint1, 3, 3);
			drawLabel(queryPoint1.x(), queryPoint1.y(), QString("q"), painter);
		}

		if (hideQuery) {
			return;
		}

		if (!stepmode)
		{
			visualizeAuto(painter);
		}
		else
		{
			visualizeStep(painter);
		}

		break;

	case 2:
		if (m_twoPointHandler.isStartingPointSet())
		{
			painter.drawEllipse(startingPoint, 3, 3);
			drawLabel(startingPoint.x(), startingPoint.y(), QString("s"), painter);
		}
		if (m_twoPointHandler.isQueryPoint1Set())
		{
			painter.drawEllipse(queryPoint1, 3, 3);
			drawLabel(queryPoint1.x(), queryPoint1.y(), QString("q1"), painter);
		}
		if (m_twoPointHandler.isQueryPoint2Set())
		{
			painter.drawEllipse(queryPoint2, 3, 3);
			drawLabel(queryPoint2.x(), queryPoint2.y(), QString("q2"), painter);
		}

		if (hideQuery) {
			return;
		}

		if ((visibilitySQ1 && !visibilitySQ2) || (!visibilitySQ1 && visibilitySQ2))
		{
			if (!stepmode)
			{
				visualizeAuto(painter);
			}
			else
			{
				visualizeStep(painter);
			}
		}

		if (!stepmode)
		{
			visualizeAuto2(painter);
		}
		else
		{
			//visualizeStep(painter);
		}

		break;

	default:
		break;
	}
}

QLineF PolygonWidget::calculateWindow(std::vector<Point_3>& path, QPointF queryPoint)
{
	const Point_2 aC = m_shortestPathHandler.getPenultimate(path, polygonC);
	QPointF a = QPointF(aC.x(), aC.y());

	m_onePointHandler.shootRayExtended(queryPoint, a, polygonC);

	const Point_2 bC = m_onePointHandler.getIntersection();
	QPointF b = QPointF(bC.x(), bC.y());

	return QLineF(a, b);
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

void PolygonWidget::visualizeAuto(QPainter& painter)
{
	if (visibilitySQ)
	{
		errorMessage = "The Query Point is Visible from the Starting Point";
		drawLabel(startingPoint.x() + 1, startingPoint.y() - 1, QString("c"), painter);
		return;
	}

	// Draw the polygon and its Delaunay triangulation
	painter.setPen(Qt::darkGray);
	for (auto edge : m_shortestPathHandler.getMesh().edges())
	{
		auto source = m_shortestPathHandler.getMesh().point(m_shortestPathHandler.getMesh().vertex(edge, 0));
		auto target = m_shortestPathHandler.getMesh().point(m_shortestPathHandler.getMesh().vertex(edge, 1));

		painter.drawLine(QPointF(source.x(), source.y()), QPointF(target.x(), target.y()));
	}

	// draw shortest path
	painter.setPen(QPen(Qt::red, 2));
	for (size_t i = 1; i < shortestPathSQ.size(); ++i)
	{
		painter.drawLine(shortestPathSQ[i - 1], shortestPathSQ[i]);
	}

	// draw point a
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	painter.drawEllipse(a, 3, 3);
	drawLabel(a.x(), a.y(), QString("a"), painter);

	// draw point b and window
	painter.drawEllipse(b, 3, 3);
	drawLabel(b.x(), b.y(), QString("b"), painter);
	painter.setPen(Qt::green);
	painter.setBrush(Qt::green);

	painter.drawLine(a, b);

	// draw LCA
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	painter.drawEllipse(lca, 3, 3);
	if (lca == a || lca == b || lca == startingPoint) {
		drawLabel(lca.x() + 5, lca.y(), QString(" = r"), painter);
	}
	else {
		drawLabel(lca.x(), lca.y(), QString("r"), painter);
	}



	for (size_t i = 1; i < pathRA.size(); ++i)
	{
		painter.setPen(QPen(Qt::blue, 2));
		painter.drawLine(pathRA[i - 1], pathRA[i]);
		//painter.setPen(Qt::gray);
		//painter.drawEllipse(pathRA[i - 1], 5, 5);
	}

	for (size_t i = 1; i < pathRB.size(); ++i)
	{
		painter.setPen(QPen(Qt::magenta, 2));
		painter.drawLine(pathRB[i - 1], pathRB[i]);
		//painter.setPen(Qt::green);
		//painter.drawEllipse(pathRB[i - 1], 5, 5);
	}

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

void PolygonWidget::visualizeStep(QPainter& painter)
{
	if (step == 1)
	{
		if (visibilitySQ)
		{
			errorMessage = "The Query Point is Visible from the Starting Point";
			drawLabel(startingPoint.x() + 5, startingPoint.y() - 1, QString(" = c"), painter);
			return;
		}
	}

	if (step >= 2)
	{
		// draw shortest path
		painter.setPen(QPen(Qt::red, 2));
		for (size_t i = 1; i < shortestPathSQ.size(); ++i)
		{
			painter.drawLine(shortestPathSQ[i - 1], shortestPathSQ[i]);
		}
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
	}

	if (step >= 3)
	{
		painter.drawEllipse(a, 3, 3);
		drawLabel(a.x(), a.y(), QString("a"), painter);
	}

	if (step >= 4)
	{
		painter.drawEllipse(b, 3, 3);
		drawLabel(b.x(), b.y(), QString("b"), painter);

		painter.setPen(Qt::green);
		painter.setBrush(Qt::green);
		painter.drawLine(a, b);

		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
	}

	if (step >= 5)
	{
		painter.drawEllipse(lca, 3, 3);
		if (lca == a || lca == b || lca == startingPoint) {
			drawLabel(lca.x() + 5, lca.y(), QString(" = r"), painter);
		}
		else {
			drawLabel(lca.x(), lca.y(), QString("r"), painter);
		}
	}

	if (step >= 6)
	{
		painter.setPen(QPen(Qt::blue, 2));
		for (size_t i = 1; i < pathRA.size(); ++i)
		{
			painter.drawLine(pathRA[i - 1], pathRA[i]);
		}

		painter.setPen(QPen(Qt::green, 2));
		for (size_t i = 1; i < pathRB.size(); ++i)
		{
			painter.drawLine(pathRB[i - 1], pathRB[i]);
		}
	}

	if (step == 7)
	{
		painter.setPen(Qt::red);
		painter.setBrush(Qt::red);
		painter.drawEllipse(c, 3, 3);
		if (c == a || c == b) {
			drawLabel(c.x() + 5, c.y(), QString(" = c"), painter);
		}
		else {
			drawLabel(c.x(), c.y(), QString("c"), painter);
		}
	}
}

void PolygonWidget::visualizeAuto2(QPainter& painter)
{
	// draw shortest paths
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::blue);
	for (size_t i = 1; i < shortestPathSQ1.size(); ++i)
	{
		painter.drawLine(shortestPathSQ1[i - 1], shortestPathSQ1[i]);
	}

	painter.setPen(Qt::red);
	painter.setBrush(Qt::red);
	for (size_t i = 1; i < shortestPathSQ2.size(); ++i)
	{
		painter.drawLine(shortestPathSQ2[i - 1], shortestPathSQ2[i]);
	}

	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);

	// draw window 1
	painter.drawEllipse(a1, 3, 3);
	drawLabel(a1.x(), a1.y(), QString("a1"), painter);

	painter.drawEllipse(b1, 3, 3);
	drawLabel(b1.x(), b1.y(), QString("b1"), painter);
	painter.setPen(Qt::green);
	painter.setBrush(Qt::green);

	painter.drawLine(a1, b1);

	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);

	// draw window 2
	painter.drawEllipse(a2, 3, 3);
	drawLabel(a2.x(), a2.y(), QString("a2"), painter);

	painter.drawEllipse(b2, 3, 3);
	drawLabel(b2.x(), b2.y(), QString("b2"), painter);
	painter.setPen(Qt::darkGreen);
	painter.setBrush(Qt::darkGreen);

	painter.drawLine(a2, b2);
}