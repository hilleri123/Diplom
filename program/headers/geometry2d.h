#pragma once

#include "metrics.h"
#include "point.h"


#include <utility>

struct Point2D
{
	explicit Point2D(double x = 0, double y = 0);
	Point2D(Point p);

	friend bool operator==(Point2D p0, Point2D p1);
	friend bool operator!=(Point2D p0, Point2D p1);
	double x, y;
};

class LineSegment2D;
using Line2DIntersection = std::pair<std::pair<bool, Point2D>, std::pair<bool, LineSegment2D>>;
struct LineSegment2D
{
	LineSegment2D(Point p3d0, Point p3d1);
	explicit LineSegment2D(Point2D p2d0 = Point2D(), Point2D p2d1 = Point2D());


	Line2DIntersection suppression(LineSegment2D line) const;
	bool in(const Point2D& p) const;
	
	const double& k() const { return _k; }
	const double& b() const { return _b; }
	const bool& err() const { return _err; }
	Point2D p0, p1;
protected:
	double _k, _b;
	bool _err = false;
};


