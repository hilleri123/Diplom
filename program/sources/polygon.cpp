#include "polygon.h"
#include <cmath>
#include <cassert>
#include <utility>

Polygon::Polygon(Point a, Point b, Point c)
	: _points({a, b, c})
{
	calc_norm();
	_D = -Ax_By_Cz(a);
}

std::pair<std::pair<bool, Point>, std::pair<bool, Line>> Polygon::suppression(const Line& line) const 
{
	std::pair<std::pair<bool, Point>, std::pair<bool, Line>> res;
	res.first.first = false;
	res.second.first = false;
	double s = Ax_By_Cz(line.start()) - _D, e = Ax_By_Cz(line.end()) - _D;
	if (s == 0 && e == 0) {
		res.second.first = true;
		res.second.second = line; //!!!!!!!!!!!!!!!!!!
	} else if (s == 0) {
		res.first.first = true;
		res.first.second = line.start();
	} else if (e == 0) {
		res.first.first = true;
		res.first.second = line.end();
	} else if (s * e < 0) {
		double t = std::abs(s) / (std::abs(s) + std::abs(e));
		assert(t <= 1 && t >= 0);
		Point p = line(t);
		assert(Ax_By_Cz(p) == _D);
		int mem = 0;
		bool ok = true;
		for (std::size_t i = 0; i < _points.size(); i++) {
			const Point& a = _points[i];
			const Point& b = _points[(i+1)%_points.size()];
			Vector v0(a, b);
			Vector v1(a, p);
			int curr = (_ABC^(v0*v1)) > 0 ? 1 : -1;
			if (mem == 0)
				mem = curr;
			else {
				ok &= mem == curr;
				mem = curr;
			}
		}
		res.first.first = ok;
		if (ok)
			res.first.second = p;
	}
	return res;
}

Vector Polygon::get_norm() const
{
	Vector res(_ABC);
	return res.normolize();
}

void Polygon::calc_norm()
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
	_ABC = Vector(Point(x,y,z));
	//_norm.normolize();
}

Point Polygon::get_point(std::size_t idx) const
{
	if (idx >= _points.size())
		throw std::invalid_argument("idx must be less then "+std::to_string(_points.size()));
	return _points[idx];
}

double Polygon::Ax_By_Cz(Point p) const
{
	return _ABC.x()*p.x() + _ABC.y()*p.y() + _ABC.z()*p.z();
}

