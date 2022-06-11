
#include "plane.h"


Plane::Plane(Point p, Vector norm, Vector ox)
	: _start_point(p), _conversion(&p, &ox, nullptr, &norm)
{}

std::pair<bool, std::pair<Point2D, Point2D>> Plane::add_polygon(const Polygon& p) const
{
	std::pair<bool, std::pair<Point2D, Point2D>> res; 
	res.first = false;
	bool has_plus = false;
	bool has_minus = false;
	for (std::size_t i = 0; i < Polygon::size; i++) {
		double z = _conversion.to(p.get_point(i)).z();
		has_plus |= z > 0;
		has_minus |= z < 0;
	}
	if (!(has_plus && has_minus))
		return res;
	res.first = true;

	int count = 0;
	for (std::size_t i = 0; i < Polygon::size; i++) {
		Point p0 = _conversion.to(p.get_point(i));
		Point p1 = _conversion.to(p.get_point((i+1)%Polygon::size));
		if (p0.z() * p1.z() > 0)
			continue;
		Line l(p0, p1);
		double sum_z = fabs(p1.z()-p0.z());
		Point r_p = l(fabs(p0.z())/sum_z);
		assert(equal(r_p.z(),0));
		if (count++ == 0)
			res.second.first = Point2D(r_p.x(), r_p.y());
		else
			res.second.second = Point2D(r_p.x(), r_p.y());
	}

	return res;
}
void Plane::add_polygons(const WingedEdgeMesh& mesh)
{
	std::vector<std::pair<Point2D, Point2D>> tmp;
	for (std::size_t idx = 0; idx < mesh.size(); ++idx) {
		auto r = add_polygon(mesh.polygon_at(idx));
		if (!r.first)
			continue;
		tmp.push_back(r.second);
	}
	std::vector<Point2D> res;
	while (tmp.size() > 0) {
		auto it = tmp.begin();
		if (res.size() == 0) {
			res.push_back(it->first);
			res.push_back(it->second);
			tmp.erase(it);
			continue;
		}
		for (; it != tmp.end(); ++it) {
			if (res.back() == it->second) {
				res.push_back(it->first);
				res.push_back(it->second);
				tmp.erase(it);
				break;
			}
		}
	}
	_polygons.emplace_back(res.begin(), res.end());
	//Polygon new_p = _conversion.to(p);
	
}

Point Plane::get_new_point(const Point& start, const Point& end) const
{
	Point2D p_start = _conversion.to(start);
	Point2D p_end = _conversion.to(end);
	Point2D res;
	for (const auto& p : _polygons) {
		res = add_point(p, start, end);
	}
	return _conversion.from(Point(res.x, res.y, 0));
}

