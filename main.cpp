#include <stdio.h>
#include <iostream>

#include "run_cmd.h"

int main(int argc, char * argv[])
{
	string output = run_cmd();
	printf("%s", output.c_str());

	return 0;
}
