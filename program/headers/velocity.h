
#pragma once

#include <cmath>
#include <iostream>

//#include "../init.h"
//#include "../geometrics/vector.h"
#include "vector.h"

class Velocity
{
public:
	explicit Velocity(double velocity = 1, double max_rotate = asin(1./2.));
	
	Velocity(const Velocity&) = default;
	Velocity(Velocity&&) = default;
	Velocity& operator=(const Velocity&) = default;
	Velocity& operator=(Velocity&&) = default;
	
	Velocity& operator=(double velocity);

	double max_rotate() const;
	double v() const;

	bool operator==(double a) const;
	friend bool operator==(double a, const Velocity& v);
	friend bool operator==(double a, Velocity&& v);
	bool operator!=(double a) const;
	friend bool operator!=(double a, const Velocity& v);
	friend bool operator!=(double a, Velocity&& v);
	bool operator>=(double a) const;
	friend bool operator>=(double a, const Velocity& v);
	friend bool operator>=(double a, Velocity&& v);
	bool operator<=(double a) const;
	friend bool operator<=(double a, const Velocity& v);
	friend bool operator<=(double a, Velocity&& v);
	bool operator<(double a) const;
	friend bool operator<(double a, const Velocity& v);
	friend bool operator<(double a, Velocity&& v);
	bool operator>(double a) const;
	friend bool operator>(double a, const Velocity& v);
	friend bool operator>(double a, Velocity&& v);

	bool operator==(const Velocity& a) const;
	friend bool operator==(const Velocity& a, const Velocity& v);
	friend bool operator==(Velocity&& a, Velocity&& v);
	bool operator!=(const Velocity& a) const;
	friend bool operator!=(const Velocity& a, const Velocity& v);
	friend bool operator!=(Velocity&& a, Velocity&& v);
	bool operator>=(const Velocity& a) const;
	friend bool operator>=(const Velocity& a, const Velocity& v);
	friend bool operator>=(Velocity&& a, Velocity&& v);
	bool operator<=(const Velocity& a) const;
	friend bool operator<=(const Velocity& a, const Velocity& v);
	friend bool operator<=(Velocity&& a, Velocity&& v);
	bool operator<(const Velocity& a) const;
	friend bool operator<(const Velocity& a, const Velocity& v);
	friend bool operator<(Velocity&& a, Velocity&& v);
	bool operator>(const Velocity& a) const;
	friend bool operator>(const Velocity& a, const Velocity& v);
	friend bool operator>(Velocity&& a, Velocity&& v);

	double operator+(double a) const;
	friend double operator+(double a, const Velocity& b);
	friend double operator+(double a, Velocity&& b);
	double operator-(double a) const;
	friend double operator-(double a, const Velocity& b);
	friend double operator-(double a, Velocity&& b);
	double operator*(double a) const;
	friend double operator*(double a, const Velocity& b);
	friend double operator*(double a, Velocity&& b);
	double operator/(double a) const;
	friend double operator/(double a, const Velocity& b);
	friend double operator/(double a, Velocity&& b);

	friend std::ostream& operator<<(std::ostream& s, const Velocity& a);
	friend std::ostream& operator<<(std::ostream& s, Velocity&& a);

	void set_stats(void const* ptr);
	void const* stats() const;

	void set_course(double curse);
	double course() const;

	~Velocity();
protected:
	double _velocity;
	//angle _max_rotate;
	double _max_rotate;
	double _course;

	void const* _stats = nullptr;
};
