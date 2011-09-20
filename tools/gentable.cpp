
//Generate tables of pseudo random numbers for noise lookups purposes.

#include <iostream>
#include <sysexits.h>
#include <boost/random/mersenne_twister.hpp> //boost::random::mt19937 generator
#include <boost/random/uniform_int.hpp>

class PRNGenerator
{
	public:
		PRNGenerator(unsigned int tablesize) : _tablesize(tablesize), dist(0,tablesize-1) { };

		unsigned int size() { return _tablesize; }
		unsigned int get() { return dist(gen); }


	private:
		boost::mt19937 gen;
		boost::uniform_int<> dist;
		unsigned int _tablesize;
};

template<class PRN>
struct header_builder
{
	header_builder(unsigned int tablesize) : _prn(tablesize) { }

	void build()
	{
		unsigned int tablesize = _prn.size();
		std::cout << "static unsigned char perm[" << tablesize << "] = { " << std::endl;
		for (unsigned int i = 0; i < tablesize; ++i)
		{
			std::cout << _prn.get();
			if (i < tablesize-1)
				std::cout << ",";
		}
		std::cout << std::endl << "};" << std::endl;
	}

	PRN _prn;
};

//output
/*
namespace 
	 struct prn
	 {
	 	static const unsigned int tablesize = 256;
		static const unsigned int tabmask = (256 -1);
		static unsigned int perm_value(x) { return permtable[ x & tabmask ]; }
	 }
*/



int main(int argc, char* argv[])
{
	unsigned int tablesize = 256;

	header_builder<PRNGenerator> hdr(tablesize);

	hdr.build();

	std::cout << "done!" << std::endl;

	return EX_OK;
};

