#include <stdio.h>
#include <stdlib.h>
#include "Country.h"
#include "fileHelper.h"

void initCountry(Country* pCountry)
{
	pCountry->name = getStrExactName("Please enter country name");
	pCountry->tripArr = NULL;
	pCountry->tripCount = 0;
}

int addTripToCountry(Country* pCountry, Trip* pTrip)
{
	pCountry->tripArr = (Trip**)realloc(pCountry->tripArr, (pCountry->tripCount + 1) * sizeof(Trip*));
	if (!pCountry->tripArr) //Allocation failed
	{
		freeTrip(pTrip);
		free(pTrip);
		return 0;
	}
	pCountry->tripArr[pCountry->tripCount] = pTrip; //Add pTrip to tripArr
	pCountry->tripCount++; //Increase tripCount

	return 1;
}

int findTripByDate(Country* pCountry, Date* pDate, Trip*** tempTripArr, int* arrSize)
{
	for (int i = 0; i < pCountry->tripCount; i++) //Loop through tripArr in pCountry
	{
		if (isDateInRange(pCountry->tripArr[i], pDate) == 1) //Check if trip's date is in range
		{
			(*arrSize)++; //Increase arrSize
			*tempTripArr = (Trip**)realloc(*tempTripArr, *arrSize * (sizeof(Trip*)));
			if (!(*tempTripArr)) //Allocation failed
				return 0;
			(*tempTripArr)[*arrSize - 1] = pCountry->tripArr[i]; //Add trip to tempTripArr
		}
	}
	return 1;
}

void printTripArr(const Country* pCountry)
{
	for (int i = 0; i < pCountry->tripCount; i++) //Loop through tripArr in pCountry
	{
		printf("Trips' serial number: %d\n", pCountry->tripArr[i]->serialNum);
		printShortTrip(pCountry->tripArr[i]);
	}
}

void printCountry(const Country* pCountry)
{
	printf("--------------------------------------------------\n");
	printf("                 COUNTRY INFORMATION              \n");
	printf("--------------------------------------------------\n");
	printf("Name: %s\n", pCountry->name);
	printf("Total Trips: %d\n", pCountry->tripCount);
	printf("--------------------------------------------------\n");
}

int saveCountryToTextFile(const Country* pCountry, FILE* fp)
{
	fprintf(fp, "%s\n", pCountry->name);
	fprintf(fp, "%d\n", pCountry->tripCount);

	for (int i = 0; i < pCountry->tripCount; i++)
		saveTripToTextFile(pCountry->tripArr[i], fp);
	return 1;
}

int loadCountryFromTextFile(Country* pCountry, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp)
{
	char temp[MAX_STR_LEN];
	myGets(temp, MAX_STR_LEN, fp);
	pCountry->name = getDynStr(temp);

	fscanf(fp, "%d\n", &pCountry->tripCount);
	pCountry->tripArr = (Trip**)calloc(pCountry->tripCount, sizeof(Trip*));
	if (!pCountry->tripArr)
		return 0;
	Trip* pTrip;
	for (int i = 0; i < pCountry->tripCount; i++)
	{
		pTrip = (Trip*)calloc(1, sizeof(Trip));
		loadTripFromTextFile(pTrip, pCountry->name, travelerArr, countTraveler, attArr, countAtt, fp);
		pCountry->tripArr[i] = pTrip;
	}

	return 1;
}

int saveCountryToBinaryFile(const Country* pCountry, FILE* fp)
{
	if (!writeStringToFile(pCountry->name, fp, "Error write country name\n"))
		return 0;

	if (!writeIntToFile(pCountry->tripCount, fp, "Error write trip count\n"))
		return 0;

	for (int i = 0; i < pCountry->tripCount; i++)
	{
		if (!saveTripToBinaryFile(pCountry->tripArr[i], fp))
			return 0;
	}
	return 1;
}

int loadCountryFromBinaryFile(Country* pCountry, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp)
{
	pCountry->name = readStringFromFile(fp, "Error reading country name\n");
	if (!pCountry->name)
		return 0;
	if (!readIntFromFile(&pCountry->tripCount, fp, "Error reading trip count\n"))
	{
		free(pCountry->name);
		return 0;
	}

	pCountry->tripArr = (Trip**)calloc(pCountry->tripCount, sizeof(Trip*));
	if (!pCountry->tripArr)
	{
		free(pCountry->name);
		return 0;
	}
	Trip* pTrip;
	for (int i = 0; i < pCountry->tripCount; i++)
	{
		pTrip = (Trip*)calloc(1, sizeof(Trip));
		if (!pTrip)
		{
			free(pCountry->tripArr);
			free(pCountry->name);
			return 0;
		}
		if (!loadTripFromBinaryFile(pTrip, pCountry->name, travelerArr, countTraveler, attArr, countAtt, fp))
		{
			free(pTrip);
			free(pCountry->tripArr);
			free(pCountry->name);
			return 0;
		}
		pCountry->tripArr[i] = pTrip;
	}

	return 1;
}