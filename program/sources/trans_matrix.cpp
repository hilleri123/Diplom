
#include "trans_matrix.h"
#include "bz_curve.h"
#include "polygon.h"



Matrix::Matrix(matrix opt, const Vector* vec, double angle)
	: _matrix(std::valarray<double>(0., 4*4))
{
	for (std::size_t i = 0; i < 4; i++)
		_matrix[i+i*4] = 1;

	switch (opt) {
	case null:
		break;
	case move:
		if (vec == nullptr) {
			err = new std::invalid_argument("in Matrix constructor vec_ptr == nullptr");
			break;
		}
		_matrix[4-1] = vec->x();
		_matrix[2*4-1] = vec->y();
		_matrix[3*4-1] = vec->z();
		break;
	case rotate:
		if (vec == nullptr) {
			err = new std::invalid_argument("in Matrix constructor vec_ptr == nullptr");
			break;
		}
		{
		double& x0 = _matrix[0];            double& x1 = _matrix[1];            double& x2 = _matrix[2];
		double& y0 = _matrix[0+4];          double& y1 = _matrix[1+4];          double& y2 = _matrix[2+4];
		double& z0 = _matrix[0+2*4];        double& z1 = _matrix[1+2*4];        double& z2 = _matrix[2+2*4];

		double norm = Vector::norm(*vec);
		double l = vec->x() / norm;	double m = vec->y() / norm;	double n = vec->z() / norm;
	
		//std::cout << "Point (" << l << " " << m << " " << n << ")" << std::endl;

		double cosA = cos(angle);
		double sinA = sin(angle);
		//std::cout << "cosA " << cosA << " sinA " << sinA << std::endl;
		//en.wikipedia.org/wiki/Transformation_matrix
		x0 = l*l*(1-cosA)+cosA;         x1 = m*l*(1-cosA)-n*sinA;       x2 = n*l*(1-cosA)+m*sinA;
		y0 = l*m*(1-cosA)+n*sinA;       y1 = m*m*(1-cosA)+cosA;         y2 = n*m*(1-cosA)-l*sinA;
		z0 = l*n*(1-cosA)-m*sinA;       z1 = m*n*(1-cosA)+l*sinA;       z2 = n*n*(1-cosA)+cosA;
		//std::cout << "[ " << x0 << " " << x1 << " " << x2 << " ]" << std::endl;
		//std::cout << "[ " << y0 << " " << y1 << " " << y2 << " ]" << std::endl;
		//std::cout << "[ " << z0 << " " << z1 << " " << z2 << " ]" << std::endl;
		}
		break;
	case comp:
		if (vec == nullptr) {
			err = new std::invalid_argument("in Matrix constructor vec_ptr == nullptr");
			break;
		}
		{
		//int last_line = 4*(4-1);
		//_matrix[last_line] = vec->x();
		//_matrix[last_line+1] = vec->y();
		//_matrix[last_line+2] = vec->z();
		_matrix[0] = vec->x();
		_matrix[1+1*4] = vec->y();
		_matrix[2+2*4] = vec->z();
		}
		break;
	default:
		err = new std::logic_error("in Matrix constructor 'switch case' in default section");
		break;
	}
}

	
bool Matrix::init()
{
	if (err == nullptr)
		return true;
	else {
		//std::exception tmp(*err);
		//delete err;
		//err = nullptr;
		//throw tmp;
		throw *err;
		return false;
	}
}


bool Matrix::operator==(const Matrix& m) const
{
	for (std::size_t i = 0; i < 4; i++) {
		if (!equal(_matrix[i], m._matrix[i]))
			return false;
	}
	return true;
}

bool Matrix::operator!=(const Matrix& m) const
{
	return !(*this == m);
}

bool Matrix::operator==(Matrix&& m) const
{
	for (std::size_t i = 0; i < 4; i++) {
		if (!equal(_matrix[i], m._matrix[i]))
			return false;
	}
	return true;
}

bool Matrix::operator!=(Matrix&& m) const
{
	return !(*this == m);
}



Matrix& Matrix::operator*=(const Matrix& m)
{
	this->operator=(*this * m);
	return *this;
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix res;
	for (std::size_t i = 0; i < 4; i++) {
		std::slice row(i*4, 4, 1);
		for (std::size_t j = 0; j < 4; j++) {
			std::slice col(j, 4, 4);
			res._matrix[j + i*4] = (m1._matrix[ row ] * m2._matrix[ col ]).sum(); 
		}
	}
	return res;
}

