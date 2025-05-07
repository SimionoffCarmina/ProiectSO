#include "treasure_hub.h"
#include "treasure_manager.h"

int main(int argc, char *argv[]){
	if(strcmp("./man", argv[0]) == 0){
		if(argc < 3){
			printf("Usage ./man --option <requirements>\n");
			exit(-1);
		}
		if(strcmp("--add", argv[1]) == 0){
			add(argv[2]);
		}
		else{
			if(strcmp("--list", argv[1]) == 0){
				list(argv[2]);
			}
			else{
				if(strcmp("--view", argv[1]) == 0){
					if(argc < 4){
						printf("Numar gresit de argumente");
						exit(-1);
					}
					else{
						view(argv[2], atoi(argv[3]));
					}
				}
				else{
					if(strcmp("--remove_hunt", argv[1]) == 0){
						rem(argv[2]);
					}
					else{
						if(strcmp("--remove_treasure", argv[1]) == 0){
							if(argc < 4){
								printf("Nr gresit de argumente");
								exit(-1);
							}
							else{
								remove_treasure(argv[2], atoi(argv[3]));
							}
						}
					}
				}
			}
		}
		add_log(argv[2], argv[1]);
	}
	else{
		if(strcmp("./hub", argv[0]) == 0){
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
						FILE *fout = fopen("options.txt", "w");
						pid_t pid;
						if(fin == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							fscanf(fin, "%d", &pid);
						}
						if(fout == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							fprintf(fout, "%d", 1);
							fclose(fout);
							if(kill(pid, SIGUSR1) == 0){
								printf("Listing hunts\n");
							}
							else{
								printf("Error sending signal");
								exit(-1);
							}
						}
						sleep(1);
						fclose(fin);
					}
				}
				if(strcmp(option, "list_treasures") == 0){
					if(is_active == 0){
						printf("Monitor is not active");
					}
					else{
						FILE *fout = fopen("options.txt", "w");
						pid_t pid;
						if(fin == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							fscanf(fin, "%d", &pid);
						}
						if(fout == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							printf("Please enter hunt:\n");
							char hunt[255];
							scanf("%s", hunt);
							fprintf(fout, "%s", hunt); 
							fflush(fout);         
							fsync(fileno(fout));  
							fclose(fout);	
							if(kill(pid, SIGUSR2) == 0){
								printf("Listing treasures \n");
							}
							else{
								printf("Error sending signal");
								exit(-1);
							}
							sleep(2);
						}
						fclose(fin);
					}
				}
				if(strcmp(option, "view_treasure") == 0){
					if(is_active == 0){
						printf("Monitor is not active");
					}
					else{
						FILE *fin = fopen("commands.txt", "r");
						FILE *fout = fopen("options.txt", "w");
						pid_t pid;
						if(fin == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							fscanf(fin, "%d", &pid);
						}
						if(fout == NULL){
							printf("Couldnt open file\n");
							exit(-1);
						}
						else{
							fprintf(fout, "%d", 3); 
							fclose(fout);
							if(kill(pid, SIGUSR3) == 0){
								printf("Viewing treasure \n");
							}
							else{
								printf("Error sending signal");
								exit(-1);
							}
							sleep(1);
						}
						fclose(fin);
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
		}
	}
	return 0;
}
