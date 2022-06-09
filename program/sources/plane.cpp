
#include "plane.h"


Plane::Plane(Point p, Vector norm, Vector ox)
	: _start_point(p), _conversion(&p, &ox, nullptr, &norm)
{}

void Plane::add_polygons(const WingedEdgeMesh& p)
{
	//Polygon new_p = _conversion.to(p);
	
}

