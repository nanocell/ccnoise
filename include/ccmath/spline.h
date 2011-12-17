
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
	

	template<typename T, bool transpose>
	struct spline_basis
	{
		//Coefficients for basis matrix
		static const Imath::Matrix44<T> coeff;
		static const Imath::Matrix44<T> coeff_transposed;
	};

	/****************************************************************************************************/
	
	template<typename T, bool transpose>
	const Imath::Matrix44<T> spline_basis<T, transpose>::coeff(
			0.0, 1.0, 0.0, 0.0,
			-0.5,0.0, 0.5, 0.0,
			1.0,-2.5, 2.0,-0.5,
			-0.5,1.5,-1.5, 0.5
			);

	template<typename T, bool transpose>
	const Imath::Matrix44<T> spline_basis<T, transpose>::coeff_transposed(
			0.0,-0.5, 1.0,-0.5,
			1.0, 0.0,-2.5, 1.5,
			0.0, 0.5, 2.0,-1.5,
			0.0, 0.0,-0.5, 0.5
			);

	/****************************************************************************************************/

	//This spline function is based on the spline implementation presented in Texturing & Modeling: A Procedural Approach, 3rd Edition.

	template<typename T>
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
			std::cerr << "Spline doesn't have enough knots. " << std::endl;
			return 0;
		}

		//Find the appropriate 4-point span of the spline
		v = clamp(x, 0.f, 1.f) * nspans;
		
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
		Imath::Vec4<value_type> r = k * spline_basis<value_type, true>::coeff_transposed;

		//Evaluate the cubic polynomial.
		return r[0] + v*(r[1] + v*(r[2] + v*r[3])); 
	}

};

#endif

