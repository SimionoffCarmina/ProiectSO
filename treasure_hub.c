#include "treasure_hub.h"
#include "treasure_manager.h"

void handle_sigusr(int sig){
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	if(strcmp(cwd, "/home/debian/ProiectSO") != 0){
		chdir("..");
	}
	if(sig == SIGUSR1){
		listHunts();
	}
	else{
		if(sig == SIGUSR2){
			FILE *fin = fopen("options.txt", "r");
			char h[255];
			if(fin == NULL){
				printf("err");
				exit(-1);
			}
			fscanf(fin, "%s", h);
			list(h);
		}
	}
}

pid_t start_monitor(){
	pid_t pid = fork();
	if(pid < 0){
		printf("Error starting monitor");
		exit(-1);
	}
	if(pid == 0){
		struct sigaction sig1;
		sig1.sa_handler = handle_sigusr;
		sig1.sa_flags = 0;
		
		if(sigaction(SIGUSR1, &sig1, NULL) == -1){
			printf("error sigaction");
			exit(-1);
		}
		
		struct sigaction sig2;
		sig2.sa_handler = handle_sigusr;
		sig2.sa_flags = 0;
		
		if(sigaction(SIGUSR2, &sig2, NULL) == -1){
			printf("err");
			exit(-1);
		}
		while(1){
			pause();
		}
	}
	return pid;
}

void stop_monitor(){
	FILE *f = fopen("commands.txt", "r");
	if(f == NULL){
		printf("Error opening file");
		exit(-1);
	}
	pid_t pid;
	if(fscanf(f, "%d", &pid) != 1){
		printf("error reading from file");
		fclose(f);
		exit(-1);
	}
	fclose(f);
	if(kill(pid, SIGTERM) == 0){
		printf("Stopped monitor\n");
	}
	else{
		printf("error stopping monitor");
		exit(-1);
	}
}
