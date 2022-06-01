
#pragma once


#include "point.h"
#include "vector.h"
#include "metrics.h"
#include "trans_matrix.h"

#include <cstdarg>
#include <array>
#include <utility>
#include <stdexcept>



class BzCurve
{
public:
//	(1-t)^3*p0+3*t*(1-t)^2*p1+3*t^2*(1-t)*p2+t^3*p3
//
	explicit BzCurve(Point p0 = Point(), Point p1 = Point(), Point p2 = Point(), Point p3 = Point());
	BzCurve(std::array<Point, 4> arr);

	BzCurve(const BzCurve&) = default;
	BzCurve(BzCurve&&) = default;
	BzCurve& operator=(const BzCurve&) = default;
	BzCurve& operator=(BzCurve&&) = default;
	

	BzCurve& operator*=(const Matrix&);

	Point at(std::size_t id) const;
	Point front() const;
	Point back() const;

	void set_len(double len) { _scale = 1./len; }
	void set_scale(double scale) { _scale = scale; }
	double get_len() const { return 1./_scale; }
	double get_scale() const { return _scale; }

	// 0 <= t <= 1
	Point operator()(double t) const;

	friend std::ostream& operator<<(std::ostream& stream, const BzCurve& curve);
	friend std::ostream& operator<<(std::ostream& stream, BzCurve&& curve);

	static BzCurve make_bz_curve_manualy(std::array<Point, 4> arr);
	static BzCurve make_bz_curve_manualy(Point p0, Point p1, Point p2, Point p3);

	virtual ~BzCurve() {}
protected:
	std::array<Point, 4> _p;

	double _scale = 1;
};
