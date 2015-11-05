#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "run_cmd.h"

#define END (const char *)0

int main(int argc, char * argv[])
{
	string output = run_cmd("/usr/bin/which", 
	"which", 
	"ls", 
	END);

	printf("%s\n", output.size() ? output.c_str() : "no result!");

	/*
		检测adb是否存在 ！
	*/
	string pathOfAdb;
	output = run_cmd("/usr/bin/which", 
	"which", 
	"adb", 
	END);

	if(output.size() == 0)
	{
		printf("can not find adb ! exit! \n");
		exit(0);
	}

	pathOfAdb = output.substr(0, output.find("adb") + 3);
	printf("\nfind adb path : %s\n", pathOfAdb.c_str());

	/*
		检测连接到的手机
	*/
	output = run_cmd(pathOfAdb.c_str(),
	"adb",
	"devices",
	END);
	printf("%s\n", output.size() ? output.c_str() : "no result!");

	printf("<<<<<<<<<<<<<<<<<<<<<<<< main out !\n");
	return 0;
}
