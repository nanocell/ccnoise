
#ifndef __CCMATH_TYPE_TRAITS_H__
#define __CCMATH_TYPE_TRAITS_H__

namespace ccmath
{

	template<typename T> 
	struct type_traits
	{
		typedef T type;
		typedef typename T::value_type value_type;
		typedef typename T::size_type size_type;
		typedef typename T::size size;
	};

};

#endif

