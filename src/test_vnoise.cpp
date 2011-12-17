
//#include <ccnoise/value_noise.h>

#include <ccmath/spline.h>

#include <ccmath/abs.h>
#include <sysexits.h>
#include <iostream>
#include <vector>

//namespace ccn = ccnoise;

int main(int argc, char* argv[])
{
	std::cout << "initialising noise context..." << std::endl;
	//ccn::noise<ccn::value, float> ctx(256);
	

	//Jotting down some ideas for generating noise.
	//std::vector< Vec3 > data_in;
	//std::vector< float > data_out;
	//ccn::noise<ccn::value, float> ctx(256);
	//ccn::turbulence<ccn::value> turb(ctx);
	//
	//noise algorithm, base type, dimensions
	//
	//ccn::noise<ccn::value, float, 3>( data_in.begin(), data_in.end(), data_out.begin() );
	//ccn::turbulence<ccn::value>
	//


	
	//ccn::value_noise<float> vnoise(256);
	//vnoise.init();

	//float f = vnoise.get(1.2f, 3.1f, 4.7f);

	//spline interpolation tests:
	std::vector<float> xknots;
	xknots.push_back(0.1f);
	xknots.push_back(0.9f);
	xknots.push_back(0.5f);
	xknots.push_back(0.3f);
	//xknots.push_back(0.9f);


	typedef std::vector<float> vecf;
	//typedef boost::call_traits<vecf>::const_reference vec_ref;
	typedef boost::add_reference<vecf>::type vec_ref;

	//std::cout << "is same: " << boost::is_same< std::vector<float> const&, vec_ref >::value << std::endl;

	float fstart = 0.f;
	float fend = 1.f;
	float finc = 0.1;
	float sample;
	
	std::cout << "x = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << sample << " ";
	}
	
	std::cout << "];" << std::endl;

	std::cout << "y = [";
	for (sample = fstart; sample < fend+finc; sample += finc)
	{
		std::cout << ccmath::spline(sample, xknots) << " ";
	}
	
	std::cout << "];" << std::endl;

	std::cout << "plot(x,y)" << std::endl;
	return EX_OK;
}

