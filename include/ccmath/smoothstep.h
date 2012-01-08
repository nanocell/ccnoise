
#ifndef __CCMATH_SMOOTHSTEP_H__
#define __CCMATH_SMOOTHSTEP_H__

namespace ccmath
{
	
	/****************************************************************************************************/

	/*
	 * Perform a smoothstep on x between min and max
	 * where T is POD type
	 */
	
	template<class T>
	inline typename boost::enable_if<boost::is_pod<T>, void>::type smoothstep(T& r, T x, T min, T max)
	{
		if (x < min)
			r = 0.f;
		else if (x>max)
			r = 1.f;
		else
		{
			r = (x-min)/(max-min);
			r = r*r*(3.f-2.f*r);
		}
	}

	/****************************************************************************************************/

	/*
	 * Perform a fractional smoothstep on x (assumes x lies in range [0,1])
	 * where T is POD type
	 */

	template<class T>
	inline typename boost::enable_if<boost::is_pod<T>, void>::type smoothstep(T& r, T x)
	{
		r = x*x*(3.f-2.f*x);
	}
	
	/****************************************************************************************************/

	/*
	 * Smoothstep on x (non-pod type) between min and max
	 */

	template<typename T>
	inline typename boost::disable_if<boost::is_pod<T>, void>::type smoothstep(T& r, const T& x, const T& min, const T& max)
	{
		for (typename type_traits<T>::size_type i = 0; i < type_traits<T>::size::value; ++i)
		{
			smoothstep(r[i], x[i], min[i], max[i]);
		}
	}
	
	/****************************************************************************************************/

	/*
	 * Smoothstep on x (non-pod type) (assumes elements of x lies in range [0,1])
	 */

	template<typename T>
	inline typename boost::disable_if<boost::is_pod<T>, void>::type smoothstep(T& r, const T& x)
	{
		for (typename type_traits<T>::size_type i = 0; i < type_traits<T>::size::value; ++i)
		{
			smoothstep(r[i], x[i]);
		}
	}

};

#endif

