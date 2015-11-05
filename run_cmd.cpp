#include "run_cmd.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>


static string readFromPipe(int pipefd) {

	FILE* pipe = fdopen(pipefd, "r");
	if (pipe == NULL) {
		
		return string("fdopen failed for pipe file descriptor: ") + strerror(errno);
	}

	char buf[512];
	bool foundMarker = false;
	string output = "";

	while (fgets(buf, sizeof(buf), pipe) != NULL) {
		output += buf;
	}

	return output;
}

static void execCmd(const char * path, char * argv[]) {

	execv(path, argv);

	// We only get here in case of errors
	printf("Could not execute logcat: %s", strerror(errno));
	return;
}

string run_cmd(const char * path, ...)
{
	char * argv[20] = {0};		//max args
	int argc = 0;

	va_list args;
	va_start(args, path);
	while(true)
	{
		char * param = va_arg(args, char *);
		if(param == (char*)0)
			break;

		argv[argc++] = param;
	}
	va_end(args);
/*
	for(int i = 0; i < argc; i++)
	{
		printf("%s\n", argv[i]);
	}
*/
	int pipeFds[2];
	if (pipe(pipeFds) < 0) {
		
		return string("Could not allocate pipe for logcat output: ") + strerror(errno);
	}

	pid_t pid;
	if ((pid = fork()) < 0) {
		
		return string("Fork for logcat execution failed: ") + strerror(errno);
	} else if (pid == 0) {
		close(pipeFds[1]);
		return readFromPipe(pipeFds[0]);
	} else {
		close(pipeFds[0]);
		if (dup2(pipeFds[1], STDOUT_FILENO) == -1) {
			
			return string("Could not redirect stdout: %s") + strerror(errno);
		}
		if (dup2(pipeFds[1], STDERR_FILENO) == -1) {
			
			return string("Could not redirect stdout: %s") + strerror(errno);
		}

		//here will never return
		execCmd(path, argv);
	}
}
