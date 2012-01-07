
#ifndef __CCMATH_FLOOR_H__
#define __CCMATH_FLOOR_H__

#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/mpl/if.hpp>

#include <ccmath/traits/type_traits.h>
#include <cmath>

namespace ccmath
{
	/****************************************************************************************************/
	
	template<typename T>
	inline typename boost::disable_if<boost::is_pod<T>, void>::type floor(T& r, const T& x)
	{
		for (typename type_traits<T>::size_type i = 0; i < type_traits<T>::size::value; ++i)
		{
			r[i] = std::floor( x[i] );
		}
	}

	/****************************************************************************************************/

	//In-place floor of variable
	template<typename T>
	inline typename boost::disable_if<boost::is_pod<T>, void>::type floor(T& x)
	{
		for (typename type_traits<T>::size_type i = 0; i < type_traits<T>::size::value; ++i)
		{
			x[i] = std::floor( x[i] );
		}
	}

	/****************************************************************************************************/

	template<typename T>
	inline typename boost::enable_if<boost::is_pod<T>, void>::type floor(T& r, const T x)
	{
		r = std::floor(x);
	}

	/****************************************************************************************************/

	//In-place floor of variable
	template<typename T>
	inline typename boost::enable_if<boost::is_pod<T>, void>::type floor(T& x)
	{
		x = std::floor(x);
	}

	/****************************************************************************************************/

};

#endif

