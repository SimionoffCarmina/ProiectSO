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
	
	printf("Enter TreasureID: "); 
	if((scanf("%d", &t.TreasureID)) != 1){
		printf("Nu s-a citit bine");
		exit(-1);
	}
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
		if(file != 1){
			write(file, &t, sizeof(Treasure));
		}
		else{
			close(file);
			printf("Couldn't open file");
			exit(-1);
		}
		close(file);
	}
	else{	
		closedir(dir);
		printf("Couldn't change directory");
		exit(-1);
	}
	closedir(dir);
}

int countTreasures(char *filename){
	int nr1 = 0;
	int file = open(filename, O_RDONLY);
	if(file){
		Treasure t;
		int size = 0;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			size += sizeof(Treasure);
			nr1++;
		}
	}
	else{	
		close(file);
		printf("Couldn't open file");
		exit(-1);
	}
	close(file);
	return nr1;
}

int countHunt(char h[]){
	int nr = 0;
	char path[255] = "./";
	strcat(path, h);
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
		    	if(strncmp(entry->d_name, "Hunt", 4) == 0) 
		    		nr += countTreasures(entry->d_name);
		}
	}
	closedir(dir);
	return nr;
}

int is_hunt_directory(char *name){
	struct stat st;
	if(stat(name, &st) == 0 && S_ISDIR(st.st_mode)){
        	return strncmp(name, "Hunt", 4) == 0;
    	}
    	return -2;
}

void listHunts(){
    	DIR *dir;
    	struct dirent *directory;
    
    	dir = opendir(".");
    
    	if(dir == NULL){
        	printf("err opening directory");
        	exit(-1);
    	}
    
    	printf("Hunts: ");
    	while((directory = readdir(dir)) != NULL){
        	if(is_hunt_directory(directory->d_name) == 1) {
        		printf("%s %d\n", directory->d_name, countHunt(directory->d_name));
        	}
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
		close(file);
		printf("Couldn't open file");
		exit(-1);
	}
	close(file);
}

void list(char h[]){
	char path[255] = "./";
	strcat(path, h);
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
		    	if(strncmp(entry->d_name, "Hunt", 4) == 0) 
		    		listFile(entry->d_name);
		}
	}
	closedir(dir);
}

void enterFile(char *filename, int TreasureID){
	int file = open(filename, O_RDONLY);
	if(file != -1){
		Treasure t;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			if(t.TreasureID == TreasureID)
				printf("%d %s %f %f %s %d\n", t.TreasureID, t.Username, t.latitude, t.longitude, t.Cluetext, t.value);
		}
	}
	else{	
		close(file);
		printf("Couldn't open file");
		exit(-1);
	}
	close(file);
}

void view(char h[], int TreasureID){
	char path[255] = "./";
	strcat(path, h);
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
		    	enterFile(entry->d_name, TreasureID);
		}
	}
	closedir(dir);
}

void search(char *filename, int TreasureID){
	int file = open(filename, O_RDONLY);
	int temp = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(file != -1){
		Treasure t;
		while(read(file, &t, sizeof(Treasure)) == sizeof(Treasure)){
			if(t.TreasureID != TreasureID)
				write(temp, &t, sizeof(Treasure));
		}
	}
	else{	
		close(file);
		printf("Couldn't open file");
		exit(-1);
	}
	if (rename("temp", filename) != 0) {
      		printf("Couldn't replace file");
        	exit(-1);
    	} 
    	close(file);
    	close(temp);
}

void remove_treasure(char h[], int TreasureID){
	char path[255] = "./";
	strcat(path, h);
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
		    	search(entry->d_name, TreasureID);
		}
	}
	closedir(dir);
}

void rem(char h[]){
	char path[255] = "rm -rf ";
	strcat(path, h);
	system(path);
}

void add_log(char h[], char log[]){
	char path[255] = "/";
	strcat(path, h);
	chdir(path);
	
	char name[255] = "logged_file";
	strcat(name, h);
	int nr = open(name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	char buf[300];
	strcpy(buf, log);
	strcat(buf, "\n");
	
	if(nr != -1){
		write(nr, buf, strlen(buf));
	}
	else{	
		printf("Couldn't open file");
		exit(-1);
	}
	char sym[600] = "";
	snprintf(sym, sizeof(sym), "%s/%s", path, name);
	char link[255] = "logged_link";
	strcat(link, h);
	printf("%s %s\n", sym, link);
	unlink(link);
	int result = symlink(sym, link);
	if(result != 0){
		printf("Aici e eroarea");
		exit(-1);
	}
	close(nr);
}
