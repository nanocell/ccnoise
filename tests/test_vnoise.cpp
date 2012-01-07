
#include <ccnoise/value_noise.h>

#include <ccmath/spline.h>

#include <ccmath/abs.h>
#include <sysexits.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	std::cout << "initialising noise context..." << std::endl;
	ccnoise::value<float> noise(1024);
	noise.init();

	float n;
	float f = 0.0f;
	float finc = 0.01;

	for (f = 0.0f; f < 1.0f; f += finc)
	{
		noise.get(n, f);
		std::cout << "[" << f << "] " << n << std::endl;
	}
	
	/*
	int count = 10000000;
	std::cout << "performing " << count << " noise calls." << std::endl;
	f = 0.f;
	for (unsigned int i = 0; i < count; ++i)
	{
		noise.get(n, f);
		f+=finc;
	}
	std::cout << "done." << std::endl;
	
	*/


	


	return EX_OK;
}

