
#ifndef __CCMATH_CONTAINER_SIZE_H__
#define __CCMATH_CONTAINER_SIZE_H__

namespace ccmath
{

	template<typename T, bool is_pod> struct container_adaptor1;

	/****************************************************************************************************/

	template<typename T>
	struct container_adaptor1<T, false>
	{
		typedef T type;
		typedef typename T::value_type value_type;
		typedef typename T::size_type size_type;

		typedef typename T::size size;
	};

	/****************************************************************************************************/

	template<typename T>
	struct container_adaptor1<T, true>
	{
		typedef T type;
		typedef T value_type;
		typedef unsigned int size_type;

		typedef typename boost::mpl::int_<1> size;
	};

	/****************************************************************************************************/

	template<typename T> 
	struct container_adaptor
	{
		typedef container_adaptor1<T, boost::is_pod<T>::value > t1;

		typedef T type;
		typedef typename t1::value_type value_type;
		typedef typename t1::size_type size_type;

		typedef typename t1::size size;
	};

};

#endif

