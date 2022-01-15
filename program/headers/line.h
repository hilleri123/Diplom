
#pragma once


#include "point.h"
#include "vector.h"
#include "trans_matrix.h"



class Line 
{
public:
	explicit Line(Point a = Point(0,0,0), Point b = Point(0,0,0));

	Line(Point, Vector);

	Line(const Line&) = default;
	Line(Line&&) = default;
	Line& operator=(const Line&) = default;
	Line& operator=(Line&&) = default;

	Point operator()(double t) const;

	Vector dir() const;

	Point start() const;
	Point end() const;

	virtual ~Line();

protected:
	Point _start;
	Point _end;
};

