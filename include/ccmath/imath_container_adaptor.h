
#ifndef __CCMATH_IMATH_CONTAINER_SIZE_H__
#define __CCMATH_IMATH_CONTAINER_SIZE_H__

#include <ccmath/container_adaptor.h>
#include <OpenEXR/ImathVec.h>

namespace ccmath
{

	//Imath::Vec2<T> adaptor
	template<typename T> 
	struct container_adaptor< Imath::Vec2<T> >
	{
		typedef Imath::Vec2<T> type;
		typedef T value_type;
		typedef unsigned int size_type;
		
		static const unsigned int size = 2;
	};

	//Imath::Vec3<T> adaptor
	template<typename T> 
	struct container_adaptor< Imath::Vec3<T> >
	{
		typedef Imath::Vec3<T> type;
		typedef T value_type;
		typedef unsigned int size_type;

		static const unsigned int size = 3;
	};
	
	//Imath::Vec4<T> adaptor
	template<typename T> 
	struct container_adaptor< Imath::Vec4<T> >
	{
		typedef Imath::Vec4<T> type;
		typedef T value_type;
		typedef unsigned int size_type;
		
		static const unsigned int size = 4;
	};

};

#endif

