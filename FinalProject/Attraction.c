#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Attraction.h"
#include "FileHelper.h"

int initAttraction(Attraction* pAtt)
{
	pAtt->name = getStrExactName("Please enter attraction name:");
	if (!pAtt->name)
		return 0; //Allocation failed
	getInt(&pAtt->price, "price per person:");
	chooseLevel(pAtt);
	return 1;
}

void chooseLevel(Attraction* pAtt)
{
	int opt;
	printf("Choose the level of the attraction\n");
	for (int i = 0; i < eNofLevels; i++)
		printf("enter %d for %s\n", i, levelStr[i]); //Display options for attraction levels
	scanf("%d", &opt);
	while (opt < 0 || opt > 2) //Validate user input
	{
		printf("Error wrong input\n");
		scanf("%d", &opt);
	}
	pAtt->level = opt; //Set the chosen level for the attraction
}

int compareAttractions(const void* v1, const void* v2)
{
	const Attraction* a1 = (const Attraction*)v1; //Cast void pointers to Attraction pointers
	const Attraction* a2 = (const Attraction*)v2;
	return strcmp(a1->name, a2->name); //Compare attraction names
}

void freeAttraction(Attraction* pAtt)
{
	free(pAtt->name); //Free memory allocated for attraction name
}

void freeAttractionPtr(void* pAttPtr)
{
	Attraction* pAtt = *(Attraction**)pAttPtr; //Cast void pointer to Attraction pointer
	freeAttraction(pAtt);
}

void printAttraction(const Attraction* pAtt)
{
	printf("%s - Level: %s\n", pAtt->name, levelStr[pAtt->level]);
}

int saveAttractionToTextFile(const Attraction* pAtt, FILE* fp)
{
	fprintf(fp, "%s\n", pAtt->name);
	fprintf(fp, "%d\n", pAtt->price);
	fprintf(fp, "%d\n", pAtt->level);
	return 1;
}

int loadAttractionFromTextFile(Attraction* pAtt, FILE* fp)
{
	char temp[MAX_STR_LEN];
	myGets(temp, MAX_STR_LEN, fp);
	pAtt->name = getDynStr(temp);

	fscanf(fp, "%d\n", &pAtt->price);
	fscanf(fp, "%d\n", &pAtt->level);

	return 1;
}

int saveAttractionToBinaryFile(const Attraction* pAtt, FILE* fp)
{
	if (!writeStringToFile(pAtt->name, fp, "Error write attraction name\n"))
		return 0;
	if (!writeIntToFile(pAtt->price, fp, "Error write attraction price\n"))
		return 0;
	if (!writeIntToFile(pAtt->level, fp, "Error write attraction level\n"))
		return 0;
	return 1;
}

int loadAttractionFromBinaryFile(Attraction* pAtt, FILE* fp)
{
	pAtt->name = readStringFromFile(fp, "Error reading attraction name\n");
	if (!pAtt->name)
		return 0;
	if (!readIntFromFile(&pAtt->price, fp, "Error reading attraction price\n"))
	{
		free(pAtt->name);
		return 0;
	}
	int temp;
	if (!readIntFromFile(&temp, fp, "Error reading attraction level\n"))
	{
		free(pAtt->name);
		return 0;
	}
	pAtt->level = temp;
	return 1;
}