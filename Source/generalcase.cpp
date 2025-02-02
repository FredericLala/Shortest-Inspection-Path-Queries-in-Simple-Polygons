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

	// Iterate through all point pairs
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

	// Return the midpoint of the two closest points (this is the waist point)
	return QPointF((waistPoint1.x() + waistPoint2.x()) / 2.0,
		(waistPoint1.y() + waistPoint2.y()) / 2.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tangent Calculation

QPointF GeneralCase::mirrorPoint(const QPointF& point, const QLineF& window) {
	// Extract the coordinates of the point and the window endpoints
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

	// Compute the denominator (a^2 + b^2)
	double denominator = a * a + b * b;

	// Reflect the point across the line
	double xPrime = px - 2 * a * (a * px + b * py + c) / denominator;
	double yPrime = py - 2 * b * (a * px + b * py + c) / denominator;

	// Return the reflected point
	return QPointF(xPrime, yPrime);
}

GeneralCase::TangentStruct GeneralCase::findTangent(const QPointF& funnelPoint,
	const QPointF& hourglassPoint,
	const QVector<QPointF>& funnelSide,
	const QVector<QPointF>& hourglassSide,
	const QLineF& window,
	Polygon_2& polygon,
	const QVector<QPointF>& hourglassSideHelper)
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

	if (funnelPoint == funnelSide.last()) {
		Polygon_2 bounds;
		bounds.push_back(Point_2(hourglassSide.first().x(), hourglassSide.first().y()));
		bounds.push_back(Point_2(hourglassSide.begin()[1].x(), hourglassSide.begin()[1].y()));
		if (hourglassSide.begin()[1] != hourglassSideHelper.begin()[1]) {
			bounds.push_back(Point_2(hourglassSideHelper.begin()[1].x(), hourglassSideHelper.begin()[1].y()));
		}
		bounds.push_back(Point_2(hourglassSideHelper.first().x(), hourglassSideHelper.first().y()));
		boundTest = bounds;

		QLineF tangentLine = QLineF(mirrorPoint(funnelPoint, window), mirrorPoint(hourglassPoint, window));
		QLineF extendedLine = extendLine(tangentLine, 0.01);
		Point_2 testPoint = Point_2(extendedLine.p1().x(), extendedLine.p1().y());
		if (bounds.has_on_unbounded_side(testPoint)) {
			tangent.failure = FUNNEL_HOURGLASS;
			return tangent;
		}
	}

	if (!isLineToFunnelTangent && !isLineToHourglassTangent) {
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

	else if (!(m_onePointHandler.checkVisibilty(funnelPoint, intersectionPoint, polygon) && m_onePointHandler.checkVisibilty(intersectionPoint, hourglassPoint, polygon))) {
		tangent.failure = VISIBILITY;
		return tangent;
	}

	QVector<QPointF> tangentPath;
	tangentPath.append(funnelPoint);
	tangentPath.append(intersectionPoint);
	tangentPath.append(hourglassPoint);

	tangent.tangentPath = tangentPath;
	return tangent;
}

bool GeneralCase::isTangent(const QVector<QPointF>& path, const QPointF& p1, const QPointF& p2) {
	double dx = p2.x() - p1.x();
	double dy = p2.y() - p1.y();

	int sign = 0;
	for (const QPointF& p : path) {
		double cross = (p.y() - p1.y()) * dx - (p.x() - p1.x()) * dy;
		int newSign = (cross > 0) - (cross < 0); // +1 if positive, -1 if negative, 0 if on the line
		if (sign == 0) sign = newSign; // First point determines reference sign
		else if (newSign != 0 && newSign != sign) return false; // Points are on different sides
	}
	return true;
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
		QLineF edge(side[i], side[i + 1]); // Create an edge from consecutive points
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
	Polygon_2& polygon, const QVector<QPointF>& hourglassSideHelper) {
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
	funnelSideTest = funnelSide;
	funnelVecSideTest = funnelVec;

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

			tangent = findTangent(funnelCandidate, hourglassCandidate, funnelVec, hourglassSide, window, polygon, hourglassSideHelper);


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
					std::cout << funnelMid - 1 << "\n";
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

GeneralCase::ConcatenatedSideStruct GeneralCase::concatenateClosed(QVector<QPointF> funnelSide, QVector<QPointF> hourglassSide, QVector<QPointF> tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;
	int indexCount = -1;

	// Process the funnel side until reaching the first tangent point
	for (const QPointF& funnelPoint : funnelSide) {
		concatenatedSide.append(funnelPoint);
		++indexCount;

		if (funnelPoint == tangent.first()) {
			concatenatedSideStruct.mFunnelIndex = indexCount;
			concatenatedSideStruct.mFunnelPoint = funnelPoint;
			break;
		}
	}

	// Add all tangent points while avoiding duplicates
	for (const QPointF& tangentPoint : tangent) {
		if (concatenatedSide.isEmpty() || concatenatedSide.last() != tangentPoint) {
			concatenatedSide.append(tangentPoint);
			++indexCount;
		}
	}

	bool hourglassPointFound = false;

	// Process the hourglass side, ensuring correct tracking of hourglass indices
	// TODO: could be better
	for (const QPointF& hourglassPoint : hourglassSide) {
		if (!hourglassPointFound && (hourglassPoint == tangent[tangent.size() - 2])) {
			concatenatedSideStruct.mHourglassIndex = indexCount;
			concatenatedSideStruct.mHourglassPoint = hourglassPoint;
			hourglassPointFound = true;
			continue;
		}

		if (hourglassPoint == tangent.last()) {
			if (!hourglassPointFound) {
				concatenatedSideStruct.mHourglassIndex = indexCount;
				concatenatedSideStruct.mHourglassPoint = hourglassPoint;
				hourglassPointFound = true;
			}
			continue;
		}

		concatenatedSide.append(hourglassPoint);
		++indexCount;
	}

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}


GeneralCase::ConcatenatedSideStruct GeneralCase::concatenateOpen1(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
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

	concatenatedSideStruct.mFunnelPoint = tangent[0];
	concatenatedSideStruct.mHourglassPoint = tangent.rbegin()[0];

	concatenatedSide.append(hourglassSide.rbegin()[0]);

	concatenatedSideStruct.concatenatedSide = concatenatedSide;
	return concatenatedSideStruct;
}


GeneralCase::ConcatenatedSideStruct GeneralCase::concatenateOpen2(QVector<QPointF>& funnelSide, QVector<QPointF>& hourglassSide, QVector<QPointF>& tangent) {
	ConcatenatedSideStruct concatenatedSideStruct;
	QVector<QPointF> concatenatedSide;

	concatenatedSide.append(funnelSide[0]);

	for (QPointF tangentPoint : tangent) {
		concatenatedSide.append(tangentPoint);
	}

	concatenatedSideStruct.mFunnelPoint = tangent[0];
	concatenatedSideStruct.mHourglassPoint = tangent.rbegin()[0];

	bool start = false;
	for (QPointF hourglassPoint : hourglassSide) {
		if (start) {
			concatenatedSide.append(hourglassPoint);
		}

		if (hourglassPoint == tangent.rbegin()[0]) {
			start = true;
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
		concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide1, tangent1);

	}
	else if (!tangent2.isEmpty()) {
		std::cout << "2 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide2, tangent2);
	}
	funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;

	funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

	if (!tangent4.isEmpty()) {
		std::cout << "4 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide2, tangent4);
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "3 not empty \n";
		concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide1, tangent3);
	}

	funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;

	funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;

	funnelStar.isRootInFunnel = false;
	return funnelStar;
}


GeneralCase::FunnelStar GeneralCase::concatenateBlockedOpenHourglass(QVector<QPointF>& tangent2, QVector<QPointF>& tangent3, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate blocked open hourglass \n";

	if (!tangent2.isEmpty()) {
		std::cout << "Open: 2 not empty \n";
		concatenatedSideStruct = concatenateOpen1(funnelSideA, hourglassSide1, tangent2);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateOpen2(funnelSideB, hourglassSide2, tangent2);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}
	else if (!tangent3.isEmpty()) {
		std::cout << "Open: 3 not empty \n";
		concatenatedSideStruct = concatenateOpen2(funnelSideA, hourglassSide1, tangent3);
		funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;

		concatenatedSideStruct = concatenateOpen1(funnelSideB, hourglassSide2, tangent3);
		funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
		funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
		funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	}

	funnelStar.isRootInFunnel = false;

	return funnelStar;
}


GeneralCase::FunnelStar GeneralCase::concatenateOpenHourglass(QVector<QPointF>& tangent1, QVector<QPointF>& tangent4, FunnelStruct& funnel, HourglassStruct& hourglass) {
	QVector<QPointF> funnelSideA = funnel.funnelSideA;
	QVector<QPointF> funnelSideB = funnel.funnelSideB;
	QVector<QPointF> hourglassSide1 = hourglass.hourglassSide1;
	QVector<QPointF> hourglassSide2 = hourglass.hourglassSide2;
	ConcatenatedSideStruct concatenatedSideStruct;
	FunnelStar funnelStar;
	std::cout << "concatenate open hourglass \n";

	concatenatedSideStruct = concatenateClosed(funnelSideA, hourglassSide1, tangent1);
	funnelStar.funnelStarSide1 = concatenatedSideStruct.concatenatedSide;
	funnelStar.m1 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m2 = concatenatedSideStruct.mHourglassPoint;
	funnelStar.m1Index = concatenatedSideStruct.mFunnelIndex;
	funnelStar.m2Index = concatenatedSideStruct.mHourglassIndex;

	concatenatedSideStruct = concatenateClosed(funnelSideB, hourglassSide2, tangent4);
	funnelStar.funnelStarSide2 = concatenatedSideStruct.concatenatedSide;
	funnelStar.m3 = concatenatedSideStruct.mFunnelPoint;
	funnelStar.m4 = concatenatedSideStruct.mHourglassPoint;
	funnelStar.m3Index = concatenatedSideStruct.mFunnelIndex;
	funnelStar.m4Index = concatenatedSideStruct.mHourglassIndex;

	funnelStar.isRootInFunnel = true;

	return funnelStar;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Optimal Point / Path Computation

QVector<QPointF> GeneralCase::mirrorFunnelPart(QVector<QPointF>& funnelStarSide, int mIndex, QLineF& window1) {
	QVector<QPointF> modifiedSide;
	for (size_t i = 0; i < funnelStarSide.size(); ++i) {
		if (i <= mIndex) {
			modifiedSide.append(mirrorPoint(funnelStarSide[i], window1));
		}
		else {
			modifiedSide.append(funnelStarSide[i]);
		}
	}

	return modifiedSide;
}

QPointF GeneralCase::computeOptimalPoint(FunnelStar& funnelStar, QLineF& window1, QLineF& window2) {
	bool onPathRootToA = false;
	QPointF vertexPerpendicularToC;
	int perpendicularIndex = 0;
	QPointF a2 = window2.p1();
	QPointF b2 = window2.p2();
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	QVector<QPointF> modifiedSide1 = mirrorFunnelPart(funnelStarSide1, funnelStar.m1Index, window1);
	QVector<QPointF> modifiedSide2 = mirrorFunnelPart(funnelStarSide2, funnelStar.m3Index, window1);

	const double angle0 = m_onePointHandler.calculateFunnelAngle(modifiedSide1.rbegin()[1], modifiedSide1.rbegin()[0], a2, b2); // theta_0
	if (angle0 > 90)
	{
		std::cout << "c = a2 \n";
		onPathRootToA = true;
		vertexPerpendicularToC = a2;
		perpendicularIndex = modifiedSide1.size() - 1;
		return a2;
	}

	const double anglek = m_onePointHandler.calculateFunnelAngle(modifiedSide2.rbegin()[1], modifiedSide2.rbegin()[0], a2, b2); // theta_k
	if (anglek < 90)
	{
		std::cout << "c = b2 \n";
		onPathRootToA = false;
		vertexPerpendicularToC = b2;
		perpendicularIndex = modifiedSide2.size() - 1;
		return b2;
	}
	//
	//
	const double anglem1 = m_onePointHandler.calculateFunnelAngle(modifiedSide1.begin()[0], modifiedSide1.begin()[1], a2, b2); // theta_m-1
	const double anglem = m_onePointHandler.calculateFunnelAngle(modifiedSide2.begin()[0], modifiedSide2.begin()[1], a2, b2);  // theta_m
	if (anglem1 == 90 || anglem == 90 || (anglem1 <= 90 && 90 < anglem))
	{
		std::cout << "c is at foot of rootStar \n";
		perpendicularIndex = 0;
		vertexPerpendicularToC = funnelStarSide1[0];
		return m_onePointHandler.calculateWindowIntersection(vertexPerpendicularToC, a2, b2);
	}

	if (anglem1 > 90)
	{
		int vertex = m_onePointHandler.binarySearchByAngleASide(modifiedSide1, a2, b2);
		int v = (modifiedSide1.size() - 1) - vertex; // root would be lowest, but needs to be highest
		std::cout << "c is foot from v" << v << "\n";
		onPathRootToA = true;
		perpendicularIndex = vertex;
		vertexPerpendicularToC = modifiedSide1[vertex];
		return m_onePointHandler.calculateWindowIntersection(modifiedSide1[vertex], a2, b2);
	}
	else
	{
		int vertex = m_onePointHandler.binarySearchByAngleBSide(modifiedSide2, a2, b2);
		int v = vertex + (modifiedSide2.size() - 1);
		std::cout << "c is foot from v" << v << "\n";
		onPathRootToA = false;
		perpendicularIndex = vertex;
		vertexPerpendicularToC = modifiedSide2[vertex];
		return m_onePointHandler.calculateWindowIntersection(modifiedSide2[vertex], a2, b2);
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
	int funnelStarRootIndex = 0;

	QPointF mirrorM1 = mirrorPoint(funnelStar.m1, window1);
	int prevIndexM1 = funnelStar.m1Index - 1;
	if (mirrorM1 == funnelStar.m2 && prevIndexM1 >= 0) {
		mirrorM1 = mirrorPoint(funnelStarSide1[prevIndexM1], window1);
	}

	QPointF mirrorM3 = mirrorPoint(funnelStar.m3, window1);
	int prevIndexM3 = funnelStar.m3Index - 1;
	if (mirrorM3 == funnelStar.m4 && prevIndexM3 >= 0) {
		mirrorM3 = mirrorPoint(funnelStarSide2[prevIndexM3], window1);
	}

	//std::cout << "m2: " << funnelStar.m2Index << "\n";
	//std::cout << "m4: " << funnelStar.m4Index << "\n";
	if (funnelStar.m1 == funnelStar.funnelStarSide1[funnelStar.m1Index]) {
		std::cout << "m1 is correct \n";
	}

	if (funnelStar.m2 == funnelStar.funnelStarSide1[funnelStar.m2Index]) {
		std::cout << "m2 is correct \n";
	}

	if (funnelStar.m3 == funnelStar.funnelStarSide2[funnelStar.m3Index]) {
		std::cout << "m3 is correct \n";
	}

	if (funnelStar.m4 == funnelStar.funnelStarSide2[funnelStar.m4Index]) {
		std::cout << "m4 is correct \n";
	}

	QPointF c = computeOptimalPoint(funnelStar, window1, window2);
	optimalPath.append(c);
	return optimalPath;
}


QVector<QPointF> GeneralCase::computeOptimalPathRootInHourglass(QLineF& window2, FunnelStar& funnelStar, QVector<QPointF>& pathStartToFunnelRoot) {
	std::cout << "root in hourglass \n";
	QVector<QPointF> pathA2ToRoot;
	QVector<QPointF> pathB2ToRoot;
	QVector<QPointF> pathRootToA2;
	QVector<QPointF> pathRootToB2;
	QVector<QPointF> optimalPath;
	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	QPointF funnelStarRoot = funnelStar.m4;
	starRoot = funnelStarRoot;
	int funnelStarRootIndex = 0;

	for (int i = funnelStarSide1.size() - 1; i >= 0; --i) {
		if (funnelStarSide1[i] != funnelStarRoot) {
			pathA2ToRoot.append(funnelStarSide1[i]);
		}
		else {
			pathA2ToRoot.append(funnelStarSide1[i]);
			break;
		}
	}
	pathRootToA2 = m_shortestPathHandler.reversePath(pathA2ToRoot);

	for (int i = funnelStarSide2.size() - 1; i >= 0; --i) {
		if (funnelStarSide2[i] != funnelStarRoot) {
			pathB2ToRoot.append(funnelStarSide2[i]);
		}
		else {
			pathB2ToRoot.append(funnelStarSide2[i]);
			break;
		}
	}
	pathRootToB2 = m_shortestPathHandler.reversePath(pathB2ToRoot);

	////
	// TEST
	PRAT = pathRootToA2;
	PRBT = pathRootToB2;
	//

	QPointF c = m_onePointHandler.computeOptimalPoint(pathRootToA2, pathRootToB2, funnelStarRoot, window2);
	QPointF vertexPerpendicularToC = m_onePointHandler.getVertexPerpendicularToC();
	if (c != vertexPerpendicularToC) {
		optimalPath.append(c);
	}

	bool start = false;
	if (m_onePointHandler.getOnPathRootToA()) {
		for (int i = funnelStarSide1.size() - 1; i >= 0; --i) {
			if (funnelStarSide1[i] == vertexPerpendicularToC) {
				start = true;
			}

			if (start) {
				optimalPath.append(funnelStarSide1[i]);
			}
		}
	}
	else {
		for (int i = funnelStarSide2.size() - 1; i >= 0; --i) {
			if (funnelStarSide2[i] == vertexPerpendicularToC) {
				start = true;
			}

			if (start) {
				optimalPath.append(funnelStarSide2[i]);
			}
		}
	}

	for (int i = pathStartToFunnelRoot.size() - 1; i >= 0; --i) {
		optimalPath.append(pathStartToFunnelRoot[i]);
	}


	return m_shortestPathHandler.reversePath(optimalPath);
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

	////
	// TEST
	firstWindow = window1;
	/////

	Polygon_2 funnelPolygon;
	for (QPointF funnelPoint : funnelSideA) {
		funnelPolygon.push_back(Point_2(funnelPoint.x(), funnelPoint.y()));
	}

	for (int i = funnelSideB.size() - 1; i > 0; --i) {
		funnelPolygon.push_back(Point_2(funnelSideB[i].x(), funnelSideB[i].y()));
	}

	if (!hourglass.isOpen) {
		QVector<QPointF> newSide1;
		QVector<QPointF> newSide2;
		QPointF waistPoint = findWaistPoint(hourglassSide1, hourglassSide2);
		waist = waistPoint;

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

	QVector<QPointF> tangent1 = tangentBinary(funnelSideA, hourglassSide1, window1, polygon, hourglassSide2);
	QVector<QPointF> tangent2 = tangentBinary(funnelSideA, hourglassSide2, window1, polygon, hourglassSide1);
	QVector<QPointF> tangent3 = tangentBinary(funnelSideB, hourglassSide1, window1, polygon, hourglassSide2);
	QVector<QPointF> tangent4 = tangentBinary(funnelSideB, hourglassSide2, window1, polygon, hourglassSide1);
	resultGeneral.tangent1 = tangent1;
	resultGeneral.tangent2 = tangent2;
	resultGeneral.tangent3 = tangent3;
	resultGeneral.tangent4 = tangent4;

	bool areOuterTangentsBlocked = (tangent1.isEmpty() || tangent4.isEmpty());


	FunnelStar funnelStar;
	if (!hourglass.isOpen) {
		funnelStar = concatenateClosedHourglass(tangent1, tangent2, tangent3, tangent4, funnel, hourglass);
	}
	else {
		if (areOuterTangentsBlocked) {
			funnelStar = concatenateBlockedOpenHourglass(tangent2, tangent3, funnel, hourglass);
		}
		else {
			funnelStar = concatenateOpenHourglass(tangent1, tangent4, funnel, hourglass);

		}
	}

	QVector<QPointF> funnelStarSide1 = funnelStar.funnelStarSide1;
	QVector<QPointF> funnelStarSide2 = funnelStar.funnelStarSide2;
	resultGeneral.concatenatedSide1 = funnelStarSide1;
	resultGeneral.concatenatedSide2 = funnelStarSide2;
	resultGeneral.m1 = funnelStar.m1;
	resultGeneral.m2 = funnelStar.m2;
	resultGeneral.m3 = funnelStar.m3;
	resultGeneral.m4 = funnelStar.m4;

	QVector<QPointF> optimalPath;
	QVector<QPointF> pathStartToFunnelRoot = m_shortestPathHandler.findShortestPath(startingPoint, funnelRoot, polygon, mesh);
	if (funnelStar.isRootInFunnel) {
		optimalPath = computeOptimalPathRootInFunnel(window1, window2, funnelStar, pathStartToFunnelRoot);
	}
	else {
		optimalPath = computeOptimalPathRootInHourglass(window2, funnelStar, pathStartToFunnelRoot);
	}

	QPointF c = optimalPath.rbegin()[0];

	resultGeneral.optimalPath = optimalPath;
	resultGeneral.optimalPoint = c;

	return resultGeneral;
}