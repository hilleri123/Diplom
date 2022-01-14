
#pragma once

#include "base_mesh.h"
#include "sphere.h"
#include "metrics.h"

#include <vector>
#include <set>
#include <utility>



class VertexMesh : public BaseMesh
{
	using VertexList = std::vector<std::pair<Point, std::set<std::size_t>>>;
public:
	VertexMesh();
	virtual ~VertexMesh() override;

	virtual bool add_polygon(const Polygon& ) override;
	virtual bool dump_to_stl(std::ostream& ) const override;
protected:
	VertexList _vertex_list;
};
