#include "mesh.h"
#include <iterator>
#include <cassert>

VertexMesh::VertexMesh() {}

VertexMesh::~VertexMesh() {}

bool VertexMesh::add_polygon(const Polygon& pl) 
{
	std::array<std::pair<Point, int>, Polygon::size> points;
	for (std::size_t idx = 0; idx < Polygon::size; idx++) {
		points[idx] = std::make_pair(pl.get_point(idx), -1);
	}
	
	std::vector<std::size_t> to_append;
	for (auto row = _vertex_list.begin(); row < _vertex_list.end(); row++) {
		for (auto p = points.begin(); p < points.end(); p++) {
			if (p->second < 0) {
				if (p->first == row->first) {
					p->second = static_cast<int>(row - _vertex_list.begin());
					to_append.push_back(p->second);
				}
			}
		}
		if (to_append.size() == points.size())
			break;
	}
	for (auto p = points.begin(); p < points.end(); p++) {
		if (p->second < 0) {
			p->second = static_cast<int>(_vertex_list.size());
			to_append.push_back(p->second);
			_vertex_list.emplace_back(p->first, std::set<std::size_t>());
		}
	}
	//ASSERT
	assert(to_append.size() == points.size());
	
	for (auto p = to_append.begin(); p < to_append.end(); p++) {
		auto& curr = _vertex_list[*p];
		curr.second.insert(to_append.begin(), p);
		curr.second.insert(p+1, to_append.end());
		//curr.second.insert(curr.second.end(), to_append.begin(), p);
		//curr.second.insert(curr.second.end(), p+1, to_append.end());
	}
	return true;
}

bool VertexMesh::dump_to_stl(std::ostream& s) const
{
	s << "solid mesh" << std::endl;
	std::set<std::size_t> printed;
	for (auto row = _vertex_list.begin(); row < _vertex_list.end(); row++) {
		//s << (row - _vertex_list.begin()) << " " << row->first << " [";
		for (auto i : row->second) {
			std::set<std::size_t> tmp;
			auto& curr = _vertex_list[i];
			std::set_intersection(row->second.begin(), row->second.end(), curr.second.begin(), curr.second.end(), std::inserter(tmp, tmp.begin()));
			assert(tmp.size() == 1);
			Polygon pl(row->first, curr.first, _vertex_list[*tmp.begin()].first);
			Vector v = pl.get_norm();
			s << "facet normal " << v.x() << " " << v.y() << " " << v.z() << std::endl;
			s << "outer loop" << std::endl;
			for (std::size_t idx = 0; idx < Polygon::size; idx++) {
				Point p = pl.get_point(idx);
				s << "vertex " << p.x() << " " << p.y() << " " << p.z() << std::endl;
			}
			s << "endloop" << std::endl;
			s << "endfacet" << std::endl;
			//s << i << ",";
		}
		printed.insert(row-_vertex_list.begin());
		//s << "]" << std::endl;

	}
	s << "endsolid mesh" << std::endl;
	return true;
}

