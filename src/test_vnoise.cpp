
#include <ccnoise/noise_context.h>
#include <sysexits.h>
#include <iostream>

namespace ccn = ccnoise;

int main(int argc, char* argv[])
{
	std::cout << "initialising noise context..." << std::endl;
	ccn::noise<ccn::value, float> ctx(256);

	std::cout << "done." << std::endl;

	return EX_OK;
}

