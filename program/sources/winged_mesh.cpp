#include "mesh.h"
#include <iterator>
#include <cassert>
#include <algorithm>

WingedEdgeMesh::WingedEdgeMesh() {}

WingedEdgeMesh::~WingedEdgeMesh() {}

bool WingedEdgeMesh::add_polygon(const Polygon& pl) 
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
			_vertex_list.emplace_back(p, std::set<std::size_t>({}));
		} else {
			p_ids.push_back(ex_p_it-_vertex_list.begin());
			//ex_p_it->second.insert(f_id);
		}
	}
	assert(p_ids.size() == Polygon::size);
	std::array<std::size_t, Polygon::size> edges;
	for (std::size_t p_idx = 0; p_idx < p_ids.size(); p_idx++) {
		std::array<std::size_t, pair_size> e_p = {p_ids[p_idx], p_ids[(p_idx+1)%p_ids.size()]};
		auto ex_e_it = std::find_if(_edge_list.begin(), _edge_list.end(), 
				[&e](const Edge& v){ 
				for (std::size_t i = 0; i < v.first.size(); i++) {
					bool any = false;
					for (std::size_t j = 0; j < e_p.size(); j++) 
						any |= v.first[i] == e_p[j];
					if (!any) return false;
				}
				return true;
				});
		if (ex_e_it == _edge_list.end()) {
			for (auto p = e_p.begin(); p != e_p.end(); p++)
				p->second.insert(_edge_list.size());
			edges.push_back(_edge_list.size());
			_edge_list.emplace_back(e_p, std::set<std::size_t>({f_id}));
		} else {
			ex_e_it->second.insert(f_id);
			edges.push_back(ex_e_it-_edge_list.begin());
		}
	}
	Face f;
	std::move(edges.begin(), edges.begin()+f.size(), f.begin());
	_face_list.push_back(f);
	
	return true;
}

bool WingedEdgeMesh::dump_to_stl(std::ostream& s) const
{
	s << "solid mesh" << std::endl;
	for (auto face = _face_list.begin(); face < _face_list.end(); face++) {
#if 0
		const auto& edge_ids_arr = *face;
		std::set<std::size_t> points_ids;
		for (auto edge_id = edge_ids_arr.begin(); edge_id != edge_ids_arr.end(); ++i) {
			const Edge& e = _edge_list[*edge_id];
			for (std::size_t point_idx : e.first) {
				points_ids.insert(point_idx);
			}
		}
		std::array<std::size_t, Polygon::size> point_ids_arr;
		std::size_t pos = 0;
		for (std::size_t point_idx : points_ids) 
			point_ids_arr[pos++] = point_idx;
		const Point& p0 = _vertex_list[point_ids_arr[0]].first;
		const Point& p1 = _vertex_list[point_ids_arr[1]].first;
		const Point& p2 = _vertex_list[point_ids_arr[2]].first;
		Polygon pl(p0, p1, p2);
#endif
		
		Polygon pl = polygon_at(face - _face_list.begin());
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

std::size_t WingedEdgeMesh::size() const
{
	return _face_list.size();
}

Polygon WingedEdgeMesh::polygon_at(std::size_t idx) const
{		
	const auto& edge_ids_arr = _face_list[idx];
	std::set<std::size_t> points_ids;
	for (auto edge_id = edge_ids_arr.begin(); edge_id != edge_ids_arr.end(); ++i) {
		const Edge& e = _edge_list[*edge_id];
		for (std::size_t point_idx : e.first) {
			points_ids.insert(point_idx);
		}
	}
	std::array<std::size_t, Polygon::size> point_ids_arr;
	std::size_t pos = 0;
	for (std::size_t point_idx : points_ids) 
		point_ids_arr[pos++] = point_idx;
	const Point& p0 = _vertex_list[point_ids_arr[0]].first;
	const Point& p1 = _vertex_list[point_ids_arr[1]].first;
	const Point& p2 = _vertex_list[point_ids_arr[2]].first;
	//auto& point_ids_arr = _face_list[idx];
	//const Point& p0 = _vertex_list[point_ids_arr[0]].first;
	//const Point& p1 = _vertex_list[point_ids_arr[1]].first;
	//const Point& p2 = _vertex_list[point_ids_arr[2]].first;
	return Polygon(p0, p1, p2);
}


std::vector<Polygon> WingedEdgeMesh::around(const Polygon& ) const
{
	return std::vector<Polygon>();
}

