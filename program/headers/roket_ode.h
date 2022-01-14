
#pragma once

#include <functional>
#include <iostream>

#include "sphere.h"
#include "roket.h"



struct RoketArgs {
	Roket roket;
	Point p;
	double t;
	double dt = 0;
	double v = 0;
	std::function<double(double)> theta;
	std::function<double(double)> gamma;

	RoketArgs(Roket r = Roket({}), Point point = Point(0,0,0), double time = 0, double(*th)(double) = nullptr, double(*gm)(double) = nullptr)
		: roket(r), p(point), t(time), theta(th), gamma(gm)
	{}

	friend std::ostream& operator<<(std::ostream& stream, const RoketArgs& args)
	{
		return stream << earth::H(args.p) << " " << args.p << " " << args.v;
		//return stream << args.p;
	}
};

void step_roket(RoketArgs& args, double v);

//v' = F(args(t), v)
double roket_ode(RoketArgs args, double v);

RoketArgs operator+(const RoketArgs& args, double dt);

RoketArgs& operator+=(RoketArgs& args, double dt);




