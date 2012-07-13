
#ifndef __GRADIENT_UTILS_H__
#define __GRADIENT_UTILS_H__

#include <ccnoise/gradient.h>

namespace ccnoise
{

	//Generic gradient utils contains no implementations of any kind
	template <typename T, int dim>
	struct gradient_utils {	};

	//***********************************************************************************************
	//Gradient Utils (2D)
	//***********************************************************************************************
	template <typename T>
	struct gradient_utils<T, 1> : gradient<T, 1>
	{
		//Generate 1D noise from 21 input
		void get_gradient(T& r, T x);
	};
	
//***********************************************************************************************

	template<typename T>
	void gradient_utils<T, 1>::get_gradient(T& r, T x)
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
			xknots[i] = this->getPrn(ix+i-1);
		}
		ccmath::spline(r, fx, xknots, 4);
	}

	//***********************************************************************************************
	//Gradient Utils (4D)
	//***********************************************************************************************
	template <typename T>
	struct gradient_utils<T, 3> : gradient<T, 3>
	{
		//Generate 1D noise from 3D input
		void get_gradient(T& r, T x, T y, T z);
	};

	template<typename T>
	void gradient_utils<T, 3>::get_gradient(T& r, T x, T y, T z)
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

		v0.x = this->getPrn(vi.x,   vi.y, vi.z, f0.x, f0.y, f0.z);
		v1.x = this->getPrn(vi.x+1, vi.y, vi.z, f1.x, f0.y, f0.z);
		ccmath::lerp(v0.y, w.x, v0.x, v1.x);

		v0.x = this->getPrn(vi.x,  vi.y+1, vi.z, f0.x, f1.y, f0.z);
		v1.x = this->getPrn(vi.x+1, vi.y+1, vi.z, f1.x, f1.y, f0.z);
		ccmath::lerp(v1.y, w.x, v0.x, v1.x);
		ccmath::lerp(v0.z, w.y, v0.y, v1.y);

		v0.x = this->getPrn(vi.x,   vi.y, vi.z+1, f0.x, f0.y, f1.z);
		v1.x = this->getPrn(vi.x+1, vi.y, vi.z+1, f1.x, f0.y, f1.z);
		ccmath::lerp(v0.y, w.x, v0.x, v1.x);

		v0.x = this->getPrn(vi.x,   vi.y+1, vi.z+1, f0.x, f1.y, f1.z);
		v1.x = this->getPrn(vi.x+1, vi.y+1, vi.z+1, f1.x, f1.y, f1.z);
		ccmath::lerp(v1.y, w.x, v0.x, v1.x);
		ccmath::lerp(v1.z, w.y, v0.y, v1.y);

		ccmath::lerp(r, w.z, v0.z, v1.z);	
	}
	
	/****************************************************************************************************/

} // ns ccnoise

#endif

