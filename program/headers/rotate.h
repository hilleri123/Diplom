
#pragma once

#include <cmath>
#include <iostream>
#include <cassert>
#include <utility>
#include <vector>

//#include "../geometrics/point.h"
//#include "../geometrics/vector.h"
//#include "velocity.h"
//#include "../init.h"
#include "point.h"
#include "vector.h"
#include "trans_matrix.h"
#include "velocity.h"
#include "init.h"
#include "bz_curve.h"
#include "xy_circle.h"
#include "log.h"
#include "sphere.h"


class Rotate : base_init
{
public:
	//explicit Rotate(Point first = Point(), Point second = Point(), Velocity v = Velocity(), Vector direction = Vector(Point(0,0,0), Point(1,0,0)), Point* center = nullptr);
	explicit Rotate(Point start = Point(), Vector start_direction = Vector(), Point distination = Point(), Vector end_direction = Vector(), Velocity v = Velocity(), Matrix m = Matrix());

	Rotate(const Rotate&) = default;
	Rotate(Rotate&&) = default;
	Rotate& operator=(const Rotate&) = default;
	Rotate& operator=(Rotate&&) = default;

	virtual bool init() override;

	virtual Point operator()(double time) const;

	virtual double max_time() const;

	virtual Point end_point() const;

	virtual Vector direction() const;

	virtual std::pair<Point, Point> line() const;

	virtual ~Rotate() override;
protected:
private:
	//bool _err = false;
	double _end_rotate;
	//Point _begin;
	Point _start;
	Point _end;
	Point _end_point;
	//Point _center;
	Velocity _velocity;
	Vector _start_direction;
	Vector _direction;

	std::size_t _line_id;
	//double _clocks_hand = 1;
	
	Matrix _matrix;

	double _R;	//radius

	//std::vector<std::pair<double, BzCurve>> _curves;
	std::vector<BzCurve> _curves;
	//double _scale = 1;
	
	//double _k = 0; // v = v0 - k * t
}; 

