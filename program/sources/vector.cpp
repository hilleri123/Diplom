
#include "vector.h"


Vector::Vector(Point first, Point second)
{
	_direction = Point(second.x()-first.x(), second.y()-first.y(), second.z()-first.z());
}

double Vector::x() const { return _direction.x(); }
double Vector::y() const { return _direction.y(); }
double Vector::z() const { return _direction.z(); }
	
bool Vector::operator==(const Vector& vector) const { return _direction == vector._direction; }
bool Vector::operator!=(const Vector& vector) const { return _direction != vector._direction; }
bool Vector::operator==(Vector&& vector) const { return _direction == vector._direction; }
bool Vector::operator!=(Vector&& vector) const { return _direction != vector._direction; }
	
// 			begin  *
Vector Vector::operator*(double a) const
{ 
	auto result = Vector();	       
	//std::cout << _direction << std::endl;
	result._direction = Point(_direction.x() * a, _direction.y() * a, _direction.z() * a);
	//std::cout << result._direction << std::endl;
	return result;
}
Vector Vector::operator*=(double a) { this->operator=( this->operator*(a) ); return *this; }
Vector operator*(double a, const Vector& vector) { return vector * a; }
Vector operator*(double a, Vector&& vector) { return vector * a; }
Vector Vector::operator*(const Vector& vector) const
{ 
	auto result = Vector();
 	auto& a = _direction;		
 	auto& b = vector._direction;		
	result._direction = Point( a.y()*b.z() - a.z()*b.y() , a.z()*b.x() - a.x()*b.z() , a.x()*b.y() - a.y()*b.x() );
	return result;
}
Vector Vector::operator*=(const Vector& vector) { this->operator=( this->operator*(vector) ); return *this; }

Vector Vector::operator*(Vector&& vector) const
{ 
	auto result = Vector();
 	auto& a = _direction;		
 	auto& b = vector._direction;		
	result._direction = Point( a.y()*b.z() - a.z()*b.y() , a.z()*b.x() - a.x()*b.z() , a.x()*b.y() - a.y()*b.x() );
	return result;
}
Vector Vector::operator*=(Vector&& vector) { this->operator=( this->operator*(vector) ); return *this; }
//		end  *
//		begin  +
Vector Vector::operator+(const Vector& vector) const
{
	auto result = Vector();	       
	result._direction = Point(_direction.x() + vector._direction.x(), _direction.y() + vector._direction.y(), _direction.z() + vector._direction.z());
	return result;
}
Vector Vector::operator+=(const Vector& vector) { this->operator=( this->operator+(vector) ); return *this; }

Vector Vector::operator+(Vector&& vector) const
{
	auto result = Vector();	       
	result._direction = Point(_direction.x() + vector._direction.x(), _direction.y() + vector._direction.y(), _direction.z() + vector._direction.z());
	return result;
}
Vector Vector::operator+=(Vector&& vector) { this->operator=( this->operator+(vector) ); return *this; }

//Point Vector::operator+(const Point& point) const
//{
	//auto result = Point(_direction.x() + point.x(), _direction.y() + point.y(), _direction.z() + point.z());
	//return result;
//}
//Point Vector::operator+(Point& point) const
//{
	//auto result = Point(_direction.x() + point.x(), _direction.y() + point.y(), _direction.z() + point.z());
	//return result;
//}
Point operator+(const Vector& vector, const Point& point)
{
	auto result = Point(vector._direction.x() + point.x(), vector._direction.y() + point.y(), vector._direction.z() + point.z());
	return result;
}
Point operator+(const Vector& vector, Point&& point)
{
	auto result = Point(vector._direction.x() + point.x(), vector._direction.y() + point.y(), vector._direction.z() + point.z());
	return result;
}
Point operator+(Vector&& vector, const Point& point)
{
	auto result = Point(vector._direction.x() + point.x(), vector._direction.y() + point.y(), vector._direction.z() + point.z());
	return result;
}
Point operator+(Vector&& vector, Point&& point)
{
	auto result = Point(vector._direction.x() + point.x(), vector._direction.y() + point.y(), vector._direction.z() + point.z());
	return result;
}
Point operator+(const Point& point, Vector&& vector) { return vector+point; };
Point operator+(const Point& point, const Vector& vector) { return vector+point; };
Point operator+(Point&& point, Vector&& vector) { return vector+point; };
Point operator+(Point&& point, const Vector& vector) { return vector+point; };
//		end +
//		begin -
Vector Vector::operator-(const Vector& vector) const
{
	double x = vector._direction.x() - _direction.x();
	double y = vector._direction.y() - _direction.y();
	double z = vector._direction.z() - _direction.z();
	auto result = Vector(Point(x, y, z));
	return result;
}
Vector Vector::operator-(Vector&& vector) const
{
	double x = vector._direction.x() - _direction.x();
	double y = vector._direction.y() - _direction.y();
	double z = vector._direction.z() - _direction.z();
	auto result = Vector(Point(x, y, z));
	return result;
}
Vector Vector::operator-=(const Vector& vector) { this->operator=(this->operator-(vector)); return *this; }
Vector Vector::operator-=(Vector&& vector) { this->operator=(this->operator-(vector)); return *this; }
//		end-
//		end-
//		begin ^
//template <typename angle>
       //angle Vector::operator^(const Vector& vector) const
