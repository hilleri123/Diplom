

#include "sphere.h"


namespace sphere {
	//double a = 100;
	//double b = 100;
	//double f = (a - b) / a;
	//double a = 6378.388;
	double a = 6378.137;
	//double f = 1./297.;
	double f = 1./298.257223263;
	double b = a - f * a;

	double error = 0.0001;

	double split_distance = 10.;


	//double R_earth = R_EARTH;
	//double R_earth = 6400;

	//Matrix flatting = 
}


double earth::radius() {
#ifdef PLANE_EARTH
	return 0;
#endif
	return sphere::a;
}

Conversion earth::flatting_conv() {
#ifdef PLANE_EARTH
	return Conversion();
#endif
	Vector flatting(Point(1,1,sphere::b / sphere::a));
	Matrix to, from;
	Matrix::multiplay_foreward_backward(to, from, Matrix::comp, &flatting);
	return Conversion(to, from);
}

Vector earth::norm(double lat, double lon) {
#ifdef PLANE_EARTH
	return Vector(Point(0,0,1));
#endif
	Point p;
	//p.by_geo(earth::local_R(lat, lon), lat, lon);
	p.by_geo(earth::radius(), lat, lon);
	//Sphere : (x/a)^2+(y/a)^2+(z/b)^2=1
	//F'x(p)*(x-px) + F'y(p)*(y-py) + F'z(p)*(z-pz) = 0 
	//Ax+By+Cz+D=0
	//A = 2px/a^2
	double x = 2*p.x() / pow(sphere::a, 2);
	//B = 2py/a^2
	double y = 2*p.y() / pow(sphere::a, 2);
	//C = 2pz/b^2
	double z = 2*p.z() / pow(sphere::b, 2);
	Vector res(Point(x, y, z));
	res.normolize();
	return res;
}

Vector earth::norm(Point p) {
	return earth::norm(p.latitude(), p.longitude());
}

double earth::H(Point p) {
#ifdef PLANE_EARTH
	return p.z();
#endif
	return p.radius() - earth::local_R(p);
}

Point earth::geo(double h, double lat, double lon) {
#ifdef PLANE_EARTH
	return Point(sphere::a*lat, sphere::a*lon, h*100);
#endif
	Point p;
	p.by_geo(earth::local_R(lat, lon)+h, lat, lon);
	return p;
}


double earth::local_R(double lat, double lon) {
#ifdef PLANE_EARTH
	return 0;
#endif
	Point p;
	p.by_geo(earth::radius(), lat, lon);
	const Conversion flatting = earth::flatting_conv();
	return flatting.to(p).radius();
}

double earth::local_R(Point p) {
	return local_R(p.latitude(), p.longitude());
}

Vector earth_south()
{
	Point O(0,0,0);
#ifdef PLANE_EARTH
	return Vector(O, Point(0,1,0));
#else
	return Vector(O, Point(0,0,earth::radius()));
#endif
}


double earth::course(Point p, Vector v) {
#if 1
	Point O(0,0,0);
		
	Vector south = earth_south();

	//Vector new_z(second, O);
	Vector new_z = earth::norm(p);
	Vector new_y = new_z * south;

	Vector new_x = new_y * new_z;
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "New ox "+new_x.normolize().to_string()+" new oy "+new_y.normolize().to_string()+" new oz "+new_z.to_string());
		
	//Conversion conv(&second, nullptr, &new_y, &new_z);

	Conversion* conv;
	INIT(conv, Conversion, &p, nullptr, &new_y, &new_z);
	Vector ox(Point(1,0,0));

	if (conv != nullptr) {

		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "Course ox("+conv->to(ox).to_string()+")");
		v = conv->to(v);
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "Course v("+v.to_string()+") ^"+std::to_string((ox^v) * copysign(1., v.y()))+
				" atan "+std::to_string(atan2(v.y(), v.x())));
		v = Vector(Point(v.x(), v.y(), 0));

		//Vector tmp(Point(cos(z2),sin(z2),0));
		delete conv;
	} else {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "conversion faild");
		return 0;
	}

	//return atan2(v.y(), v.x());
