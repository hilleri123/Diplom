
#include "line.h"



Line::Line(Point p0, Point p1, Matrix m)
	: _point(p0), _vector(Vector(p0,p1)), _matrix(m)
{}

Line::Line(Point p, Vector v, Matrix m)
	: _point(p0), _vector(v), _matrix(m)
{}


bool Line::init() const 
{
	return Vector::norm(_vector) != 0;
	//return !equal(Vector::norm(_vector), 0);
}

Point Line::operator()(double t) const
{}


Line::~Line()
{}
