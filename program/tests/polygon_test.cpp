
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

//#include "function.h"
//#include "csv_parser.h"
//#include "space/functions/velocity.h"
//#include "compose_function.h"
//#include "csv_parser.h"
//#include "velocity.h"
#include "sphere.h"
#include "log.h"
#include "mesh.h"

#include "roket_ode.h"

#include "runge_kuta.h"

#include <cstdlib>
#include <boost/program_options.hpp>


int main(int argc, char** argv)	
{	
#if 0
	std::cout << "Hi" << std::endl;
	VertexMesh mesh;
	double dlat = 0.10;
	double dlon = 0.10;
	for (double lat = 0; lat < atan(1)*8; lat += dlat) {
		for (double lon = 0; lon < atan(1)*8; lon += dlon) {
			Point p0 = earth::geo(0, lat,      lon);
			Point p1 = earth::geo(0, lat+dlat, lon);
			Point p2 = earth::geo(0, lat,      lon+dlon);
			Point p3 = earth::geo(0, lat+dlat, lon+dlon);
			Polygon pl0(p0, p1, p2);
			Polygon pl1(p2, p3, p1);
			mesh.add_polygon(pl0);
			mesh.add_polygon(pl1);
		}
	}
	std::ofstream file(STL_MESH_FILE);
	mesh.dump_to_stl(file);
	file.close();
	std::cout << "Bye" << std::endl;
#endif
	Point o(0,0,0);
	Point p100(1,0,0);
	Point p010(0,1,0);
	Point p001(0,0,1);
	Point p011(0,1,1);
	Point p002(0,0,2);
	Point p003(0,0,3);
	Point p022(0,2,2);
	Point p021(0,2,1);
	Point p00_1(0,0,-1);
	Point p_110(-1,1,0);
	Point p_1_10(-1,-1,0);
	{
		Line l0(p001, p00_1);
		Polygon pl0(o, p100, p010);
		auto res = pl0.suppression(l0);
		assert(res.first.first);
		assert(res.first.second == 0);
	}
	{
		Line l0(p001, p00_1);
		Polygon pl0(p_1_10, p100, p010);
		auto res = pl0.suppression(l0);
		assert(res.first.first);
		assert(res.first.second == 0);
	}
	{
		Line l0(p001, p00_1);
		Polygon pl0(p_110, p_1_10, p100);
		auto res = pl0.suppression(l0);
		assert(res.first.first);
		assert(res.first.second == o);
	}
	{
		Line l0(p001, p00_1);
		Polygon pl0(p001, p100, p010);
		auto res = pl0.suppression(l0);
		assert(res.first.first);
		assert(res.first.second == p001);
	}
	{
		Line l0(p001, p00_1);
		Polygon pl0(p_1_10, p100, p00_1);
		auto res = pl0.suppression(l0);
		assert(res.first.first);
		assert(res.first.second == p00_1);
	}
	{
		Line l0(p001, p_1_10);
		Polygon pl0(o, p100, p010);
		auto res = pl0.suppression(l0);
		assert(!res.first.first);
	}
	{
		BzCurve c0(p003, p022, p021, p010);
		Polygon pl0(o, p100, p010);
		auto res = pl0.suppression(c0);
		assert(res.first);
		assert(res.second == p010);
	}
	{
		BzCurve c0(p003, p002, p001, p00_1);
		Polygon pl0(o, p002, p010);
		auto res = pl0.suppression(c0);
		assert(res.first);
		assert(res.second == p002);
	}
	{
		BzCurve c0(o, p010, p011, p001);
		Polygon pl0(p003, p100, p011);
		auto res = pl0.suppression(c0);
		assert(res.first);
		assert(res.second == p011);
	}
	{
		double a = 100;
		Point x(a,0,0);
		Point y(0,a,0);
		Point z(0,0,a);
		Point last(a,a,a);
		std::srand(std::time(nullptr));
		Point p1(std::rand()/((RAND_MAX + 1u)/a), std::rand()/((RAND_MAX + 1u)/a), std::rand()/((RAND_MAX + 1u)/a));
		Point p2(std::rand()/((RAND_MAX + 1u)/a), std::rand()/((RAND_MAX + 1u)/a), std::rand()/((RAND_MAX + 1u)/a));
		BzCurve c0(o, p1, p2, last);
		Polygon pl0(x, y, z);
		auto res = pl0.suppression(c0);
		assert(res.first);
		std::cout << res.second << std::endl;
	}
}
