
#include "compose_function.h"

Interval::Interval(double begin, double end)
	: _begin(begin), _end(end)
{}

bool Interval::in_interval(double time, const Interval interval)
{
	return time <= interval._end && time >= interval._begin;
}

bool Interval::in(double time) const 
{
	return time <= _end && time >= _begin;
}

double Interval::begin() const 
{
	return _begin;
}

double Interval::end() const
{
	return _end;
}	



Function::Function(const std::vector<std::pair<Point, Velocity>>& points)
{
	//std::cout << "constructor" << std::endl;
	if (points.size() < 2)
		return;
	auto& trajectory = *combine(points);
	//std::cout << "trajectory size = " << trajectory.size() << std::endl;
	double time_b = 0;
	double time_e = 0;
	for (auto i = trajectory.begin(); i < trajectory.end(); i++) {
		//std::cout << "!" << std::endl;
		double max_time = i->max_time();
		time_e += max_time;
		//_function.push_back( std::make_pair<PartOfFunction<double, t>&, Interval<t>&>(*i, std::ref(Interval<t>(time_b, time_e))) );
		//_function.push_back( std::make_pair<PartOfFunction<double, t>&, Interval<t>&>(std::ref(*i), std::ref(interval)) );
		//auto pair = std::make_pair(*i, interval);
		//std::cout << "push_back" << std::endl;
		//_function.push_back(std::make_pair(*i, interval));
		//_function.emplace_back(pair);
		//std::cout << "begin " << time_b << " end " << time_e << std::endl;
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "begin "+std::to_string(time_b)+" end "+std::to_string(time_e));
		_function.emplace_back(*i, Interval(time_b, time_e));
		//std::cout << "pushed_back" << std::endl;
		time_b += max_time;
	}
	delete &trajectory;
}
	
#ifdef OPTIM
std::pair<Point, Velocity> Function::operator()(double time)
#else
std::pair<Point, Velocity> Function::operator()(double time) const
#endif
{
	if (_function.size() == 0) {
		//std::cerr << "null function" << std::endl;
		my_log::log_it(my_log::level::fatal, __FUNCTION_NAME__, "null function");
		return std::make_pair(Point(), Velocity());
	}
#ifdef OPTIM
        if (Interval::in_interval(time, std::get<1>(*last))) {
                const PartOfFunction& part = std::get<0>(*last);
                return part(time - std::get<1>(*last).begin());
        } else if (last+1 != _function.end())
                if (Interval::in_interval(time, std::get<1>(*(last+1)))) {
                    const PartOfFunction& part = std::get<0>(*(last+1));
                    return part(time - std::get<1>(*(last+1)).begin());
                }
#endif
	for (auto i = _function.begin(); i < _function.end(); i++) {
		//std::cout << "check" << std::endl;
		if (Interval::in_interval(time, std::get<1>(*i))) {
			//std::cout << "in interval " << std::get<1>(*i).begin() << std::endl;
			const PartOfFunction& part = std::get<0>(*i);
#ifdef OPTIM
                        last = i;
#endif
			return part(time - std::get<1>(*i).begin());
			//return std::make_pair(part(time - std::get<1>(*i).begin()), part.stats());
		}
	}
	const PartOfFunction& part = std::get<0>(_function.back());
#ifdef OPTIM
        last = i;
#endif
	//return std::make_pair(part(time), part.stats());
	return part(time);
}

Interval Function::interval(std::size_t i) const
{
	if (i < _function.size()) {
		return std::get<1>(_function.at(i));
	} else {
		return std::get<1>(_function.back());
	}
}

std::size_t Function::find_interval(double t) const
{
	for (auto i = _function.begin(); i < _function.end(); i++) {
		if (std::get<1>(*i).in(t)) {
			return i - _function.begin();
		}
	}
	return _function.size() - 1;
}

double Function::max_time() const
{
	if (_function.size() == 0)
		return 0;
	return std::get<1>(_function.back()).end();
}

Function::~Function()
{}

