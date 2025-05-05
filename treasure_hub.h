#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void handle_sigusr(int sig);
pid_t start_monitor();
void stop_monitor();
