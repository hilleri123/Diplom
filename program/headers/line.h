
#pragma once


#include "point.h"
#include "vector.h"
#include "trans_matrix.h"



class Line : base_init
{
public:
	explicit Line(Point, Point, Matrix m = Matrix());

	Line(Point, Vector, Matrix m = Matrix());

	Line(const Line&) = default;
	Line(Line&&) = default;
	Line& operator=(const Line&) = default;
	Line& operator=(Line&&) = default;

	virtual bool init() override;

	Point operator()(double t) const;



	virtual ~Line() override;

protected:
	Point _point;
	Vector _vector;
	Matrix _matrix;
};

