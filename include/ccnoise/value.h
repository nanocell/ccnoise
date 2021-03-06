
#ifndef __value_H__
#define __value_H__

#include <boost/scoped_ptr.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/int.hpp>
#include <vector>

#include <ccmath/abs.h>
#include <ccmath/floor.h>
#include <ccmath/spline.h>
#include <ccmath/traits/imath_type_traits.h>


namespace ccnoise
{

	template<typename T>
	class value
	{
		public:
			value(unsigned int tablesize);
			value();
			~value();

			void init();

			//Generate 1D noise value from 4D input (x,y,z,time)
			//void get(T& r, T x, T y, T z, T w);
			
			////Generate 3D noise value from 3D input.
			//template<typename VecT>
			//inline typename boost::disable_if< boost::is_floating_point<VecT>, void>::type get(VecT& r, const VecT& x)
			//{
			//	get1<VecT>(r,x);
			//}
			//
			////Generate 1D noise value from 1D input.
			//template<typename VecT>
			//inline typename boost::enable_if< boost::is_floating_point<VecT>, void>::type get(VecT& r, VecT x)
			//{
			//	get2<VecT>(r,x);
			//}

			//Generate 1D noise value from 1D input.
			void get(T& r, T x);
			//Generate 1D noise value from 2D input.
			void get(T& r, T x, T y);
			//Generate 1D noise value from 3D input
			void get(T& r, T x, T y, T z);
			//Generate 1D noise value from 4D input
			void get(T& r, T x, T y, T z, T w);

			//Generate N-d noise value from 3D input
			//The dimension is defined the size trait
			//template<typename V>
			//void get(V& r, T x, T y, T z);

			/* Populate a table containing pseudo random values.
			 * The generation of this table may differ depending on the noise algorithm.
			 * For value noise, it is a table of PRNs between -1 and 1.
			 *
			 */
		
		protected:

			//Generate 3D noise from 3D vector input
			template <typename VecT>
			void get1(VecT& r, const VecT& x);
			
			//Generate 1D noise from 1D vector input
			template <typename VecT>
			void get2(VecT& r, VecT x);

			//TODO: Move the random value calculation to a more general purpose utility class
			//      Think: random ID seed/value lookups.
	
			//TODO: Implement getPermutedIndex as recursive template function (templated on dimension)
			// e.g. getPermutedIndex<4>(ix, iy, iz, it);
			unsigned int getPermutedIndex(int ix, int iy, int iz, int iw) { return perm( ix + perm(iy + perm(iz + perm(iw))) ); }
			unsigned int getPermutedIndex(int ix, int iy, int iz) { return perm( ix + perm(iy + perm(iz)) ); }
			unsigned int getPermutedIndex(int ix) { return perm( ix ); }
			
			unsigned int perm(int i) { return _permtable[ ccmath::abs(i % _tablesize) ]; }
			
