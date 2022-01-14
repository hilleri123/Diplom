
#include "part_of_function.h"


namespace timebackstep {
	static double step = 0.01; 
}


PartOfFunction::PartOfFunction(const Point& first, const Point& second, const Velocity& v, const Vector& direction, const Vector& end_direction)
	: _begin(first), _end(second), _velocity(v), _direction(direction), _end_direction(end_direction)
{}

bool PartOfFunction::init() {
	Point O(0,0,0);	//Центр Земли
	//Маршрут rotate(AB) climb(BC) ortodorm(CD) rotate(DE)
	const Point& A = _begin;
	Point B;
	Point C;
	Point D;
	const Point& E = _end;

	//form sphere.h/sphere.cpp
	//double lat1 = 0, lat2 = 0, L = 0, z1 = 0, z2 = 0, s = 0;

	//double l = Vector(O, _begin)^Vector(O, _end)*(Point::norm(O, _begin));
	Vector ox(Point(1,0,0));
	Vector oy(Point(0,1,0));
	Vector oz(Point(0,0,1));

	//Vector OA(O, A);

	double R = _velocity.v() / _velocity.max_rotate();

	Vector normA = earth::norm(A);

	//my_log::log_it(my_log::level::info, __FUNCTION_NAME__, "normA "+normA.to_string()+" A "+A.to_string());
	//std::cout << "normA " << normA << " A " << A << std::endl;

	//Conversion con_start(&A, nullptr, &_direction, &OA);
	Conversion* con_start;
	INIT(con_start, Conversion, &A, nullptr, &_direction, &normA);
	if (con_start == nullptr) {
		//std::cerr << "first Conversion faild" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "first Conversion faild");
		return false;
	}

	//std::cout << A << " " << _direction << " " << normA << std::endl;
	//std::cout << con_start->to_matrix() << std::endl;


	Point distination = con_start->to(E);
	
#if 0
	lat1 = A.latitude();
	lat2 = E.latitude();
	L = E.longitude() - A.longitude();
	if (!inverse(lat1, lat2, L, s, z1, z2))
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "inverse didnt work A -> E");
	Vector dist_vec = earth::course_to_vec(distination, z1);
	dist_vec *= 3 * R / Vector::norm(dist_vec);
	distination = con_start->to(A) + dist_vec;

	s = 3*R;
	if (!direct(lat1, z1, s, lat2, L, z2))
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "direct didnt work A(R) ->(3R) E");
	distination.by_geo(A.radius(), lat2, A.longitude()+L);
	
	lat1 = distination.latitude();
	lat2 = E.latitude();
	L = E.longitude() - distination.longitude();
	if (!inverse(lat1, lat2, L, s, z1, z2))
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "inverse didnt work A -> E");
	//Vector dist_vec = earth::course_to_vec(distination, z1);
	dist_vec = earth::course_to_vec(distination, z1);
