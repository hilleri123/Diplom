
#include "line.h"



Line::Line(Point p0, Point p1)
	: _start(p0), _end(p1)
{}

Line::Line(Point p, Vector v)
	: _start(p), _end(p+v)
{}


Vector Line::dir() const 
{
	return Vector(_start, _end).normolize();
}


Point Line::start() const 
{
	return _start;
}

Point Line::end() const 
{
	return _end;
}

Point Line::operator()(double t) const
{
	if (t < 0)
		return _start;
	else if (t > 1)
		return _end;
	return _start+t*Vector(_start, _end);
}



Line::~Line()
{}
