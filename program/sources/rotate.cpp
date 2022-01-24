
#include "rotate.h"


//static BzCurve make_Bz(std::array<Point, 4> arr)
//{
	//std::array<Vector, 4> v(arr.begin(), arr.end());
	//arr[1] = (18*v[1]-9*v[2]-5*v[0]+2*v[3]) / 6 + Point(0,0,0);
	//arr[2] = (18*v[2]-9*v[1]-5*v[3]+2*v[0]) / 6 + Point(0,0,0);
	//return BzCurve(arr);
//}



Rotate::Rotate(Point start, Vector start_direction, Point distination, Vector end_direction, Velocity v, Matrix m)
	: _start(start), _end(distination), _velocity(v), _start_direction(start_direction), _direction(end_direction), _matrix(m)
	//: _matrix(m)
{}

bool Rotate::init()
{
	if (!(equal(_end.z(), 0) && equal(_direction.z(), 0) && equal(_start.z(), 0) && equal(_start_direction.z(), 0))) {
		//std::cout << "z must be 0 " << _start << _start_direction << _end << _direction << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "z must be 0"+_start.to_string()+_start_direction.to_string()+_end.to_string()+_direction.to_string());
		return false;
	}
	Vector ox(Point(1,0,0));
	Vector oy(Point(0,1,0));
	Vector oz(Point(0,0,1));

	//std::cout << _start << std::endl;
	_start = Point(_start.x(), _start.y(), 0);
	_end = Point(_end.x(), _end.y(), 0);

	_start_direction = Vector(Point(_start_direction.x(), _start_direction.y(), 0));
	_direction = Vector(Point(_direction.x(), _direction.y(), 0));


	Vector move_vec(_start);
	//std::cout << move_vec << std::endl;
	
	//double angle = _start_direction^oy;
	double angle = atan2(_start_direction.x(), _start_direction.y());

	Matrix move_mat_fore, move_mat_back;
	Matrix::multiplay_foreward_backward(move_mat_fore, move_mat_back, Matrix::move, &move_vec);
	Matrix rotate_mat_fore, rotate_mat_back;
	Matrix::multiplay_foreward_backward(rotate_mat_fore, rotate_mat_back, Matrix::rotate, &oz, angle);

	Matrix tmp = move_mat_back * rotate_mat_back;
	//Matrix tmp = move_mat_fore * rotate_mat_fore;
	//std::cout << tmp << std::endl;
	_direction = tmp(_direction);
	_end = tmp(_end);

	//std::cout << tmp(oy*Vector::norm(_start_direction)) << std::endl << _start_direction << std::endl;
	assert(oy*Vector::norm(_start_direction) == tmp(_start_direction));

	//_matrix = move_mat_fore * rotate_mat_fore * m;
	_matrix = _matrix * move_mat_fore * rotate_mat_fore;
	
	//std::cout << "rotate" << std::endl << _end << " " << end_direction << std::endl << _matrix << std::endl << m << std::endl;
	///////////////////////////////////

	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "START ROTATE "+_matrix(_start).to_string()+" "
			+std::to_string(earth::course(_matrix(_start),_matrix(_start_direction)))
			+" END ROTATE "+_matrix(_end).to_string()+" "+std::to_string(earth::course(_matrix(_end),_matrix(_direction))));

	_R = _velocity.v() / _velocity.max_rotate();

	//std::cout << _end << " " << _direction << " " << _R << std::endl;
	//b -begin 
	std::vector<xyCircle> b_circle;
	b_circle.emplace_back(_R, Point(0,_R,0), false);
	b_circle.emplace_back(_R, Point(0,-_R,0), true);

	xyCircle tmp_c(_R, _end);
	//lc - left circle
	Point lc = tmp_c(_end+_direction, -2*atan(1));
	//rc - right circle
	Point rc = tmp_c(_end+_direction, 2*atan(1));
	//e -end 
	//std::cout << "lc " << lc << " rc " << rc << std::endl;
	std::vector<xyCircle> e_circle;
	e_circle.emplace_back(_R, lc, false);
	e_circle.emplace_back(_R, rc, true);

	std::pair<Point, Point> line;
	double len = -1;
	xyCircle first(0);
	xyCircle second(0);
	for (auto i = b_circle.begin(); i < b_circle.end(); i++) {
		for (auto j = e_circle.begin(); j < e_circle.end(); j++) {
			std::pair<Point, Point> tmp = i->get_tangent(*j);
			if (std::get<0>(tmp) != std::get<1>(tmp)) {

				//std::cout << "center " << i->center() << " " << j->center() << std::endl;
				//std::cout << "role " << i->role() << " " << j->role() << std::endl;

				double tmp_len = 0;
				tmp_len += i->angl(Point(0,0,0), std::get<0>(tmp))*_R;
				tmp_len += Point::norm(std::get<0>(tmp), std::get<1>(tmp));
				tmp_len += j->angl(std::get<1>(tmp), _end)*_R;

				//std::cout << "b andl " << (i->angl(Point(0,0,0), std::get<0>(tmp))/atan(1)*45) << std::endl;
				//std::cout << "e andl " << (j->angl(_end, std::get<1>(tmp))/atan(1)*45) << std::endl;

				//std::cout << "len " << tmp_len << " (" << len << ")" << std::endl;
				//std::cout << "tmp line " << std::get<0>(tmp) << " " << std::get<1>(tmp) << std::endl;
				if (tmp_len < len || len < 0) {
					len = tmp_len;
					line = tmp;
					first = *i;
					second = *j;
				}
			} else if (i->role() == j->role()) {
				len = 0;
				line = std::make_pair(_end, _end);
				first = *i;
				second = *j;
				break;
			}
		}
		if (len == 0)
			break;
	}

	//std::cout << "line " << std::get<0>(line) << " " << std::get<1>(line) << std::endl;

	if (len < 0) {
		//std::cerr << "in Rotate len < 0" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "in Rotate len < 0");
		return false;
	}

	

	double scale = 1;
	std::array<Point, 4> tmp_arr;

	//std::cout << "first " << first.center() << std::endl;

	double angl = first.angl(Point(0,0,0), std::get<0>(line));
	double delta = angl / 8;
	for (double tmp_angl = 0; less(tmp_angl, angl); tmp_angl += delta) {
		for (int i = 0; i < 4; i++) {
			// 3 = 4 - 1
			tmp_arr[i] = first(Point(0,0,0), tmp_angl + static_cast<double>(i)*delta/3);
			//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "first "+tmp_arr[i].to_string()+" /"+_matrix(tmp_arr[i]).to_string()+"/");
			//std::cout << "i(" << i << ") tmp_angl(" << (tmp_angl + static_cast<double>(i)*delta/3) << ") " << tmp_arr[i] << std::endl;
		}
		BzCurve curve(tmp_arr);
		curve *= _matrix;
		//curve.set_len(delta*_R);
		curve.set_scale(_velocity.v() / (delta*_R));
		//_curves.push_back(std::make_pair(delta*_R, curve));
		_curves.push_back(curve);
	}

	scale = _velocity.v() / Point::norm(std::get<0>(line), std::get<1>(line));
	Vector tmp_v(std::get<0>(line), std::get<1>(line));
	for (int i = 0; i < 4; i++) {
		tmp_arr[i] = std::get<0>(line) + tmp_v*(static_cast<double>(i)/3);
		//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "line "+tmp_arr[i].to_string()+" /"+_matrix(tmp_arr[i]).to_string()+"/");
	}
	BzCurve l_curve(tmp_arr);
	l_curve *= _matrix;
	//std::cout << "l Bz " << l_curve(0) << " to " << l_curve(1) << std::endl;
	_line_id = _curves.size();

	//_curves.push_back(std::make_pair(scale, l_curve));
	l_curve.set_scale(scale);
	_curves.push_back(l_curve);

	//std::cout << "second " << second.center() << std::endl;

	angl = second.angl(std::get<1>(line), _end);
	delta = angl / 8;
	for (double tmp_angl = 0; less(tmp_angl, angl); tmp_angl += delta) {
		for (int i = 0; i < 4; i++) {
			// 3 = 4 - 1
			tmp_arr[i] = second(std::get<1>(line), tmp_angl + static_cast<double>(i)*delta/3);
			//my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "second "+tmp_arr[i].to_string()+" /"+_matrix(tmp_arr[i]).to_string()+"/");
			//std::cout << "i(" << i << ") tmp_angl(" << (tmp_angl + static_cast<double>(i)*delta/3) << ") " << tmp_arr[i] << std::endl;
		}
		BzCurve curve(tmp_arr);
		curve *= _matrix;
		curve.set_scale(_velocity.v() / delta*_R);
		//curve.set_len(delta*_R);
		//std::cout << "Bz " << curve(0) << " to " << curve(1) << std::endl;
		//_curves.push_back(std::make_pair(delta*_R, curve));
		_curves.push_back(curve);
	}
	//for (auto i = _curves.begin(); i < _curves.end(); i++) {
		//auto& curve = std::get<1>(*i);
		//std::cout << (i-_curves.begin()) << " len " << std::get<0>(*i) <<
			//" [" << curve(0) << curve(0.33) << " " << curve(0.66)
			//<< " " << curve(1) << "]" << std::endl;
		//std::cout << curve << std::endl;
	//}
	return true;
}

