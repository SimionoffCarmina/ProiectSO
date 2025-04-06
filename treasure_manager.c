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
	
	if(chdir(path) == 0){
		char filename[255] = "";
		strcat(filename, h);
		strcat(filename, ".txt");
		FILE *file = fopen(filename, "w");
		if(file){
			fprintf(file, "%d %s %f %f %s %d\n", t.TreasureID, t.Username, t.latitude, t.longitude, t.Cluetext, t.value);
			
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
}

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("Usage ./p --option <requirements>\n");
		exit(-1);
	}
	if(strcmp("--add", argv[1]) == 0){
		add(argv[2]);
	}
	return 0;
}
