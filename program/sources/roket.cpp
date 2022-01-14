
#include "roket.h"

Stage::Stage(double uG, double G, double M, double m_t)
	: _uG(uG), _G(G), _M(M), _m_t(m_t)
{}


double Stage::T_max() const
{
	//std::cout << "T_max()" << std::endl;
	//std::cout << _m_t << " " << _uG << " " << earth::Fg(0) << " " << _G << std::endl;
	return _m_t * _uG * earth::Fg(0) / _G;
}


double Stage::m(double t) const
{
	if (t > T_max())
		return _M - _m_t;
	return (_M - _m_t) + (_m_t - t * _G / _uG / earth::Fg(0));
}



double Stage::G(double t) const
{
	if (t > T_max())
		return 0;
	return _G;
}





Roket::Roket(const std::vector<Stage>& stages)
	: _stages(stages)
{}

double Roket::T_max() const
{
	double res = 0;
	for (auto i = _stages.begin(); i != _stages.end(); ++i)
		res += i->T_max();
	return res;
}


double Roket::m(double t) const
{
	double tmp_t = t;
	double sum_m = 0;
	for (auto i = _stages.begin(); i < _stages.end(); ++i) {
		if (tmp_t < 0)
			sum_m += i->m(0);
		else if (tmp_t < i->T_max() || i+1 == _stages.end()) 
			sum_m += i->m(tmp_t);
		tmp_t -= i->T_max();
	}
	return sum_m;
}



double Roket::G(double t) const
{
	double tmp_t = t;
	for (auto i = _stages.begin(); i != _stages.end(); ++i) {
		if (tmp_t < i->T_max())
			return i->G(tmp_t);
		tmp_t -= i->T_max();
	}
	return 0;
}





