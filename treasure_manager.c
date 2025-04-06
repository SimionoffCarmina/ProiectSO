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
		strcat(filename, ".b");
		FILE *file = fopen(filename, "wb");
		if(file){
			int n1 = strlen(t.Username);
			int n2 = strlen(t.Cluetext);
			
			fwrite(&t.TreasureID, sizeof(int), 1, file);
			fwrite(t.Username, n1 * sizeof(char), 1, file);
			fwrite(&t.latitude, sizeof(float), 1, file);
			fwrite(&t.longitude, sizeof(float), 1, file);
			fwrite(t.Cluetext, n2 * sizeof(char), 1, file);
			fwrite(&t.value, sizeof(int), 1, file);
		}
		else{
			printf("Couldn't open file");
			exit(-1);
		}
		fclose(file);
	}
	else{
		printf("Couldn't change directory");
		exit(-1);
	}
	closedir(dir);
}

void list(char h[]){
	char path[255] = "./";
	strcat(path, h);
	if(chdir(path) == 0){
		char filename[255] = "";
		strcat(filename, h);
		strcat(filename, ".b");
		FILE *file = fopen(filename, "rb");
		if(file){
			int TreasureID, value;
			char Username[255], Cluetext[255];
			float latitude, longitude;
			fread(&TreasureID, sizeof(int), 1, file);
			char c;
			int k = 0;
			fread(&c, sizeof(char), 1, file);
			while(c != ' ' && k < 255){
				Username[k++] = c;
				fread(&c, sizeof(char), 1, file);
			}
			Username[k] = '\0';
			fread(&latitude, sizeof(float), 1, file);
			fread(&longitude, sizeof(float), 1, file);
			k = 0;
			fread(&c, sizeof(char), 1, file);
			while(c != ' ' && k < 255){
				Cluetext[k++] = c;
				fread(&c, sizeof(char), 1, file);
			}
			Cluetext[k] = '\0';
			fread(&value, sizeof(int), 1, file);
			printf("%d %s %f %f %s %d\n", TreasureID, Username, latitude, longitude, Cluetext, value);
			
		}
		else{	
			printf("Couldn't open file");
			exit(-1);
		}
	}
	else{
		printf("Couldn't open directory");
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
	}
	return 0;
}