Point Matrix::operator()(Point point) const
{
	std::valarray<double> vec = {point.x(), point.y(), point.z(), 1.};
	double x = (_matrix[ std::slice(0, 4, 1) ] * vec).sum(); 
	double y = (_matrix[ std::slice(4, 4, 1) ] * vec).sum(); 
	double z = (_matrix[ std::slice(2*4, 4, 1) ] * vec).sum(); 
	return std::move(Point(x, y, z));
}

Vector Matrix::operator()(Vector vector) const
{

	Point begin = this->operator()(Point(0,0,0));
	Point end = this->operator()(Point(vector.x(), vector.y(), vector.z()));

	return std::move(Vector(begin, end));
}

BzCurve Matrix::operator()(BzCurve curve) const
{

	std::array<Point, 4> tmp;
	for (std::size_t i = 0; i < 4; ++i)
		tmp[i] = this->operator()(curve.at(i));
	return std::move(BzCurve::make_bz_curve_manualy(tmp));
}

Polygon Matrix::operator()(Polygon polygon) const
{
	std::array<Point, Polygon::size> tmp;

	for (std::size_t i = 0; i < Polygon::size; ++i)
		tmp[i] = this->operator()(polygon.get_point(i));

	return std::move(Polygon(std::move(tmp)));
}


std::size_t Matrix::size() const { return 4; }


bool Matrix::multiplay_foreward_backward(Matrix& foreward, Matrix& backward, matrix opt, const Vector* vec_ptr, double angle)
{
	Matrix foreward_copy = foreward;
	Matrix* tmp;
	INIT(tmp, Matrix, opt, vec_ptr, angle);
	if (tmp == nullptr)
		return false;
	foreward *= *tmp;
	delete tmp;
	if (vec_ptr != nullptr) {
		Vector tmp_vec;
		if (opt == matrix::comp)
			tmp_vec = Vector(Point(1./vec_ptr->x(), 1./vec_ptr->y(), 1./vec_ptr->z()));
		else
			tmp_vec = *vec_ptr * (-1);
		INIT(tmp, Matrix, opt, &tmp_vec, angle);
	} else {
		INIT(tmp, Matrix, opt, vec_ptr, angle);
	}
	if (tmp == nullptr) {
		foreward = foreward_copy;
		return false;
	}
	backward = *tmp * backward;
	delete tmp;
	return true;
}


std::ostream& operator<<(std::ostream& stream, const Matrix& m)
{
	std::size_t len = std::to_string(*std::max_element(std::begin(m._matrix), std::end(m._matrix), 
				[](const double& a, const double& b){ return std::to_string(a).size() < std::to_string(b).size();}
				)).size() + 2;
	for (std::size_t i = 0; i < 4; i++) {
		std::cout << "|";
		for (std::size_t j = 0 ; j < 4; j++) {
			std::string d = std::to_string(m._matrix[i*4+j]);
			//std::cerr << " len(" << len << ") size(" << d.size() << ") -(" << (len - d.size()) << ") d(" << d << ")" << std::endl;
			std::string off(len - d.size(), ' ');
			std::cout << off << d;
			//std::cout << "\t" << m._matrix[i*4 + j];
		}
		std::cout << "|" << std::endl;
	}

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Matrix&& m)
{
	std::size_t len = std::to_string(*std::max_element(std::begin(m._matrix), std::end(m._matrix), 
				[](const double& a, const double& b){ return std::to_string(a).size() < std::to_string(b).size();}
				)).size() + 2;
	for (std::size_t i = 0; i < 4; i++) {
		std::cout << "|";
		for (std::size_t j = 0 ; j < 4; j++) {
			std::string d = std::to_string(m._matrix[i*4+j]);
			std::string off(len - d.size(), ' ');
			std::cout << off << d;
			//std::cout << "\t" << m._matrix[i*4 + j];
		}
		std::cout << "|" << std::endl;
	}

	return stream;
}




Matrix::~Matrix()
{
	if (err != nullptr)
		delete err;
}

Conversion::Conversion(const Point* pos, const Vector* ox, const Vector* oy, const Vector* oz)
	: pos(pos), ox(ox), oy(oy), oz(oz)
{}