#if 0
bool Rotate::init() const
{
	if (!(equal(_end.z(), 0) && equal(_direction.z(), 0))) {
		return false;
	} else {
		return true;
	}
}
#endif


std::pair<Point, Point> Rotate::line() const
{
	//const BzCurve& line = std::get<1>(_curves.at(_line_id));
	const BzCurve& line = _curves.at(_line_id);
	Point first = _matrix(line.front());
	Point second = _matrix(line.back());
	return std::make_pair(first, second);
}


Point Rotate::operator()(double time) const 
{
	if (time > max_time()) {
		return _matrix(_end);
	} else if (time < 0) {
		return _matrix(Point(0,0,0));
	}



	double t = time;
	for (auto i = _curves.begin(); i < _curves.end(); i++) {
		assert(!equal(i->get_len(), 0));
		if (t <= i->get_len())
			return (*i)(t);
		else
			t -= i->get_len();
		//assert(!equal(std::get<0>(*i), 0));
		//if (t <= std::get<0>(*i))
			//return std::get<1>(*i)(t / std::get<0>(*i));
		//else
			//t -= std::get<0>(*i);
	}

	return _matrix(_end);
}

double Rotate::max_time() const 
{
	double sum = 0;
	for (auto i = _curves.begin(); i < _curves.end(); i++) {
		//sum += std::get<0>(*i);
		sum += i->get_len();
	}
	return sum;
}

Vector Rotate::direction() const 
{
	return _matrix(_direction);
}

Point Rotate::end_point() const
{
	return _matrix(_end);
}

Rotate::~Rotate()
{}

std::vector<Point> Rotate::check(const Polygon& p) const
{
	//std::cout << "rotate check " << max_time() << std::endl;
	std::vector<Point> result = check_curves(p, _curves);
	my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "found suppression "+std::to_string(result.size()));
	return result;
	//return check_curves(p, _curves);
}

