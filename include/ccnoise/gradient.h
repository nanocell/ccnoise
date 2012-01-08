
#ifndef __gradient_H__
#define __gradient_H__

#include <boost/scoped_ptr.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <vector>
#include <cmath>

#include <ccmath/abs.h>
#include <ccmath/floor.h>
#include <ccmath/lerp.h>
#include <ccmath/smoothstep.h>
#include <ccmath/spline.h>

namespace ccnoise
{

	template<typename T>
	class gradient
	{
		public:
			gradient(unsigned int tablesize);
			gradient();
			~gradient();

			void init();

			//Generate 1D noise value from 3D input
			void get(T& r, T x, T y, T z);
			//Generate 1D noise value from 1D input.
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
			T getPrn(int ix, int iy, int iz, float fx, float fy, float fz) 
			{
				T* t = &_prntable[ getPermutedIndex(ix,iy,iz)*3 ];
				return t[0]*fx + t[1]*fy + t[2]*fz;
			}

			//T getPrn(int ix, float fx) 
			//{ 
			//	return _prntable[ getPermutedIndex(ix)*3 ] * fx; 
			//}

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
	void gradient<T>::populate_prn_table(IteratorT it_begin, IteratorT it_end)
	{
		//Perform a random shuffle, using the mersenne twister and random number generator
		boost::mt19937 gen; // generate raw random numbers
		boost::uniform_real<> dist(0.f,1.f); // distribution of random numbers to generate.
		boost::variate_generator< boost::mt19937, boost::uniform_real<> > draw_rnd(gen, dist);

		float z, r, theta;

		//Fill the data table with pseudo random values between -1 and 1
		for (IteratorT it = it_begin; it != it_end; )
		{
			z = 1.f - 2.f*draw_rnd(); // draw the next random number
			r = sqrtf(1.f-z*z);
			theta = 2.f * M_PI * draw_rnd();
			*it = r*cosf(theta); ++it;
			*it = r*sinf(theta); ++it;
			*it = z; ++it;
		}
	}
	
	//************************************************************************************************


	template<typename T>
	gradient<T>::gradient(unsigned int tablesize)
		: _tablesize(tablesize)
	{

	}

	/****************************************************************************************************/

	template<typename T>
	gradient<T>::gradient()
	{
		_tablesize = 256;
	}

	/****************************************************************************************************/

	template<typename T>
	gradient<T>::~gradient()
	{

	}

	/****************************************************************************************************/
	
	template<typename T>
	void gradient<T>::init()
	{
		init_permutation_table();
		init_prn_table();
	}

	/****************************************************************************************************/

	template<typename T>
	void gradient<T>::get(T& r, T x, T y, T z)
	{

		Imath::Vec3<T> v(x,y,z);
		Imath::Vec3<T> vi;
		Imath::Vec3<T> f0,f1;
		Imath::Vec3<T> w;

		ccmath::floor(vi, v);
		f0 = v-vi; //fractions vector
		f1 = f0 - Imath::Vec3<T>(1, 1, 1);
		ccmath::smoothstep(w, f0);

		Imath::Vec3<T> v0, v1;

		v0.x = getPrn(vi.x,   vi.y, vi.z, f0.x, f0.y, f0.z);
		v1.x = getPrn(vi.x+1, vi.y, vi.z, f1.x, f0.y, f0.z);
		ccmath::lerp(v0.y, w.x, v0.x, v1.x);

		v0.x = getPrn(vi.x,  vi.y+1, vi.z, f0.x, f1.y, f0.z);
		v1.x = getPrn(vi.x+1, vi.y+1, vi.z, f1.x, f1.y, f0.z);
		ccmath::lerp(v1.y, w.x, v0.x, v1.x);
		ccmath::lerp(v0.z, w.y, v0.y, v1.y);

		v0.x = getPrn(vi.x,   vi.y, vi.z+1, f0.x, f0.y, f1.z);
		v1.x = getPrn(vi.x+1, vi.y, vi.z+1, f1.x, f0.y, f1.z);
		ccmath::lerp(v0.y, w.x, v0.x, v1.x);

		v0.x = getPrn(vi.x,   vi.y+1, vi.z+1, f0.x, f1.y, f1.z);
		v1.x = getPrn(vi.x+1, vi.y+1, vi.z+1, f1.x, f1.y, f1.z);
		ccmath::lerp(v1.y, w.x, v0.x, v1.x);
		ccmath::lerp(v1.z, w.y, v0.y, v1.y);

		ccmath::lerp(r, w.z, v0.z, v1.z);	
	}
	
	/****************************************************************************************************/

	template<typename T>
	void gradient<T>::get(T& r, T x)
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
	void gradient<T>::init_permutation_table() 
	{
		_permtable.resize(_tablesize);

		//Fill the permtable with values and permute them
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
	void gradient<T>::init_prn_table() 
	{
		_prntable.resize(_tablesize * 3);

		populate_prn_table(_prntable.begin(), _prntable.end());

		//The data table is now populated and can be used with the NoiseT policy to generate some noise.
	}

	/****************************************************************************************************/
};

#endif

