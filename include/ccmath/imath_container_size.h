
#ifndef __CCMATH_IMATH_CONTAINER_SIZE_H__
#define __CCMATH_IMATH_CONTAINER_SIZE_H__

#include <ccmath/container_size.h>
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
		
		static inline size_type size(const Imath::Vec2<T>& container) { return 2; }
		static inline void resize(Imath::Vec2<T>& container, size_type entries) { }
	};

	//Imath::Vec3<T> adaptor
	template<typename T> 
	struct container_adaptor< Imath::Vec3<T> >
	{
		typedef Imath::Vec3<T> type;
		typedef T value_type;
		typedef unsigned int size_type;
		
		static inline size_type size(const Imath::Vec3<T>& container) { return 3; }
		static inline void resize(Imath::Vec3<T>& container, size_type entries) { }
	};
	
	//Imath::Vec4<T> adaptor
	template<typename T> 
	struct container_adaptor< Imath::Vec4<T> >
	{
		typedef Imath::Vec4<T> type;
		typedef T value_type;
		typedef unsigned int size_type;
		
		static inline size_type size(const Imath::Vec4<T>& container) { return 4; }
		static inline void resize(Imath::Vec4<T>& container, size_type entries) { }
	};

};

#endif

