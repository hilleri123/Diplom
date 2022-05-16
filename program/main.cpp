
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
#if 1
	std::cout << "Hi" << std::endl;
	FaceMesh mesh;
	const double pi = atan(1)*4;
#if 0
	HeightTable table("tables/new_new_api_table.csv");
	double dlat = 0.04;
	double dlon = 0.04;
#else
	HeightTable table("tables/new_new_api_table.csv");
	//HeightTable table("kaka.csv");
	double dlat = 0.10;
	double dlon = 0.10;
#endif
#define PART
#ifdef PART
	double min_lat = 0.5292036732051038 - 0.12;
	double min_lon = 0.65; 
	double max_lat = 0.5292036732051038 + 0.12;
	double max_lon = 2;
#endif
	for (double lat = -pi/2; lat < pi/2.; lat += lat+dlat >= pi/2. ? pi/2.-lat : dlat) {
		for (double lon = 0; lon < 2*pi; lon += lon+dlon >= 2*pi ? 2*pi-lon : dlon) {
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

#endif
#if 0
	Point p0(0,0,0);
	Point p1(1,0,0);
	Point p2(0,1,0);
	Point lp0(0,0,1);
	Point lp1(0,0,-1);
	Line l0(lp0, lp1);
	Polygon pl0(p0, p1, p2);
	auto res = plo.suppression();
#endif




#if 1
	namespace po = boost::program_options;

	double h = 1;
	std::string in;
	std::string out;
	//char* in = argv[1];
	//char* out = nullptr;


	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("input", po::value<std::string>(), "input file")
		("output", po::value<std::string>(), "output file")
		("step", po::value<double>(), "time step");
		
	po::variables_map vmap;
	po::store(po::parse_command_line(argc, argv, desc), vmap);
	po::notify(vmap);

	if (vmap.count("help")) {
		std::cout << desc << std::endl;
		return 1;
	}

	if (vmap.count("step"))
		h = vmap["step"].as<double>();

	if (vmap.count("input"))
		in = vmap["input"].as<std::string>();

	if (vmap.count("output"))
		out = vmap["output"].as<std::string>();
	std::cout << "h = " << h << std::endl;
	std::cout << "in = " << in << std::endl;
	std::cout << "out = " << out << std::endl;
	std::cout << std::endl;

	if (in.empty()) {
		std::cout << "There's no input" << std::endl;
		return 0;
	}

#if 0
	if (argc < 2) {
		return 2;
	}
	in = std::string(argv[1]);
	for (int i = 2; i < argc; i++) {
		if (!strcmp(argv[i], "-h")) {
			if (i+1 < argc) {
				if (argv[i+1][0] != '-') {
					h = std::stod(argv[i+1]);
				}
			}
		} else if (!strcmp(argv[i], "-o")) {
			if (i+1 < argc) {
				if (argv[i+1][0] != '-') {
					out = std::string(argv[i+1]);
				}
			}
		}

	}
#endif

	//auto& data = csv_parser_read(in, earth::radius());
	TrajectoryLoader loader;
	auto& data = loader.csv_parser_read(in);
	Function a(data);
	
	std::cout << h << std::endl;
	std::cout << a.max_time() << std::endl;
	std::ofstream stream;
#if 0
	std::ofstream plot;
	plot.open("plot.txt");
	
	for (auto i = data.begin(); i < data.end(); i++)
		plot << std::get<0>(*i).x() << " " 
			<< std::get<0>(*i).y() << " "
			<< std::get<0>(*i).z() << std::endl;
	
	plot.close();
#endif

	//std::cout << in << std::endl;
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, in);
	if (out.size() > 0) {
		//std::cout << out << std::endl;
		my_log::log_it(my_log::level::info, __FUNCTION_NAME__, out);
		stream.open(out);
		if (!stream.is_open()) {
			throw std::ofstream::failure("outfile not open");
		}
	}
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, "Height "+std::to_string(h));
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, "Time "+std::to_string(a.max_time()));


	//const Conversion flatting = earth::flatting_conv();

	for (double time = 0; time < a.max_time()+h; time+=h) {
		auto pair = a(time);
		Point point = std::get<0>(pair);
		Velocity velocity = std::get<1>(pair);
		double lat = point.latitude();
		double lon = point.longitude();

		//Point tmp;
		//double local_R = flatting.to(tmp.by_geo(earth::radius(), lat, lon)).radius();

		double r = point.radius() - earth::local_R(lat, lon);

		if (stream.is_open()) {
			//stream << time << " " << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			stream << point.x() << " " << point.y() << " " << point.z() << std::endl;
			//stream << time << " " << point.radius() << " " << point.latitude() << " " << point.longitude() << " " << velocity << std::endl;
#if 0
			stream << time << " H(" << r << ") lat(" << lat << ") lon(" << lon << ") v(" << velocity.v() << ", " << (velocity.max_rotate() * h)
			       	<< ") c(" << velocity.course() << ")" << std::endl;
#endif
			//stream << time << " " << a(time).radius() << " " << a(time).latitude() << " " << a(time).longitude() << std::endl;
		} else {
			//std::cout << time << " " << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			//std::cout << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			//std::cout << time << " " << a(time).radius() << " " << a(time).latitude() << " " << a(time).longitude() << std::endl;
			//std::cout << time << " " << point.radius() << " " << point.latitude() << " " << point.longitude() << " " << velocity << std::endl;
			std::cout << time << " " << r << " " << lat << " " << lon << " " << velocity << std::endl;
		}
	}
	if (stream.is_open()) {
		stream.close();
	}

#if 1
	std::vector<Point> points = a.check(mesh);
	std::cout << "suppression start" << std::endl;
	for (auto point : points) {
		//std::cout << p << std::endl;
		std::cout << point.x() << " " << point.y() << " " << point.z() << std::endl;
	}
	std::cout << "suppression end" << std::endl;
#endif

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
				std::cout << point.x() << " " << point.y() << " " << point.z() << std::endl;
			}
		}
	}

		//Point tmp;
	for (auto point : points) {
		//std::cout << p << std::endl;
		std::cout << point.x() << " " << point.y() << " " << point.z() << std::endl;
	}
	std::cout << "ex suppression end" << std::endl;
#endif
#endif
	return 0;
};

