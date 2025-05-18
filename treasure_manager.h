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

void add(char h[]);
char *listHunts();
void listFile(char *fileName);
void list(char h[]);
void enterFile(char *fileName, int TreasureID);
void view(char h[], int TreasureID);
void search(char *filename, int TreasureID);
void remove_treasure(char h[], int TreasuerID);
void rem(char h[]);
void add_log(char h[], char *log);
