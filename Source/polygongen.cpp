#include "polygongen.h"

PolygonGen::PolygonGen()
{
}

// Function to generate a random polygon and return the points
Polygon_2 PolygonGen::generateRandomPolygon(int size)
{
    Polygon_2 polygon;
    std::list<Point_2> point_set;
    CGAL::Random rand;

    const double RADIUS = 250;
    const int MAX_POLY_SIZE = 100;

    if (size < 4)
    {
        size = rand.get_int(4, MAX_POLY_SIZE);
    }

    // copy size points from the generator, eliminating duplicates
    CGAL::copy_n_unique(Point_generator(RADIUS), size, std::back_inserter(point_set));

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

Polygon_2 PolygonGen::exampleFive() const
{
    Polygon_2 p;
    p.push_back(Point_2(111, -169));
    p.push_back(Point_2(41, -131));
    p.push_back(Point_2(43, -158));
    p.push_back(Point_2(201, -236));
    p.push_back(Point_2(222, -220));
    p.push_back(Point_2(216, 229));
    p.push_back(Point_2(190, 206));
    p.push_back(Point_2(161, 227));
    p.push_back(Point_2(176, 240));
    p.push_back(Point_2(-26, 249));
    p.push_back(Point_2(-132, 246));
    p.push_back(Point_2(-137, 228));
    p.push_back(Point_2(-178, 114));
    p.push_back(Point_2(-155, 241));
    p.push_back(Point_2(-198, 183));
    p.push_back(Point_2(-222, 120));
    p.push_back(Point_2(-187, -26));
    p.push_back(Point_2(-71, -49));
    p.push_back(Point_2(-66, -75));
    p.push_back(Point_2(-91, -192));
    p.push_back(Point_2(-202, -70));
    p.push_back(Point_2(-247, -216));
    p.push_back(Point_2(-186, -93));
    p.push_back(Point_2(-151, -245));
    p.push_back(Point_2(53, -241));
    p.push_back(Point_2(-96, -233));
    p.push_back(Point_2(-13, -201));
    p.push_back(Point_2(76, -23));
    p.push_back(Point_2(75, 31));
    p.push_back(Point_2(11, -37));
    p.push_back(Point_2(-80, -30));
    p.push_back(Point_2(-58, 94));
    p.push_back(Point_2(-79, 197));
    p.push_back(Point_2(-64, 197));
    p.push_back(Point_2(37, 219));
    p.push_back(Point_2(58, 211));
    p.push_back(Point_2(156, 223));
    p.push_back(Point_2(102, 214));
    p.push_back(Point_2(-14, 171));
    p.push_back(Point_2(-24, 134));
    p.push_back(Point_2(114, 135));
    p.push_back(Point_2(83, 112));
    p.push_back(Point_2(120, 134));
    p.push_back(Point_2(193, 127));
    p.push_back(Point_2(128, 112));
    p.push_back(Point_2(112, 94));
    p.push_back(Point_2(56, 119));
    p.push_back(Point_2(38, 91));
    p.push_back(Point_2(-39, 100));
    p.push_back(Point_2(80, 74));
    p.push_back(Point_2(89, -51));
    p.push_back(Point_2(152, 5));
    p.push_back(Point_2(160, 21));
    p.push_back(Point_2(204, 144));
    p.push_back(Point_2(200, 4));
    p.push_back(Point_2(99, -76));
    p.push_back(Point_2(35, -113));
    p.push_back(Point_2(117, -154));
    p.push_back(Point_2(132, -140));
    p.push_back(Point_2(185, -196));

    return p;
}

