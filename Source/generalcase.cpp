#include "generalcase.h"

GeneralCase::GeneralCase()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hourglass Construction
GeneralCase::HourglassStruct GeneralCase::constructHourglass(QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh) {
	GeneralCase::HourglassStruct hourglass;
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> hourglassSide1 = m_shortestPathHandler.findShortestPath(a1, a2, polygon, mesh);
	QVector<QPointF> hourglassSide2 = m_shortestPathHandler.findShortestPath(b1, b2, polygon, mesh);

	QVector<QPointF> hourglassSide1Alt = m_shortestPathHandler.findShortestPath(a1, b2, polygon, mesh);
	QVector<QPointF> hourglassSide2Alt = m_shortestPathHandler.findShortestPath(b1, a2, polygon, mesh);

	bool sideOneDegen = std::adjacent_find(hourglassSide1.begin(), hourglassSide1.end(), std::not_equal_to<>()) == hourglassSide1.end();
	bool sideTwoDegen = std::adjacent_find(hourglassSide2.begin(), hourglassSide2.end(), std::not_equal_to<>()) == hourglassSide2.end();

	if (sideOneDegen || sideTwoDegen) {
		std::cout << "one side is degen \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
		return hourglass;
	}
	else if (hourglassSide1.size() == 1 || hourglassSide2.size() == 1) {
		std::cout << "one side is degen \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
		return hourglass;
	}

	if (isHourglassOpenCheck(hourglassSide1, hourglassSide2)) {
		std::cout << "open hourglass \n";
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
		hourglass.isOpen = true;
	}
	else {
		std::cout << "closed hourglass \n";
		hourglass.isOpen = false;
		if (isHourglassOpenCheck(hourglassSide1Alt, hourglassSide2Alt)) {
			std::cout << "alt hourglass is open \n";
			hourglassSide1 = hourglassSide1Alt;
			hourglassSide2 = hourglassSide2Alt;
			hourglass.isOpen = true;
		}
		hourglass.hourglassSide1 = hourglassSide1;
		hourglass.hourglassSide2 = hourglassSide2;
	}

	return hourglass;
}

bool GeneralCase::isHourglassOpenCheck(QVector<QPointF>& hourglassSide1, QVector<QPointF>& hourglassSide2) {
	Polygon_2 polygon;
	for (QPointF p : hourglassSide1) {
		polygon.push_back(Point_2(p.x(), p.y()));
	}

	for (int i = hourglassSide2.size() - 1; i >= 0; --i) {
		polygon.push_back(Point_2(hourglassSide2[i].x(), hourglassSide2[i].y()));
	}

	if (polygon.is_simple()) {
		return true;
	}

	return false;
}

