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

Polygon_2 PolygonGen::qOneRoot() const
{
    Polygon_2 p;
    p.push_back(Point_2(354,55));
    p.push_back(Point_2(260,161));
    p.push_back(Point_2(281,413));
    p.push_back(Point_2(-156,377));
    p.push_back(Point_2(104,160));
    p.push_back(Point_2(33,258));
    p.push_back(Point_2(91,229));
    p.push_back(Point_2(214,-44));
    p.push_back(Point_2(39,123));
    p.push_back(Point_2(95,-164));
    p.push_back(Point_2(34,24));
    p.push_back(Point_2(-138,76));
    p.push_back(Point_2(-328,366));
    p.push_back(Point_2(-219,80));
    p.push_back(Point_2(-161,-18));
    p.push_back(Point_2(-174,-97));
    p.push_back(Point_2(-416,-380));
    p.push_back(Point_2(-263,-226));
    p.push_back(Point_2(-114,-295));
    p.push_back(Point_2(-236,-360));
    p.push_back(Point_2(-72,-332));
    p.push_back(Point_2(162,-196));
    p.push_back(Point_2(159,-294));
    p.push_back(Point_2(218,-335));
    p.push_back(Point_2(345,-340));
    p.push_back(Point_2(428,-284));
    p.push_back(Point_2(428,130));
    p.push_back(Point_2(395,-41));
    p.push_back(Point_2(352,-274));
    p.push_back(Point_2(379,26));

    return p;
}

Polygon_2 PolygonGen::qOneA() const
{
    Polygon_2 p;
    p.push_back(Point_2(-138,-119));
    p.push_back(Point_2(21,6));
    p.push_back(Point_2(341,69));
    p.push_back(Point_2(420,218));
    p.push_back(Point_2(415,283));
    p.push_back(Point_2(407,335));
    p.push_back(Point_2(345,398));
    p.push_back(Point_2(374,272));
    p.push_back(Point_2(76,383));
    p.push_back(Point_2(174,254));
    p.push_back(Point_2(-198,142));
    p.push_back(Point_2(14,341));
    p.push_back(Point_2(-195,228));
    p.push_back(Point_2(-353,219));
    p.push_back(Point_2(-375,125));
    p.push_back(Point_2(-232,175));
    p.push_back(Point_2(-345,65));
    p.push_back(Point_2(-423,-19));
    p.push_back(Point_2(-305,-42));
    p.push_back(Point_2(-281,-23));
    p.push_back(Point_2(-163,-182));
    p.push_back(Point_2(-352,-214));
    p.push_back(Point_2(-364,-226));
    p.push_back(Point_2(-43,-195));
    p.push_back(Point_2(271,-406));
    p.push_back(Point_2(396,-309));
    p.push_back(Point_2(331,-188));
    p.push_back(Point_2(169,-58));
    p.push_back(Point_2(401,22));
    p.push_back(Point_2(360,54));

    return p;
}

Polygon_2 PolygonGen::qOneMiddle() const
{
    Polygon_2 p;
    p.push_back(Point_2(-308,131));
    p.push_back(Point_2(-380,136));
    p.push_back(Point_2(-54,267));
    p.push_back(Point_2(-182,352));
    p.push_back(Point_2(-392,218));
    p.push_back(Point_2(-428,103));
    p.push_back(Point_2(-390,-139));
    p.push_back(Point_2(-225,-32));
    p.push_back(Point_2(-149,-83));
    p.push_back(Point_2(-391,-218));
    p.push_back(Point_2(-67,-328));
    p.push_back(Point_2(-199,-385));
    p.push_back(Point_2(-322,-423));
    p.push_back(Point_2(297,-371));
    p.push_back(Point_2(363,-319));
    p.push_back(Point_2(425,13));
    p.push_back(Point_2(347,-275));
    p.push_back(Point_2(247,-287));
    p.push_back(Point_2(225,-282));
    p.push_back(Point_2(414,57));
    p.push_back(Point_2(298,-44));
    p.push_back(Point_2(232,-141));
    p.push_back(Point_2(133,-52));
    p.push_back(Point_2(398,335));
    p.push_back(Point_2(-22,404));
    p.push_back(Point_2(74,253));
    p.push_back(Point_2(78,-32));
    p.push_back(Point_2(-250,28));
    p.push_back(Point_2(-277,138));
    p.push_back(Point_2(-173,167));

    return p;
}

