
#ifndef __CCMATH_SPLINE_H__
#define __CCMATH_SPLINE_H__

#include <boost/call_traits.hpp>
#include <boost/type_traits.hpp>
#include <OpenEXR/ImathMatrix.h>
#include <iostream>
#include <ccmath/clamp.h>
#include <vector>

namespace ccmath
{

	//For a derivation of Catmull Rom splines and basis matrix, refer to "Catmull Rom splines", Christopher Twigg, 2003.
	
	//This spline function was based on the spline implementation presented in Texturing & Modeling: A Procedural Approach, 3rd Edition.


	template<typename T, bool transpose>
	struct spline_basis
	{
		//Coefficients for basis matrix
		static const Imath::Matrix44<T> coeff;

		static const T c00, c01, c02, c03;
		static const T c10, c11, c12, c13;
		static const T c20, c21, c22, c23;
		static const T c30, c31, c32, c33;
	};

	/****************************************************************************************************/

	template<typename T, bool transpose>
	const Imath::Matrix44<T> spline_basis<T, transpose>::coeff(
			c00, c01, c02, c03,
			c10, c11, c12, c13,
			c20, c21, c22, c23,
			c30, c31, c32, c33
			);

	template<typename T>
	struct spline_basis<T, false>
	{
		static const Imath::Matrix44<T> coeff;

		static const T c00 = 0.0;
		static const T c01 = 1.0;
		static const T c02 = 0.0;
		static const T c03 = 0.0;
		static const T c10 =-0.5;
		static const T c11 = 0.0;
		static const T c12 = 0.5;
		static const T c13 = 0.0;
		static const T c20 = 0.0;
		static const T c21 = 1.0;
		static const T c22 = 0.0;
		static const T c23 = 0.0;
	};
	
	template<typename T>
	struct spline_basis<T, true>
	{
		static const Imath::Matrix44<T> coeff;

		static const T c00 = 0.0;
		static const T c01 = 1.0;
		static const T c02 = 0.0;
		static const T c03 = 0.0;
	};

/*		0.0, 1.0, 0.0, 0.0,
			-0.5, 0.0, 0.5, 0.0,
			1.0, -2.5, 2.0, -0.5, 
			-0.5, 1.5, -1.5, 0.5 */

/* 0.00000  -0.50000   1.00000  -0.50000
   1.00000   0.00000  -2.50000   1.50000
   0.00000   0.50000   2.00000  -1.50000
   0.00000   0.00000  -0.50000   0.50000
*/

	/****************************************************************************************************/

	template<typename T>
	//typename T::value_type spline(typename T::value_type x, typename boost::call_traits<T>::param_type knots)
	//float spline(float x, typename boost::call_traits<T>::const_reference knots)
	
	typename T::value_type spline(typename T::value_type x, T const& knots)
	{
		typedef typename T::value_type value_type;

		typename T::const_iterator knotIt = knots.begin();
		value_type v;

		size_t span;
		size_t numKnots = knots.size();
		size_t nspans = numKnots - 3;

		if (numKnots < 4) // illegal
		{
			std::cerr << "Spline has too few knots. " << std::endl;
			return 0;
		}

		//std::cout << "nspans: " << nspans << std::endl;
		//Find the appropriate 4-point span of the spline
		v = clamp(x, 0.f, 1.f) * nspans;
		//std::cout << "v: " << v << std::endl;
		
		span = static_cast<size_t>( floor(v) );

		if (span >= numKnots - 3)
			span = numKnots - 3;
		v -= span;

		knotIt += span; // advance the iterator to the correct place
		value_type k0 = *knotIt; ++knotIt;
		value_type k1 = *knotIt; ++knotIt;
		value_type k2 = *knotIt; ++knotIt;
		value_type k3 = *knotIt; 

		Imath::Vec4<value_type> k(k0, k1, k2, k3);
		//Calculate the coefficients of the cubic polynomial
		Imath::Vec4<value_type> r = k * spline_basis<value_type, false>::value;


		//Evaluate the cubic polynomial.
		return r[0] + x*(r[1] + x*(r[2] + x*r[3])); 
	}

};

#endif

