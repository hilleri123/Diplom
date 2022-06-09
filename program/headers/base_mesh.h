
#pragma once

#include "point.h"
#include "vector.h"
#include "polygon.h"

#include <ostream>

class BaseMesh {
public:
	BaseMesh() {}

	BaseMesh(const BaseMesh&) = default;
	BaseMesh(BaseMesh&&) = default;
	BaseMesh& operator=(const BaseMesh&) = default;
	BaseMesh& operator=(BaseMesh&&) = default;

	virtual bool add_polygon(const Polygon& ) = 0;
	virtual bool dump_to_stl(std::ostream&) const = 0;

	virtual std::vector<Polygon> around(const Polygon& ) const = 0;

	virtual ~BaseMesh() {}



};



