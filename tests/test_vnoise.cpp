
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


	//ccnoise::value_noise<float> vnoise(256);
	//vnoise.init();


	


	return EX_OK;
}

