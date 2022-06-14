
#include <iostream>
#include <fstream>
#include <cstring>

//#include "function.h"
//#include "csv_parser.h"
//#include "space/functions/velocity.h"
#include "compose_function.h"
#include "csv_parser.h"
#include "velocity.h"
#include "sphere.h"
#include "log.h"
#include "mesh.h"
#include "plane.h"
#include "height_table.h"

#include "roket_ode.h"

#include "runge_kuta.h"

#include <boost/program_options.hpp>


static void add_to_mesh(BaseMesh* mesh, HeightTable& t, std::array<double, 4>&& lat_lon)
{
	double& lat  = lat_lon[0];
	double& dlat = lat_lon[1];
	double& lon  = lat_lon[2];
	double& dlon = lat_lon[3];
	Point p0 = earth::geo(t.height_at(lat     , lon     ), lat,      lon);
	Point p1 = earth::geo(t.height_at(lat+dlat, lon     ), lat+dlat, lon);
	Point p2 = earth::geo(t.height_at(lat     , lon+dlon), lat,      lon+dlon);
	Point p3 = earth::geo(t.height_at(lat+dlat, lon+dlon), lat+dlat, lon+dlon);
	Polygon pl0(p0, p1, p2);
	Polygon pl1(p2, p3, p1);
	mesh->add_polygon(pl0);
	mesh->add_polygon(pl1);
}


int main(int argc, char** argv)	
{	
	std::cout << "Hi" << std::endl;
	//FaceMesh mesh;
	WingedEdgeMesh mesh;
	const double pi = atan(1)*4;

	//HeightTable table("tables/new_new_api_table.csv");
	HeightTable table("tables/1109.csv");
	//HeightTable table("kaka.csv");
	double dlat = atan(1)/45;
	double dlon = atan(1)/45;
#if 0
	double lat_start = -pi/2.;
	double lat_end = pi/2.;
	double lon_start = 0;
	double lon_end = pi*2.;
#else
	double lat_start = -8*dlat;
	double lat_end = 12*dlat;
	double lon_start = -10*dlon;
	double lon_end = 5*dlon;
#endif
//#define PART
#ifdef PART
	double min_lat = 0.5292036732051038 - 0.12;
	double min_lon = 0.65; 
	double max_lat = 0.5292036732051038 + 0.12;
	double max_lon = 2;
#endif
	for (double lat = lat_start; lat < lat_end; lat += lat+dlat >= lat_end ? lat_end-lat : dlat) {
		for (double lon = lon_start; lon < lon_end; lon += lon+dlon >= lon_end ? lon_end-lon : dlon) {
			//std::cout << "add" << std::endl;
#ifdef PART
			if (min_lat <= lat && lat <= max_lat && min_lon <= lon && lon <= max_lon)
#endif
				add_to_mesh(&mesh, table, {lat, dlat, lon, dlon});
			//std::cout << "added" << std::endl;
		}
	}
	std::ofstream file(STL_MESH_FILE);
	mesh.dump_to_stl(file);
	file.close();
	std::cout << "Bye" << std::endl;


	namespace po = boost::program_options;

	double h = 1;
	std::string out = "results/pic_result.txt";
	std::cout << h << std::endl;
	std::ofstream stream(out);

	std::vector<std::pair<Point, Velocity>> data;
	Velocity v(0.3);

	std::vector<Point> trj = {
		Point(0,0,2),
		Point(-6,2,2),
		//Point(-2,6.5,2),
		//Point(5,1,2),
		//Point(5,5,2),
		//Point(2,9,2),
		Point(-10,0,2),
		//Point(0,10,2),
	};

#define CAL
#ifdef CAL
	std::array<Rotate, 2> rotates;
	Rotate *tmp_rotate;
	Vector ox(Point(1,0,0));
	Vector oy(Point(0,1,0));
	Conversion* con;
	double T = 0;
	{
		Vector s(Point(-1,0,0));
		Vector d = Vector(trj[1])-Vector(trj[0]);
		INIT(con, Conversion, &trj[0], &ox, &oy, nullptr);
		INIT(tmp_rotate, Rotate, con->to(trj[0]), con->to(s), con->to(trj[1]), con->to(Vector(Point(-2,1,0))), v, con->from_matrix());
		if (tmp_rotate != nullptr) {
			rotates[0] = *tmp_rotate;
			T += rotates[0].max_time();
		} else
			std::cout << "shit 0" << std::endl;
	}
	{
		Vector s = rotates[0].direction()*(-1);
		Vector d = Vector(trj[2])-Vector(trj[1]);
		INIT(con, Conversion, &trj[1], &ox, &oy, nullptr);
		INIT(tmp_rotate, Rotate, con->to(trj[1]), con->to(s), con->to(trj[2]), con->to(Vector(Point(-1,-1,0))), v, con->from_matrix());
		if (tmp_rotate != nullptr) {
			rotates[1] = *tmp_rotate;
			T += rotates[1].max_time();
		} else
			std::cout << "shit 1" << std::endl;
	}
	for (double t = 0; t < T; t += h) {
		Point point;
		double tmp_t = t;
		for (std::size_t i = 0; i < rotates.size(); tmp_t-=rotates[i++].max_time())
			if (tmp_t < rotates[i].max_time()) {
				point = rotates[i](tmp_t);
				break;
			}
		if (stream.is_open()) {
			stream << point.x() << " " << point.y() << " " << point.z() << std::endl;
		}
	}
	if (stream.is_open()) {
		stream.close();
	}
	return 0;
};
#else 

	Vector pl_n(Point(0,0,1)), pl_x(Point(1,0,0));

	for (const Point& p : trj)
		data.push_back(std::make_pair(p, v));
	//auto& data = csv_parser_read(in, earth::radius());
	//TrajectoryLoader loader;
	//auto& data = loader.csv_parser_read(in);
	Function a(data);
	std::cout << a.max_time() << std::endl;
	

	int count = 0;

	std::vector<Suppression> points = a.check(mesh);
	while (points.size() > 0 && count-- > 0) {
		std::cout << "suppression start" << std::endl;
		for (auto sup : points) {
			Point point = sup.sup;
			std::cout << point << std::endl;
		}
		std::cout << "suppression end" << std::endl;
	
		std::cout << "new start" << std::endl;
		Point new_point;
		bool set = false;
		for (auto sup : points) {
			Point point = sup.sup;
			Plane pl(point, pl_n, pl_x);
			pl.add_polygons(mesh);
			auto p = pl.get_new_point(sup.begin, sup.end);
			std::cout << p << std::endl;
			if (!set) {
				new_point = p;
				set = true;
			}
		}
		std::cout << "new end" << std::endl;
		const auto& sup = points.front();
		auto j = ++data.begin();
		std::cout << "data.size = " << data.size() << std::endl;
		for (auto i = data.begin(); i != data.end() && j != data.end(); ++i, ++j) {
			if (i->first == sup.begin && j->first == sup.end) {
				data.insert(j, std::make_pair(new_point, v));
				break;
			}
		}
		std::cout << "data.size = " << data.size() << std::endl;
		for (const auto& pair : data)
			std::cout << pair.first << " ";
		std::cout << std::endl;
		std::cout << a.max_time() << std::endl;
		a = Function(data);
		std::cout << a.max_time() << std::endl;
		points = a.check(mesh);
	}
	
	for (double time = 0; time < a.max_time()+h; time+=h) {
		auto pair = a(time);
		Point point = std::get<0>(pair);
		Velocity velocity = std::get<1>(pair);
		double lat = point.latitude();
		double lon = point.longitude();
		double r = point.radius() - earth::local_R(lat, lon);

		if (stream.is_open()) {
			stream << point.x() << " " << point.y() << " " << point.z() << std::endl;
		} else {
			std::cout << time << " " << r << " " << lat << " " << lon << " " << velocity << std::endl;
		}
	}
	if (stream.is_open()) {
		stream.close();
	}
