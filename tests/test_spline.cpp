
//#include <ccnoise/value_noise.h>

#include <ccmath/spline.h>
#include <ccmath/abs.h>

#include <sysexits.h>
#include <iostream>
#include <vector>
#include <limits>

//namespace ccn = ccnoise;

int main(int argc, char* argv[])
{
	std::cout << "ccmath::spline interpolation test." << std::endl;

	//spline interpolation tests:
	std::vector<float> xknots;
	xknots.push_back(0.1f);
	xknots.push_back(0.9f);
	xknots.push_back(0.5f);
	xknots.push_back(0.3f);


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

	std::cout << "knots: " << xknots[0] << ", " << xknots[1] << ", " << xknots[2] << ", " << xknots[3] << std::endl;
	std::cout << "spline(-5, knots) == 0.9  =>  " << ( ccmath::abs(ccmath::spline(-5.f,xknots) - xknots[1]) < std::numeric_limits<float>::epsilon() ) << std::endl;
	std::cout << "spline( 0, knots) == 0.9  =>  " << ( ccmath::abs(ccmath::spline( 0.f,xknots) - xknots[1]) < std::numeric_limits<float>::epsilon() ) << std::endl;
	std::cout << "spline( 1, knots) == 0.5  =>  " << ( ccmath::abs(ccmath::spline( 1.f,xknots) - xknots[2]) < std::numeric_limits<float>::epsilon() ) << std::endl;
	std::cout << "spline( 3, knots) == 0.5  =>  " << ( ccmath::abs(ccmath::spline( 3.f,xknots) - xknots[2]) < std::numeric_limits<float>::epsilon() ) << std::endl;

	//TODO: Add multidimensional spline interpolation support, and subsequent tests.

	return EX_OK;
}

