#include "treasure_hub.h"

pid_t start_monitor(){
	pid_t pid = fork();
	if(pid < 0){
		printf("Error starting monitor");
		exit(-1);
	}
	if(pid == 0){
		exit(-1);
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

void handle_sigusr(int sig){
	if(sig == SIGUSR1){
		FILE *option = fopen("options.txt", "r");
		char op[30];
		fscanf(option, "%s", op);
		printf("aaa");
	}
}

void list_hunts(){
	
}

int main(int argc, char *argv[]){
	int is_active = 0;
	char option[255];
	while(1){
		printf("Please choose an operation:\n1)start_monitor\n2)list_hunts\n3)list_treasures\n4)view_treasure\n5)stop_monitor\n6)exit\n");
		fgets(option, sizeof(option), stdin);
		option[strcspn(option, "\n")] = 0;
		if(strcmp(option, "start_monitor") == 0){
			if(is_active == 1){
				printf("Monitor already started\n");
			}
			else{
				pid_t pid = start_monitor();
				printf("Monitor started with pid: %d\n", pid);
				FILE *fin = fopen("commands.txt", "w");
				if(fin == NULL){
					printf("error opening file");
					exit(-1);
				}
				fprintf(fin, "%d", pid);
				fclose(fin);
				is_active = 1;
			}
		}
		if(strcmp(option, "stop_monitor") == 0){
			if(is_active == 0){
				printf("Monitor is not active\n");
			}
			else{
				is_active = 0;
				stop_monitor();
			}
		}
		if(strcmp(option, "list_hunts") == 0){
			if(is_active == 0){
				printf("Monitor is not active");
			}
			else{
				FILE *fin = fopen("commands.txt", "r");
				FILE *fout = fopen("option.txt", "w");
				pid_t pid;
				if(fin == NULL){
					printf("Couldnt open file\n");
					exit(-1);
				}
				else{
					fscanf(fin, "%d", &pid);
				}
				fclose(fin);
				if(fout == NULL){
					printf("Couldnt open file\n");
					exit(-1);
				}
				else{
					fprintf(fout, "%s", "list_hunts");
					if(kill(pid, SIGUSR1) == 0){
						printf("Listing hunts\n");
					}
					else{
						printf("Error sending signal");
						exit(-1);
					}
				}
			}
		}
		if(strcmp(option, "exit") == 0){
			if(is_active == 1){
				printf("Monitor still active, stop it first\n");
			}
			else{
				break;
			}
		}
	}
	return 0;
}