			//Get the PRN on the lattice associated with the integer lattice coords
			T getPrn(int ix, int iy, int iz, int iw) { return _prntable[ getPermutedIndex(ix, iy, iz, iw) ]; }
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
	void value<T>::populate_prn_table(IteratorT it_begin, IteratorT it_end)
	{
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
	value<T>::value(unsigned int tablesize)
		: _tablesize(tablesize)
	{

	}

	/****************************************************************************************************/

	template<typename T>
	value<T>::value()
	{
		_tablesize = 1024;
	}

	/****************************************************************************************************/

	template<typename T>
	value<T>::~value()
	{

	}

	/****************************************************************************************************/
	
	template<typename T>
	void value<T>::init()
	{
		init_permutation_table();
		init_prn_table();
	}

	
	/****************************************************************************************************/

	template<typename T>
	void value<T>::get(T& r, T x)
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

	//Generate 1D noise value from 2D input.
	template<typename T>
	void value<T>::get(T& r, T x, T y)
	{
		int ix, iy;
		int i, j;
		T fx, fy;
		T xknots[4], yknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;
		iy = floor(y);
		fy = y - iy;

		for (j = 0; j < 4; ++j)
		{
			for (i = 0; i < 4; ++i)
			{
				xknots[i] = getPrn(ix+i-1, iy+j-1);
			}
			ccmath::spline(yknots[j], fx, xknots, 4);
		}
		ccmath::spline(r, fy, yknots, 4);
	}
	
	/****************************************************************************************************/

	//1D noise value from 4D input (x,y,z,time)
	template<typename T>
	void value<T>::get(T& r, T x, T y, T z, T w)
	{
		int ix, iy, iz, iw;
		int i, j, k, l;
		T fx, fy, fz, fw;
		T xknots[4], yknots[4], zknots[4], wknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;
		iy = floor(y);
		fy = y - iy;
		iz = floor(z);
		fz = z - iz;
		iw = floor(w);
		fw = w - iw;

		for (l = 0; l < 4; ++l)
		{
			for (k = 0; k < 4; ++k)
			{
				for (j = 0; j < 4; ++j)
				{
					for (i = 0; i < 4; ++i)
					{
						xknots[i] = getPrn(ix+i-1, iy+j-1, iz+k-1, iw+l-1);
					}
					ccmath::spline(yknots[j], fx, xknots, 4);
				}
				ccmath::spline(zknots[k], fy, yknots, 4);
			}
			ccmath::spline(wknots[l], fz, zknots, 4);
		}
		ccmath::spline(r, fw, wknots, 4);
	}

	/****************************************************************************************************/

	template<typename T>
	void value<T>::get(T& r, T x, T y, T z)
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
	
//	template <typename T> template <typename VecT>
//	void value<T>::get1(VecT& r, const VecT& x)
//	{
//		namespace mpl = boost::mpl;
//		//Get 3D noise value from 3D input
//
//		//The VecT is assume to have at least a size of 3, and has a value_type that matches T
//		BOOST_STATIC_ASSERT(( mpl::greater_equal< typename ccmath::type_traits<VecT>::size, mpl::int_<3> >::value ));
//		BOOST_STATIC_ASSERT(( boost::is_same< typename ccmath::type_traits<VecT>::value_type, T >::value )); 
//
//		int ix, iy, iz;
//		int i, j, k;
//		VecT f, fi;
//		VecT xknots[4], yknots[4], zknots[4];
//
//		//simple cubic catmull rom spline interpolation implementation
//
//		ccmath::floor(fi, x);
//		f = x - fi;
//
//		for (k = 0; k < 4; ++k)
//		{
//			for (j = 0; j < 4; ++j)
//			{
//				for (i = 0; i < 4; ++i)
//				{
//					//Generate a vector with 3 different PRNs by simply rotating the order of the input coordinates
//					xknots[i] = VecT( getPrn(fi[0]+i-1, fi[1]+j-1, fi[2]+k-1), getPrn(fi[2]+k-1, fi[0]+i-1, f[1]+j-1), getPrn(fi[1]+j-1, fi[2]+k-1, fi[0]+i-1) );
//				}
//				ccmath::spline(yknots[j], f[0], xknots, 4);
//			}
//			ccmath::spline(zknots[k], f[1], yknots, 4);
//		}
//		ccmath::spline(r, f[2], zknots, 4);
//	} 
//
//	/****************************************************************************************************/
//	template <typename T> template <typename VecT>
//	void value<T>::get2(VecT& r, VecT x)
//	{
//		//Get 1D noise value from 1D input
//		int ix;
//		int i;
//		T fx;
//		T xknots[4];
	
	//Generate 1D noise value from 4D input
	template<typename T>
	void value<T>::get(T& r, T x, T y, T z, T w)
	{
		int ix, iy, iz, iw;
		int i, j, k, l;
		T fx, fy, fz, fw;
		T xknots[4], yknots[4], zknots[4], wknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;
		iy = floor(y);
		fy = y - iy;
		iz = floor(z);
		fz = z - iz;
		iw = floor(w);
		fw = w - iz;;

		for (l = 0; l < 4; ++l)
		{
			for (k = 0; k < 4; ++k)
			{
				for (j = 0; j < 4; ++j)
				{
					for (i = 0; i < 4; ++i)
					{
						xknots[i] = getPrn(ix+i-1, iy+j-1, iz+k-1, iw+l-1);
					}
					ccmath::spline(yknots[j], fx, xknots, 4);
				}
				ccmath::spline(zknots[k], fy, yknots, 4);
			}
			ccmath::spline(wknots[k], fz, zknots, 4); 
		}
		ccmath::spline(r, fw, wknots, 4);
	}

	/****************************************************************************************************/
	
	//Generate N-d noise value from 4D input
	template<typename T> template<typename VecT>
	void value<T>::get(VecT& r, T x, T y, T z, T w)
	{
		int ix, iy, iz, iw;
		int i, j, k, l;
		T fx, fy, fz, fw;
		T xknots[4], yknots[4], zknots[4], wknots[4];

		//simple cubic catmull rom spline interpolation implementation

		ix = floor(x);
		fx = x - ix;
		iy = floor(y);
		fy = y - iy;
		iz = floor(z);
		fz = z - iz;
		iw = floor(w);
		fw = w - iz;;

		for (l = 0; l < 4; ++l)
		{
			for (k = 0; k < 4; ++k)
			{
				for (j = 0; j < 4; ++j)
				{
					for (i = 0; i < 4; ++i)
					{
						xknots[i] = getPrn(ix+i-1, iy+j-1, iz+k-1, iw+l-1);
					}
					ccmath::spline(yknots[j], fx, xknots, 4);
				}
				ccmath::spline(zknots[k], fy, yknots, 4);
			}
			ccmath::spline(wknots[k], fz, zknots, 4); 
		}
		ccmath::spline(r, fw, wknots, 4);
	}
	/****************************************************************************************************/

	template<typename T>
	void value<T>::init_permutation_table() 
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
	void value<T>::init_prn_table() 
	{
		_prntable.resize(_tablesize);

		populate_prn_table(_prntable.begin(), _prntable.end());

		//The data table is now populated and can be used with the NoiseT policy to generate some noise.
	}

	/****************************************************************************************************/
};

#endif

