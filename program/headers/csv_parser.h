
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
std::vector<std::pair<Point, Velocity>>& csv_parser_read(std::string file);