QPointF GeneralCase::findWaistPoint(const QVector<QPointF>& side1, const QVector<QPointF>& side2) {
	double minDist = std::numeric_limits<double>::max();
	Point_2 waistPoint1, waistPoint2;

	for (const QPointF& p1 : side1) {
		for (const QPointF& p2 : side2) {
			double dist = CGAL::squared_distance(Point_2(p1.x(), p1.y()), Point_2(p2.x(), p2.y()));
			if (dist < minDist) {
				minDist = dist;
				waistPoint1 = Point_2(p1.x(), p1.y());
				waistPoint2 = Point_2(p2.x(), p2.y());
			}
		}
	}

	// Return the midpoint of the two closest points (waist point)
	return QPointF((waistPoint1.x() + waistPoint2.x()) / 2.0,
		(waistPoint1.y() + waistPoint2.y()) / 2.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tangent Calculation

QPointF GeneralCase::mirrorPoint(const QPointF& point, const QLineF& window) {
	double px = point.x();
	double py = point.y();
	double x1 = window.p1().x();
	double y1 = window.p1().y();
	double x2 = window.p2().x();
	double y2 = window.p2().y();

	// Compute the coefficients of the line equation ax + by + c = 0
	double a = y2 - y1;
	double b = x1 - x2;
	double c = x2 * y1 - x1 * y2;

	double denominator = a * a + b * b;

	// Reflect the point across the line
	double xPrime = px - 2 * a * (a * px + b * py + c) / denominator;
	double yPrime = py - 2 * b * (a * px + b * py + c) / denominator;

	return QPointF(xPrime, yPrime);
}

bool GeneralCase::areEqual(const QPointF& a, const QPointF b) {
	const double EPSILON = 1e-9; // A small tolerance value
	bool equalX = std::abs(a.x() - b.x()) < EPSILON;
	bool equalY = std::abs(a.y() - b.y()) < EPSILON;

	return (equalX && equalY);
}

GeneralCase::TangentStruct GeneralCase::findTangent(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon,
	const QVector<QPointF>& funnelSideHelper1,
	const QVector<QPointF>& funnelSideHelper2)
{
	QPointF intersectionPoint;
	QPointF mirroredPoint = mirrorPoint(hourglassPoint, window);
	QLineF line = QLineF(funnelPoint, mirroredPoint);
	QLineF::IntersectType type = line.intersects(window, &intersectionPoint);
	TangentStruct tangent;

	tangent.funnelIntersections = numberOfIntersections(QLineF(funnelPoint, intersectionPoint), funnelSide);
	tangent.hourglassIntersections = numberOfIntersections(QLineF(intersectionPoint, hourglassPoint), hourglassSide);


	QVector<QPointF> mirrorSide;

	for (QPointF point : hourglassSide) {
		mirrorSide.append(mirrorPoint(point, window));
	}

	bool isLineToFunnelTangent = isTangent(funnelSide, funnelPoint, mirroredPoint);
	bool isLineToHourglassTangent = isTangent(mirrorSide, funnelPoint, mirroredPoint);

	//if (funnelPoint == funnelSide.last()) {
	QPointF cba = funnelPoint;
	QPointF abc = funnelSide.last();
	if (areEqual(funnelPoint, funnelSide.last())) {
		Polygon_2 bounds;
		bounds.push_back(Point_2(funnelSideHelper1.last().x(), funnelSideHelper1.last().y()));
		bounds.push_back(Point_2(funnelSideHelper1.rbegin()[1].x(), funnelSideHelper1.rbegin()[1].y()));
		if (funnelSideHelper1.rbegin()[1] != funnelSideHelper2.rbegin()[1]) {
			bounds.push_back(Point_2(funnelSideHelper2.rbegin()[1].x(), funnelSideHelper2.rbegin()[1].y()));
		}
		bounds.push_back(Point_2(funnelSideHelper2.last().x(), funnelSideHelper2.last().y()));

		QLineF tangentLine = QLineF(mirrorPoint(funnelPoint, window), mirrorPoint(hourglassPoint, window));
		QLineF extendedLine = extendLine(tangentLine, 0.01);
		Point_2 testPoint = Point_2(extendedLine.p1().x(), extendedLine.p1().y());
		if (!bounds.is_simple()) {
			std::cout << "Error: Bound Polygon is not simple \n";
			tangent.failure = FUNNEL_HOURGLASS;
			return tangent;
		}

		if (bounds.has_on_unbounded_side(testPoint)) {
			tangent.failure = FUNNEL_HOURGLASS;
			return tangent;
		}
	}

	if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		tangent.failure = VISIBILITY;
		return tangent;
	}
	else if (!isLineToFunnelTangent && !isLineToHourglassTangent) {
		tangent.failure = FUNNEL_HOURGLASS;
		return tangent;
	}
	else if (!isLineToFunnelTangent) {
		tangent.failure = FUNNEL;
		return tangent;
	}
	else if (!isLineToHourglassTangent) {
		tangent.failure = HOURGLASS;
		return tangent;
	}


	QVector<QPointF> tangentPath;
	tangentPath.append(funnelPoint);
	tangentPath.append(intersectionPoint);
	tangentPath.append(hourglassPoint);

	//if (funnelPoint == funnelSide.first() && hourglassPoint == hourglassSide.last()) {
	if (areEqual(funnelPoint, funnelSide.first()) && areEqual(hourglassPoint, hourglassSide.last())) {
		if (numberOfIntersections(window, tangentPath) == 0) {
			tangent.failure = FUNNEL_HOURGLASS;
			return tangent;
		}
	}

	tangent.tangentPath = tangentPath;
	return tangent;
}

bool GeneralCase::isTangent(const QVector<QPointF>& path, const QPointF& p1, const QPointF& p2) {
	double dx = p2.x() - p1.x();
	double dy = p2.y() - p1.y();
	const double epsilon = 1e-9;
	int sign = 0;

	for (const QPointF& p : path) {
		double cross = (p.y() - p1.y()) * dx - (p.x() - p1.x()) * dy;
		// Treat very small values as zero (collinear points)
		if (fabs(cross) < epsilon) continue;

		int newSign = (cross > 0) ? 1 : -1;
		if (sign == 0) {
			sign = newSign; // First non-zero sign sets the reference
		}
		else if (newSign != sign) {
			return false; // If we get opposite signs, path crosses the line
		}
	}
	return true; // If no conflicts, it's a tangent
}

QLineF GeneralCase::extendLine(const QLineF& line, const double extensionFactor) {
	// Extend the line slightly
	QPointF direction = line.p2() - line.p1();
	direction *= extensionFactor / std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

	QPointF newStart = line.p1() - direction;
	QPointF newEnd = line.p2() + direction;

	QLineF extendedLine(newStart, newEnd);
	return extendedLine;
}

int GeneralCase::numberOfIntersections(const QLineF& line, const QVector<QPointF>& side) {
	int intersectionCount = 0;
	bool overlap = false;
	QVector<QPointF> intersections;
	// Iterate over the edges of the side
	for (int i = 0; i < side.size() - 1; ++i) {
		QLineF edge(side[i], side[i + 1]);
		QPointF intersectionPoint;

		// Check if line intersects the edge
		QLineF::IntersectType type = line.intersects(edge, &intersectionPoint);

		if (type == QLineF::BoundedIntersection) {
			intersections.append(intersectionPoint);
		}
	}

	for (QPointF intersection : intersections) {
		if (intersection == line.p1() || intersection == line.p2()) {
			continue;
		}
		else {
			intersectionCount++;
		}
	}

	if (overlap) {
		intersectionCount = std::numeric_limits<int>::max();
	}

	return intersectionCount;
}

QVector<QPointF> GeneralCase::tangentBinary(const QVector<QPointF>& funnelSide, const QVector<QPointF>& hourglassSide, const QLineF& window,
	Polygon_2& polygon, const QVector<QPointF>& funnelSideHelper1, const QVector<QPointF>& funnelSideHelper2) {
	QVector<QPointF> funnelVec = funnelSide;
	QPointF mirroredPoint;
	QLineF line;
	QPointF reflectionPoint;
	QPointF intersectionPoint;
	bool funnelTangency = false;
	int funnelIntersections = 0;

	bool edgeFunnel = false;
	bool edgeHourglass = false;
	bool edgeWindow = false;

	int funnelLeft = 0;
	int funnelRight = funnelVec.size() - 1;

	int hourglassLeft = 1;
	int hourglassRight = hourglassSide.size() - 1;

	TangentStruct tangent;

	while (funnelLeft <= funnelRight) {
		int funnelMid = (funnelLeft + funnelRight + 1) / 2;

		QPointF funnelCandidate = funnelVec[funnelMid];

		hourglassLeft = 1;
		hourglassRight = hourglassSide.size() - 1;
		if (hourglassLeft > hourglassRight) {
			return  tangent.tangentPath;;
		}
		while (hourglassLeft <= hourglassRight) {
			int hourglassMid = (hourglassLeft + hourglassRight) / 2;
			QPointF hourglassCandidate = hourglassSide[hourglassMid];

			int funnelIntersections = tangent.funnelIntersections;
			int hourglassIntersections = tangent.hourglassIntersections;

			tangent = findTangent(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon, funnelSideHelper1, funnelSideHelper2);


			switch (tangent.failure)
			{
			case GeneralCase::NONE: // case a.
				std::cout << "case a \n";
				return tangent.tangentPath;
				break;
			case GeneralCase::FUNNEL_HOURGLASS:
				if (funnelIntersections == 0 && hourglassIntersections == 0) { // case f.
					std::cout << "case f \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1 && hourglassIntersections == 0) { // case g.
					std::cout << "case g \n";
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 0 && hourglassIntersections == 1) { // case h.
					std::cout << "case h \n";
					funnelRight = funnelMid - 1;
				}

				if (funnelIntersections >= 1 && hourglassIntersections >= 1) { // case i.
					std::cout << "case i \n";
					QPointF mirroredPoint = mirrorPoint(hourglassCandidate, window);
					QLineF m = QLineF(funnelCandidate, mirroredPoint);
					QLineF lp;
					QLineF lq;
					QPointF intersectionPoint;

					if (funnelMid + 1 < funnelVec.size()) {
						lp = QLineF(funnelCandidate, funnelVec[funnelMid + 1]);
					}
					else {
						lp = QLineF(funnelVec[funnelMid - 1], funnelCandidate);
					}

					if (hourglassMid - 1 >= 0) {
						lq = QLineF(mirroredPoint, mirrorPoint(hourglassSide[hourglassMid - 1], window));
					}
					else {
						lq = QLineF(mirrorPoint(hourglassSide[hourglassMid + 1], window), mirroredPoint);
					}

					QLineF::IntersectType type = lp.intersects(lq, &intersectionPoint);

					Point_2 a = Point_2(window.p1().x(), window.p1().y());
					Point_2 b = Point_2(window.p2().x(), window.p2().y());

					if (CGAL::orientation(a, b, Point_2(intersectionPoint.x(), intersectionPoint.y()))
						== CGAL::orientation(a, b, Point_2(funnelCandidate.x(), funnelCandidate.y()))) { // case i2.
						std::cout << "case i2 \n";
						funnelLeft = funnelMid + 1;
					}
					else { // case i1.
						std::cout << "case i1 \n";
						hourglassRight = hourglassMid - 1;
					}
				}
				break;

			case GeneralCase::FUNNEL:
				if (funnelIntersections == 0) { // case d.
					std::cout << "case d \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (funnelIntersections == 1) { // case e.
					std::cout << "case e \n";
					funnelLeft = funnelMid + 1;
					hourglassLeft = hourglassMid + 1;
				}

				break;
			case GeneralCase::HOURGLASS:
				if (hourglassIntersections == 0) { // case b.
					std::cout << "case b \n";
					funnelRight = funnelMid - 1;
					hourglassLeft = hourglassMid + 1;
				}

				if (hourglassIntersections == 1) { // case c.
					std::cout << "case c \n";
					funnelRight = funnelMid - 1;
					hourglassRight = hourglassMid - 1;
				}

				break;
			case GeneralCase::VISIBILITY:
				std::cout << "vis \n";
				hourglassRight = hourglassMid - 1;
				if (hourglassRight <= hourglassLeft) {
					funnelVec.removeAt(funnelMid);
				}

				if (funnelVec.isEmpty()) {
					return tangent.tangentPath;
				}
				break;
			default:
				std::cout << "default \n";

				break;
			}
		}
	}
	return tangent.tangentPath;
}


QVector<QPointF> GeneralCase::removeElementFromVector(QVector<QPointF> vector, int index) {
	QVector<QPointF> newVec;

	if (vector.isEmpty() || vector.size() == 1) {
		return newVec;
	}

	for (size_t i = 0; i < vector.size(); ++i) {
		if (i == index) {
			continue;
		}
		else {
			newVec.append(vector[i]);
		}
	}

	return newVec;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hourglass Funnel Concatenation

GeneralCase::ConcatenatedSideStruct GeneralCase::concatenateClosedHelper(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;

	// Process the funnel side until reaching the first tangent point
	for (const QPointF& funnelPoint : funnelSide) {
		concatenatedSide.append(funnelPoint);

		if (funnelPoint == tangent.first()) {
			concatenatedSideStruct.mFunnelIndex = concatenatedSide.size() - 1;
			concatenatedSideStruct.mFunnelPoint = funnelPoint;
			break;
		}
	}

	for (const QPointF& tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}
	concatenatedSideStruct.mHourglassIndex = concatenatedSide.size() - 1;
	concatenatedSideStruct.mHourglassPoint = tangent.last();

	bool hourglassPointFound = false;

	bool start = false;
	for (const QPointF& hourglassPoint : hourglassSide) {
		if (hourglassPoint == tangent.last()) {
			start = true;
			continue;
		}

		if (start) {
			concatenatedSide.append(hourglassPoint);
		}
	}

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}


GeneralCase::FunnelStar GeneralCase::concatenateClosedHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate closed hourglass \n";

	if (!tangent1.isEmpty()) {
		std::cout << "1 not empty \n";
		concatenatedSideStruct = concatenateClosedHelper(funnelSideA, hourglassSide1, tangent1);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateClosedHelper(funnelSideA, hourglassSide2, tangent1);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	else if (!tangent2.isEmpty()) {
		std::cout << "2 not empty \n";
		concatenatedSideStruct = concatenateClosedHelper(funnelSideA, hourglassSide1, tangent2);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateClosedHelper(funnelSideA, hourglassSide2, tangent2);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	else if (!tangent4.isEmpty()) {
		std::cout << "4 not empty \n";
		concatenatedSideStruct = concatenateClosedHelper(funnelSideB, hourglassSide1, tangent4);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateClosedHelper(funnelSideB, hourglassSide2, tangent4);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "3 not empty \n";
		concatenatedSideStruct = concatenateClosedHelper(funnelSideB, hourglassSide1, tangent3);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateClosedHelper(funnelSideB, hourglassSide2, tangent3);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}

	funnelStar.isRootInFunnel = false;
	return funnelStar;
}

GeneralCase::ConcatenatedSideStruct GeneralCase::concatenateOpenHelper(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;

	for (QPointF point : funnelSide) {
		if (point != tangent[0]) {
			concatenatedSide.append(point);
		}
		else {
			break;
		}
	}

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}

	concatenatedSideStruct.mFunnelPoint = tangent.first();
	concatenatedSideStruct.mHourglassPoint = tangent.last();

	bool start = false;
	for (QPointF point : hourglassSide) {
		if (point == tangent.last()) {
			start = true;
			continue;
		}

		if (start) {
			concatenatedSide.append(point);
		}
	}

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}

GeneralCase::FunnelStar GeneralCase::concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate open hourglass \n";

	if (!tangent1.isEmpty()) {
		std::cout << "1 not empty \n";
		concatenatedSideStruct = concatenateOpenHelper(funnelSideA, hourglassSide1, tangent1);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;
	}
	if (!tangent2.isEmpty()) {
		std::cout << "2 not empty \n";
		concatenatedSideStruct = concatenateOpenHelper(funnelSideA, hourglassSide2, tangent2);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	if (!tangent3.isEmpty()) {
		std::cout << "3 not empty \n";
		concatenatedSideStruct = concatenateOpenHelper(funnelSideB, hourglassSide1, tangent3);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;
	}
	if (!tangent4.isEmpty()) {
		std::cout << "4 not empty \n";
		concatenatedSideStruct = concatenateOpenHelper(funnelSideB, hourglassSide2, tangent4);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}

	funnelStar.isRootInFunnel = true;

	return funnelStar;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Optimal Point / Path Computation

int GeneralCase::getLCAIndex(QVector<QPointF>& path1, QVector<QPointF>& path2)
{
	int lcaIndex = 0;

	// Iterate over both paths until they diverge
	size_t minLength = std::min(path1.size(), path2.size());
	for (size_t i = 0; i < minLength; ++i)
	{
		if (path1[i] == path2[i])
		{
			lcaIndex = i;
		}
		else
		{
			break; // Paths diverge, stop the search
		}
	}

	return lcaIndex;
}

GeneralCase::OptimalPointStruct GeneralCase::computeOptimalPoint(QVector<QPointF>& side1, QVector<QPointF>& side2, QLineF& window1, QLineF& window2) {
	OptimalPointStruct optimalPointStruct;
	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	const double angle0 = m_onePointHandler.calculateFunnelAngle(side1.rbegin()[1], side1.rbegin()[0], a2, b2); // theta_0
	if (angle0 > 90)
	{
		std::cout << "c = a2 \n";
		optimalPointStruct.optimalPoint = a2;
		optimalPointStruct.perpendicularPointIndex = side1.size() - 1;
		optimalPointStruct.onPathRootToA = true;
		return optimalPointStruct;
	}

	const double anglek = m_onePointHandler.calculateFunnelAngle(side2.rbegin()[1], side2.rbegin()[0], a2, b2); // theta_k
	if (anglek < 90)
	{
		std::cout << "c = b2 \n";
		optimalPointStruct.optimalPoint = b2;
		optimalPointStruct.perpendicularPointIndex = side2.size() - 1;
		optimalPointStruct.onPathRootToA = false;
		return optimalPointStruct;
	}
	//
	//
	const double anglem1 = m_onePointHandler.calculateFunnelAngle(side1.begin()[0], side1.begin()[1], a2, b2); // theta_m-1
	const double anglem = m_onePointHandler.calculateFunnelAngle(side2.begin()[0], side2.begin()[1], a2, b2);  // theta_m
	if (anglem1 == 90 || anglem == 90 || (anglem1 <= 90 && 90 < anglem))
	{
		std::cout << "c is at foot of rootStar \n";
		optimalPointStruct.optimalPoint = m_onePointHandler.calculateWindowIntersection(side1.first(), a2, b2);;
		optimalPointStruct.perpendicularPointIndex = 0;
		optimalPointStruct.onPathRootToA = true;
		return optimalPointStruct;
	}

	if (anglem1 > 90)
	{
		int vertex = m_onePointHandler.binarySearchByAngleASide(side1, a2, b2);
		int v = (side1.size() - 1) - vertex; // root would be lowest, but needs to be highest
		std::cout << "c is foot from v" << v << "\n";
		optimalPointStruct.optimalPoint = m_onePointHandler.calculateWindowIntersection(side1[vertex], a2, b2);
		optimalPointStruct.perpendicularPointIndex = vertex;
		optimalPointStruct.onPathRootToA = true;
		return optimalPointStruct;

	}
	else
	{
		int vertex = m_onePointHandler.binarySearchByAngleBSide(side2, a2, b2);
		int v = vertex + (side2.size() - 1);
		std::cout << "c is foot from v" << v << "\n";
		optimalPointStruct.optimalPoint = m_onePointHandler.calculateWindowIntersection(side2[vertex], a2, b2);
		optimalPointStruct.perpendicularPointIndex = vertex;
		optimalPointStruct.onPathRootToA = false;
		return optimalPointStruct;
	}
}

QVector<QPointF> GeneralCase::computeOptimalPathRootInFunnel(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot) {
	std::cout << "root in funnel \n";
	QVector<QPointF> pathRootToA2;
	QVector<QPointF> pathRootToB2;
	QVector<QPointF> optimalPath;
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	QPointF funnelStarRoot;
	int funnelStarRootIndex = getLCAIndex(funnelStarSide1, funnelStarSide2);


	pathRootToA2.append(mirrorPoint(funnelStarSide1[funnelStarRootIndex], window1));
	for (size_t i = funnelStarRootIndex+1; i < funnelStarSide1.size(); ++i) {

		pathRootToA2.append(funnelStarSide1[i]);
	}

	pathRootToB2.append(mirrorPoint(funnelStarSide2[funnelStarRootIndex], window1));
	for (size_t i = funnelStarRootIndex+1; i < funnelStarSide2.size(); ++i) {
		pathRootToB2.append(funnelStarSide2[i]);
	}

	OptimalPointStruct c = computeOptimalPoint(pathRootToA2, pathRootToB2, window1, window2);
	int perpendicularPointIndex = c.perpendicularPointIndex + funnelStarRootIndex;

	if (c.onPathRootToA) {
		for (size_t i = 0; i <= perpendicularPointIndex; ++i) {
			optimalPath.append(funnelStarSide1[i]);
		}

		if (perpendicularPointIndex == funnelStarSide1.size() - 1) { // if c == a 
			return optimalPath;
		}
	}
	else {
		for (size_t i = 0; i <= perpendicularPointIndex; ++i) {
			optimalPath.append(funnelStarSide2[i]);
		}

		if (perpendicularPointIndex == funnelStarSide2.size() - 1) { // if c == b
			return optimalPath;
		}
	}

	optimalPath.append(c.optimalPoint);
	return optimalPath;
}


QVector<QPointF> GeneralCase::computeOptimalPathRootInHourglass(QLineF& window1, QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot) {
	std::cout << "root in hourglass \n";
	QVector<QPointF> pathRootToA2;
	QVector<QPointF> pathRootToB2;
	QVector<QPointF> optimalPath;
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	int funnelStarRootIndex = getLCAIndex(funnelStarSide1, funnelStarSide2);

	for (size_t i = funnelStarRootIndex; i < funnelStarSide1.size(); ++i) {

		pathRootToA2.append(funnelStarSide1[i]);
	}

	for (size_t i = funnelStarRootIndex; i < funnelStarSide2.size(); ++i) {
		pathRootToB2.append(funnelStarSide2[i]);
	}

	OptimalPointStruct c = computeOptimalPoint(pathRootToA2, pathRootToB2, window1, window2);
	int perpendicularPointIndex = c.perpendicularPointIndex + funnelStarRootIndex;

	if (c.onPathRootToA) {
		for (size_t i = 0; i <= perpendicularPointIndex; ++i) {
			optimalPath.append(funnelStarSide1[i]);
		}

		if (perpendicularPointIndex == funnelStarSide1.size() - 1) { // if c == a 
			return optimalPath;
		}
	}
	else {
		for (size_t i = 0; i <= perpendicularPointIndex; ++i) {
			optimalPath.append(funnelStarSide2[i]);
		}

		if (perpendicularPointIndex == funnelStarSide2.size() - 1) { // if c == b
			return optimalPath;
		}
	}

	optimalPath.append(c.optimalPoint);
	return optimalPath;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execution

GeneralCase::GeneralCaseResult GeneralCase::executeGeneralCase(QPointF& startingPoint, QLineF& window1, QLineF& window2, Polygon_2& polygon, Surface_mesh& mesh)
{
	GeneralCaseResult resultGeneral;
	QPointF a1 = window1.p1();
	QPointF b1 = window1.p2();

	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();

	QVector<QPointF> pathStartToA1 = m_shortestPathHandler.findShortestPath(startingPoint, a1, polygon, mesh);
	QVector<QPointF> pathStartToB1 = m_shortestPathHandler.findShortestPath(startingPoint, b1, polygon, mesh);

	QPointF penultimatePointSToA1 = pathStartToA1.begin()[1];
	QPointF penultimatePointSToB1 = pathStartToB1.begin()[1];
	QPointF funnelRoot;

	if (penultimatePointSToA1 != penultimatePointSToB1)
	{
		funnelRoot = startingPoint;
	}
	else
	{
		funnelRoot = m_shortestPathHandler.getLCA(pathStartToA1, pathStartToB1);
	}
	resultGeneral.funnelRoot = funnelRoot;

	FunnelStruct funnel;
	QVector<QPointF> funnelSideA = m_shortestPathHandler.findShortestPath(funnelRoot, a1, polygon, mesh);
	funnel.funnelSideA = funnelSideA;
	resultGeneral.funnelSideA = funnelSideA;
	QVector<QPointF> funnelSideB = m_shortestPathHandler.findShortestPath(funnelRoot, b1, polygon, mesh);
	funnel.funnelSideB = funnelSideB;
	resultGeneral.funnelSideB = funnelSideB;

	HourglassStruct hourglass = constructHourglass(window1, window2, polygon, mesh);
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	resultGeneral.hourglassSide1 = hourglassSide1;
	resultGeneral.hourglassSide2 = hourglassSide2;

	if (!hourglass.isOpen) {
		QVector<QPointF> newSide1;
		QVector<QPointF> newSide2;
		QPointF waistPoint = findWaistPoint(hourglassSide1, hourglassSide2);

		for (QPointF hourglassPoint : hourglassSide1) {
			if (hourglassPoint == waistPoint) {
				newSide1.append(hourglassPoint);
				break;
			}
			newSide1.append(hourglassPoint);
		}

		for (QPointF hourglassPoint : hourglassSide2) {
			if (hourglassPoint == waistPoint) {
				newSide2.append(hourglassPoint);
				break;
			}
			newSide2.append(hourglassPoint);
		}

		hourglassSide1 = newSide1;
		hourglassSide2 = newSide2;
	}

	std::cout << "tangent1 calculations: \n";
	QVector<QPointF> tangent1 = tangentBinary(funnelSideA, hourglassSide1, window1, polygon, funnelSideA, funnelSideB);
	std::cout << "tangent2 calculations: \n";
	QVector<QPointF> tangent2 = tangentBinary(funnelSideA, hourglassSide2, window1, polygon, funnelSideA, funnelSideB);
	std::cout << "tangent3 calculations: \n";
	QVector<QPointF> tangent3 = tangentBinary(funnelSideB, hourglassSide1, window1, polygon, funnelSideA, funnelSideB);
	std::cout << "tangent4 calculations: \n";
	QVector<QPointF> tangent4 = tangentBinary(funnelSideB, hourglassSide2, window1, polygon, funnelSideA, funnelSideB);
	resultGeneral.tangent1 = tangent1;
	resultGeneral.tangent2 = tangent2;
	resultGeneral.tangent3 = tangent3;
	resultGeneral.tangent4 = tangent4;

	if (tangent1.isEmpty() && tangent2.isEmpty() && tangent3.isEmpty() && tangent4.isEmpty()) {
		std::cout << "Error: No valid tangent was found! \n";
		return resultGeneral;
	}

	FunnelStar funnelStar;
	if (!hourglass.isOpen) {
		funnelStar = concatenateClosedHourglass(tangent1, tangent2, tangent3, tangent4, funnel, hourglass);
	}
	else {
		funnelStar = concatenateOpenHourglass(tangent1, tangent2, tangent3, tangent4, funnel, hourglass);
	}

	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	resultGeneral.concatenatedSide1 = funnelStarSide1;
	resultGeneral.concatenatedSide2 = funnelStarSide2;
	resultGeneral.m1 = funnelStar.m1;
	resultGeneral.m2 = funnelStar.m2;
	resultGeneral.m3 = funnelStar.m3;
	resultGeneral.m4 = funnelStar.m4;

	if (funnelStarSide1.isEmpty() || funnelStarSide2.isEmpty()) {
		std::cout << "Error: no two valid sides were constructed! \n";
		return resultGeneral;
	}

	QVector<QPointF> optimalPath;
	QVector<QPointF> pathStartToFunnelRoot = m_shortestPathHandler.findShortestPath(startingPoint, funnelRoot, polygon, mesh);
	if (funnelStar.isRootInFunnel) {
		optimalPath = computeOptimalPathRootInFunnel(window1, window2, funnelStar, pathStartToFunnelRoot);
	}
	else {
		optimalPath = computeOptimalPathRootInHourglass(window1, window2, funnelStar, pathStartToFunnelRoot);
	}

	if (optimalPath.isEmpty()) {
		std::cout << "Error: Optimal Path is empty! \n";
		return resultGeneral;
	}

	QPointF c = optimalPath.rbegin()[0];

	resultGeneral.optimalPath = optimalPath;
	resultGeneral.optimalPoint = c;

	return resultGeneral;
}