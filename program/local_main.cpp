
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

#include "roket_ode.h"

#include "runge_kuta.h"

#include <boost/program_options.hpp>


int main(int argc, char** argv)	
{	
#if 1
	std::cout << "Hi" << std::endl;
	VertexMesh mesh;
	double dlat = 0.10;
	double dlon = 0.10;
	int count = 2;
	double poslat = 0.4;
	double poslon = 0.4;
	for (double lat = 0; lat < atan(1)*8; lat += dlat) {
		for (double lon = 0; lon < atan(1)*8; lon += dlon) {
			if (count <= 0)
				break;
			if (lat > poslat && lon > poslon) {
				Point p0 = earth::geo(0, lat,      lon);
				Point p1 = earth::geo(0, lat+dlat, lon);
				Point p2 = earth::geo(0, lat,      lon+dlon);
				Point p3 = earth::geo(0, lat+dlat, lon+dlon);
				Polygon pl0(p0, p1, p2);
				Polygon pl1(p2, p3, p1);
				mesh.add_polygon(pl0);
				mesh.add_polygon(pl1);
				count--;
			}
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
#if 0
	Stage s0(250, 4000000, 171000,170000);
	Stage s1(270, 800000, 101000,100000);
	Stage s2(320, 290000, 9500+25000,25000);
	Stage s012(250, 4000000, 171000+101000+9500+25000,170000);
	Stage s12(270, 800000, 101000+9500+25000,100000);
	Roket r0(std::vector<Stage>({s0, s1, s2}));
	Roket r(std::vector<Stage>({s0, s1, s2}));
	//Roket r(std::vector<Stage>({s0, s12}));
	//Roket r(std::vector<Stage>({s012}));
#define TT_max 710.
#define P_t 340.
	RoketArgs args(r, earth::geo(0,0,0), 0.,
		       	[](double t){if (P_t > TT_max) { if (t >= TT_max) return -atan(1)*4; else return 0.;
			} else if (t < P_t) return 0.;
			else if (t < TT_max) { if ((t-P_t)*0.08 > atan(1)*2) return atan(1)*2; else return (t*P_t)*0.08;
			} else if ((t-P_t)*0.08 - (t-TT_max)*0.1 > -atan(1)*4) return -atan(1)*4; else return (t-P_t)*0.08 - (t-TT_max)*0.1; },
		       	[](double){return 0.1;});
	ArgsContainer<RoketArgs, double> c(args,0);
	std::cout << "Runge" << std::endl;
	//RungeKutta4<double, RoketArgs, double> __a( &roket_ode, c);
	RungeKutta4<double, RoketArgs, double> __a( [](RoketArgs args, double v) {return roket_ode(args, v);}, c);
	double step = 10;
	__a.set_step(step);
	for (int i = 0; i < r0.T_max() * 2. / step; i++) {
		std::cout << __a.current() << std::endl;
		__a.step();
		//std::cout << (step * i) << ", " << __a.step() << std::endl;
	}
	std::cout << s0.T_max() << " " << s1.T_max() << " " << s2.T_max() << " " << r.T_max() << std::endl;
#endif




#if 0
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
	auto& data = csv_parser_read(in);
	Function a(data);
	
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
	//std::cout << h << std::endl;
	//std::cout << a.max_time() << std::endl;


	const Conversion flatting = earth::flatting_conv();

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
			//stream << a(time).x() << " " << a(time).y() << " " << a(time).z() << std::endl;
			//stream << time << " " << point.radius() << " " << point.latitude() << " " << point.longitude() << " " << velocity << std::endl;
			stream << time << " H(" << r << ") lat(" << lat << ") lon(" << lon << ") v(" << velocity.v() << ", " << (velocity.max_rotate() * h)
			       	<< ") c(" << velocity.course() << ")" << std::endl;
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
	delete &data;
#endif
	return 0;
};

