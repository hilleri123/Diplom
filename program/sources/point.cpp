
#include "point.h"



Point::Point(double x, double y, double z)
	: _arr({x,y,z})
{
	//_arr = {x, y, z};
}

Point& Point::by_geo(double r, double latitude, double longitude)
{
	_arr[0] = r * cos(latitude) * cos(longitude);
	_arr[1] = r * cos(latitude) * sin(longitude);
	_arr[2] = r * sin(latitude);
	return *this;
}

double Point::operator[](std::size_t index) const {
	if (index == 0) {
		return _arr[0];
	} else if (index == 1) {
		return _arr[1];
	} else if (index == 2) {
		return _arr[2];
	} else {
		throw std::out_of_range("point");
	}
}

double Point::x() const { return _arr[0]; }
double Point::y() const { return _arr[1]; }
double Point::z() const { return _arr[2]; }

double Point::latitude() const
{
	return atan2(_arr[2], sqrt(pow(_arr[0], 2) + pow(_arr[1], 2)));
}

double Point::longitude() const 
{
	double phi = atan2(_arr[1], _arr[0]);
	return phi;
}
double Point::radius() const 
{
	return sqrt(_arr[0]*_arr[0]+_arr[1]*_arr[1]+_arr[2]*_arr[2]);
}

bool Point::operator==(const Point& point) const
{
	for (std::size_t i = 0; i < 3; i++) {
		if (!equal(this->operator[](i), point[i]))
			return false;
	}
	return true;
}
bool Point::operator!=(const Point& point) const { return !this->operator==(point); }

bool Point::operator==(Point&& point) const
{
	for (std::size_t i = 0; i < 3; i++) {
		if (!equal(this->operator[](i), point[i]))
			return false;
	}
	return true;
}
bool Point::operator!=(Point&& point) const { return !this->operator==(point); }
	
std::string Point::to_string() const {
#ifdef PRINT_USE_GEO
	std::string res = "(" + std::to_string(radius()) + " " + std::to_string(latitude()) + " " + std::to_string(longitude()) + ")";
#else
	std::string res = "(" + std::to_string(_arr[0]) + " " + std::to_string(_arr[1]) + " " + std::to_string(_arr[2]) + ")";
#endif
	return res;
}

std::ostream& operator<<(std::ostream& stream, const Point& point) {
	//stream << "(" << point._arr[0] << " " << point._arr[1] << " " << point._arr[2] << ")";
	stream << point.to_string();
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Point&& point) {
	//stream << "(" << point._arr[0] << " " << point._arr[1] << " " << point._arr[2] << ")";
	stream << point.to_string();
	return stream;
}


double Point::norm(const Point& first, const Point& second)
{
	return sqrt(pow(second.x() - first.x(), 2) + pow(second.y() - first.y(), 2) + pow(second.z() - first.z(), 2));
}

double Point::norm(Point&& first, const Point& second)
{
	return sqrt(pow(second.x() - first.x(), 2) + pow(second.y() - first.y(), 2) + pow(second.z() - first.z(), 2));
}

double Point::norm(const Point& first, Point&& second)
{
	return sqrt(pow(second.x() - first.x(), 2) + pow(second.y() - first.y(), 2) + pow(second.z() - first.z(), 2));
}

double Point::norm(Point&& first, Point&& second)
{
	return sqrt(pow(second.x() - first.x(), 2) + pow(second.y() - first.y(), 2) + pow(second.z() - first.z(), 2));
}

