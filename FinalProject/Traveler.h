#ifndef _TRAVELER_
#define _TRAVELER_

#include "Insurance.h"

typedef struct
{
	int id;
	char* name;
	int age;
	Insurance ins;
}Traveler;

void initTraveler(Traveler* pTraveler, int travelerId);
void upgradeInsurance(Traveler* pTraveler);
int compareTravelerByAge(const void* v1, const void* v2);
int compareTravelerByName(const void* v1, const void* v2);
int compareTravelerByInsPrice(const void* v1, const void* v2);
void printTraveler(const Traveler* pTraveler);
void printTravelerPtr(void* pTravelerPtr);
void freeTravelerPtr(void* pTravelerPtr);
int saveTravelerToTextFile(const Traveler* pTraveler, FILE* fp);
int loadTravelerFromTextFile(Traveler* pTraveler, FILE* fp);
int saveTravelerToBinaryFile(const Traveler* pTraveler, FILE* fp);
int loadTravelerFromBinaryFile(Traveler* pTraveler, FILE* fp);

#endif