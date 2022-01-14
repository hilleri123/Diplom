
#pragma once

#include <vector>
#include <utility>

#include <iostream>

#include "part_of_function.h"
#include "velocity.h"
#include "log.h"


std::vector<PartOfFunction>* combine(const std::vector<std::pair<Point, Velocity>>& points);

