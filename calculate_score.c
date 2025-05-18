#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>

typedef struct Treasure{
	int TreasureID;
	char Username[255];
	float latitude;
	float longitude;
	char Cluetext[255];
	int value;
	time_t rawtime;
}Treasure;

void listFile(char *filename){
	int file = open(filename, O_RDONLY);
	int uservalue = 0;
	if(file){
		Treasure t;
		int size = 0;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			size += sizeof(Treasure);
			uservalue += t.value;
		}
		printf("%s %d\n", t.Username, uservalue);
	}
	else{	
		close(file);
		printf("Couldn't open file");
		exit(-1);
	}
	close(file);
}

int main(int argc, char *argv[]){
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <hunt_folder>\n", argv[0]);
		return 1;
    	}
    	char path[255] = "./";
	strcat(path, argv[1]);
	DIR *dir = opendir(path);
	if(dir == NULL){
		closedir(dir);
		printf("Couldn't open directory");
		exit(-1);
	}
	else{
		chdir(path);
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL){
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		    		continue;
		    	if(strncmp(entry->d_name, "Hunt", 4) == 0){
		    		listFile(entry->d_name);
		    	}
		}
	}
	closedir(dir);
    	return 0;
}
