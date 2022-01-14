
#include "roket_ode.h"

//#define __MY_DEBUG


void step_roket(RoketArgs& args, double v)
{
#ifdef __MY_DEBUG
	std::cout << "step_roket" << std::endl;
#endif
	args.v = v;
	double t = args.t + args.dt;
	Point tmp;
	double lat0, z0, s, lat1, L, z1;
	lat0 = args.p.latitude();
	z0 = args.gamma(t);
	s = sin(args.theta(t)) * v * args.dt; 
	//std::cout << "s " << s << " sin " << sin(args.theta(t)) << std::endl;

	direct(lat0, z0, s, lat1, L, z1);
	//std::cout << "v " << v << " " << cos(args.theta(t)) << " " << args.dt << std::endl;
	tmp.by_geo(args.p.radius() + cos(args.theta(t)) * v * args.dt, lat1, args.p.longitude()+L);
	//std::cout << args.p << std::endl;
	args.p = tmp;
	//std::cout << args.p << std::endl;
#ifdef __MY_DEBUG
	std::cout << "step_roket end" << std::endl;
#endif
}

double roket_ode(RoketArgs args, double v)
{
#ifdef __MY_DEBUG
	std::cout << "roket_ode" << std::endl;
#endif
	double t = args.t + args.dt;
	step_roket(args, v);

	double res_v = args.roket.G(t) / args.roket.m(t);

	res_v -= earth::k(earth::H(args.p)) * v * v / args.roket.m(t);

	double y_v = res_v * cos(args.theta(t));
	double x_v = res_v * sin(args.theta(t));
	y_v -= earth::Fg(earth::H(args.p));
	res_v = sqrt(y_v*y_v + x_v*x_v);
	//std::cout << "res_v(" << res_v << ") y_v(" << y_v << ") x_v(" << x_v << ")" << std::endl;
	//std::cout << "theta " << args.theta(t) << std::endl;
#ifdef __MY_DEBUG
	std::cout << "roket G " << args.roket.G(t) << std::endl;
	std::cout << "roket m " << args.roket.m(t) << std::endl;
	std::cout << "Fk " << (earth::k(earth::H(args.p)) * v * v / args.roket.m(t)) << std::endl;
	std::cout << "Fg " << (earth::Fg(earth::H(args.p)) * cos(args.theta(t))) << std::endl;

	std::cout << "v(" << v << ") dv(" << res_v << ")" << std::endl;

	std::cout << "roket_ode end" << std::endl;
#endif
	return res_v;
}

RoketArgs operator+(const RoketArgs& args, double dt)
{
#ifdef __MY_DEBUG
	std::cout << "operator+" << std::endl;
#endif
	RoketArgs res = args;
	if (res.dt > 0)
		res.t += res.dt;
	res.dt = dt;
#ifdef __MY_DEBUG
	std::cout << "operator+ end" << std::endl;
#endif
	return res;
}

RoketArgs& operator+=(RoketArgs& args, double dt)
{
#ifdef __MY_DEBUG
	std::cout << "operator+=" << std::endl;
#endif
	//std::cout << args.p << std::endl;
	step_roket(args, args.v);
	//std::cout << args.p << std::endl;
	args.t += dt;
	args.dt = 0;
#ifdef __MY_DEBUG
	std::cout << "operator+= end" << std::endl;
#endif
	return args;
}

