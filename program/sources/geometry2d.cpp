
#include "geometry2d.h"
#include <cmath>


Point2D::Point2D(Point p) 
	: x(p.x()), y(p.y())
{}

Point2D::Point2D(double x, double y)
	: x(x), y(y)
{}

bool operator==(Point2D p0, Point2D p1) 
{ 
	return equal(p0.x, p1.x) && equal(p0.y, p1.y); 
}

bool operator!=(Point2D p0, Point2D p1) 
{ 
	return !(p0==p1);
}

LineSegment2D::LineSegment2D(Point p3d0, Point p3d1) 
	: p0(p3d0.x() < p3d1.x() ? p3d0 : p3d1), 
	p1(p3d0.x() < p3d1.x() ? p3d1 : p3d0)
{
	_err = p0 == p1;
	if (_err)
		return;
	_k = (p0.y-p1.y)/(p0.x-p1.y);
	_b = p0.y-_k*p0.x;
}

LineSegment2D::LineSegment2D(Point2D p2d0, Point2D p2d1) 
	: p0(p2d0.x < p2d1.x ? p2d0 : p2d1), 
	p1(p2d0.x < p2d1.x ? p2d1 : p2d0)
{
	_err = p0 == p1;
	if (_err)
		return;
	_k = (p0.y-p1.y)/(p0.x-p1.y);
	_b = p0.y-_k*p0.x;
}

Line2DIntersection LineSegment2D::suppression(LineSegment2D line) const
{
	Line2DIntersection res;
	res.first.first = false;
	res.second.first = false;

	if (equal(_k, line._k) && equal(_b, line._b)) {
		Point2D left = p0.x < line.p0.x ? line.p0 : p0;
		Point2D right = p1.x < line.p1.x ? p1 : line.p1;
		if (left.x > right.x)
			return res;
		res.second.first = true;
		res.second.second.p0 = left;
		res.second.second.p1 = right;
		return res;
	}

	Point2D res_point;
	res_point.x = (_b - line._b) / (line._k - _k);
	res_point.y = res_point.x * _k + _b;
	res.first.first = line.in(res_point) && in(res_point);
	if (res.first.first)
		res.first.second = res_point;

	return res;
}

bool LineSegment2D::in(const Point2D& p) const
{
	if (!equal(p.y, p.x*_k+_b))
		return false;
	return less(p0.x, p.x) && less(p.x, p1.x);
}

bool LineSegment2D::left(const Point2D& p) const
{
	double y = p.x*_k+_b;
	return y < p.y;
}


DirectedPolygon2D::DirectedPolygon2D(std::initializer_list<Point2D>&& list)
	: points(list)
{}





std::pair<Point2D, Point2D> DirectedPolygon2D::touches(const Point2D& sight) const
{
	std::pair<Point2D, Point2D> res;
	enum enum_pos {none, left, right};
	enum_pos prev_pos = none;
	for (std::size_t pos = 0; pos < points.size(); pos++) {
		const Point2D& p0 = points[pos];
		const Point2D& p1 = points[(pos+1)%points.size()];
		bool is_left = (p0.x < p1.x)^LineSegment2D(p0, p1).left(sight);
		enum_pos curr_pos = is_left ? left : right;
		if (prev_pos != none)
			if (curr_pos != prev_pos) {
				if (prev_pos == right)
					res.first = p0;
				else
					res.second = p0;
			}
		prev_pos = curr_pos;
	}
	return res;
}

std::pair<Point2D, Point2D> make_new(const DirectedPolygon2D& pol, const Point& sight)
{
	const std::size_t pair_size = 2;
	const double d = 1;
	std::array<Point2D, pair_size> res;
	auto tmp_t = pol.touches(sight);
	std::array<Point2D, pair_size> touches = {tmp_t.first, tmp_t.second};
	for (std::size_t pos_in_pair = 0; pos_in_pair < pair_size; pos_in_pair++) {
		const Point2D& touch = touches[pos_in_pair];
		double k = -1. / LineSegment2D(touch, sight).k();
		double b = touch.y - touch.x * k;
		double D = d / sqrt(1+k*k);
		double x_r = -touch.x + D;
		double x_l = -touch.x - D;
		Point2D p_r(x_r, k*x_r+b);
		Point2D p_l(x_l, k*x_l+b);
		LineSegment2D l_r(sight, p_r);
		LineSegment2D l_l(sight, p_l);
		for (std::size_t pos = 0; pos < pol.points.size(); pos++) {
			const Point2D& p0 = pol.points[pos];
			const Point2D& p1 = pol.points[(pos+1)%pol.points.size()];
			LineSegment2D l(p0, p1);
			if (l.suppression(l_r).first.first || l.suppression(l_r).second.first) {
				res[pos_in_pair] = p_l;
				break;
			}
			if (l.suppression(l_l).first.first || l.suppression(l_l).second.first) {
				res[pos_in_pair] = p_r;
				break;
			}
		}
	}

	return std::make_pair(res[0], res[1]);
}


