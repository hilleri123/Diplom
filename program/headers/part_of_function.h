
#pragma once

#include <memory>

#include <iostream>

//#include "../geometrics/point.h"
//#include "../geometrics/vector.h"
//#include "../init.h"
//#include "rotate.h"
#include "point.h"
#include "polygon.h"
#include "vector.h"
#include "init.h"
#include "velocity.h"
#include "rotate.h"
#include "trans_matrix.h"
#include "xy_circle.h"
#include "sphere.h"
#include "log.h"

class PartOfFunction : public base_init, public CheckSuppression
{
public:
	explicit PartOfFunction(const Point& first = *std::unique_ptr<Point>(new Point), const Point& second = *std::unique_ptr<Point>(new Point),
		       	const Velocity& v = *std::unique_ptr<Velocity>(new Velocity), const Vector& s_direction = *std::unique_ptr<Vector>(new Vector),
			const Vector& e_direction = *std::unique_ptr<Vector>(new Vector));

	PartOfFunction(const PartOfFunction&) = default;
	PartOfFunction(PartOfFunction&&) = default;
	PartOfFunction& operator=(const PartOfFunction&) = default;
	PartOfFunction& operator=(PartOfFunction&&) = default;

	virtual bool init() override;
	
	virtual std::pair<Point, Velocity> operator()(double time) const;

	virtual double max_time() const;

	virtual Vector direction() const;

	virtual Velocity stats() const;

	virtual std::vector<Point> check(const Polygon&) const override;

	virtual ~PartOfFunction() override;

protected:
	Point _begin;
	Point _end;
	Velocity _velocity;
	Vector _direction;
	Vector _end_direction;

private:
	Rotate _start;
	Rotate _climb;
	Rotate _finish;

	std::vector<BzCurve> _curves;

	double _alpha;
};
