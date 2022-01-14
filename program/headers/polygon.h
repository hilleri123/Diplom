
#pragma once

#include "point.h"
#include "vector.h"
#include "metrics.h"
#include "line.h"

#include <array>
#include <utility>

class Polygon
{
public:
	Polygon(Point, Point, Point);
	//Polygon(Point, Vector);

	Polygon(const Polygon&) = default;
	Polygon(Polygon&&) = default;
	Polygon& operator=(const Polygon&) = default;
	Polygon& operator=(Polygon&&) = default;

	Vector get_norm() const;

	Point get_point(std::size_t) const;

	constexpr static const std::size_t size = 3;
protected:
	std::array<Point, 3> _points;
};

