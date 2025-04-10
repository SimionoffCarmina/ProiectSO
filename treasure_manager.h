#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

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
void list(char h[]);
void view(char h[], int TreasureID);
void remove_treasure(char h[], int TreasureID);
