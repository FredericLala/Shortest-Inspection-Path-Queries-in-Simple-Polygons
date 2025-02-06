#include "polygongen.h"

PolygonGen::PolygonGen()
{
}

// Function to generate a random polygon and return the points
Polygon_2 PolygonGen::generateRandomPolygon(int size, double radius)
{
	Polygon_2 polygon;
	std::list<Point_2> point_set;
	CGAL::Random rand;

	const int MAX_POLY_SIZE = 100;

	if (size < 4)
	{
		size = rand.get_int(4, MAX_POLY_SIZE);
	}

	// copy size points from the generator, eliminating duplicates
	CGAL::copy_n_unique(Point_generator(radius), size, std::back_inserter(point_set));

	// Generate the polygon from the points
	CGAL::random_polygon_2(point_set.size(), std::back_inserter(polygon), point_set.begin());

	return polygon;
}

Polygon_2 PolygonGen::exampleOne() const
{
	Polygon_2 p;
	p.push_back(Point_2(18, -52));
	p.push_back(Point_2(96, -98));
	p.push_back(Point_2(46, -189));
	p.push_back(Point_2(141, -251));
	p.push_back(Point_2(-230, -113));
	p.push_back(Point_2(-63, 108));
	p.push_back(Point_2(-195, 115));
	p.push_back(Point_2(-193, 190));
	p.push_back(Point_2(-43, 189));
	p.push_back(Point_2(66, 89));
	p.push_back(Point_2(72, 190));
	p.push_back(Point_2(290, 189));
	p.push_back(Point_2(210, 126));
	p.push_back(Point_2(385, -145));
	p.push_back(Point_2(211, -201));
	p.push_back(Point_2(179, 10));
	p.push_back(Point_2(79, 20));
	p.push_back(Point_2(-48, -23));
	p.push_back(Point_2(-63, -129));
	p.push_back(Point_2(-20, -100));
	p.push_back(Point_2(9, -142));

	return p;
}

Polygon_2 PolygonGen::exampleTwo() const
{
	Polygon_2 p;
	p.push_back(Point_2(-84, -140));
	p.push_back(Point_2(-138, -94));
	p.push_back(Point_2(-60, 13));
	p.push_back(Point_2(-168, -37));
	p.push_back(Point_2(-243, -11));
	p.push_back(Point_2(-238, 102));
	p.push_back(Point_2(-180, 153));
	p.push_back(Point_2(46, 127));
	p.push_back(Point_2(142, 153));
	p.push_back(Point_2(183, 122));
	p.push_back(Point_2(123, -72));
	p.push_back(Point_2(44, -16));
	p.push_back(Point_2(57, -121));

	return p;
}

Polygon_2 PolygonGen::exampleThree() const
{
	Polygon_2 p;
	p.push_back(Point_2(-223, -23));
	p.push_back(Point_2(-264, 71));
	p.push_back(Point_2(-108, 52));
	p.push_back(Point_2(16, 81));
	p.push_back(Point_2(33, 5));
	p.push_back(Point_2(189, -42));
	p.push_back(Point_2(189, -141));
	p.push_back(Point_2(40, -97));
	p.push_back(Point_2(-54, -153));
	p.push_back(Point_2(-166, -124));
	p.push_back(Point_2(-169, 3));

	return p;
}

Polygon_2 PolygonGen::exampleFour() const
{
	Polygon_2 p;
	p.push_back(Point_2(-201, -40));
	p.push_back(Point_2(-196, -73));
	p.push_back(Point_2(-198, -153));
	p.push_back(Point_2(-179, -180));
	p.push_back(Point_2(-120, -221));
	p.push_back(Point_2(34, -212));
	p.push_back(Point_2(60, -246));
	p.push_back(Point_2(168, -225));
	p.push_back(Point_2(225, -200));
	p.push_back(Point_2(94, -11));
	p.push_back(Point_2(205, 16));
	p.push_back(Point_2(242, -170));
	p.push_back(Point_2(217, 176));
	p.push_back(Point_2(199, 203));
	p.push_back(Point_2(164, 60));
	p.push_back(Point_2(155, 75));
	p.push_back(Point_2(67, -12));
	p.push_back(Point_2(68, 49));
	p.push_back(Point_2(55, 89));
	p.push_back(Point_2(41, 113));
	p.push_back(Point_2(-22, 190));
	p.push_back(Point_2(117, 174));
	p.push_back(Point_2(149, 143));
	p.push_back(Point_2(161, 223));
	p.push_back(Point_2(80, 204));
	p.push_back(Point_2(-239, 230));
	p.push_back(Point_2(-182, 199));
	p.push_back(Point_2(-32, 202));
	p.push_back(Point_2(-4, 146));
	p.push_back(Point_2(-198, 199));
	p.push_back(Point_2(-138, 146));
	p.push_back(Point_2(-196, 116));
	p.push_back(Point_2(-153, 78));
	p.push_back(Point_2(-140, 88));
	p.push_back(Point_2(-74, 94));
	p.push_back(Point_2(24, -149));
	p.push_back(Point_2(-36, -87));
	p.push_back(Point_2(-81, -26));
	p.push_back(Point_2(-46, 4));
	p.push_back(Point_2(-145, 55));

	return p;
}

