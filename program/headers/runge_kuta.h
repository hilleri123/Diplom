
#pragma once

#include <ode_solver.h>




template<class RES, class ARG0, class ARG1>
class RungeKutta4 : public OdeSolver<RES, ARG0, ARG1>
{
public:
	using OdeS = OdeSolver<RES, ARG0, ARG1>;
	using Args = ArgsContainer<ARG0, ARG1>;
	explicit RungeKutta4(RES (*f_ptr)(ARG0, ARG1), const Args& initial) : OdeS(f_ptr, initial)
	{}

	virtual RES step() override
	{
		//aliases
		auto& _cur = OdeS::_current;
		auto& _st = OdeS::_step;

		RES k1 = OdeS::_f_ptr(_cur.x(), _cur.y());
		RES k2 = OdeS::_f_ptr(_cur.x()+(_st/2.), _cur.y()+(_st/2.*k1));
		RES k3 = OdeS::_f_ptr(_cur.x()+(_st/2.), _cur.y()+(_st/2.*k2));
		RES k4 = OdeS::_f_ptr(_cur.x()+(_st), _cur.y()+(_st*k3));
		//!!!!
		//
		_cur.x().v = _cur.y();
		_cur.x().dt = _st;
		_cur.x() += _st;
		_cur.y() += _st/6.*(k1+2.*k2+2.*k3+k4);
		return _cur.y();
	}
	
	virtual ~RungeKutta4() override {} 
};

