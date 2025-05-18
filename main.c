#include "treasure_hub.h"
#include "treasure_manager.h"
#include <sys/wait.h>


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
			int pipefd[2];
			while(1){
				printf("Please choose an operation:\n1)start_monitor\n2)list_hunts\n3)list_treasures\n4)view_treasure\n5)stop_monitor\n6)calculate_score\n7)exit\n");
				fgets(option, sizeof(option), stdin);
				option[strcspn(option, "\n")] = 0;
				if(strcmp(option, "start_monitor") == 0){
					if(is_active == 1){
						printf("Monitor already started\n");
					}
					else{
						pid_t pid = start_monitor(pipefd);
						printf("Monitor started with pid: %d\n", pid);
						int fd = open("commands.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
						if(fd == -1){
							printf("error opening file\n");
							exit(-1);
						}
						char buf[32];
						int len = snprintf(buf, sizeof(buf), "%d", pid);
						if(write(fd, buf, len) != len){
							printf("Error writing");
							exit(-1);
						}
						close(fd);
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
						int fd_in = open("commands.txt", O_RDONLY);
						if(fd_in == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						
						char pid_buf[16] = {0};
						ssize_t read_bytes = read(fd_in, pid_buf, sizeof(pid_buf) - 1);
						close(fd_in);
						if(read_bytes <= 0){
							printf("error reading");
							exit(-1);
						}
						
						pid_t pid = atoi(pid_buf);
						
						int fd_out = open("options.txt", O_WRONLY, O_CREAT, O_TRUNC, 0644);
						if(fd_out == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						
						char cmd_buf[] = "3\n";
						if(write(fd_out, cmd_buf, strlen(cmd_buf)) != (ssize_t)strlen(cmd_buf)) {
							printf("Err writing\n");
							close(fd_out);
							exit(-1);
						}
						close(fd_out);
						if(kill(pid, SIGUSR1) == 0){
							printf("Listing hunts\n");
							sleep(1);
							
							char buffer[256];
							ssize_t bytes;
							int found_end = 0;
							
							while((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0){
								buffer[bytes] = '\0';
								printf("%s", buffer);
								if(strstr(buffer, "<<END>>") != NULL){
									found_end = 1;
									break;
								}
							}
							if(!found_end){
								printf("Did not receive <<END>> marker\n");
							}
						}
						else{
							printf("err sending signal");
							exit(-1);
						}
					}
				}
				if(strcmp(option, "list_treasures") == 0){
					if(is_active == 0){
						printf("Monitor is not active");
					}
					else{
						int fd_in = open("commands.txt", O_RDONLY);
						if(fd_in == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						
						char pid_buf[16] = {0};
						ssize_t read_bytes = read(fd_in, pid_buf, sizeof(pid_buf) - 1);
						close(fd_in);
						if(read_bytes <= 0){
							printf("error reading");
							exit(-1);
						}
						
						pid_t pid = atoi(pid_buf);
						
						int fd_out = open("options.txt", O_WRONLY, O_CREAT, O_TRUNC, 0644);
						if(fd_out == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						printf("Enter hunt\n");
						char hunt[255];
						scanf("%s", hunt);
						
						char outbuf[512];
						int outlen = snprintf(outbuf, sizeof(outbuf), "1 %s \n", hunt);
						
						ssize_t written = write(fd_out, outbuf, outlen);
						if(written != outlen){
							printf("err writing");
							close(fd_out);
						}
						close(fd_out);
						
						if(kill(pid, SIGUSR1) == 0){
							printf("Listing treasures\n");
							
							char buffer[256];
							ssize_t bytes;
							int found_end = 0;
							
							while((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
								buffer[bytes] = '\0';
								printf("%s", buffer);
								if(strstr(buffer, "<<END>>") != NULL){
									found_end = 1;
									break;
								}
							}
							if(!found_end){
								printf("Did not receive end marker\n");
							}
						}
						else{
							printf("error sending signal");
							exit(-1);
						}
						
					}
				}
				if(strcmp(option, "view_treasure") == 0){
					if(is_active == 0){
						printf("Monitor is not active");
					}
					else{
						int fd_in = open("commands.txt", O_RDONLY);
						if(fd_in == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						
						char pid_buf[16] = {0};
						ssize_t read_bytes = read(fd_in, pid_buf, sizeof(pid_buf) - 1);
						close(fd_in);
						if(read_bytes <= 0){
							printf("error reading");
							exit(-1);
						}
						
						pid_t pid = atoi(pid_buf);
						
						int fd_out = open("options.txt", O_WRONLY, O_CREAT, O_TRUNC, 0644);
						if(fd_out == -1){
							printf("couldnt open file\n");
							exit(-1);
						}
						printf("Enter hunt and treasure\n");
						char hunt[255];
						int id;
						scanf("%s %d", hunt, &id);
						
						char outbuf[512];
						int outlen = snprintf(outbuf, sizeof(outbuf), "2 %s %d\n", hunt, id);
						
						ssize_t written = write(fd_out, outbuf, outlen);
						if(written != outlen){
							printf("err writing");
							close(fd_out);
							exit(-1);
						}
						close(fd_out);
						if(kill(pid, SIGUSR1) == 0){
							printf("Listing treasures\n");
							
							char buffer[256];
							ssize_t bytes;
							int found_end = 0;
							
							while((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
								buffer[bytes] = '\0';
								printf("%s", buffer);
								if(strstr(buffer, "<<END>>") != NULL){
									found_end = 1;
									break;
								}
							}
							if(!found_end){
								printf("Did not receive end marker\n");
							}
						}
						else{
							printf("error sending signal");
							exit(-1);
						}
						
					}
				}
				if(strcmp(option, "calculate_score") == 0){
					if(is_active == 0){
						printf("Monitor is not active");
						exit(-1);
					}
					else{
						DIR *dir = opendir(".");
						if(!dir){
							printf("dir error");
							exit(-1);
						}
						
						struct dirent *entry;
						while((entry = readdir(dir)) != NULL){
							if(entry->d_type == DT_DIR && strncmp(entry->d_name, "Hunt", 4) == 0){
								int pipefd[2];
								if(pipe(pipefd) == -1){
									printf("pipe error");
									exit(-1);
								}
								
								pid_t pid = fork();
								if(pid < 0){
									printf("fork error");
									exit(-1);
								}
								if(pid == 0){
									close(pipefd[0]);
									dup2(pipefd[1], STDOUT_FILENO);
									close(pipefd[1]);
									
									execl("./calculate_score", "calculate_score", entry->d_name, NULL);
									perror("exec failed");
									exit(-1);
								}
								else{
									close(pipefd[1]);
									char buffer[256];
									ssize_t bytes;
									while((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0){
										buffer[bytes] = '\0';
										printf("%s", buffer);
									}
									close(pipefd[0]);
									
									waitpid(pid, NULL, 0);
								}
							}
						}
						closedir(dir);
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
