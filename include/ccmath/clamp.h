
#ifndef __CCMATH_CLAMP_H__
#define __CCMATH_CLAMP_H__

#include <boost/type_traits.hpp>

#include <ccmath/container_adaptor.h>

#include <iostream>

namespace ccmath
{
	/****************************************************************************************************/
	
	template<int dim, typename T> 
	struct clamp1
	{
		static inline const T clamp(const T& x, typename container_adaptor<T>::value_type a, typename container_adaptor<T>::value_type b)
		{
			T v;

			//NOTE: The resize is not used here anymore, since we will be expecting statically size types, not dynamic
			//containers such as std::vector.
			//container_adaptor<T>::resize( v, container_adaptor<T>::size(x) );

			for (unsigned int i = 0; i < dim; ++i)
			{
				v[i] = (x[i] < a ? a : (x[i] > b ? b : x[i]));
			}
			return v;
		}
	};

	/****************************************************************************************************/

	template<typename T>
	struct clamp1<1, T>
	{
		static inline const T clamp(const T x, T a, T b)
		{
			return (x < a ? a : (x > b ? b : x) );
		}
	};

	
	/****************************************************************************************************/

	template<int dim, typename T>
	inline const T clamp(const T& x, typename container_adaptor<T>::value_type a, typename container_adaptor<T>::value_type b)
	{
		return clamp1<dim, T>::clamp(x, a, b);
	}

	/****************************************************************************************************/

	//Dimensionless clamp defaults to 1 and assumes all the types are the same. 
	//Typically would be used for PODs.
	template<typename T>
	inline T clamp(T x, T a, T b)
	{
		return clamp1<1,T>::clamp(x, a, b);
	}

	/****************************************************************************************************/

};

#endif

