


#include "xy_circle.h"



	

//хотим касательные
//если this->role() == circle.role()
//то касательные(отрезки) не должны пересекаться =
//= -> a = acos((R-r)/l)
//инче касетельные(отрезки) пересекаются x
//x -> a = acos((R+r)/l)
std::pair<Point, Point> xyCircle::get_tangent(const xyCircle& circle) const
{
	double l = Point::norm(center(), circle.center());
	if (l == 0)
		return std::make_pair(Point(), Point());
	double a;
	double b;
	if (_orientation == circle.role()) {
		a = acos((r()-circle.r())/l);
		b = 4*atan(1) - a;
	} else {
		a = acos((r()+circle.r())/l);
		b = a;
	}
	a *= -1;
	return std::make_pair( (*this)(circle.center(), a), circle(center(), b));
}


Point xyCircle::operator()(Point p, double angl) const
{
	double x = p.x() - _center.x();
	double y = p.y() - _center.y();

	double cur_angl = atan2(y,x);
	if (_orientation)
		cur_angl-=angl;
	else
		cur_angl+=angl;

	x = _r * cos(cur_angl) + _center.x();
	y = _r * sin(cur_angl) + _center.y();

	return Point(x, y, _center.z());
}

double xyCircle::angl(Point p0, Point p1) const
{
	double x0 = p0.x() - _center.x();
	double y0 = p0.y() - _center.y();
	double x1 = p1.x() - _center.x();
	double y1 = p1.y() - _center.y();


	double angl0 = atan2(y0,x0);
	double angl1 = atan2(y1,x1);

	double res = 0;

	if (_orientation)
		res = angl0-angl1;
	else
		res = angl1-angl0;

	double pi = atan(1)*4;
	
	for (; res > pi*2; res -= pi*2);
	for (; res < 0; res += pi*2);
	return res;

}

