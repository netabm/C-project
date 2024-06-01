#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "Traveler.h"
#include "FileHelper.h"

void initTraveler(Traveler* pTraveler, int travelerId)
{
	pTraveler->name = getStrExactName("Please enter traveler name");
	pTraveler->id = travelerId;
	getInt(&pTraveler->age, "age");
	initInsurance(&pTraveler->ins);
}

void upgradeInsurance(Traveler* pTraveler)
{
	upgradeToExtreme(&pTraveler->ins);
	printf("Insurance has been upgraded to extreme\n");
}

int compareTravelerByAge(const void* v1, const void* v2)
{
	const Traveler* t1 = *(const Traveler**)v1; //Cast void pointers to Traveler pointers
	const Traveler* t2 = *(const Traveler**)v2;
	return t1->age - t2->age; //Compare travelers by age
}

int compareTravelerByName(const void* v1, const void* v2)
{
	const Traveler* t1 = *(const Traveler**)v1;
	const Traveler* t2 = *(const Traveler**)v2;
	return strcmp(t1->name, t2->name); //Compare travelers by name
}

int compareTravelerByInsPrice(const void* v1, const void* v2)
{
	const Traveler* t1 = *(const Traveler**)v1;
	const Traveler* t2 = *(const Traveler**)v2;
	return t1->ins.finalPrice - t2->ins.finalPrice; //Compare travelers by insurance price
}

void printTraveler(const Traveler* pTraveler)
{
	printf("Traveler's name: %s, id: %d, age: %d\n", pTraveler->name, pTraveler->id, pTraveler->age);
	printInsurance(&pTraveler->ins);
	printf("--------------------------------------------------\n");
}

void printTravelerPtr(void* pTravelerPtr)
{
	const Traveler* temp = *(Traveler**)pTravelerPtr; //Cast void pointer to Traveler pointer
	printTraveler(temp);
}

void freeTravelerPtr(void* pTravelerPtr)
{
	Traveler* temp = *(Traveler**)pTravelerPtr;
	free(temp->name);
}

int saveTravelerToTextFile(const Traveler* pTraveler, FILE* fp)
{
	if (!pTraveler)
		return 0;
	fprintf(fp, "%d\n", pTraveler->id);
	fprintf(fp, "%s\n", pTraveler->name);
	fprintf(fp, "%d\n", pTraveler->age);
	saveInsuranceToTextFile(&pTraveler->ins, fp);
	return 1;
}

int loadTravelerFromTextFile(Traveler* pTraveler, FILE* fp)
{
	fscanf(fp, "%d\n", &pTraveler->id);

	char temp[MAX_STR_LEN];
	myGets(temp, MAX_STR_LEN, fp);
	pTraveler->name = getDynStr(temp);

	fscanf(fp, "%d\n", &pTraveler->age);

	loadInsuranceFromTextFile(&pTraveler->ins, fp);
	return 1;
}

int saveTravelerToBinaryFile(const Traveler* pTraveler, FILE* fp)
{
	if (!writeIntToFile(pTraveler->id, fp, "Error write traveler ID\n"))
		return 0;

	if (!writeStringToFile(pTraveler->name, fp, "Error write traveler name\n"))
		return 0;

	if (!writeIntToFile(pTraveler->age, fp, "Error write traveler age\n"))
		return 0;

	if (!saveInsuranceToBinaryFileCompressed(&pTraveler->ins, fp))
	{
		printf("Error write insurance\n");
		return 0;
	}
	return 1;
}

int loadTravelerFromBinaryFile(Traveler* pTraveler, FILE* fp)
{
	if (!readIntFromFile(&pTraveler->id, fp, "Error write traveler ID\n"))
		return 0;
	pTraveler->name = readStringFromFile(fp, "Error reading traveler name\n");
	if (!pTraveler->name)
	{
		fclose(fp);
		return 0;
	}
	if (!readIntFromFile(&pTraveler->age, fp, "Error write traveler age\n"))
	{
		free(pTraveler->name);
		return 0;
	}
	loadInsuranceFromBinaryFileCompressed(&pTraveler->ins, fp);
	return 1;
}