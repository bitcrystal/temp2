#include "random.h"
namespace myrand
{
	void _srand(int seed)
	{
		if(seed==0)
		{
			srand(1420297560);
		} else if (seed < 0) {
			srand(time(NULL));
		} else {
			srand(seed);
		}
	}

	int _rand()
	{
		return rand();
	}

	int _rand(unsigned int min, unsigned int max)
	{
		if(min>=max)
			return -1;
		int number=0;
		while(true)
		{
			number=rand()%max;
			if(number>min)
				break;
		}
		return number;
	}
}