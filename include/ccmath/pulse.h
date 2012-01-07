
#ifndef __CCMATH_PULSE_H__
#define __CCMATH_PULSE_H__

#include <ccmath/step.h>

namespace ccmath
{
	/****************************************************************************************************/

	template<typename T>
	inline void pulse(T& r, T min, T max, T x )
	{
		r = (step(min,x) - step(max,x));
	}

	/****************************************************************************************************/

};

#endif

