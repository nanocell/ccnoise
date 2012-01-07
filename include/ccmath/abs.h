
#ifndef __CCMATH_ABS_H__
#define __CCMATH_ABS_H__

#include <cmath>
#include <cstdlib>
#include <boost/type_traits.hpp>

namespace ccmath
{

	template <typename T>
	inline typename boost::disable_if< boost::is_unsigned<T>, T>::type abs(T x)
	{
		return (x < 0 ? -x : x);
	}

	//If the arguments
	template <typename T>
	inline typename boost::enable_if< boost::is_unsigned<T>, T>::type abs(T x)
	{
		return x;
	}
};

#endif

