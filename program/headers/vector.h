

#pragma once

#include <cmath>

#include <cassert>
#include <iostream>

//#include "../init.h"
//#include "point.h"
#include "init.h"
#include "point.h"


class Vector : base_init
{
public:
	//explicit Vector(const Point& first = *std::unique_ptr<Point>(new Point()), const Point& second = *std::unique_ptr<Point>(new Point()));
	explicit Vector(Point first, Point second);

	Vector(Point p = Point()) : _direction(p) {};

	Vector(const Vector& ) = default;
	Vector(Vector&& ) = default;
	Vector& operator=(const Vector& ) = default;
	Vector& operator=(Vector&& ) = default;
	
	double x() const;
	double y() const;
	double z() const;

	bool operator==(const Vector& vector) const;
	bool operator!=(const Vector& vector) const;
	bool operator==(Vector&& vector) const;
	bool operator!=(Vector&& vector) const;
	
	// 			begin  *
	Vector operator*(double a) const;
	Vector operator*=(double a);
	friend Vector operator*(double a, const Vector& vector);
	friend Vector operator*(double a, Vector&& vector);

	Vector operator*(const Vector& vector) const;
	Vector operator*=(const Vector& vector);

	Vector operator*(Vector&& vector) const;
	Vector operator*=(Vector&& vector);
	//		end  *

	//		begin  +
	Vector operator+(const Vector& vector) const;
	Vector operator+=(const Vector& vector);

	Vector operator+(Vector&& vector) const;
	Vector operator+=(Vector&& vector);

	//Point operator+(const Point& point) const
	//{
		//auto result = Point(_direction.x() + point.x(), _direction.y() + point.y(), _direction.z() + point.z());
		//return result;
	//}
	//Point operator+(Point& point) const
	//{
		//auto result = Point(_direction.x() + point.x(), _direction.y() + point.y(), _direction.z() + point.z());
		//return result;
	//}
	friend Point operator+(const Vector& vector, const Point& point);
	friend Point operator+(const Vector& vector, Point&& point);
	friend Point operator+(Vector&& vector, const Point& point);
	friend Point operator+(Vector&& vector, Point&& point);
	friend Point operator+(const Point& point, Vector&& vector);
	friend Point operator+(const Point& point, const Vector& vector);
	friend Point operator+(Point&& point, Vector&& vector);
	friend Point operator+(Point&& point, const Vector& vector);
	//		end +

	//		begin -
	Vector operator-(const Vector& vector) const;
	Vector operator-(Vector&& vector) const;

	Vector operator-=(const Vector& vector);
	Vector operator-=(Vector&& vector);
	//		end-
	//		end-

	//		begin ^
       	double operator^(const Vector& vector) const;

       	double operator^(Vector&& vector) const;

	double xy_angle(const Vector& vector) const;
	double xy_angle(Vector&& vector) const;
	double xz_angle(const Vector& vector) const;
	double xz_angle(Vector&& vector) const;
	double yz_angle(const Vector& vector) const;
	double yz_angle(Vector&& vector) const;
	//		end^

	//		begin rotate
	Vector rotate_x(double alpha) const;
	Vector rotate_y(double alpha) const;
	Vector rotate_z(double alpha) const;
	Vector rotate(const Vector& cvector, double beta) const;
	Vector rotate(Vector&& vector, double beta) const;

	//		end rotate
	
	Vector projection(const Vector& plane) const;
	Vector projection(Vector&& plane) const;

	std::string to_string() const;

	friend std::ostream& operator<<(std::ostream& stream, const Vector& vector);
	friend std::ostream& operator<<(std::ostream& stream, Vector&& vector);

	static double norm(const Vector& vector);
	static double norm(Vector&& vector);

	Vector& normolize();

	bool check() const;

	virtual ~Vector();

protected:
	static double angle_in_2d(double x0, double y0, double x1, double y1);

	//Point& _direction = *new Point();
	Point _direction;
};
