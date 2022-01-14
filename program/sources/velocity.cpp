
#include "velocity.h"

Velocity::Velocity(double velocity, double max_rotate)
	: _velocity(velocity), _max_rotate(max_rotate)
{}
	
Velocity& Velocity::operator=(double velocity) { _velocity = velocity; return *this; }

double Velocity::max_rotate() const { return _max_rotate; }
double Velocity::v() const { return _velocity; }

bool Velocity::operator==(double a) const {return v() == a; }
bool operator==(double a, const Velocity& v) {return v.v() == a; }
bool operator==(double a, Velocity&& v) {return v.v() == a; }
bool Velocity::operator!=(double a) const {return v() != a; }
bool operator!=(double a, const Velocity& v) {return v.v() != a; }
bool operator!=(double a, Velocity&& v) {return v.v() != a; }
bool Velocity::operator>=(double a) const {return v() >= a; }
bool operator>=(double a, const Velocity& v) {return v.v() >= a; }
bool operator>=(double a, Velocity&& v) {return v.v() >= a; }
bool Velocity::operator<=(double a) const {return v() <= a; }
bool operator<=(double a, const Velocity& v) {return v.v() <= a; }
bool operator<=(double a, Velocity&& v) {return v.v() <= a; }
bool Velocity::operator<(double a) const {return v() < a; }
bool operator<(double a, const Velocity& v) {return v.v() < a; }
bool operator<(double a, Velocity&& v) {return v.v() < a; }
bool Velocity::operator>(double a) const {return v() > a; }
bool operator>(double a, const Velocity& v) {return v.v() > a; }
bool operator>(double a, Velocity&& v) {return v.v() > a; }

bool Velocity::operator==(const Velocity& a) const {return v() == a.v(); }
bool operator==(const Velocity& a, const Velocity& v) {return v.v() == a.v(); }
bool operator==(Velocity&& a, Velocity&& v) {return v.v() == a.v(); }
bool Velocity::operator!=(const Velocity& a) const {return v() != a.v(); }
bool operator!=(const Velocity& a, const Velocity& v) {return v.v() != a.v(); }
bool operator!=(Velocity&& a, Velocity&& v) {return v.v() != a.v(); }
bool Velocity::operator>=(const Velocity& a) const {return v() >= a.v(); }
bool operator>=(const Velocity& a, const Velocity& v) {return v.v() >= a.v(); }
bool operator>=(Velocity&& a, Velocity&& v) {return v.v() >= a.v(); }
bool Velocity::operator<=(const Velocity& a) const {return v() <= a.v(); }
bool operator<=(const Velocity& a, const Velocity& v) {return v.v() <= a.v(); }
bool operator<=(Velocity&& a, Velocity&& v) {return v.v() <= a.v(); }
bool Velocity::operator<(const Velocity& a) const {return v() < a.v(); }
bool operator<(const Velocity& a, const Velocity& v) {return v.v() < a.v(); }
bool operator<(Velocity&& a, Velocity&& v) {return v.v() < a.v(); }
bool Velocity::operator>(const Velocity& a) const {return v() > a.v(); }
bool operator>(const Velocity& a, const Velocity& v) {return v.v() > a.v(); }
bool operator>(Velocity&& a, Velocity&& v) {return v.v() > a.v(); }

double Velocity::operator+(double a) const { return v() + a; }
double operator+(double a, const Velocity& b) { return a + b; }
double operator+(double a, Velocity&& b) { return a + b; }
double Velocity::operator-(double a) const { return v() - a; }
double operator-(double a, const Velocity& b) { return a - b._velocity; }
double operator-(double a, Velocity&& b) { return a - b._velocity; }
double Velocity::operator*(double a) const { return v() * a; }
double operator*(double a, const Velocity& b) { return a * b; }
double operator*(double a, Velocity&& b) { return a * b; }
double Velocity::operator/(double a) const { return v() / a; }
double operator/(double a, const Velocity& b) { return a / b._velocity; }
double operator/(double a, Velocity&& b) { return a / b._velocity; }

std::ostream& operator<<(std::ostream& s, const Velocity& a)
{
	s << "v: " << a.v() << " max_rotate " << a.max_rotate();
	return s;
}

std::ostream& operator<<(std::ostream& s, Velocity&& a)
{
	s << "v: " << a.v() << " max_rotate " << a.max_rotate();
	return s;
}

void Velocity::set_stats(void const* ptr) {
	_stats = ptr;
}

void const* Velocity::stats() const {
	return _stats;
}

void Velocity::set_course(double course) {
	_course = course;
}

double Velocity::course() const {
	return _course;
}


Velocity::~Velocity()
{}

