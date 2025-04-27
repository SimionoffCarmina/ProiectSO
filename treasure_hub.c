#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void start_monitor(){
	pid_t pid = fork();
	
	if(pid < 0){
		printf("Couldnt fork");
		exit(-1);
	}
	else{
		if(pid == 0){
			printf("Monitor started %d\n", getpid());
		}
		else{
			printf("Background program, pid: %d\n", pid);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage ./p <command>\n"); 
		exit(-1);
	}
	if(strcmp("start_monitor", argv[1]) == 0){
		start_monitor();
	}
	return 0;
}
