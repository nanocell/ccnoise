
#include <ccnoise/value.h>

#include <ccmath/spline.h>
#include <ccmath/abs.h>

#include <OpenEXR/ImathVec.h>
#include <ccmath/traits/type_traits.h>

#include <sysexits.h>
#include <iostream>
#include <vector>

#include <boost/progress.hpp>

void test_1d_noise()
{	
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
}

void test_3d_noise()
{	
	ccnoise::value<float> noise(1024);
	noise.init();

	Imath::Vec3<float> x(1.f, 2.f, 3.f);
	Imath::Vec3<float> r;

	noise.get(r, x);

	std::cout << "3d noise test: " << std::endl;
	std::cout << "input: " << x << std::endl;
	std::cout << "output: " << r << std::endl;
}

void perform_noise_calls(size_t count)
{
	boost::progress_timer t;
	boost::progress_display t_display(count);
	std::cout << "performing " << count << " noise calls." << std::endl;

	ccnoise::value<float> noise(1024);
	noise.init();

	float f = 0.f;
	float n, finc = 0.001;
	for (unsigned int i = 0; i < count; ++i)
	{
		noise.get(n, f);
		f+=finc;
		++t_display;
	}
	std::cout << "done." << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "initialising noise context..." << std::endl;

	test_1d_noise();

	test_3d_noise();

	perform_noise_calls(10000000);
	
	return EX_OK;
}

