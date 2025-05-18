#include "treasure_hub.h"
#include "treasure_manager.h"

void handle_sigusr(int sig){
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	if(strcmp(cwd, "/home/debian/ProiectSO") != 0){
		chdir("..");
	}
	if(sig == SIGUSR1){
		int fd = open("options.txt", O_RDONLY);
		if(fd == -1){
			printf("Error opening file");
			exit(-1);
		}
		char buffer[512];
		ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
		if (n <= 0) {
		    perror("Error reading options.txt");
		    close(fd);
		    exit(EXIT_FAILURE);
		}
		buffer[n] = '\0'; 

		close(fd);

		int op, id;
		char h[255];

		sscanf(buffer, "%d %s %d", &op, h, &id);
		if(op == 1){
			list(h);
		}
		if(op == 2){
			view(h, id);
			fflush(stdout);
		
		}
		if(op == 3){
			listHunts();
			fflush(stdout);
		}
		printf("<<END>>\n");
		fflush(stdout);
	}
}

pid_t start_monitor(int *out_pipe){
	int pipefd[2];
	if(pipe(pipefd) == -1){
		printf("Pipe error");
		exit(-1);
	}
	
	pid_t pid = fork();
	if(pid < 0){
		printf("Error starting monitor");
		exit(-1);
	}
	if(pid == 0){
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		setvbuf(stdout, NULL, _IONBF, 0);
		
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
	close(pipefd[1]);
	*out_pipe = pipefd[0];
	return pid;
}

void stop_monitor(){
	int fd = open("commands.txt", O_RDONLY);
	if(fd == -1){
		printf("Error opening file");
		exit(-1);
	}
	char buf[32] = {0};
	ssize_t n = read(fd, buf, sizeof(buf) - 1);
	if(n <= 0){
		printf("Error reading from file");
		close(fd);
		exit(-1);
	}
	close(fd);
	
	pid_t pid = atoi(buf);
	if(pid <= 0){
		printf("Couldnt read");
		exit(-1);
	}
	if(kill(pid, SIGTERM) == 0){
		printf("Stopped monitor\n");
	}
	else{
		printf("error stopping monitor");
		exit(-1);
	}
}