double Vector::operator^(const Vector& vector) const
{
	//std::cout << "this " << (*this+Point()) << " vector " << (vector+Point()) << std::endl;
	assert(vector.check());
	assert(this->check());
	double s = _direction.x() * vector._direction.x() + _direction.y() * vector._direction.y() + _direction.z() * vector._direction.z();
#if 0
	std::cout << "^ s(" << s << ") this(" << Vector::norm(*this) << ") vec(" << Vector::norm(vector) << ") casA(" <<
	    	( s / Vector::norm(*this) / Vector::norm(vector)) << ") A(" <<
	    	acos( s / Vector::norm(*this) / Vector::norm(vector)) << ")" << std::endl;
#endif
	if ((Vector::norm(*this) != 0 && Vector::norm(vector) != 0) && this->operator!=(vector)) {
		assert(equal_or_less(s, Vector::norm(*this)*Vector::norm(vector)));
		double cosA = s / Vector::norm(*this) / Vector::norm(vector);
		if (equal(cosA, 1)) {
			return 0;
		} else if (equal(cosA, -1)) {
			return 4*atan(1);
		} else {
			return acos(cosA);
		}
	} else {
		return 0;
	}
}

//template <typename angle>
       //angle Vector::operator^(Vector&& vector) const
double Vector::operator^(Vector&& vector) const
{
	//std::cout << "this " << (*this+Point()) << " vector " << (vector+Point()) << std::endl;
	assert(vector.check());
	assert(this->check());
	double s = _direction.x() * vector._direction.x() + _direction.y() * vector._direction.y() + _direction.z() * vector._direction.z();
	if ((Vector::norm(*this) != 0 && Vector::norm(vector) != 0) && this->operator!=(vector)) {
		assert(equal_or_less(s, Vector::norm(*this)*Vector::norm(vector)));
		double cosA = s / Vector::norm(*this) / Vector::norm(vector);
		if (equal(cosA, 1)) {
			return 0;
		} else if (equal(cosA, -1)) {
			return 4*atan(1);
		} else {
			return acos(cosA);
		}
	} else {
		return 0;
	}
}
double Vector::xy_angle(const Vector& vector) const
{
	return Vector::angle_in_2d(_direction.x(), _direction.y(), vector._direction.x(), vector._direction.y());
}
double Vector::xy_angle(Vector&& vector) const
{
	return Vector::angle_in_2d(_direction.x(), _direction.y(), vector._direction.x(), vector._direction.y());
}
double Vector::xz_angle(const Vector& vector) const
{
	return Vector::angle_in_2d(_direction.x(), _direction.z(), vector._direction.x(), vector._direction.z());
}
double Vector::xz_angle(Vector&& vector) const
{
	return Vector::angle_in_2d(_direction.x(), _direction.z(), vector._direction.x(), vector._direction.z());
}
double Vector::yz_angle(const Vector& vector) const
{
	return Vector::angle_in_2d(_direction.y(), _direction.z(), vector._direction.y(), vector._direction.z());
}
double Vector::yz_angle(Vector&& vector) const
{
	return Vector::angle_in_2d(_direction.y(), _direction.z(), vector._direction.y(), vector._direction.z());
}

//		end^

//		begin rotate
//template <typename angle>
#if 0
//Vector& rotate_x(angle alpha) const
Vector Vector::rotate_x(double alpha) const
{
	double x = _direction.x();
	double y = _direction.y() * cos(alpha) - _direction.z() * sin(alpha);
	double z = _direction.y() * sin(alpha) + _direction.z() * cos(alpha);
	auto result = Vector(Point(0,0,0), Point(x, y, z));
	return result;
}

//template <typename angle>
//Vector& rotate_y(angle alpha) const
Vector Vector::rotate_y(double alpha) const
{
	double x = _direction.x() * cos(alpha) + _direction.z() * sin(alpha);
	double y = _direction.y();
	double z = _direction.x() * (-sin(alpha)) + _direction.z() * cos(alpha);
	auto result = Vector(Point(0,0,0), Point(x, y, z));
	return result;
}

//template <typename angle>
//Vector& rotate_z(angle alpha) const
Vector Vector::rotate_z(double alpha) const
{
	double x = _direction.x() * cos(alpha) - _direction.y() * sin(alpha);
	double y = _direction.x() * sin(alpha) + _direction.y() * cos(alpha);
	double z = _direction.z();
	auto result = Vector(Point(0,0,0), Point(x, y, z));
	return result;
}
#endif

