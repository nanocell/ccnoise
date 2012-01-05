
#ifndef __VALUE_NOISE_H__
#define __VALUE_NOISE_H__

#include <boost/scoped_ptr.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <vector>
//#include <cmath>

#include <ccmath/spline.h>
#include <ccmath/abs.h>

namespace ccnoise
{

	template<typename T>
	class value_noise
	{
		public:
			value_noise(unsigned int tablesize);
			value_noise();
			~value_noise();

			void init();

			//Generate 1D noise value from 3D input
			void get(T& r, T x, T y, T z);
			void get(T& r, T x);

			/* Populate a table containing pseudo random values.
			 * The generation of this table may differ depending on the noise algorithm.
			 * For value noise, it is a table of PRNs between -1 and 1.
			 *
			 */
		
		protected:
	
			//TODO: Implement getPermutedIndex as recursive template function (templated on dimension)
			// e.g. getPermutedIndex<4>(ix, iy, iz, it);
			unsigned int getPermutedIndex(int ix, int iy, int iz) { return perm( ix + perm(iy + perm(iz)) ); }
			unsigned int getPermutedIndex(int ix) { return perm( ix ); }
			
			unsigned int perm(int i) { return _permtable[ ccmath::abs(i % _tablesize) ]; }

			//Get the PRN on the lattice associated with the integer lattice coords
			T getPrn(int ix, int iy, int iz) { return _prntable[ getPermutedIndex(ix, iy, iz) ]; }
			T getPrn(int ix) { return _prntable[ getPermutedIndex(ix) ]; }

			void init_permutation_table();
			void init_prn_table();
			
			template<typename IteratorT>
			void populate_prn_table(IteratorT it_begin, IteratorT it_end);

			unsigned int _tablesize;
			std::vector<T> _prntable;
			std::vector<unsigned int> _permtable;
	};

	/****************************************************************************************************/

	template <typename T> template<typename IteratorT>
	void value_noise<T>::populate_prn_table(IteratorT it_begin, IteratorT it_end)
	{
		std::cout << "value_noise::populate_prn_table" << std::endl;	
		
		//Perform a random shuffle, using the mersenne twister and random number generator
		boost::mt19937 gen; // generate raw random numbers
		boost::uniform_real<> dist(-1,1); // distribution of random numbers to generate.
		boost::variate_generator< boost::mt19937, boost::uniform_real<> > draw_rnd(gen, dist);

		//Fill the data table with pseudo random values between -1 and 1
		for (IteratorT it = it_begin; it != it_end; ++it)
		{
			*it = draw_rnd(); // draw the next random number
		}
	}
	
	//************************************************************************************************


	template<typename T>
	value_noise<T>::value_noise(unsigned int tablesize)
		: _tablesize(tablesize)
	{

	}

	/****************************************************************************************************/

	template<typename T>
	value_noise<T>::value_noise()
	{
		_tablesize = 256;
	}

	/****************************************************************************************************/

	template<typename T>
	value_noise<T>::~value_noise()
	{

	}

	/****************************************************************************************************/
	
	template<typename T>
	void value_noise<T>::init()
	{
		init_permutation_table();
		init_prn_table();
	}

	/****************************************************************************************************/

	template<typename T>
	void value_noise<T>::get(T& r, T x, T y, T z)
	{
		int ix, iy, iz;
		int i, j, k;
		T fx, fy, fz;
		T xknots[4], yknots[4], zknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;
		iy = floor(y);
		fy = y - iy;
		iz = floor(z);
		fz = z - iz;

		for (k = 0; k < 4; ++k)
		{
			for (j = 0; j < 4; ++j)
			{
				for (i = 0; i < 4; ++i)
				{
					xknots[i] = getPrn(ix+i-1, iy+j-1, iz+k-1);
				}
				ccmath::spline(yknots[j], fx, xknots, 4);
			}
			ccmath::spline(zknots[k], fy, yknots, 4);
		}
		ccmath::spline(r, fz, zknots, 4);
	}
	
	/****************************************************************************************************/

	template<typename T>
	void value_noise<T>::get(T& r, T x)
	{
		int ix;
		int i;
		T fx;
		T xknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;

		for (i = 0; i < 4; ++i)
		{
			xknots[i] = getPrn(ix+i-1);
		}
		ccmath::spline(r, fx, xknots, 4);
	}

	/****************************************************************************************************/

	template<typename T>
	void value_noise<T>::init_permutation_table() 
	{
		_permtable.resize(_tablesize);

		//Fill the permtable with values and permute them
		std::cout << "value_noise::init_permutation_table..." << std::endl;
		for (int i = 0; i < _tablesize; ++i)
		{
			_permtable[i] = i;
		}

		//Perform a random shuffle, using the mersenne twister and random number generator
		boost::mt19937 gen; // generate raw random numbers
		boost::uniform_int<> dist(0,_tablesize-1); // distribution of random numbers to generate.
		boost::variate_generator< boost::mt19937, boost::uniform_int<> > draw_rnd(gen, dist);

		boost::random_shuffle(_permtable, draw_rnd);

		//The permtable is now shuffled, and will serve as random lookups into the data table.
	}

	//************************************************************************************************

	template<typename T>
	void value_noise<T>::init_prn_table() 
	{
		std::cout << "value_noise::init_prn_table..." << std::endl;
		_prntable.resize(_tablesize);

		populate_prn_table(_prntable.begin(), _prntable.end());

		//The data table is now populated and can be used with the NoiseT policy to generate some noise.
	}

	/****************************************************************************************************/

};

#endif

