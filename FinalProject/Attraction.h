#ifndef _ATTRACTION_
#define _ATTRACTION_

#include "General.h"

typedef enum { eEasy, eMedium, eHard, eNofLevels } attractionLevel;
static const char* levelStr[eNofLevels] = { "Easy", "Medium", "Hard" };

typedef struct
{
	char* name;
	int price;
	attractionLevel level;
}Attraction;

int initAttraction(Attraction* pAtt);
void chooseLevel(Attraction* pAtt);
int compareAttractions(const void* v1, const void* v2);
void freeAttraction(Attraction* pAtt);
void freeAttractionPtr(void* pAttPtr);
void printAttraction(const Attraction* pAtt);
int saveAttractionToTextFile(const Attraction* pAtt, FILE* fp);
int loadAttractionFromTextFile(Attraction* pAtt, FILE* fp);
int saveAttractionToBinaryFile(const Attraction* pAtt, FILE* fp);
int loadAttractionFromBinaryFile(Attraction* pAtt, FILE* fp);

#endif
