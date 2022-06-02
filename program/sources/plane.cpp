
#include "plane.h"


Plane::Plane(Point p, Vector norm, Vector ox)
	: _start_point(p), _conversion(&p, &ox, nullptr, &norm)
{}

void Plane::add_polygon(const Polygon& p)
{
	Polygon new_p = _conversion.to(p);
	std::array<Point2D, Polygon::size> projection;
	for (std::size_t i = 0; i < Polygon::size; ++i) {
		Point point(new_p.get_point(i).x(),new_p.get_point(i).y(),0);
		assert(equal(point.z(), 0));
		projection[i] = Point2D(point);
	}
	
}

