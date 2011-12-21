
#ifndef __CCMATH_CLAMP_H__
#define __CCMATH_CLAMP_H__

#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/mpl/if.hpp>

#include <ccmath/container_adaptor.h>

#include <iostream>

namespace ccmath
{
	/****************************************************************************************************/

	template<typename T>
	typename boost::disable_if<boost::is_pod<T>, void>::type clamp(T& x, typename container_adaptor<T>::value_type min, typename container_adaptor<T>::value_type max)
	{
		for (typename container_adaptor<T>::size_type i = 0; i < container_adaptor<T>::size::value; ++i)
		{
			if (x[i]<min)
				x[i] = min;
			else if (x[i]>max)
				x[i] = max;
		}
	}

	/****************************************************************************************************/

	template<typename T>
	typename boost::enable_if<boost::is_pod<T>, void>::type clamp(T& x, const T min, const T max)
	{
		if (x<min)
			x = min;
		else if (x>max)
			x = max;
	}

	/****************************************************************************************************/

};

#endif

