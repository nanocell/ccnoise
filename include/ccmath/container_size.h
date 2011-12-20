
#ifndef __CCMATH_CONTAINER_SIZE_H__
#define __CCMATH_CONTAINER_SIZE_H__

namespace ccmath
{

	template<typename T> 
	struct container_adaptor
	{
		typedef T type;
		typedef typename T::value_type value_type;
		typedef typename T::size_type size_type;

		static inline typename T::size_type size(const T& container) { return container.size(); }

		static inline void resize(T& container, typename T::size_type entries) { container.resize(entries); }
	};

};

#endif