#else

	//v = v.normolize();
	Point&& tmp_p = v.normolize()*(sphere::split_distance/10)+p;
	double lat1 = p.latitude(), lat2 = tmp_p.latitude(), L = tmp_p.longitude() - p.longitude(), z1 = 0, z2 = 0, s = 0;
	if (inverse(lat1, lat2, L, s, z1, z2)) {
		std::string tmp_str = "inv lat1(" + std::to_string(lat1) + ") lat2(" + std::to_string(lat2) + ") L(" + std::to_string(L)
			+ ") z1(" + std::to_string(z1) + ") z2(" + std::to_string(z2) + ") s(" + std::to_string(s) + ")";
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
		std::cout << p << " " << tmp_p << " ... " << tmp_str << std::endl;
		std::cout << "Course old("<<std::to_string(atan2(v.y(), v.x()))<<") new("<<std::to_string(z1)<<") s("<<s<<")"<<std::endl;
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "Course old("+std::to_string(atan2(v.y(), v.x()))+") new("+std::to_string(z1)+")");
		return z1;
	} else
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "inverse faild");
#endif
	double angle = 2*atan(1) - atan2(v.y(), v.x());
	if (abs(angle) > atan(1)*8)
		angle *= -1;
	return angle;
}

Vector earth::course_to_vec(Point p, double c) {
#if 1
	Point O(0,0,0);
	c = 2*atan(1) - c;
	if (abs(c) > atan(1)*8)
		c *= -1;
		
	Vector south = earth_south();
	//Vector new_z(second, O);
	Vector new_z = earth::norm(p);
	Vector new_y = new_z * south;
		
	//Conversion conv(&second, nullptr, &new_y, &new_z);
	Vector tmp(Point(cos(c),sin(c),0));

	Conversion* conv;
	INIT(conv, Conversion, &p, nullptr, &new_y, &new_z);

	if (conv != nullptr) {
		tmp = conv->from(tmp);
		delete conv;
	} else {
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "conversion faild");
	}

	return tmp;
#else
	Vector res;
	double lat1 = p.latitude(), lat2 = 0, L = 0, z1 = c, z2 = 0, s = sphere::split_distance/10.;
	
	if (direct(lat1, z1, s, lat2, L, z2)) {
		Vector new_z = earth::norm(p);
		Point tmp(p.radius(), lat2, p.longitude()+L);
		res = Vector(p, tmp);
		Vector new_y = new_z * res;
		Conversion* conv;
		INIT(conv, Conversion, &p, nullptr, &new_y, &new_z);
	
		if (conv != nullptr) {
			res = conv->from(Vector(Point(1, 0, 0)));
			delete conv;
		} else {
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "conversion faild");
		}

		//res = new_y * new_z;
		//return res;
	} else
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "direct faild");
	return res;
#endif
}

double earth::Fg(double h) {
	//6.67*10^-11 * 5.972*10^24 = ... * 10^13
	return 6.67 * 5.972 * pow(10., 13) / (earth::radius() + h) / (earth::radius() + h);
}

double earth::k(double h) {
	//return 0.000001;
	return 0;
	//return 10*0.23*pow(10., -0.00000000123*h);
}


bool direct(const double& lat1, const double& z1, const double& s, double& lat2, double& L, double& z2) {

	if (lat1 != lat1 || z1 != z1 || s != s) {
		//std::cerr << "nan found lat1(" << lat1 << ") z1(" << z1 << ") s(" << s << ")" << std::endl;
		std::string tmp_str = "nan found lat1(" + std::to_string(lat1) + ") z1(" + std::to_string(z1) + ") s(" + std::to_string(s) + ")";
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, tmp_str);
		return false;
	}

	//double U1_2 = atan( (1. - sphere::f) * tan(lat1) );
	double U1 = atan2( (1. - sphere::f) * sin(lat1), cos(lat1) );
	//double o1_2 = atan( tan(U1) / cos(z1) );
	double o1 = atan2( sin(U1) , cos(U1) * cos(z1) );
	double z = asin(cos(U1) * sin(z1));	//!!!!!!!!!!!!!!!!!!!!!!!!!! +- pi    =>  cos(z) switches sign

