
#ifndef __PARM_DEFAULTS_H__
#define __PARM_DEFAULTS_H__

namespace cchdk
{

	/****************************************************************************************************/
	
	template<std::size_t N>
	struct Defaults
	{
		static const std::size_t elements = N;
		typedef Defaults<N> type;

		PRM_Default* get() { return &(this->default_values[0]); }

		PRM_Default& operator[](std::size_t i) { return default_values[i]; }
		//Unfortunately, extended initializer lists are not available yet (unless using C++0x), so
		//we'll generate explicit constructors for Defaults object of various sizes (MakeDefaults functions below).
		boost::array<PRM_Default, N> default_values;
	};

	/****************************************************************************************************/

	template<>
	struct Defaults<0> 
	{
		static const std::size_t elements = 0;
		typedef Defaults<0> type;

		PRM_Default* get() { return 0; }
	};

	/****************************************************************************************************/

	template<std::size_t N = 0>
	struct NoDefaultsT : Defaults<0> {};

	typedef NoDefaultsT<0> NoDefaults;

	typedef Defaults<1> Defaults1;
	typedef Defaults<2> Defaults2;
	typedef Defaults<3> Defaults3;
	typedef Defaults<4> Defaults4;

	/****************************************************************************************************/

	Defaults1 MakeDefaults(float d0) { Defaults1 d; d[0] = d0; return d; }
	Defaults2 MakeDefaults(float d0, float d1) { Defaults2 d; d[0] = d0; d[1] = d1; return d; }
	Defaults3 MakeDefaults(float d0, float d1, float d2) { Defaults3 d; d[0] = d0; d[1] = d1; d[2] = d2; return d; }
	Defaults4 MakeDefaults(float d0, float d1, float d2, float d3) { Defaults4 d; d[0] = d0; d[1] = d1; d[2] = d2; d[3] = d3; return d; }

	/****************************************************************************************************/
}

#endif

