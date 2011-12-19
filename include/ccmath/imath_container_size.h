
#ifndef __CCMATH_IMATH_CONTAINER_SIZE_H__
#define __CCMATH_IMATH_CONTAINER_SIZE_H__

#include <container_size.h>
#include <OpenEXR/ImathVec.h>

namespace ccmath
{

	template<> 
	struct container_adaptor< Imath::Vec2<T> >
	{
		typedef Imath::Vec2<T> type;
		typedef T value_type;

		typedef unsigned int size_type;
		static inline unsigned int size(const T& container) { return 2; }
	};

};

#endif

