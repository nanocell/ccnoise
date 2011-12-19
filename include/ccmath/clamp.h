
#ifndef __CCMATH_CLAMP_H__
#define __CCMATH_CLAMP_H__

#include <ccmath/container_size.h>
#include <boost/type_traits.hpp>

#include <iostream>

namespace ccmath
{
	template<typename T, bool is_pod> struct clamp1;

	template<typename T>
	struct clamp1<T, true>
	{
		static inline T clamp(T x, T a, T b)
		{
			return (x < a ? a : (x > b ? b : x) );
		}
	};

	template<typename T>
	struct clamp1<T, false>
	{
		static inline const T clamp(const T& x, int a, const int b)
		{
			typename container_adaptor<T>::size_type i, entries;
			T result;

			entries = container_adaptor<T>::size(x);
			//container_adaptor<T>::resize(result, entries);
			result.resize(entries);

			for (i = 0; i < x.size(); ++i)
			{
				result[i] = (x[i] < a ? a : (x[i] > b ? b : x[i]) );
			}

			return result;
		}
	};

	//TODO: this clamp is only applicable for POD types.
	//      build specialisation for containers (arrays, lists, IMath:Vec*)
	
	/****************************************************************************************************/

	template<typename T>
	inline T clamp(T x, T a, T b)
	{
		// min(max(x,a),b)
		//return (x < a ? a : (x > b ? b : x) );
		return clamp1<T, boost::is_arithmetic<T>::value>::clamp(x,a,b);
	}

	/****************************************************************************************************/
	
	template<typename T>
	inline T clamp(T x, typename T::value_type a, typename T::value_type b)
	{
		// min(max(x,a),b)
		//return (x < a ? a : (x > b ? b : x) );
		return clamp1<T, boost::is_arithmetic<T>::value>::clamp(x,a,b);
	}

	/****************************************************************************************************/

};

#endif

