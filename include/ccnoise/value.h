
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

			/* Populate a table containing pseudo random values.
			 * The generation of this table may differ depending on the noise algorithm.
			 * For value noise, it is a table of PRNs between -1 and 1.
			 *
			 */
			template<typename IteratorT>
			static void populate_prn_table(IteratorT it_begin, IteratorT it_end);

	};

	value::value() 
	{

	};

	value::~value() 
	{

	};

	template<typename IteratorT>
	void value::populate_prn_table(IteratorT it_begin, IteratorT it_end)
	{
		std::cout << "populating data table for ccnoise::value" << std::endl;	
		
		//Perform a random shuffle, using the mersenne twister and random number generator
		boost::mt19937 gen; // generate raw random numbers
		boost::uniform_real<> dist(-1,1); // distribution of random numbers to generate.
		boost::variate_generator< boost::mt19937, boost::uniform_real<> > draw_rnd(gen, dist);

		//Fill the data table with pseudo random values between -1 and 1
		for (IteratorT it = it_begin; it != it_end; ++it)
		{
			*it = draw_rnd(); // draw the next random number
			std::cout << "Drew number: " << *it << std::endl;
		}
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

