#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct Treasure{
	int TreasureID;
	char Username[255];
	float latitude;
	float longitude;
	char Cluetext[255];
	int value;
}Treasure;

void add(char h[]);
