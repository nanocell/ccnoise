
#ifndef __CCMATH_STEP_H__
#define __CCMATH_STEP_H__

namespace ccmath
{
	/****************************************************************************************************/

	template<typename T>
	inline void step(T& r, T edge, T x)
	{
		r = (x >= edge);
	}
	/****************************************************************************************************/

};

#endif
