
#pragma once


#include "point.h"
#include "vector.h"
#include "metrics.h"

#include <cstdarg>
#include <vector>
#include <utility>
#include <stdexcept>
#include <cmath>



class xyCircle
{
public:
//	(1-t)^3*p0+3*t*(1-t)^2*p1+3*t^2*(1-t)*p2+t^3*p3
//
	explicit xyCircle(double r, Point p = Point(), bool o = true) : _center(p), _r(r), _orientation(o) {}

	xyCircle(const xyCircle&) = default;
	xyCircle(xyCircle&&) = default;
	xyCircle& operator=(const xyCircle&) = default;
	xyCircle& operator=(xyCircle&&) = default;
	

	std::pair<Point, Point> get_tangent(const xyCircle& ) const;

	Point center() const { return _center; }
	double r() const { return _r; }
	bool role() const { return _orientation; }

	// 0 <= t <= 1
	Point operator()(Point p, double angl) const;

	double angl(Point p0, Point p1) const;


	virtual ~xyCircle() {}
protected:
	Point _center;
	double _r;
	//true - clock hands rule
	bool _orientation;
};
