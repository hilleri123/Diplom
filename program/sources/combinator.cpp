
#include "combinator.h"


std::vector<PartOfFunction>* combine(const std::vector<std::pair<Point, Velocity>>& points)
{
	using PF = PartOfFunction;
	auto& result = *new std::vector<PF>();
	PF* part;
	
	//std::cout << "Combine" << std::endl;
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, "Combine");
	for (auto i = points.begin(); i < points.end(); i++) {
		//std::cout << std::get<0>(*i) << " v(" << std::get<1>(*i).v() << ")" << std::endl;
		const Point& point = std::get<0>(*i);
		std::string tmp = " H("+std::to_string(earth::H(point))+") lat("+std::to_string(point.latitude()*45/atan(1))+") long("+std::to_string(point.longitude()*45/atan(1))+")";
		my_log::log_it(my_log::level::info, __FUNCTION_NAME__, point.to_string() + " v(" + std::to_string(std::get<1>(*i).v()) + ") "+tmp);
	}
	
	Point fp = std::get<0>(*points.begin());
	Point sp = std::get<0>(*(points.begin()+1));
	double lat1 = fp.latitude(), lat2 = sp.latitude(), L = sp.longitude() - fp.longitude(), s = 0, z1 = 0, z2 = 0;
	inverse(lat1, lat2, L, s, z1, z2);

	//auto direction = Vector(std::get<0>(*points.begin()), std::get<0>(*(points.begin()+1)));
	//auto&& fv =  Vector(Point(), std::get<0>(*points.begin()));
	//auto&& sv =  Vector(Point(), std::get<0>(*(points.begin()+1)));
	Vector&& fv = earth::norm(std::get<0>(*points.begin()));
	Vector&& sv = earth::norm(std::get<0>(*(points.begin()+1)));
	//Vector&& fs = Vector(std::get<0>(*points.begin()), std::get<0>(*(points.begin()+1)));
	Vector&& fs_ort = fv * sv;
	//std::cout << "pre dir" << fv << sv << fs_ort << std::endl;
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "pre dir " + fv.to_string() + sv.to_string() + fs_ort.to_string());
	//Vector direction = fs_ort * fv;
	Vector direction = earth::course_to_vec(fp, z1);

	for (auto i = points.begin(); i+1 < points.end(); i++) {
		//std::cout << "direction " << direction << std::endl;
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "direction " + direction.to_string());
		//std::cout << "c v " << std::get<1>(*i).v() << ", " << std::get<1>(*i).max_rotate()  << std::endl;
		INIT (part, PF, std::get<0>(*i), std::get<0>(*(i+1)), std::get<1>(*i), direction);
		//INIT (part, PF, std::get<0>(*i), std::get<0>(*(i+1)), Velocity(std::get<1>(*i), vector));
		if (part == nullptr) {
			//std::cerr << "no part here " << (i - points.begin()) << std::endl;
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "no part here " + std::to_string(i - points.begin()));
		} else {
			//std::cout << "max_time " << part->max_time() << std::endl;
			direction = part->direction();
			result.push_back(*part);
			delete part;
		}
		//std::cout << "+part" << std::endl;
	}
	return &result;
}