#if 0
	std::string tmp_str = "direct U1(" + std::to_string(U1) + ") sinU1(" + std::to_string(sin(U1)) + ") cosU1(" + std::to_string(cos(U1))
	       	+ ") U1_2(" + std::to_string(U1_2) + ") sinU1_2(" + std::to_string(sin(U1_2)) + ") cosU1_2(" + std::to_string(cos(U1_2))
	       	+ ") o1(" + std::to_string(o1) + ") o1_2(" + std::to_string(o1_2) + ")";
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
#endif

	//u_2 = u^2
	double u_2 = pow(cos(z), 2.) * (pow(sphere::a, 2.) - pow(sphere::b, 2.)) / pow(sphere::b, 2.);

	double A = 1. + (u_2 / 16384.) * (4096. + u_2 * (-768. + u_2 * (320. - 175. * u_2)));
	double B = (u_2 / 1024.) * (256. + u_2 * (-128. + u_2 * (74. - 47. * u_2)));

	double o = s / (sphere::b * A);

	double delta_o;

	//om2 = 2 * om
	double om2;

	//std::cout << "u_2 " << u_2 << " A " << A << " B " << B << std::endl;
	//int max = 0;

#if 0
	tmp_str = "direct2 lat1(" + std::to_string(lat1) + ") z1(" + std::to_string(z1) + ") s(" + std::to_string(s)
	       	+ ") U1(" + std::to_string(U1) + ") o1(" + std::to_string(o1) + ") z(" + std::to_string(z)
	       	+ ") u_2(" + std::to_string(u_2) + ") A(" + std::to_string(A) + ") B(" + std::to_string(B)
		+ ") o(" + std::to_string(o) + ")";
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
#endif
	do {
		om2 = 2. * o1 + o;
		delta_o = B * sin(o) * (cos(om2) + B / 4. * (cos(o) * (-1. + 2. * pow(cos(om2), 2))
				       	- B / 6. * cos(om2) * (-3. + 4. * pow(sin(o), 2)) * (-3. + 4. * pow(cos(om2), 2))));
		//o = s / (sphere::b * A) + delta_o;
		o += delta_o;

		//std::cout << "fabs(delta_o) " << fabs(delta_o) << " o " << o << std::endl;
		//max++;

	} while (less(sphere::error, fabs(delta_o))/* && max < 10*/);
	//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "o "+std::to_string(o)+" delta_o "+std::to_string(delta_o));

	//---------------------------------------


#if 0
	tmp_str = "sin(lat1) = "+std::to_string(sin(lat1)) + " cos(lat1) = " + std::to_string(cos(lat1))+
		+" sin(lat2) = "+ std::to_string((sin(U1) * cos(o) + cos(U1) * sin(o) * cos(z1))) +
		+" cos(lat2) = " + std::to_string((1. - sphere::f) * pow( (pow(sin(z), 2) + pow( (sin(U1) * sin(o) - cos(U1) * cos(o) * cos(z1)) , 2)) , 0.5)) +
		+" sin(lat2)1 = "+ std::to_string((sin(U1))/* / (1. - sphere::f)*/) +
		//+" cos(lat2)1 = " + std::to_string(cos(U1) * (1. - sphere::f));
		//+" cos(lat2)1 = " + std::to_string((1. - sphere::f) * pow( (pow(sin(z), 2) + pow( (- cos(U1)* cos(z1)) , 2)) , 0.5));
		+" cos(lat2)1 = " + std::to_string((1. - sphere::f) * pow( (pow(sin(z), 2) + pow( (- cos(U1)* cos(z1)) , 2)) , 0.5));
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
#endif
	//lat2 = atan( (sin(U1) * cos(o) + cos(U1) * sin(o) * cos(z1))
		       	  // / ((1. - sphere::f) * pow( (pow(sin(z), 2) + pow( (sin(U1) * sin(o) - cos(U1) * cos(o) * cos(z1)) , 2)) , 0.5)) );
	lat2 = atan2( (sin(U1) * cos(o) + cos(U1) * sin(o) * cos(z1)) / (1. - sphere::f) ,
		       	 pow( (pow(sin(z), 2) + pow( (sin(U1) * sin(o) - cos(U1) * cos(o) * cos(z1)) , 2)) , 0.5) );

	//double k = atan( (sin(o) * sin(z1)) / (cos(U1) * cos(o) - sin(U1) * sin(o) * cos(z1)) );
	double k = atan2( sin(o) * sin(z1) , cos(U1) * cos(o) - sin(U1) * sin(o) * cos(z1) );

	double C = (sphere::f / 16.) * pow(cos(z), 2) * (4. + sphere::f * (4. - 3. * pow(cos(z), 2)));

	L = k - (1. - C) * sphere::f * sin(z) * (o + C * sin(o) * (cos(om2) + C * cos(o) * (-1. + 2. * pow(cos(om2), 2))));

	//z2 = atan( sin(z) / (-sin(U1) * sin(o) + cos(U1) * cos(o) * cos(z1)) );
	z2 = atan2( sin(z) , -sin(U1) * sin(o) + cos(U1) * cos(o) * cos(z1) );


	return true;
}