bool Conversion::init() {
	std::vector<const Vector*> axis;
	if (ox != nullptr)
		axis.push_back(ox);
	if (oy != nullptr)
		axis.push_back(oy);
	if (oz != nullptr)
		axis.push_back(oz);

	std::string message = "Converstion (";
	//std::cout << "Conversion (";
	for (auto i = axis.begin(); i < axis.end(); i++)
		if (*i != nullptr)
			//std::cout << **i;
			message += (*i)->to_string();
	//std::cout << ")" << std::endl;
	message += ")";
	my_log::log_it(my_log::level::info, __FUNCTION_NAME__, message);

	if (axis.size() < 2) {
		//err = new std::invalid_argument("get 1 axis or less expected 2 or more");
		//throw std::invalid_argument("get 1 axis or less expected 2 or more");
		//std::cerr << "get 1 axis or less expected 2 or more" << std::endl;
		my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "get 1 axis or less expected 2 or more");
		return false;
	}

	const double pi_2 = atan(1)*2;
	for (auto i = axis.begin(); i < axis.end()-1; i++) {
		for (auto j = i+1; j < axis.end(); j++) {
			double angle = (**i)^(**j);
			//std::cout << "angle " << **i << **j << " " << angle << " 2pi " << pi_2 << std::endl;
			if (!equal(angle, pi_2)) {
				my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "angle " + (*i)->to_string() + (*j)->to_string() + " " + std::to_string(angle) + " 2pi " + std::to_string(pi_2));
				//std::cout << "angle " << **i << **j << " " << angle << " 2pi " << pi_2 << std::endl;
				//err = new std::invalid_argument("angle between axis != pi/2");
				//std::cerr << "angle between axis != pi/2" << std::endl;
				my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "angle between axis != pi/2");
				//throw std::invalid_argument("angle between axis != pi/2");
				return false;
			}
		}
	}

	if (axis.size() == 3) {
		Vector tmp_oz = (*ox) * (*oy);
		double angle = tmp_oz^(*oz);
		if (!is_null(angle)) {
			//err = new std::invalid_argument("axis system isnt right-handed");
			//throw std::invalid_argument("axis system isnt right-handed");
			//std::cerr << "axis system isnt right-handed" << std::endl;
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "axis system isnt right-handed");
			return false;
		}
	}

	//if (err == nullptr) {
	{
		Vector c_ox(Point(1,0,0));
		Vector c_oy(Point(0,1,0));
		Vector c_oz(Point(0,0,1));

		if (pos != nullptr) {
			Vector move_vec(*pos);
			bool res = Matrix::multiplay_foreward_backward(_from, _to, Matrix::move, &move_vec);
			if (!res) {
				//std::cerr << "in Conversion multiplay_foreward_backward(..., Matrix::move ,...) function didnt work" << std::endl;
				my_log::log_it(my_log::level::warning, __FUNCTION_NAME__, "in Conversion multiplay_foreward_backward(..., Matrix::move ,...) function didnt work");
				//err = new std::logic_error("cant construct move matrix");
				//throw std::logic_error("cant construct move matrix");
				//std::cerr << "cant construct move matrix" << std::endl;
				my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "cant construct move matrix");
				return false;
			}
		}
		Vector tmp;
		if (axis.size() == 2) {
			if (ox == nullptr) {
				tmp = (*oy)*(*oz);
				ox = &tmp;
			} else if (oy == nullptr) {
				tmp = -1 * ((*ox)*(*oz));
				oy = &tmp;
			} else if (oz == nullptr) {
				tmp = (*ox)*(*oy);
				oz = &tmp;
			}
		}

		//Ax+By+Cz+D=0 - plane
		//D1 = D2 = 0
		//oz = (A, B, C)
		//interseption vector(1, a, 0)
		//A+Ba=0 => a = -A/B
		//Vector inter(Point(1, -(oz->x()/oz->y()), 0));
		Vector inter = (*oz)*c_oz; //counter clockwise 
		if (!equal(Vector::norm(inter), 0)) {
			//std::cout << "inter " << inter << std::endl; 
			double angle = -((*oz)^c_oz); //because rotation matrix clockwise
			//std::cout << "angle oz " << (angle / atan(1) * 45.) << std::endl;
			bool res = Matrix::multiplay_foreward_backward(_from, _to, Matrix::rotate, &inter, angle);
			if (!res) {
				//std::cerr << "in Conversion multiplay_foreward_backward(..., Matrix::rotate ,...) function didnt work" << std::endl;
				my_log::log_it(my_log::level::warning, __FUNCTION_NAME__, "in Conversion multiplay_foreward_backward(..., Matrix::rotate ,...) function didnt work");
				//err = new std::logic_error("cant construct rotate matrix");
				//throw std::logic_error("cant construct rotate matrix");
				//std::cerr << "cant construct rotate matrix" << std::endl;
				my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "cant construct rotate matrix");
				return false;
			}
		}

		//std::cout << _from << std::endl << _to << std::endl;

		//std::cout << "oz " << *oz << " " << from(c_oz*Vector::norm(*oz)) << std::endl;
		assert(*oz == from(c_oz*Vector::norm(*oz)));

		Vector tmp_ox = to(*ox);
		Vector tmp_oy = to(*oy);
		tmp_ox.normolize();
		tmp_oy.normolize();
		//std::cout << "orig x " << *ox << " y " << *oy << std::endl;
		//std::cout << "tmp x " << tmp_ox << " y " << tmp_oy << std::endl;
		//std::cout << "norm angle " << (((*ox)^tmp_ox) / atan(1) * 45.) << std::endl;
		double angle = tmp_ox^c_ox;
		if (tmp_ox.y() < 0)
			angle = -angle;
		//std::cout << "angle ox " << (angle / atan(1) * 45.) << " no n " << (((*ox)^c_ox) / atan(1) * 45.) << std::endl;
		//std::cout << "end to " << tmp_ox << " " << tmp_oy << " angle " << angle << std::endl;

		Matrix tmp_mat(Matrix::rotate, &c_oz, angle);
		//std::cout << tmp_ox << " " << tmp_mat(c_ox) << " " << tmp_mat(tmp_ox) << std::endl << tmp_mat << std::endl;

		bool res = Matrix::multiplay_foreward_backward(_from, _to, Matrix::rotate, &c_oz, angle);
		if (!res) {
			//std::cerr << "in Conversion multiplay_foreward_backward(..., Matrix::rotate ,...) function didnt work" << std::endl;
			my_log::log_it(my_log::level::warning, __FUNCTION_NAME__, "in Conversion multiplay_foreward_backward(..., Matrix::rotate ,...) function didnt work");
			//err = new std::logic_error("cant construct rotate matrix");
			//throw std::logic_error("cant construct rotate matrix");
			//std::cerr << "cant construct rotate matrix" << std::endl;
			my_log::log_it(my_log::level::error, __FUNCTION_NAME__, "cant construct rotate matrix");
			return false;
		}
		//std::cout << _from << std::endl << _to << std::endl;

		//std::cout << "ox " << *ox << " " << from(c_ox*Vector::norm(*ox)) << std::endl;
		//std::cout << "ox " << *ox << " " << from(c_ox*Vector::norm(*ox)) << std::endl;
		//assert(*ox == from(c_ox*Vector::norm(*ox)));
		Vector norm_tmp = *ox;
		//std::cout << "norm ox " << norm_tmp.normolize() << " " << from(c_ox).normolize() << std::endl;
		my_log::log_it(my_log::level::debug, __FUNCTION_NAME__, "norm ox " + norm_tmp.normolize().to_string() + " " + from(c_ox).normolize().to_string());
		assert(norm_tmp.normolize() == from(c_ox).normolize());
		//std::cout << "oy " << *oy << " " << from(c_oy*Vector::norm(*oy)) << std::endl;
		//assert(*oy == from(c_oy*Vector::norm(*oy)));
		norm_tmp = *oy;
		//std::cout << "norm oy " << norm_tmp.normolize() << " " << from(c_oy).normolize() << std::endl;
		assert(norm_tmp.normolize() == from(c_oy).normolize());
		//std::cout << "oz " << *oz << " " << from(c_oz*Vector::norm(*oz)) << std::endl;
		//assert(*oz == from(c_oz*Vector::norm(*oz)));
		norm_tmp = *oz;
		//std::cout << "norm oz " << norm_tmp.normolize() << " " << from(c_oz).normolize() << std::endl;
		assert(norm_tmp.normolize() == from(c_oz).normolize());
		
		assert(_from*_to == _to*_from);
	}
	//std::cout << "end" << std::endl;
	return true;
}


#if 0
bool Conversion::init() const
{
	if (err == nullptr) {
		return true;
	} else {
		throw *err;
		return false;
	}
}
#endif

Conversion::~Conversion()
{
	//if (err != nullptr)
		//delete err;
}


