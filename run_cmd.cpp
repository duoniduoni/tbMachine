#include "run_cmd.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


static void readFromPipe(int pipefd) {

	FILE* pipe = fdopen(pipefd, "r");
	if (pipe == NULL) {
		printf("fdopen failed for pipe file descriptor %d: %s", pipefd, strerror(errno));
		return;
	}

	char buf[512];
	bool foundMarker = false;
	long totalSize = 0;
	while (fgets(buf, sizeof(buf), pipe) != NULL) {
		int len = strlen(buf);
		printf("%s", buf);
	}

	printf("Broken pipe : %s\n", strerror(ferror(pipe)));
	return;
}

static void execCmd() {

	execl("/bin/ls", "ls", "-l", (char *)0);

	// We only get here in case of errors
	printf("Could not execute logcat: %s", strerror(errno));
	return;
}

void run_cmd()
{
	int pipeFds[2];
	if (pipe(pipeFds) < 0) {
		printf("Could not allocate pipe for logcat output: %s", strerror(errno));
		return;
	}

	pid_t pid;
	if ((pid = fork()) < 0) {
		printf("Fork for logcat execution failed: %s", strerror(errno));
		return;
	} else if (pid == 0) {
		close(pipeFds[1]);
		readFromPipe(pipeFds[0]);
	} else {
		close(pipeFds[0]);
		if (dup2(pipeFds[1], STDOUT_FILENO) == -1) {
			printf("Could not redirect stdout: %s", strerror(errno));
			return;
		}
		if (dup2(pipeFds[1], STDERR_FILENO) == -1) {
			printf("Could not redirect stdout: %s", strerror(errno));
			return;
		}
		execCmd();
	}
}