Polygon_2 PolygonGen::qTwoIntersection() const
{
	Polygon_2 p;
    p.push_back(Point_2(-300,214));
    p.push_back(Point_2(-313,198));
    p.push_back(Point_2(-377,98));
    p.push_back(Point_2(-305,85));
    p.push_back(Point_2(-330,-96));
    p.push_back(Point_2(-295,-219));
    p.push_back(Point_2(266,-413));
    p.push_back(Point_2(422,18));
    p.push_back(Point_2(163,406));
    p.push_back(Point_2(123,320));
    p.push_back(Point_2(39,363));
    p.push_back(Point_2(256,55));
    p.push_back(Point_2(292,13));
    p.push_back(Point_2(257,-71));
    p.push_back(Point_2(254,-56));
    p.push_back(Point_2(210,42));
    p.push_back(Point_2(-165,348));
    p.push_back(Point_2(-298,317));
    p.push_back(Point_2(-50,-64));
    p.push_back(Point_2(-83,-223));

	return p;
}

Polygon_2 PolygonGen::qTwoDomination() const
{
	Polygon_2 p;
    p.push_back(Point_2(236,90));
    p.push_back(Point_2(291,-28));
    p.push_back(Point_2(262,36));
    p.push_back(Point_2(215,415));
    p.push_back(Point_2(-302,396));
    p.push_back(Point_2(-402,189));
    p.push_back(Point_2(-32,313));
    p.push_back(Point_2(-74,163));
    p.push_back(Point_2(-120,94));
    p.push_back(Point_2(-133,-13));
    p.push_back(Point_2(-75,-154));
    p.push_back(Point_2(-48,-257));
    p.push_back(Point_2(-259,-283));
    p.push_back(Point_2(-192,-330));
    p.push_back(Point_2(-187,-368));
    p.push_back(Point_2(52,-423));
    p.push_back(Point_2(115,-357));
    p.push_back(Point_2(349,-83));
    p.push_back(Point_2(23,130));
    p.push_back(Point_2(113,179));

	return p;
}

Polygon_2 PolygonGen::qTwoGeneralClosed() const
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

Polygon_2 PolygonGen::qTwoGeneralAltOpen() const
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

Polygon_2 PolygonGen::qTwoGeneralOpen() const
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
    p.push_back(Point_2(218,-300));
    p.push_back(Point_2(144,-325));
    p.push_back(Point_2(239,-339));
    p.push_back(Point_2(182,-369));
    p.push_back(Point_2(401,-343));
    p.push_back(Point_2(356,-251));
    p.push_back(Point_2(388,208));
    p.push_back(Point_2(345,173));
    p.push_back(Point_2(318,115));
    p.push_back(Point_2(148,292));
    p.push_back(Point_2(224,233));
    p.push_back(Point_2(320,143));
    p.push_back(Point_2(377,251));
    p.push_back(Point_2(392,382));
    p.push_back(Point_2(296,285));
    p.push_back(Point_2(372,269));
    p.push_back(Point_2(123,307));
    p.push_back(Point_2(198,400));
    p.push_back(Point_2(-42,414));
    p.push_back(Point_2(-213,383));
    p.push_back(Point_2(-356,382));
    p.push_back(Point_2(-312,361));
    p.push_back(Point_2(-317,250));
    p.push_back(Point_2(-393,275));
    p.push_back(Point_2(-376,192));
    p.push_back(Point_2(-237,100));
    p.push_back(Point_2(-23,200));
    p.push_back(Point_2(-32,304));
    p.push_back(Point_2(9,373));
    p.push_back(Point_2(-1,310));
    p.push_back(Point_2(-16,60));
    p.push_back(Point_2(-193,-195));
    p.push_back(Point_2(-378,-32));
    p.push_back(Point_2(-423,-146));
    p.push_back(Point_2(-248,-219));
    p.push_back(Point_2(-298,-285));
    p.push_back(Point_2(-274,-347));
    p.push_back(Point_2(126,-362));
    p.push_back(Point_2(35,-298));
    p.push_back(Point_2(-137,-316));
    p.push_back(Point_2(-128,-228));
    p.push_back(Point_2(-109,-218));
    p.push_back(Point_2(-85,-212));
    p.push_back(Point_2(-147,-153));
    p.push_back(Point_2(-22,-123));
    p.push_back(Point_2(44,-126));
    p.push_back(Point_2(206,-111));
    p.push_back(Point_2(-29,-194));
    p.push_back(Point_2(248,-163));
    p.push_back(Point_2(262,-151));

	return p;
}
