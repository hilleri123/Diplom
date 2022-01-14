
#pragma once

#include <vector>
#include <utility>

#include <iostream>

//#include "space/functions/combinator.h"
//#include "space/functions/part_of_function.h"
//#include "space/geometrics/point.h"
//#include "space/functions/double.h"
#include "combinator.h"
#include "part_of_function.h"
#include "point.h"
#include "velocity.h"
#include "log.h"



class Interval
{
public:
	explicit Interval(double begin, double end);
	static bool in_interval(double time, const Interval interval);
	bool in(double time) const;
	double begin() const;
	double end() const;
protected:
	double _begin;
	double _end;
};




class Function
{
public:
	explicit Function(const std::vector<std::pair<Point, Velocity>>& points);
	
	//Point operator()(double time) const;
	std::pair<Point, Velocity> operator()(double time) const;

	Interval interval(std::size_t) const;

	std::size_t find_interval(double) const;

	double max_time() const;

	~Function();

protected:
	std::size_t _last_id = 0;
	bool _changed = true;

	std::vector<std::pair<PartOfFunction, Interval>> _function;
};