#endif

	distination = Point(distination.x(), distination.y(), 0);
	Vector dist_vec = Vector(con_start->to(A), distination);
	dist_vec *= 3 * R / Vector::norm(dist_vec);
	distination = con_start->to(A) + dist_vec;
	//std::cout << distination << std::endl;

	//std::cout << "start " << A << con_start->to(A) << std::endl;
	//_start = Rotate(con_start->to(A), con_start->to(_direction), distination, dist_vec, _velocity, con_start->from_matrix());
	Rotate* tmp_rotate = nullptr;
	//INIT(tmp_rotate, Rotate, con_start->to(A), con_start->to(_direction), con_start->to(distination), con_start->to(dist_vec), _velocity, con_start->from_matrix());
	INIT(tmp_rotate, Rotate, con_start->to(A), con_start->to(_direction), distination, dist_vec, _velocity, con_start->from_matrix());
	if (tmp_rotate == nullptr) {
		//std::cerr << "first rotate faild" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "first rotate faild");
		delete con_start;
		return false;
	} else {
		_start = *tmp_rotate;
		delete tmp_rotate;
	}
	if (con_start != nullptr) {
		delete con_start;
	}

	B = _start.end_point();

	//std::cout << "start " << A << con_start->to(A) << con_start->from(A) << _start(0) << std::endl;
	//std::cout << A << _start(0) << std::endl;
	assert(A == _start(0));
	//std::cout << std::get<0>(_start.line()) << " " << std::get<1>(_start.line()) << std::endl;

	//Vector OB(O, B);

	Vector normB = earth::norm(B);

	Vector tmp_direction = _start.direction();
	//Vector tmp_direction_z = OB * tmp_direction;
	Vector tmp_direction_z = normB * tmp_direction;

	//Conversion con_climb(&B, nullptr, &tmp_direction, &tmp_direction_z);
	Conversion* con_climb;
	INIT(con_climb, Conversion, &B, nullptr, &tmp_direction, &tmp_direction_z);
	if (con_climb == nullptr) {
		//std::cerr << "climb Conversion faild" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "climb Conversion faild");
		return false;
	}
	//std::cout << "init " << con_climb->init() << std::endl;
	//std::cout << "OB " << OB << " to(" << con_climb->to(OB) << ") from(" << con_climb->from(OB) << ")" << std::endl;
	//Vector H = OB.normolize() * (earth::H(E) - earth::H(B));
	//std::cout << "B " << earth::H(B) << " E " << earth::H(E) << std::endl;
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, "B " + std::to_string(earth::H(B)) + " E " + std::to_string(earth::H(E)));
	Vector H = normB.normolize() * (earth::H(E) - earth::H(B));
	distination = con_climb->to(B) + 4 * R * oy + con_climb->to(H);


	//std::cout << "climb dist(" << earth::H(con_climb->from(distination)) << ")" << std::endl;
	//std::cout << "H " << con_climb->to(H) << " distination " << distination << std::endl;

	//_climb = Rotate(con_climb->to(B), con_climb->to(tmp_direction), distination, dist_vec, v, con_climb->from_matrix());
	INIT(tmp_rotate, Rotate, con_climb->to(B), con_climb->to(tmp_direction), distination, dist_vec, _velocity, con_climb->from_matrix());
	if (tmp_rotate == nullptr) {
		std::cerr << "first rotate faild" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "first rotate faild");
		delete con_climb;
		return false;
	} else {
		_climb = *tmp_rotate;
		delete tmp_rotate;
	}
	if (con_climb != nullptr) {
		delete con_climb;
	}

	//_climb = Rotate(trans(A), trans(direction), distination, dist_vec, v, obr);
	C = _climb.end_point();

	//_finish = Rotate(trans(D), trans(direction), trans(E), trans(end_direction), v, obr);
	D = E;

	//_curves = orthodoxy(_start.end_point(), _finish(0));
	_curves = orthodoxy(C, D, &_end_direction);

	for (auto i = _curves.begin(); i < _curves.end(); i++) {
		assert(i->get_len() != 0);
		i->set_scale(_velocity.v() / i->get_len());
	}


	//std::cout << earth::H(A) << " <= " << earth::H(B) << " < " << earth::H(C) << " = " << earth::H(D) << " >= " << earth::H(E) << std::endl;
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, std::to_string(earth::H(A))+" <= "+std::to_string(earth::H(B))+" < "+std::to_string(earth::H(C))+" = "+std::to_string(earth::H(D))+" >= "+std::to_string(earth::H(E)));
	//std::cout << A.radius() << " " << B.radius() << " " << C.radius() << " " << D.radius() << " " << E.radius() << std::endl;
	

	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, std::string("start ")+_start(0).to_string()+" "+_start(_start.max_time()).to_string());
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, std::string("climb ")+_climb(0).to_string()+" "+_climb(_climb.max_time()).to_string());
	for (auto i = _curves.begin(); i < _curves.end(); i++) {
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, std::string("curve ")+(*i)(0).to_string()+" "+(*i)(i->get_len()).to_string());
	}
	
	return true;
