#ifndef RANDOM_H
#define RANDOM_H
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
namespace myrand
{
	extern void _srand(int seed);
	extern int _rand();
	extern int _rand(unsigned int min, unsigned int max);
}
#endif