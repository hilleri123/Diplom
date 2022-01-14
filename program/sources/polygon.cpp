#include "polygon.h"

Polygon::Polygon(Point a, Point b, Point c)
	: _points({a, b, c})
{}

Vector Polygon::get_norm() const
{
	std::array<double, Polygon::size> xs;
	std::array<double, Polygon::size> ys;
	std::array<double, Polygon::size> zs;
	for (std::size_t i = 0; i < _points.size(); i++) {
		xs[i] = _points[i].x();
		ys[i] = _points[i].y();
		zs[i] = _points[i].z();
	}
	double x = (ys[1]-ys[0])*(zs[2]-zs[0]) - (ys[2]-ys[0])*(zs[1]-zs[0]);
	double y = (xs[2]-xs[0])*(zs[1]-zs[0]) - (xs[1]-xs[0])*(zs[2]-zs[0]);
	double z = (xs[1]-xs[0])*(ys[2]-ys[0]) - (xs[2]-xs[0])*(ys[1]-ys[0]);
	Vector res(Point(x,y,z));
	res.normolize();
	return res;
}

Point Polygon::get_point(std::size_t idx) const
{
	if (idx >= _points.size())
		throw std::invalid_argument("idx must be less then "+std::to_string(_points.size()));
	return _points[idx];
}


