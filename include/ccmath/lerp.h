
#ifndef __CCMATH_LERP_H__
#define __CCMATH_LERP_H__

namespace ccmath
{
	/****************************************************************************************************/

	template<typename T>
	inline void lerp(T& r, T min, T max, float bias )
	{
		r = min + (max-min)*bias;
	}
	/****************************************************************************************************/

};

#endif

