
#pragma once

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <functional>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <cmath>

//#include "space/geometrics/point.h"
//#include "space/functions/velocity.h"
#include "point.h"
#include "velocity.h"
#include "sphere.h"
#include "log.h"


//std::vector<std::pair<Point, Velocity>>& csv_parser_read(std::string file, double R = earth::radius());
class TrajectoryLoader
{
public:
	using trajectory_vec = std::vector<std::pair<Point, Velocity>>;

	explicit TrajectoryLoader() {}
	const trajectory_vec& trajectory() const { return _trj;}

	const trajectory_vec& csv_parser_read(std::string file);
	//std::vector<std::pair<Point, Velocity>>& csv_parser_read(std::string file);
protected:
	trajectory_vec _trj;

};
