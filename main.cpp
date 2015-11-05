#include <stdio.h>
#include <iostream>

#include "run_cmd.h"

int main(int argc, char * argv[])
{
	string output = run_cmd("/usr/bin/which", 
	"which", 
	"ls", 
	(const char *)0);

	printf("%s\n", output.size() ? output.c_str() : "no result!");

	output = run_cmd("/usr/bin/which", 
	"which", 
	"adb", 
	(const char *)0);

	printf("%s\n", output.size() ? output.c_str() : "no result!");

	output = run_cmd("/bin/ls", 
	"ls", 
	"-a", 
	(const char *)0);

	printf("%s\n", output.size() ? output.c_str() : "no result!");

	printf("<<<<<<<<<<<<<<<<<<<<<<<< main out !\n");
	return 0;
}
