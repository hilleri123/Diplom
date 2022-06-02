
#pragma once


#include "trans_matrix.h"
#include "point.h"
#include "mesh.h"
#include "polygon.h"
#include "metrics.h"
#include "geometry2d.h"

#include <memory>
#include <cassert>
#include <vector>

class Plane
{
public:
	explicit Plane(Point p, Vector norm, Vector ox);

	void add_polygon(const Polygon& p);
protected:
	Point _start_point;
	Conversion _conversion;
	std::vector<Point2D> _projection_general_polygon;
};

