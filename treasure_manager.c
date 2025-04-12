#include "treasure_manager.h"

void add(char h[]){
	char path[255] = "./";
	strcat(path, h);
	
	DIR *dir = opendir(path);
	if(dir){
		printf("Hunt already exists, adding treasure to hunt\n");
	}
	else{
		if(mkdir(h, 0777) == 0){
			printf("Added a new hunt\n");
		}
		else{
			printf("Directory could not be created\n");
			exit(-1);
		}
	}
	
	Treasure t;
	
	printf("Enter TreasureID: "); scanf("%d", &t.TreasureID);
	printf("Enter Username: "); scanf("%s", t.Username);
	printf("Enter latitude: "); scanf("%f", &t.latitude);
	printf("Enter longitude: "); scanf("%f", &t.longitude);
	printf("Enter Cluetext: "); scanf("%s", t.Cluetext);
	printf("Enter value: "); scanf("%d", &t.value);
	
	time(&t.rawtime);
	
	if(chdir(path) == 0){
		char filename[255] = "";
		strcat(filename, h);
		strcat(filename, t.Username);
		strcat(filename, ".b");
		int file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
		if(file){
			write(file, &t, sizeof(Treasure));
		}
		else{
			printf("Couldn't open file");
			exit(-1);
		}
	}
	else{
		printf("Couldn't change directory");
		exit(-1);
	}
	closedir(dir);
}

void listFile(char *filename){
	int file = open(filename, O_RDONLY);
	if(file){
		Treasure t;
		int size = 0;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			size += sizeof(Treasure);
			printf("%d %s %f %f %s %d\n", t.TreasureID, t.Username, t.latitude, t.longitude, t.Cluetext, t.value);
		}
		printf("Last time updated: %s", asctime(localtime(&t.rawtime)));
		printf("Size of file: %d\n", size);
	}
	else{	
		printf("Couldn't open file");
		exit(-1);
	}
}

void list(char h[]){
	char path[255] = "./";
	strcat(path, h);
	DIR *dir = opendir(path);
	if(dir == NULL){
		printf("Couldn't open directory");
		exit(-1);
	}
	else{
		chdir(path);
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL){
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		    		continue;
		    	listFile(entry->d_name);
		}
	}
}

void enterFile(char *filename, int TreasureID){
	int file = open(filename, O_RDONLY);
	if(file){
		Treasure t;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			if(t.TreasureID == TreasureID)
				printf("%d %s %f %f %s %d\n", t.TreasureID, t.Username, t.latitude, t.longitude, t.Cluetext, t.value);
		}
	}
	else{	
		printf("Couldn't open file");
		exit(-1);
	}
}

void view(char h[], int TreasureID){
	char path[255] = "./";
	strcat(path, h);
	DIR *dir = opendir(path);
	if(dir == NULL){
		printf("Couldn't open directory");
		exit(-1);
	}
	else{
		chdir(path);
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL){
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		    		continue;
		    	enterFile(entry->d_name, TreasureID);
		}
	}
}

void search(char *filename, int TreasureID){
	int file = open(filename, O_RDONLY);
	int temp = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(file){
		Treasure t;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			if(t.TreasureID != TreasureID)
				write(temp, &t, sizeof(Treasure));
		}
	}
	else{	
		printf("Couldn't open file");
		exit(-1);
	}
	if (rename("temp", filename) != 0) {
      		printf("Couldn't replace file");
        	exit(-1);
    	} 
}

void remove_treasure(char h[], int TreasureID){
	char path[255] = "./";
	strcat(path, h);
	DIR *dir = opendir(path);
	if(dir == NULL){
		printf("Couldn't open directory");
		exit(-1);
	}
	else{
		chdir(path);
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL){
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		    		continue;
		    	search(entry->d_name, TreasureID);
		}
	}
}

void rem(char h[]){
	char path[255] = "rm -rf ";
	strcat(path, h);
	system(path);
}

void add_log(char h[], char *log){
	char path[255] = "./";
	strcat(path, h);
	chdir(path);
	strcat(log, "\n");
	int nr = open("logged_file", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(nr){
		write(nr, log, sizeof(log));
	}
	else{	
		printf("Couldn't open file");
		exit(-1);
	}
}

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Usage ./p --option <requirements>\n");
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
	return 0;
}