bool inverse(const double& lat1, const double& lat2, const double& L, double& s, double& z1, double& z2) {

	if (lat1 != lat1 || lat2 != lat2 || L != L) {
		//std::cerr << "nan found lat1(" << lat1 << ") lat2(" << lat2 << ") L(" << L << ")" << std::endl;
		std::string tmp_str = "nan found lat1(" + std::to_string(lat1) + ") lat2(" + std::to_string(lat2) + ") L(" + std::to_string(L) + ")";
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, tmp_str);
		return false;
	}
	
	double k = L;
	double prev_k = 0;

	double U1 = atan2( (1. - sphere::f) * sin(lat1), cos(lat1) );
	double U2 = atan2( (1. - sphere::f) * sin(lat2), cos(lat2) );

	//std::cout << "U1 " << U1 << " U2 " << U2 << std::endl;

	double o;
	//om2 = 2 * om
	double om2;

	do {
		//equals
		double sin_o = pow( ( pow(cos(U2) * sin(k), 2) + pow( (cos(U1) * sin(U2) - sin(U1) * cos(U2) * cos(k)) , 2) ) , 0.5);
		double cos_o = sin(U1) * sin(U2) + cos(U1) * cos(U2) * cos(k);
		//o = atan(sin_o / cos_o);
		o = atan2(sin_o, cos_o);


		//double z = asin( cos(U1) * cos(U2) * sin(k) / sin(o) );
		double z = asin( round_one( cos(U1) * cos(U2) * sin(k) / sin_o ) );

		//om2 = 2 * om
		//om2 = acos( cos(o) - 2. * sin(U1) * sin(U2) / pow(cos(z), 2) );
		om2 = acos( round_one( cos_o - 2. * sin(U1) * sin(U2) / pow(cos(z), 2) ) );

		double C = (sphere::f / 16.) * pow(cos(z), 2) * (4. + sphere::f * (4. - 3. * pow(cos(z), 2)));

		//std::cout << "o " << o << " z " << z << " om2 " << om2 << " C " << C << std::endl;

		prev_k = k;
		//k = L + (1. - C) * sphere::f * sin(z) * (o + C * sin(o) * (cos(om2) + C * cos(o) * (-1. + 2. * pow(cos(om2), 2))));
		k = L + (1. - C) * sphere::f * sin(z) * (o + C * sin_o * (cos(om2) + C * cos_o * (-1. + 2. * pow(cos(om2), 2))));
		
		//std::cout << "fabs(k - prev_k) " << fabs(k - prev_k) << std::endl;

	} while (less(sphere::error, fabs(k - prev_k)));

	//z1 = atan( (cos(U2) * sin(k)) / (cos(U1) * sin(U2) - sin(U1) * cos(U2) * cos(k)) );
	z1 = atan2( cos(U2) * sin(k) , cos(U1) * sin(U2) - sin(U1) * cos(U2) * cos(k) );

	//z2 = atan( (cos(U1) * sin(k)) / (- sin(U1) * cos(U2) + cos(U1) * sin(U2) * cos(k)) );
	z2 = atan2( cos(U1) * sin(k) , - sin(U1) * cos(U2) + cos(U1) * sin(U2) * cos(k) );

	//u_2 = u^2
	double u_2 = pow(cos(z1), 2.)* (pow(sphere::a, 2.) - pow(sphere::b, 2.)) / pow(sphere::b, 2.);

	double A = 1. + (u_2 / 16384.) * (4096. + u_2 * (-768. + u_2 * (320. - 175. * u_2)));
	double B = (u_2 / 1024.) * (256. + u_2 * (-128. + u_2 * (74. - 47. * u_2)));

	double delta_o = B * sin(o) * (cos(om2) + B / 4. * (cos(o) * (-1. + 2. * pow(cos(om2), 2))
				       	- B / 6. * cos(om2) * (-3. + 4. * pow(sin(o), 2)) * (-3. + 4. * pow(cos(om2), 2))));
	s = sphere::b * A * (o - delta_o);


	return true;
}


