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
		res.first.first = in(p);
		if (res.first.first)
			res.first.second = p;
	}
	return res;
}

std::pair<bool, Point> Polygon::suppression(const BzCurve& curve) const 
{
	std::pair<bool, Point> res;
	res.first = false;

	double a = 0, b = 0, c = 0, d = 0;
#define CALC_AXIS_A(axis) \
	a += _ABC.axis() * (-curve.at(0).axis() + 3*curve.at(1).axis() - 3*curve.at(2).axis() + curve.at(3).axis());
	CALC_AXIS_A(x)
	CALC_AXIS_A(y)
	CALC_AXIS_A(z)
#undef CALC_AXIS_A
#define CALC_AXIS_B(axis) \
	b += 3*_ABC.axis() * (curve.at(0).axis() - 2*curve.at(1).axis() + curve.at(2).axis());
	CALC_AXIS_B(x)
	CALC_AXIS_B(y)
	CALC_AXIS_B(z)
#undef CALC_AXIS_B
#define CALC_AXIS_C(axis) \
	c += 3*_ABC.axis() * (-curve.at(0).axis() + curve.at(1).axis());
	CALC_AXIS_C(x)
	CALC_AXIS_C(y)
	CALC_AXIS_C(z)
#undef CALC_AXIS_C
#define CALC_AXIS_D(axis) \
	d += _ABC.axis() * curve.at(0).axis();
	CALC_AXIS_D(x)
	CALC_AXIS_D(y)
	CALC_AXIS_D(z)
#undef CALC_AXIS_D
	d += _D;
	
	std::vector<double> ts;
	if (equal(a, 0)) {
		//std::cout << "a == 0" << std::endl;
		if (equal(b,0)) {
			//std::cout << "b == 0" << std::endl;
			if (equal(c,0)) {
				//std::cout << "c == 0" << std::endl;
				return res;
			} else { // c t + d = 0
				ts.emplace_back(-d/c);
			}

		} else { //b t2 + c t + d = 0
			double D = std::pow(c, 2) - 4.*b*d;
			ts.emplace_back((-c+std::sqrt(D))/2./b);
			ts.emplace_back((-c-std::sqrt(D))/2./b);
		}
	} else { //a t3 + b t2 + c t + d = 0
		double p = (3*a*c-std::pow(b, 2)) / 3. / std::pow(a, 2);
		double q = (2*std::pow(b, 3)-9.*a*b*c+27.*std::pow(a, 2)*d) / std::pow(3.*a, 3);
		double Q = std::pow(p/3., 3) + std::pow(q/2., 2);
		double alpha = std::pow(-q/2.+std::sqrt(Q), 1./3.);
		double beta = std::pow(-q/2.-std::sqrt(Q), 1./3.);
	
		if (equal(Q, 0)) {
			//std::cout << "norm Q == 0" << std::endl; //!!!!!!!!
		} else if (more(Q, 0)) {
			//std::cout << "bad Q > 0" << std::endl; //!!!!!!!!
		} else {
			//std::cout << "good Q < 0" << std::endl; //!!!!!!!!
		}
		double g_0 = alpha + beta;
		ts.emplace_back(g_0 - b / 3. / a);
	}
	//Вернется последнее подходящее
	for (double t : ts) {
		if (t < 0 || t > 1) {
			Point point = curve(t);

			bool point_in = in(point);
			if (point_in) {
				res.first = point_in;
				res.second = point;
			}
		}
	}
	
	return res;
}

Vector Polygon::get_norm() const
{
	Vector res(_ABC);
	return res.normolize();
}

bool Polygon::in(const Point& p) const
{
	if (!equal(Ax_By_Cz(p), _D))
		return false;
	int mem = 0;
	bool ok = true;
	for (std::size_t i = 0; i < _points.size(); i++) {
		const Point& a = _points[i];
		const Point& b = _points[(i+1)%_points.size()];
		Vector v0(a, b);
		Vector v1(a, p);
		// v0 || v1 ???
		double angle = v0^v1;
		if (angle == 0 && equal_or_less(Vector::norm(v1), Vector::norm(v0))) {
			return true;
		} else if (angle == 4*atan(1)) {
			return false;
		}
		int curr = (_ABC^(v0*v1)) > 0 ? 1 : -1;
		if (mem == 0)
			mem = curr;
		else {
			ok &= mem == curr;
			mem = curr;
		}
	}
	return ok;
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


std::vector<Point> CheckSuppression::check_curves(const Polygon& p, const std::vector<BzCurve>& curves) const
{
	std::vector<Point> result;
	for (auto i = curves.begin(); i < curves.end(); ++i) {
		std::pair<bool, Point> res = p.suppression(*i);
		if (res.first)
			result.push_back(res.second);
	}
	return result;
}

