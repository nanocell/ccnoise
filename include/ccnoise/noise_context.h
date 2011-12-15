
#ifndef __CC_noise_H__
#define __CC_noise_H__

#include <boost/scoped_ptr.hpp>
#include <boost/numeric/interval.hpp> //for boost::abs
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>

#include <iostream>
#include <vector>
#include <list>

namespace ccnoise
{

	//noise stores the table of
	//random numbers and the index permutation table.
	//This struct is templated on the data type that is stored in the table
	//used for random number lookups
	template<typename NoiseT, typename T>
	struct noise
	{
		noise(unsigned int tsize);
		~noise() {}

		void init_permutation_table();
		void init_data_table();

		unsigned int tablesize;
		std::vector<T> datatable;
		std::vector<unsigned int> permtable;
	};
	
	//************************************************************************************************

	template<typename NoiseT, typename T>
	noise<NoiseT, T>::noise(unsigned int tsize) :
		tablesize(tsize)
	{
		std::cout << "Constructing Noise Template." << std::endl;
		init_permutation_table();
		init_data_table();

	}
	
	//************************************************************************************************
	
	template<typename NoiseT, typename T>
	void noise<NoiseT, T>::init_permutation_table() 
	{
		permtable.resize(tablesize);

		//Fill the permtable with values and permute them
		std::cout << "Filling tables..." << std::endl;
		for (int i = 0; i < tablesize; ++i)
		{
			permtable[i] = i;
		}

		//Perform a random shuffle, using the mersenne twister and random number generator
		boost::mt19937 gen; // generate raw random numbers
		boost::uniform_int<> dist(0,tablesize-1); // distribution of random numbers to generate.
		boost::variate_generator< boost::mt19937, boost::uniform_int<> > draw_rnd(gen, dist);

		boost::random_shuffle(permtable, draw_rnd);

		//The permtable is now shuffled, and will serve as random lookups into the data table.
	}
	
	//************************************************************************************************
	
	template<typename NoiseT, typename T>
	void noise<NoiseT, T>::init_data_table() 
	{
		datatable.resize(tablesize);

		NoiseT::populate_data_table(datatable.begin(), datatable.end());

		//The data table is now populated and can be used with the NoiseT policy to generate some noise.
	}

	//************************************************************************************************


	/*
	template< <class> class NoiseT, typename ContextT = noise>
	class noise : public NoiseT<ContextT>
	{
		public:
			noise();
			~noise();

			void init(); // initialise value noise
	};

	template< <class> class NoiseT, typename ContextT>
	void noise<NoiseT, ContextT>::init()
	{
		
	}
	*/

};

#endif