Vector Vector::rotate(const Vector& vector, double beta) const
{
	//std::cout << "rotate" << std::endl;
	assert(this->check());
	assert(vector.check());
	if (equal(sin(beta), sin(atan(1)*4)) && equal(cos(beta), cos(atan(1)*4))) {
		return this->operator*(-1);
	} else if (equal(sin(beta), sin(0)) && equal(cos(beta), cos(0))) {
		return Vector(*this);
	}
	double alpha = this->operator^(vector);
	//std::cout << "cos alpha " << cos(alpha) << std::endl;
	assert(!equal(fabs(cos(alpha)), 1));
	//std::cout << "this " << (*this+(Point())) << " vector " << (vector+Point()) << std::endl;
	Vector y_vector = vector - this->operator*(Vector::norm(vector) * cos(alpha) / Vector::norm(*this));
	y_vector *= (Vector::norm(*this) / Vector::norm(y_vector));
	//std::cout << "this " << (*this+(Point())) << " y_vector " << (y_vector+Point()) << std::endl;
	auto result = y_vector * sin(beta) + this->operator*(cos(beta));
	if (!equal(cos(this->operator^(y_vector)), 0)) { 
		std::cout << cos(this->operator^(y_vector)) << " this " << (*this+(Point())) << " y_vector " << (y_vector+Point()) << std::endl;
	}
	assert(equal(cos(this->operator^(y_vector)), 0));
	//result *= (Vector::norm(*this) / Vector::norm(result));
	//auto result = y_vector * sin(fabs(beta)) + this->operator*(cos(fabs(beta)));
	//std::cout << "end rotate" << std::endl;
	return result;
}

Vector Vector::rotate(Vector&& vector, double beta) const
{
	//std::cout << "rotate" << std::endl;
	assert(this->check());
	assert(vector.check());
	if (equal(sin(beta), sin(atan(1)*4)) && equal(cos(beta), cos(atan(1)*4))) {
		return this->operator*(-1);
	} else if (equal(sin(beta), sin(0)) && equal(cos(beta), cos(0))) {
		return Vector(*this);
	}
	double alpha = this->operator^(vector);
	//std::cout << alpha << std::endl;
	assert(!equal(fabs(cos(alpha)), 1));
	//std::cout << "this " << (*this+(Point())) << " vector " << (vector+Point()) << std::endl;
	Vector y_vector = vector - this->operator*(Vector::norm(vector) * cos(alpha) / Vector::norm(*this));
	y_vector *= (Vector::norm(*this) / Vector::norm(y_vector));
	//std::cout << "this " << (*this+(Point())) << " y_vector " << (y_vector+Point()) << std::endl;
	auto result = y_vector * sin(beta) + this->operator*(cos(beta));
	if (!equal(cos(this->operator^(y_vector)), 0)) { 
		std::cout << cos(this->operator^(y_vector)) << " this " << (*this+(Point())) << " y_vector " << (y_vector+Point()) << std::endl;
		//std::cout << cos(this->operator^(y_vector)) << std::endl;
	}
	assert(equal(cos(this->operator^(y_vector)), 0));
	//result *= (Vector::norm(*this) / Vector::norm(result));
	//auto result = y_vector * sin(fabs(beta)) + this->operator*(cos(fabs(beta)));
	//std::cout << "end rotate" << std::endl;
	return result;
}

//		end rotate

std::string Vector::to_string() const {
	std::string res = "[" + std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z()) + "]";
	return res;
}

std::ostream& operator<<(std::ostream& stream, const Vector& vector) {
	//stream << "[" << vector.x() << " " << vector.y() << " " << vector.z() << "]";
	stream << vector.to_string();
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vector&& vector) {
	//stream << "[" << vector.x() << " " << vector.y() << " " << vector.z() << "]";
	stream << vector.to_string();
	return stream;
}

Vector& Vector::normolize() {
	double len = Vector::norm(*this);
	_direction = Point(_direction.x()/len, _direction.y()/len, _direction.z()/len);
	return *this;
}

bool Vector::check() const
{
	return !((_direction.x()!=_direction.x())
		|| (_direction.y()!=_direction.y())
		|| (_direction.z()!=_direction.z()));
}

Vector Vector::projection(const Vector& plane) const
{
	auto angle = this->operator^(plane);
	auto& vector = *new Vector(plane);
	if (angle < atan(1)*2) {
		vector *= -1;
	}
	return this->operator+(vector * sin(angle) * (Vector::norm(*this) / Vector::norm(vector)));
}
Vector Vector::projection(Vector&& plane) const
{
	auto angle = this->operator^(plane);
	auto& vector = *new Vector(plane);
	if (angle < atan(1)*2) {
		vector *= -1;
	}
	return this->operator+(vector * sin(angle) * (Vector::norm(*this) / Vector::norm(vector)));
}

double Vector::norm(const Vector& vector) { return Point::norm(Point(0,0,0), vector._direction); }
double Vector::norm(Vector&& vector) { return Point::norm(Point(0,0,0), vector._direction); }

Vector::~Vector()
{}

double Vector::angle_in_2d(double x0, double y0, double x1, double y1) 
{
	double a0 = atan(y0 / x0);
	double a1 = atan(y1 / x1);
	if (x0 < 0) {
		a0 += atan(1)*2;
	}
	if (x1 < 0) {
		a1 += atan(1)*2;
	}
	return a1 - a0;
}

