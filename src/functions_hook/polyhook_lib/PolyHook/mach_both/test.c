#include "whereami_ex.h"
#include "whereami_ex.c"

#include <stdio.h>

int main()
{
	printf("%s\n",getExecutablePath('/'));
}
