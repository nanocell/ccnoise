
#ifndef __NL_VALUE_H__
#define __NL_VALUE_H__

#include <boost/scoped_ptr.hpp>
#include <boost/numeric/interval.hpp>

namespace ccnoise
{


	class value
	{
		public:
			value();
			~value();

			unsigned int tablesize(unsigned int tsize) { return tsize; }

			template<typename IteratorT>
			static void populate_data_table(IteratorT begin, IteratorT end);

	};

	value::value() 
	{

	};

	value::~value() 
	{

	};

	template<typename IteratorT>
	static void populate_data_table(IteratorT begin, IteratorT end)
	{
		

	}

	//************************************************************************************************

	class gradient
	{
		public:
			gradient();
			~gradient();
			
			unsigned int tablesize(unsigned int tsize) { return tsize*3; }


	};

	gradient::gradient()
	{

	}

	gradient::~gradient()
	{

	}




};

#endif

