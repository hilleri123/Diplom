#include "mesh.h"
#include <iterator>
#include <cassert>
#include <algorithm>

FaceMesh::FaceMesh() {}

FaceMesh::~FaceMesh() {}

bool FaceMesh::add_polygon(const Polygon& pl) 
{
	std::size_t f_id = _face_list.size();
	std::vector<std::size_t> p_ids;
	for (std::size_t idx = 0; idx < Polygon::size; idx++) {
		Point p = pl.get_point(idx);
		//exsisted point iterator
		auto ex_p_it = std::find_if(_vertex_list.begin(), _vertex_list.end(), 
				[&p](const Vertex& v){return v.first == p;});
		if (ex_p_it == _vertex_list.end()) {
			p_ids.push_back(_vertex_list.size());
			_vertex_list.emplace_back(p, std::set<std::size_t>({f_id,}));
		} else {
			p_ids.push_back(ex_p_it-_vertex_list.begin());
			ex_p_it->second.insert(f_id);
		}
	}
	assert(p_ids.size() == Polygon::size);
	Face f;
	std::move(p_ids.begin(), p_ids.begin()+f.size(), f.begin());
	_face_list.push_back(f);
	
	return true;
}

bool FaceMesh::dump_to_stl(std::ostream& s) const
{
	s << "solid mesh" << std::endl;
	for (auto face = _face_list.begin(); face < _face_list.end(); face++) {
		auto& point_ids_arr = *face;
		const Point& p0 = _vertex_list[point_ids_arr[0]].first;
		const Point& p1 = _vertex_list[point_ids_arr[1]].first;
		const Point& p2 = _vertex_list[point_ids_arr[2]].first;
		Polygon pl(p0, p1, p2);
		Vector v = pl.get_norm();
		s << "facet normal " << v.x() << " " << v.y() << " " << v.z() << std::endl;
		s << "outer loop" << std::endl;
		for (std::size_t idx = 0; idx < Polygon::size; idx++) {
			Point p = pl.get_point(idx);
			s << "vertex " << p.x() << " " << p.y() << " " << p.z() << std::endl;
		}
		s << "endloop" << std::endl;
		s << "endfacet" << std::endl;

	}
	s << "endsolid mesh" << std::endl;
	return true;
}

std::size_t FaceMesh::size() const
{
	return _face_list.size();
}

Polygon FaceMesh::polygon_at(std::size_t idx) const
{
	auto& point_ids_arr = _face_list[idx];
	const Point& p0 = _vertex_list[point_ids_arr[0]].first;
	const Point& p1 = _vertex_list[point_ids_arr[1]].first;
	const Point& p2 = _vertex_list[point_ids_arr[2]].first;
	return Polygon(p0, p1, p2);
}
