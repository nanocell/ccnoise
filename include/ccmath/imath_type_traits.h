
#ifndef __CCMATH_IMATH_TYPE_TRAITS_H__
#define __CCMATH_IMATH_TYPE_TRAITS_H__

#include <ccmath/type_traits.h>
#include <boost/mpl/int.hpp>
#include <OpenEXR/ImathVec.h>

namespace ccmath
{

	//Imath::Vec2<T> adaptor
	template<typename T> 
	struct type_traits< Imath::Vec2<T> >
	{
		typedef Imath::Vec2<T> type;
		typedef T value_type;
		typedef unsigned int size_type;

		typedef boost::mpl::int_<2> size;
		
	};

	//Imath::Vec3<T> adaptor
	template<typename T> 
	struct type_traits< Imath::Vec3<T> >
	{
		typedef Imath::Vec3<T> type;
		typedef T value_type;
		typedef unsigned int size_type;

		typedef boost::mpl::int_<3> size;
	};
	
	//Imath::Vec4<T> adaptor
	template<typename T> 
	struct type_traits< Imath::Vec4<T> >
	{
		typedef Imath::Vec4<T> type;
		typedef T value_type;
		typedef unsigned int size_type;
		
		typedef boost::mpl::int_<4> size;
	};

};

#endif