// Polygon in BA
/*
Polygon_2 PolygonGen::exampleFive() const
{
	Polygon_2 p;
	p.push_back(Point_2(-166, 137));
	p.push_back(Point_2(-216, 60));
	p.push_back(Point_2(-276, 206));
	p.push_back(Point_2(-118, 239));
	p.push_back(Point_2(-37, 92));
	p.push_back(Point_2(109, -13));
	p.push_back(Point_2(274, -83));
	p.push_back(Point_2(297, 25));
	p.push_back(Point_2(370, 67));
	p.push_back(Point_2(396, -133));
	p.push_back(Point_2(254, -260));
	p.push_back(Point_2(65, -164));
	p.push_back(Point_2(-93, -256));
	p.push_back(Point_2(-394, -150));
	p.push_back(Point_2(-202, -92));
	p.push_back(Point_2(-124, 58));

	return p;
}
*/

Polygon_2 PolygonGen::altIsOpen() const
{
	Polygon_2 p;
	p.push_back(Point_2(-341, 151));
	p.push_back(Point_2(-366, 67));
	p.push_back(Point_2(-441, 91));
	p.push_back(Point_2(-413, 153));
	p.push_back(Point_2(-467, 191));
	p.push_back(Point_2(-523, 323));
	p.push_back(Point_2(-489, 355));
	p.push_back(Point_2(-210, 334));
	p.push_back(Point_2(-296, 205));
	p.push_back(Point_2(-156, 236));
	p.push_back(Point_2(-72, 223));
	p.push_back(Point_2(30, 2));
	p.push_back(Point_2(134, 86));
	p.push_back(Point_2(201, 30));
	p.push_back(Point_2(174, -114));
	p.push_back(Point_2(216, -244));
	p.push_back(Point_2(162, -257));
	p.push_back(Point_2(58, -178));
	p.push_back(Point_2(-74, -174));
	p.push_back(Point_2(-130, -240));
	p.push_back(Point_2(-371, -241));
	p.push_back(Point_2(-417, -203));
	p.push_back(Point_2(-581, -137));
	p.push_back(Point_2(-406, -16));
	p.push_back(Point_2(-487, -114));
	p.push_back(Point_2(-417, -160));
	p.push_back(Point_2(-282, -177));
	p.push_back(Point_2(-74, 30));
	p.push_back(Point_2(-125, 44));
	p.push_back(Point_2(-260, -102));
	p.push_back(Point_2(-236, 113));

	return p;
}

Polygon_2 PolygonGen::openHourglass() const
{
	Polygon_2 p;
	p.push_back(Point_2(25, 157));
	p.push_back(Point_2(87, 186));
	p.push_back(Point_2(-122, 173));
	p.push_back(Point_2(-137, 190));
	p.push_back(Point_2(-145, 176));
	p.push_back(Point_2(-146, 147));
	p.push_back(Point_2(-165, -56));
	p.push_back(Point_2(-198, -79));
	p.push_back(Point_2(-200, -41));
	p.push_back(Point_2(-182, 42));
	p.push_back(Point_2(-192, 103));
	p.push_back(Point_2(-203, 112));
	p.push_back(Point_2(-229, -13));
	p.push_back(Point_2(-241, -118));
	p.push_back(Point_2(-202, -182));
	p.push_back(Point_2(-243, -247));
	p.push_back(Point_2(-215, -217));
	p.push_back(Point_2(-206, -209));
	p.push_back(Point_2(-90, -132));
	p.push_back(Point_2(-171, -227));
	p.push_back(Point_2(39, -150));
	p.push_back(Point_2(10, -219));
	p.push_back(Point_2(174, -232));
	p.push_back(Point_2(129, -167));
	p.push_back(Point_2(184, -103));
	p.push_back(Point_2(239, -76));
	p.push_back(Point_2(235, -66));
	p.push_back(Point_2(208, -53));
	p.push_back(Point_2(215, -39));
	p.push_back(Point_2(238, -1));
	p.push_back(Point_2(240, 146));
	p.push_back(Point_2(145, 74));
	p.push_back(Point_2(87, 113));
	p.push_back(Point_2(79, 10));
	p.push_back(Point_2(113, -72));
	p.push_back(Point_2(37, -134));
	p.push_back(Point_2(-1, -20));
	p.push_back(Point_2(-32, 38));
	p.push_back(Point_2(-34, 73));
	p.push_back(Point_2(45, 62));

	return p;
}

Polygon_2 PolygonGen::testPolygon() const
{
	Polygon_2 p;
	p.push_back(Point_2(285, -129));
	p.push_back(Point_2(119, -33));
	p.push_back(Point_2(78, -58));
	p.push_back(Point_2(148, -96));
	p.push_back(Point_2(151, -264));
	p.push_back(Point_2(368, -317));
	p.push_back(Point_2(-280, -317));
	p.push_back(Point_2(-316, -192));
	p.push_back(Point_2(-4, -221));
	p.push_back(Point_2(3, -193));
	p.push_back(Point_2(-44, -134));
	p.push_back(Point_2(-64, 50));
	p.push_back(Point_2(-150, 95));
	p.push_back(Point_2(-285, 70));
	p.push_back(Point_2(-333, 208));
	p.push_back(Point_2(-8, 320));
	p.push_back(Point_2(227, 207));
	p.push_back(Point_2(231, 253));
	p.push_back(Point_2(291, 253));
	p.push_back(Point_2(520, -312));
	p.push_back(Point_2(298, 86));
	p.push_back(Point_2(115, 120));
	p.push_back(Point_2(246, 18));
	p.push_back(Point_2(74, 119));
	p.push_back(Point_2(31, 68));
	p.push_back(Point_2(158, -10));
	p.push_back(Point_2(316, -61));
	p.push_back(Point_2(347, -86));
	p.push_back(Point_2(355, -180));

	return p;
}