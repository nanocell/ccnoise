
#ifndef __CCMATH_ABS_H__
#define __CCMATH_ABS_H__

#include <cmath>
#include <cstdlib>
#include <boost/type_traits.hpp>

namespace ccmath
{

	template <typename T, bool is_integral>
	struct abs1 { };

	/****************************************************************************************************/

	//integral abs call
	template<typename T>
	struct abs1<T, true>
	{
		static inline T apply(T v)
		{
			return std::abs(v);
		}
	};

	/****************************************************************************************************/

	//real abs call
	template<typename T>
	struct abs1<T, false>
	{
		static inline T apply(T v)
		{
			return std::fabs(v);
		}
	};

	/****************************************************************************************************/


	/*
	template <typename T>
	inline T abs(T v)
	{
		return abs1<T, boost::is_integral<T>::value>::apply(v); 
	}
	*/

	//custom abs implementation
	
	template <typename T>
	inline T abs(T x)
	{
		return (x < 0 ? -x : x);
	}

};

#endif

