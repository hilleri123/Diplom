
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

	Point get_new_point(const Point& start, const Point& end) const;

	void add_polygons(const WingedEdgeMesh& p);
protected:
	std::pair<bool, std::pair<Point2D, Point2D>> add_polygon(const Polygon& p) const;

	Point _start_point;
	Conversion _conversion;
	std::vector<DirectedPolygon2D> _polygons;
};

