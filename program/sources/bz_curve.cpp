
#include "bz_curve.h"

static std::array<Point, 4>&& make_Bz(std::array<Point, 4>&& arr)
{
	std::array<Vector, 4> v;
	for (int i = 0; i < 4; i++) 
		v[i] = Vector(arr[i]);
	arr[1] = (18.*v[1]+(-9.*v[2])+(-5.*v[0])+2.*v[3]) * (1. / 6.) + Point(0,0,0);
	arr[2] = (18.*v[2]+(-9.*v[1])+(-5.*v[3])+2.*v[0]) * (1. / 6.) + Point(0,0,0);
	return std::move(arr);
}


BzCurve::BzCurve(Point p0, Point p1, Point p2, Point p3)
	: _p(make_Bz({p0, p1, p2, p3}))
{}

BzCurve::BzCurve(std::array<Point, 4> arr)
	: _p(make_Bz(std::move(arr)))
{}

BzCurve& BzCurve::operator*=(const Matrix& m)
{
	for (auto i = _p.begin(); i < _p.end(); i++) {
		*i = m(*i);
	}
	return *this;
}


Point BzCurve::at(std::size_t id) const
{
	return _p.at(id);
}

Point BzCurve::front() const
{
	return _p.front();
}

Point BzCurve::back() const
{
	return _p.back();
}

	// 0 <= t <= 1 if _scale == 1
	// 0 <= t*_scale <= 1
Point BzCurve::operator()(double t) const
{
	std::array<Vector, 4> v;
	for (int i = 0; i < 4; i++)
		v[i] = Vector(_p[i]);
	t *= _scale;
	//(1-t)^3*p0+3t(1-t)^2*p1+3t^2(1-t)*p2+t^3*p3
	double inv_t = 1 - t;
	Point res = (pow(inv_t, 3)*v[0]
			+ 3*t*pow(inv_t, 2)*v[1]
			+ 3*pow(t, 2)*inv_t*v[2]
			+ pow(t, 3)*v[3]) + Point();
	return res;
}

std::ostream& operator<<(std::ostream& stream, const BzCurve& curve) {
	std::cout << "{";
	for (auto i = curve._p.begin(); i < curve._p.end(); i++) 
		std::cout << " " << *i;
	std::cout << " }";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, BzCurve&& curve) {
	std::cout << "{";
	for (auto i = curve._p.begin(); i < curve._p.end(); i++) 
		std::cout << " " << *i;
	std::cout << " }";
	return stream;
}

BzCurve BzCurve::make_bz_curve_manualy(std::array<Point, 4> arr)
{
	BzCurve r;
	r._p = arr;
	return r;
}

BzCurve BzCurve::make_bz_curve_manualy(Point p0, Point p1, Point p2, Point p3)
{
	BzCurve r;
	r._p = {p0,p1,p2,p3};
	return r;
}

