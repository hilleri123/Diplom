
#pragma once

#include "point.h"
#include "vector.h"
#include "bz_curve.h"
#include "metrics.h"
#include "line.h"

#include <array>
#include <utility>

class Polygon
{
public:
	Polygon(Point, Point, Point);
	//Polygon(Point, Vector);

	Polygon(const Polygon&) = default;
	Polygon(Polygon&&) = default;
	Polygon& operator=(const Polygon&) = default;
	Polygon& operator=(Polygon&&) = default;

	std::pair<std::pair<bool, Point>, std::pair<bool, Line>> suppression(const Line&) const;
	std::pair<bool, Point> suppression(const BzCurve&) const;

	Vector get_norm() const;

	Point get_point(std::size_t) const;

	constexpr static const std::size_t size = 3;
protected:
	bool in(const Point&) const;

	void calc_norm();
	// A*x+B*y+C*z
	double Ax_By_Cz(Point) const;

	std::array<Point, 3> _points;
	Vector _ABC;
	double _D;

};


class CheckSuppression
{
public:
	virtual std::vector<Point> check(const Polygon&) const = 0;
protected:
	std::vector<Point> check_curves(const Polygon&, const std::vector<BzCurve>& ) const;
};