#if 0
	Point second = m_second;
	std::cout << "!!!!! first " << first << " second " << second << " dir " << (Point()+direction) << std::endl; 

	double h = m_second.radius()-first.radius();
	second.by_geo(first.radius(), second.latitude(), second.longitude());

	Vector&& fv = Vector(Point(), first);
	Vector&& sv = Vector(Point(), second);

	Vector&& sv_projection = sv - fv * cos(fv^sv);

	std::cout << "sv_projection " << (sv_projection + Point()) << std::endl;

	if (!equal(abs(cos(sv_projection^direction)), 1)) {
		Vector delta = direction.rotate(sv_projection, 2*atan(1));
		double alpha = asin(v.v() / v.max_rotate() / Vector::norm(fv));
		Vector&& norm_Q = fv.rotate(delta, alpha); 	// rotation plane's norm
		Point Q = Point() + norm_Q * cos(norm_Q^fv);

		std::cout << "Q " << Q << " radius " << Q.radius() << std::endl;

		double D = -(Q.radius() * Q.radius()); 	// Ax+By+Cz+D=0 A*A+B*B+C*C=R*R => D=-R*R
		double dr = first.x() * second.x() + first.y() * second.y() + first.z() * second.z() + D;	// Ax+By+Cz+D=dr
		if (copysign(1, D) == copysign(1, dr)) {	// on same side ( (0,0,0) and second) 
		} else {
		}

		Point F = Point() + sv * (-D / (Q.x() * second.x() + Q.y() * second.y() + Q.z() * second.z())); 	// Ax+By+Cz+D & (O, second) = F
		F = Q + Vector(Q, F) * (copysign(1, cos(fv^sv)));	// new F 


		std::cout << "F " << F << " second " << second << std::endl;

		_rotate = Rotate(first, F, v, direction);
	} else {
		_rotate = Rotate(first, first, v, direction);
	}
	std::cout << "^ " << (Vector(Point(), _rotate.end_point())^Vector(Point(), second)) << std::endl;
	_alpha = v.v() * log(fabs(1 + h / first.radius())) / ((Vector(Point(), _rotate.end_point())^Vector(Point(), second)) * first.radius());

	std::cout << "alpha " << _alpha << std::endl;
	std::cout << "R first " << first.radius() << " rotate " << _rotate.end_point().radius() << " second " << second.radius() << std::endl;
	_direction_f = sv.rotate(Vector(Point(), _rotate.end_point()), -2*atan(1));
#endif
}

	
//bool PartOfFunction::init() 
//{
	//if (_begin == _end || _velocity == 0 || !_rotate.init()) {
		//std::cout << "pof init" << std::endl;
		//return false;
	//} else {
		//return true;
	//}
	//return true;
//}

