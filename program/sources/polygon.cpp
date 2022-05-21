#include "polygon.h"
#include <cmath>
#include <cassert>
#include <complex>
#include <utility>
#include <string>

Polygon::Polygon(Point a, Point b, Point c)
	: _points({a, b, c})
{
	calc_norm();
	_D = -Ax_By_Cz(a) / Vector::norm(_ABC);
	_ABC.normolize();
	//std::cout << _ABC << _D << std::endl;
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


#define D_PRINT_F(a, ...) \
	D_PRINT_A(a) << D_PRINT_E(__VA_ARGS__)
#define D_PRINT_E(a, ...) \
	D_PRINT_A(a) << D_PRINT_D(__VA_ARGS__)
#define D_PRINT_D(a, ...) \
	D_PRINT_A(a) << D_PRINT_C(__VA_ARGS__)
#define D_PRINT_C(a, ...) \
	D_PRINT_A(a) << D_PRINT_B(__VA_ARGS__)
#define D_PRINT_B(a, ...) \
	D_PRINT_A(a) << D_PRINT_A(__VA_ARGS__)
#define D_PRINT_A(a) \
	(#a+std::string(" = ")) << a << " "
	
	
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
		//std::cout << D_PRINT_D(a, b, c, d) << std::endl;
		double p = (3.*a*c-std::pow(b, 2)) / 3. / std::pow(a, 2);
		double q = (2.*std::pow(b, 3)-9.*a*b*c+27.*std::pow(a, 2)*d) / std::pow(3.*a, 3);
		double Q = std::pow(p/3., 3) + std::pow(q/2., 2);
		std::complex<double> alpha = std::pow(-q/2.+std::pow(std::complex<double>(Q),0.5), 1./3.);
		std::complex<double> beta = std::pow(-q/2.-std::pow(std::complex<double>(Q),0.5), 1./3.);
		//std::cout << D_PRINT_E(p, q, Q, alpha, beta) << std::endl;
	
		std::vector<std::complex<double>> gs;
		std::complex<double> i(0,1);
		std::complex<double> g_0 = alpha + beta;
		std::complex<double> g_1 = - (alpha + beta) / 2. + i * (alpha - beta) / 2. * std::sqrt(3);
		std::complex<double> g_2 = - (alpha + beta) / 2. - i * (alpha - beta) / 2. * std::sqrt(3);
		//std::cout << "g0 = " << g_0 << " g1 = " << g_1 << " g2 = " << g_2 << std::endl;
		//std::cout << D_PRINT_C(g_0, g_1, g_2) << std::endl;
#if 0
		if (equal(Q, 0)) {
			gs.push_back(g_0.real());
			gs.push_back(g_1.real());
			//std::cout << "norm Q == 0" << std::endl; //!!!!!!!!
		} else if (more(Q, 0)) {
			gs.push_back(g_0.real());
			//std::cout << "bad Q > 0" << std::endl; //!!!!!!!!
		} else {
			gs.push_back(g_0.real());
			gs.push_back(g_1.real());
			gs.push_back(g_2.real());
			//std::cout << "good Q < 0" << std::endl; //!!!!!!!!
		}
#endif
		gs.push_back(g_0);
		gs.push_back(g_1);
		gs.push_back(g_2);
		//for (const double& g : gs)
		for (const std::complex<double>& g : gs)
			if (equal(g.imag(), 0))
				ts.emplace_back(g.real() - b / 3. / a);
	}
	//Вернется последнее подходящее
	for (double t : ts) {
		assert(equal(Ax_By_Cz(curve(t)), _D));
		//if (t < 0 || t > 1) {
		//std::cout << t << std::endl;
		if (t >= 0 || t <= curve.get_len()) {
			Point point = curve(t);

			bool point_in = in(point);
			double x = point.x();
			double y = point.y();
			double z = point.z();
			//std::cout << D_PRINT_F(t, x, y, z, Ax_By_Cz(point), _D) << std::endl;
#define CHECK(xx,yy,zz) \
	if (xx-100 < x && x < xx+100 && yy-100 < y && y < yy+100 && zz-100 < z && z < zz+100) \
		std::cout << "!!!!!!!!!!!!!!!!!!" << std::endl;
			//CHECK(5738.68, -2871, 582.413)
			//CHECK(5863.65, -2665.83, 385.971)
			//CHECK(7981.79, -1405.43, 2092.79)
			//CHECK(7530.15, -1774.39, 1956.49)

			//if (point.radius() < 10000)
				//std::cout << D_PRINT_F(t, x, y, z, Ax_By_Cz(point), _D) << std::endl;

			if (point_in) {
				res.first = point_in;
				res.second = point;
				std::cout << "!" << point << std::endl;
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

