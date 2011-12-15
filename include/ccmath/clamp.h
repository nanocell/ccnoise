
#ifndef __CCMATH_CLAMP_H__
#define __CCMATH_CLMAP_H__

namespace ccmath
{

	template<typename T>
	inline T clamp(T x, T a, T b)
	{
		// min(max(x,a),b)
		return (x < a ? a : (x > b ? b : x) );
	}

};

#endif

