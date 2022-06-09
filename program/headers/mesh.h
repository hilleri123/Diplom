
#pragma once

#include "base_mesh.h"
#include "sphere.h"
#include "metrics.h"

#include <vector>
#include <set>
#include <utility>



class VertexMesh : public BaseMesh
{
	using VertexPoint = std::pair<Point, std::set<std::size_t>>;
	using VertexList = std::vector<VertexPoint>;
public:
	VertexMesh();
	virtual ~VertexMesh() override;

	virtual bool add_polygon(const Polygon& ) override;
	virtual bool dump_to_stl(std::ostream& ) const override;
		
	virtual std::vector<Polygon> around(const Polygon& ) const override;
protected:
	VertexList _vertex_list;
};

class FaceMesh : public BaseMesh
{
	using Vertex = std::pair<Point, std::set<std::size_t>>;
	using VertexList = std::vector<Vertex>;
	using Face = std::array<std::size_t, Polygon::size>;
	using FaceList = std::vector<Face>;
public:
	FaceMesh();
	virtual ~FaceMesh() override;

	virtual bool add_polygon(const Polygon& ) override;
	virtual bool dump_to_stl(std::ostream& ) const override;

	virtual std::vector<Polygon> around(const Polygon& ) const override;

	std::size_t size() const;
	Polygon polygon_at(std::size_t idx) const;
protected:
	VertexList _vertex_list;
	FaceList _face_list;
};

class WingedEdgeMesh : public BaseMesh
{
	constexpr static const std::size_t pair_size = 2;

	using Vertex = std::pair<Point, std::set<std::size_t>>;
	using VertexList = std::vector<Vertex>;
	//< < point, point >, < polygon, polygon > > 
	using Edge = std::pair<std::array<std::size_t, pair_size>, std::set<std::size_t>>;
	using EdgeList = std::vector<Edge>;
	using Face = std::array<std::size_t, Polygon::size>;
	using FaceList = std::vector<Face>;
public:
	WingedEdgeMesh();
	virtual ~WingedEdgeMesh() override;

	virtual bool add_polygon(const Polygon& ) override;
	virtual bool dump_to_stl(std::ostream& ) const override;

	virtual std::vector<Polygon> around(const Polygon& ) const override;

	std::size_t size() const;
	Polygon polygon_at(std::size_t idx) const;
protected:
	VertexList _vertex_list;
	EdgeList _edge_list;
	FaceList _face_list;
};
