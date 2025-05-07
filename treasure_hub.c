#include "treasure_hub.h"
#include "treasure_manager.h"

void handle_sigusr(int sig){
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	if(strcmp(cwd, "/home/debian/ProiectSO") != 0){
		chdir("..");
	}
	if(sig == SIGUSR1){
		FILE *fin = fopen("options.txt", "r");
		int op;
		char h[255];
		int id;
		if(fin == NULL){
			printf("err");
			exit(-1);
		}
		rewind(fin);
		fscanf(fin, "%d %s %d", &op, h, &id);
		if(op == 1){
			list(h);
		}
		if(op == 2){
			fflush(stdout);
			view(h, id);
		
		}
		if(op == 3){
			listHunts();
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