#define EXTRA_SUPP
#ifdef EXTRA_SUPP
	std::cout << "ex suppression start" << std::endl;
	for (double time = 0; time < a.max_time(); time+=h) {
		Point point0 = a(time).first;
		Point point1 = a(time+h).first;
		Line l(point0, point1);
		for (std::size_t idx = 0; idx < mesh.size(); idx++) {
			Polygon p = mesh.polygon_at(idx);
			auto res = p.suppression(l);
			if (res.first.first) {
				Point point = res.first.second;
				std::cout << point << std::endl;
			}
		}
	}
	std::cout << "ex suppression end" << std::endl;
#endif
#define BASE_SUPP
#ifdef BASE_SUPP
	std::cout << "base suppression start" << std::endl;
	for (std::size_t i = 0; i < trj.size()-1; i++) {
		Point point0 = trj[i];
		Point point1 = trj[i+1];
		Line l(point0, point1);
		for (std::size_t idx = 0; idx < mesh.size(); idx++) {
			Polygon p = mesh.polygon_at(idx);
#if 0
			std::cout << "p - ";
			for (std::size_t j = 0; j < Polygon::size; j++)
				std::cout << p.get_point(j) << " ";
			std::cout << std::endl;
#endif
			auto res = p.suppression(l);
			if (res.first.first) {
				Point point = res.first.second;
				std::cout << point << std::endl;
			}
		}
	}
	std::cout << "base suppression end" << std::endl;
#endif
	return 0;
};

#endif
