
#ifndef __CCMATH_SPLINE_H__
#define __CCMATH_SPLINE_H__

#include <boost/call_traits.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include <OpenEXR/ImathMatrix.h>
#include <iostream>
#include <ccmath/clamp.h>
#include <ccmath/floor.h>
#include <vector>

namespace ccmath
{

	//For a derivation of Catmull Rom splines and the basis matrix, refer to "Catmull Rom splines", Christopher Twigg, 2003.
	
	/****************************************************************************************************/
	
	template<typename T>
	struct spline_basis_normal
	{
		typedef spline_basis_normal<T> type;
		typedef T value_type;
		typedef Imath::Matrix44<T> matrix_type;

		//Coefficients for basis matrix
		static const matrix_type coeff;
	};

	template<typename T>
	const Imath::Matrix44<T> spline_basis_normal<T>::coeff(
			0.0, 1.0, 0.0, 0.0,
			-0.5,0.0, 0.5, 0.0,
			1.0,-2.5, 2.0,-0.5,
			-0.5,1.5,-1.5, 0.5
			);

	/****************************************************************************************************/

	template<typename T>
	struct spline_basis_transposed
	{
		typedef spline_basis_transposed<T> type;
		typedef T value_type;
		typedef Imath::Matrix44<T> matrix_type;

		//Coefficients for basis matrix
		static const matrix_type coeff;
	};

	template<typename T>
	const Imath::Matrix44<T> spline_basis_transposed<T>::coeff(
			0.0,-0.5, 1.0,-0.5,
			1.0, 0.0,-2.5, 1.5,
			0.0, 0.5, 2.0,-1.5,
			0.0, 0.0,-0.5, 0.5
			);

	/****************************************************************************************************/

	template<typename T, bool transpose>
	struct spline_basis : 
		boost::mpl::eval_if< boost::mpl::bool_<transpose>, spline_basis_normal<T>, spline_basis_transposed<T> >::type
	{	};


	/****************************************************************************************************/

	//This spline function is based on the spline implementation presented in Texturing & Modeling: A Procedural Approach, 3rd Edition.
	
	/****************************************************************************************************/

	template<typename sampleT, typename knotValueType, typename knotItType>
	//void spline1(typename knotT::value_type& result, const sampleT& x, knotIt knots, size_t numKnots)
	void spline1(knotValueType& result, const sampleT& x, knotItType knotIterator, size_t numKnots)
	{
		//sampleT must a floating point value
		BOOST_STATIC_ASSERT(( boost::is_floating_point<sampleT>::value ));

		typedef knotValueType knot_value_t;

		knotItType knotIt = knotIterator;
		sampleT v, w;

		size_t span;
		size_t nspans = numKnots - 3;

		if (numKnots < 4) // illegal
		{
			//TODO: throw exception
			std::cerr << "Spline doesn't have enough knots. " << std::endl;
			return;
		}
		
		//Find the appropriate 4-point span of the spline
		v = x;
		clamp(v, 0.f, 1.f);
		v = v * nspans;

		ccmath::floor(w,v);
		span = static_cast<size_t>( w );

		if (span > numKnots - 4)
			span = numKnots - 4;

		if (nspans > 1)
		{
			//We don't need to adjust these values if we only have one span.
			v -= span;
			knotIt += span; // advance the iterator to the correct span
		}

		const knot_value_t& k0 = *knotIt; ++knotIt;
		const knot_value_t& k1 = *knotIt; ++knotIt;
		const knot_value_t& k2 = *knotIt; ++knotIt;
		const knot_value_t& k3 = *knotIt; 

		Imath::Vec4<knot_value_t> k(k0, k1, k2, k3);
		const typename spline_basis<float, true>::matrix_type& m = spline_basis<float, false>::coeff;

		//Calculate the coefficients of the cubic polynomial
		Imath::Vec4<knot_value_t> r = k * m;

		//Evaluate the cubic polynomial.
		result = r[0] + v*(r[1] + v*(r[2] + v*r[3]));
		//result = ((r[2] + v*r[3])*v + r[1])*v + r[0];
	}
	
	/****************************************************************************************************/

	template<typename sampleT, typename knotValueType, typename knotItType>
	//void spline1(typename knotT::value_type& result, const sampleT& x, knotIt knots, size_t numKnots)
	void spline2(knotValueType& result, const sampleT& x, knotItType knotIterator, size_t numKnots)
	{
		//sampleT must a floating point value
		BOOST_STATIC_ASSERT(( boost::is_floating_point<sampleT>::value ));

		typedef knotValueType knot_value_t;

		knotItType knotIt = knotIterator;
		sampleT v,w;

		size_t span;
		size_t nspans = numKnots - 3;

		if (numKnots < 4) // illegal
		{
			//TODO: throw exception
			std::cerr << "Spline doesn't have enough knots. " << std::endl;
			return;
		}
		
		//Find the appropriate 4-point span of the spline
		v = x;
		clamp(v, 0.f, 1.f);
		v = v * nspans;

		ccmath::floor(w,v);
		span = static_cast<size_t>( w );

		if (span > numKnots - 4)
			span = numKnots - 4;

		if (nspans > 1)
		{
			//We don't need to adjust these values if we only have one span.
			v -= span;
			knotIt += span; // advance the iterator to the correct span
		}

		const knot_value_t& k0 = *knotIt; ++knotIt;
		const knot_value_t& k1 = *knotIt; ++knotIt;
		const knot_value_t& k2 = *knotIt; ++knotIt;
		const knot_value_t& k3 = *knotIt; 

		Imath::Vec4<knot_value_t> k(k0, k1, k2, k3);
		const typename spline_basis<float, true>::matrix_type& m = spline_basis<float, false>::coeff;

		//Calculate the coefficients of the cubic polynomial
		Imath::Vec4<knot_value_t> r = k * m;

		//Evaluate the cubic polynomial.
		result = r[0] + v*(r[1] + v*(r[2] + v*r[3]));
	}

	/****************************************************************************************************/

	//Input: result reference, spline sample, structure with indexable knots (operator[] overload)
	template<typename sampleT, typename knotT>
	inline void spline(typename knotT::value_type& result, const sampleT& x, const knotT& knots)
	{
		spline2<sampleT, typename knotT::value_type, typename knotT::const_iterator>(result, x, knots.begin(), knots.size() );
	}
	
	/****************************************************************************************************/
	
	//Input: result reference, spline sample, pointer to array of knots, number of knots
	template<typename sampleT, typename knotT>
	inline void spline(knotT& result, const sampleT& x, const knotT* knots, size_t numknots)
	{
		spline1<sampleT, knotT, const knotT*>(result, x, knots, numknots );
	}

	/****************************************************************************************************/

	//TODO: Enable this function only if knotT is a container with in iterator
	//      or add a compile time exception to verify this
	template<typename sampleT, typename knotT>
	inline typename knotT::value_type spline(const sampleT& x, const knotT& knots)
	{
		typename knotT::value_type r;
		spline(r, x, knots); //invoke the container
		return r;
	}
	
	/****************************************************************************************************/
	
	template<typename sampleT, typename knotT>
	inline knotT spline(const sampleT& x, const knotT* knots, size_t numknots)
	{
		knotT r;
		spline(r, x, knots, numknots);
		return r;
	}
	
	/****************************************************************************************************/

};

#endif

