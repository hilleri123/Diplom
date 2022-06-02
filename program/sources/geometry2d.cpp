
#include "geometry2d.h"
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
	