std::pair<Point, Velocity> PartOfFunction::operator()(double time) const
{


#if 0
	std::cout << "time(" << time << ") max_time(" << max_time() << ") _start.max_time(" << _start.max_time() << ") _climb.max_time(" << _climb.max_time()
	       	<< ") _finish.max_time(" << _finish.max_time() << ")" << std::endl;
#endif
	Velocity tmp_v = _velocity;

	if (time > max_time()) {
		tmp_v.set_course(earth::course(_end, _end_direction));
		return std::make_pair(_end, tmp_v);
	} else if (time < 0) {
		tmp_v.set_course(earth::course(_begin, _direction));
		return std::make_pair(_begin, tmp_v);
	}

	double t = time;
#define SET_COURSE(functr, t, v, dir) \
	Vector _tmp_tmp_vector; \
	if (t - timebackstep::step < 0) \
		_tmp_tmp_vector = dir; \
	else \
		_tmp_tmp_vector = Vector(functr(t-timebackstep::step), functr(t)); \
	v.set_course(earth::course(functr(t), _tmp_tmp_vector));


	if (t <= _start.max_time()) {
		SET_COURSE(_start, t, tmp_v, _direction)
		return std::make_pair(_start(t), tmp_v);
	} else
		t -= _start.max_time();

	if (t <= _climb.max_time()) {
		SET_COURSE(_climb, t, tmp_v, _start.direction())
		return std::make_pair(_climb(t), tmp_v);
	} else
		t -= _climb.max_time();

	for (auto i = _curves.begin(); i < _curves.end(); i++) {
		assert(!equal(i->get_len(), 0));
		//std::cout << "get_len(" << i->get_len() << ")" << std::endl;
		if (t <= i->get_len()) {
			SET_COURSE((*i), t, tmp_v, _climb.direction())
			return std::make_pair((*i)(t), tmp_v);
		} else
			t -= i->get_len();
	}

	//if (t <= _finish.max_time())
		//return std::make_pair(__finish(t), tmp_v);
	
	//std::size_t ind = static_cast<std::size_t>(time);
	//return _curves.at(ind)(time - ind);

	//if (time < _rotate.max_time()) {
		//return _rotate(time);
	//} else {
		//time -= _rotate.max_time();
	//}
	//std::cout << "line " << time << std::endl;

	//assert(equal(_rotate.end_point().radius(), _end.radius()));

	//Vector first(Point(), _rotate.end_point());
	//Vector second(Point(), _end);
	//double part = (time - _rotate.max_time()) * (_velocity / Point::norm(_rotate.end_point(), _end));
	//std::cout << "part " << part << std::endl;
	//auto&& vector = _direction_f * part;

	//!!!!!!!!!!!!!!!! mb rotation err

	//auto&& vector = first.rotate(second, time / (max_time() - _rotate.max_time()) * (second^first)) * (1 + _alpha * time);
	//auto&& vector = first.rotate(second, _velocity / _alpha / Vector::norm(first) * log(fabs(1 + _alpha*time)) ) * (1 + _alpha * time);
	//std::cout << "or " << (second^first) << " " << (_velocity / _alpha / Vector::norm(first) * log(fabs(1 + _alpha*max_time()))) << std::endl;
	//std::cout << "R " << (Vector::norm(first) * abs(_alpha*max_time())) << " r " << Vector::norm(second) << std::endl;
	//std::cout << "R " << Vector::norm(first) << " a " << _alpha << " m_t " << max_time() << std::endl;
	//std::cout << "v(" << _velocity.v() << ")/a(" <<  _alpha << ")/r(" << Vector::norm(first) << ")*log(" << log(abs(1 + _alpha*time)) << ")" << std::endl;
	//std::cout << "log = 1 + (" << (_alpha*time) << ")" << std::endl;
	//std::cout << "% " << (_velocity / _alpha / Vector::norm(first) * log(abs(1 + _alpha*time))) << " " << (time/(max_time() - _rotate.max_time()*(second^first))) << std::endl;
	//std::cout << "f " << (Vector::norm(first) * _alpha * time) << std::endl;

	//return Point();
	
#undef SET_COURSE
	return std::make_pair(_end, tmp_v);
}

double PartOfFunction::max_time() const
{
	//auto a = _rotate.end_point();
	//std::cout << "!" << (Point::norm(a, _end) / _double) << std::endl;
	//std::cout << "max time " << static_cast<t>(Point::norm(a, _end) / _double) << " between " << a.x() << " " << a.y() << " " << a.z() << " and " << _end.x() << " " << _end.y() << " " << _end.z() << std::endl;
	//return (Point::norm(_rotate.end_point(), _end) / _velocity) + _rotate.max_time();
	//Vector&& ev = Vector(Point(), _rotate.end_point()); 
	//Vector&& sv = Vector(Point(), _end);
	//std::cout << "re " << _rotate.end_point() << " s " << _end << " r " << _rotate.end_point().radius() << " " << _end.radius() << std::endl;
	//assert(equal(Vector::norm(ev), Vector::norm(sv)));
	//return ((Vector::norm(ev)*(ev^sv)) / _velocity) + _rotate.max_time();
	//std::cout << "T " <<((_end.radius() - _rotate.end_point().radius()) / _alpha / _rotate.end_point().radius()) << " " << _rotate.max_time() << std::endl;
	//return ((_end.radius() - _rotate.end_point().radius()) / _alpha / _rotate.end_point().radius()) + _rotate.max_time();
	//return 1;
	double s = 0;
	for (auto i = _curves.begin(); i < _curves.end(); i++)
		s += i->get_len();
	return _start.max_time() + _climb.max_time() + s + _finish.max_time();
	//return _start.max_time() + _climb.max_time() + _curves.size() + _finish.max_time();
}

Vector PartOfFunction::direction() const
{
	return _end_direction;
}


Velocity PartOfFunction::stats() const
{
	return _velocity;
}


PartOfFunction::~PartOfFunction()
{}