std::vector<BzCurve> orthodoxy(const Point& first_point, const Point& second, Vector* direction) {

	std::vector<BzCurve> result;
	//std::cout << "bz curve h1(" << (first_point.radius() - earth::local_R(first_point)) << ") h2(" << (second.radius() - earth::local_R(second)) << ")" << std::endl;
	//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "first point "+first_point.to_string());

	double lat1, lat2, L;
	double s, z1, z2;

	const std::size_t N = 4;
	std::array<Point, N> tmp;
	//tmp.front() = first;

	Point first = first_point;

	do {
		tmp.fill(first);

		//lat1 = first.latitude();
		//lat2 = second.latitude();
		//L = second.longitude() - first.longitude();

		//if (!inverse(lat1, lat2, L, s, z1, z2)) {
			//std::cerr << "bool inverse(const double& lat1, const double& lat2, const double& L, double& s, double& z1, double& z2) failed" << std::endl;
		//}
		//std::cout << "S : " << s << std::endl;
		//std::cout << "z2 " << z2 << std::endl;

		//std::size_t n = static_cast<std::size_t>(s / sphere::split_distance) + 1;

		//double tmp_s = sphere::split_distance / (N - 1);
		//double tmp_tmp_s = s;

		//for (std::size_t i = 0; i < n; i++) {
		//if (i - 1 == n)
		//if (s < tmp_s)
			//tmp_s = (s - sphere::split_distance * i) / (N - 1);
			//tmp_s = s / (N - 1);

		//std::cout << "tmp_s : " << tmp_s << std::endl;

		const Conversion flatting = earth::flatting_conv();

		double curr_len = 0;

		for (std::size_t j = 0; j < N - 1; j++) {
			Point& curr = tmp.at(j);
			Point& next = tmp.at(j+1);

			lat1 = curr.latitude();
			lat2 = second.latitude();
			L = second.longitude() - curr.longitude();

			if (!inverse(lat1, lat2, L, s, z1, z2)) {
				//std::cerr << "bool inverse(const double& lat1, const double& lat2, const double& L, double& s, double& z1, double& z2) failed" << std::endl;
				std::string tmp_str = "bool inverse(const double& lat1, const double& lat2, const double& L, double& s, double& z1, double& z2) failed";
				my_log::log_it(my_log::level::error, __FUNCTION_NAME__, tmp_str);
			}
			{
				std::string tmp_str = "inv lat1(" + std::to_string(lat1) + ") lat2(" + std::to_string(lat2) + ") L(" + std::to_string(L)
				       	+ ") z1(" + std::to_string(z1) + ") z2(" + std::to_string(z2) + ") s(" + std::to_string(s) + ")";
				my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
			}
			//std::cout << "lat1 " << lat1 << " lat2 " << lat2 << " L " << L << " s " << s << std::endl;
			//std::cout << "s " << s << " z1 " << z1 << " z2 " << z2 << std::endl;

			double tmp_s = sphere::split_distance / (N - 1);
			if (s < tmp_s)
				tmp_s = s / (N - 1);

			//std::cout << "curr " << curr << std::endl;

			//lat1 = curr.latitude();

			if (!direct(lat1, z1, tmp_s, lat2, L, z2)) {
				//std::cerr << "bool direct(const double& lat1, const double& z1, const double& s, double& lat2, double& L, double& z2) failed" << std::endl;
				std::string tmp_str = "bool direct(const double& lat1, const double& z1, const double& s, double& lat2, double& L, double& z2) failed";
				my_log::log_it(my_log::level::error, __FUNCTION_NAME__, tmp_str);
			}
			{
				std::string tmp_str = "dir lat1(" + std::to_string(lat1) + ") lat2(" + std::to_string(lat2) + ") L(" + std::to_string(L)
				       	+ ") z1(" + std::to_string(z1) + ") z2(" + std::to_string(z2) + ") tmp_s(" + std::to_string(tmp_s) + ")";
				my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
			}

			//std::cout << "lat2 " << lat2 << " L " << L << " z2 " << z2 << std::endl;
			//next.by_geo(curr.radius(), lat2, curr.longitude() + L);
			//Point tmp_point;
			//double h1 = flatting.to(tmp_point.by_geo(earth::radius(), curr.latitude(), curr.longitude())).radius();
			//double h2 = flatting.to(tmp_point.by_geo(earth::radius(), lat2, curr.longitude() + L)).radius();

			double h1 = earth::local_R(curr);
			double h2 = earth::local_R(lat2, curr.longitude() + L);

			double H = (curr.radius() - h1) + h2;
			next.by_geo(H, lat2, curr.longitude() + L);
			//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "new point "+next.to_string());
			//std::cout << "bz curve " << next << " h1(" << (curr.radius() - h1) << ") h2(" << (next.radius() - h2) << ")" << std::endl;
			//z1 = z2;

			//tmp_tmp_s -= tmp_s;
			//std::cout << "z " << z1 << std::endl;
			//std::cout << "tmp tmp s " << tmp_tmp_s << std::endl;
			curr_len += tmp_s;
		}

		result.emplace_back(tmp);

		
#ifdef PLANE_EARTH
		result.back().set_len(curr_len);
#else
		result.back().set_len(curr_len * first.radius() / earth::radius());
#endif
		//result.back().set_len(curr_len);
		//result.back().set_len(tmp_s * (N - 1));

		//s -= tmp_s * (N - 1);
		
		//tmp.front() = tmp.back();
		first = tmp.back();
	} while (more(s, 0));

	if (direction != nullptr) {
		//double tmp_z2 = atan(1)*2 - z2;
		double tmp_z2 = z2;
		*direction = earth::course_to_vec(second, tmp_z2);
		//std::cout << "course " << earth::course(second, *direction) << " z2 " << tmp_z2 << std::endl;
		//std::cout << "sin " << sin(earth::course(second, *direction)) << " z " <<  sin(tmp_z2) << " cos " << cos(earth::course(second, *direction)) << " z " << cos(tmp_z2) << std::endl;

		std::string tmp_str = "dir course " + std::to_string(earth::course(second, *direction)) + " z2 course " + std::to_string(tmp_z2);
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, tmp_str);
		//std::cout << tmp_str << std::endl;
		assert(equal(sin(earth::course(second, *direction)), sin(tmp_z2)) && equal(cos(earth::course(second, *direction)), cos(tmp_z2)));
		//Point O(0,0,0);
		
		//Vector south(O, Point(0,0,earth::radius()));
		//Vector new_z(second, O);
		//Vector new_z = earth::norm(second);
		//Vector new_y = new_z * south;
		
		//Conversion conv(&second, nullptr, &new_y, &new_z);

		//Conversion* conv;
		//INIT(conv, Conversion, &second, nullptr, &new_y, &new_z);

		//Vector tmp(Point(cos(z2),sin(z2),0));

		//*direction = conv->from(tmp);

		//delete conv;
		//std::cout << *direction << std::endl;
	}
		

	return result;
}


