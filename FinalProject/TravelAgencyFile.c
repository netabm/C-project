#include <stdio.h>
#include <stdlib.h>

#include "TravelAgencyFile.h"
#include "Macros.h"

int saveAgencyToTextFile(TravelAgency* pAgency, const char* fileName)
{
	FILE* fp;
	OPEN_FILE(fp, fileName, "w", "Error open travel agency file to write");

	fprintf(fp, "%s\n", pAgency->name);

	fprintf(fp, "%d\n", pAgency->allTravelerCount);
	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		if (!saveTravelerToTextFile(pAgency->allTravelerArr[i], fp))
		{
			printf("Error write traveler\n");
			CLOSE_RETURN_ZERO(fp);
		}
	}

	fprintf(fp, "%d\n", pAgency->allAttCount);
	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		if (!saveAttractionToTextFile(pAgency->allAttArr[i], fp))
		{
			printf("Error write attraction\n");
			CLOSE_RETURN_ZERO(fp);
		}
	}

	fprintf(fp, "%d\n", pAgency->countryCount);
	for (int i = 0; i < pAgency->countryCount; i++)
	{
		if (!saveCountryToTextFile(pAgency->countryArr[i], fp))
		{
			printf("Error write country\n");
			CLOSE_RETURN_ZERO(fp);
		}
	}
	
	fprintf(fp, "%d\n", pAgency->numOfReviews);

	for (int i = 0; i < pAgency->numOfReviews; i++)
	{
		if (!saveReviewToTextFile(&pAgency->reviews[i], fp))
		{
			printf("Error write review\n");
			CLOSE_RETURN_ZERO(fp);
		}
	}
	
	fprintf(fp, "%lf\n", pAgency->finalScore);

	CLOSE_RETURN_ONE(fp);
}

int loadAgencyFromTextFile(TravelAgency* pAgency, const char* fileName)
{
	FILE* fp;
	OPEN_FILE(fp, fileName, "r", "Error open travel agency file");

	char temp[MAX_STR_LEN];
	myGets(temp, MAX_STR_LEN, fp);
	pAgency->name = getDynStr(temp);

	fscanf(fp, "%d\n", &pAgency->allTravelerCount);
	pAgency->allTravelerArr = (Traveler**)calloc(pAgency->allTravelerCount, sizeof(Traveler*));
	if (!pAgency->allTravelerArr)
		CLOSE_RETURN_ZERO(fp);

	Traveler* pTraveler;
	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		pTraveler = (Traveler*)calloc(1, sizeof(Traveler));
		if (!pTraveler)
			break;
		if (!loadTravelerFromTextFile(pTraveler, fp))
		{
			free(pTraveler);
			free(pAgency->allTravelerArr);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->allTravelerArr[i] = pTraveler;
	}

	fscanf(fp, "%d\n", &pAgency->allAttCount);
	pAgency->allAttArr = (Attraction**)calloc(pAgency->allAttCount, sizeof(Attraction*));
	if (!pAgency->allAttArr)
	{
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		CLOSE_RETURN_ZERO(fp);
	}

	Attraction* pAtt;
	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		pAtt = (Attraction*)calloc(1, sizeof(Attraction));
		if (!pAtt)
			break;
		if (!loadAttractionFromTextFile(pAtt, fp))
		{
			free(pAtt);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->allAttArr[i] = pAtt;
	}

	fscanf(fp, "%d\n", &pAgency->countryCount);
	pAgency->countryArr = (Country**)calloc(pAgency->countryCount, sizeof(Country*));
	if (!pAgency->countryArr)
	{
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		CLOSE_RETURN_ZERO(fp);
	}

	Country* pCountry;
	for (int i = 0; i < pAgency->countryCount; i++)
	{
		pCountry = (Country*)calloc(1, sizeof(Country));
		if (!pCountry)
			break;
		if (!loadCountryFromTextFile(pCountry, pAgency->allTravelerArr, pAgency->allTravelerCount, pAgency->allAttArr, pAgency->allAttCount, fp))
		{
			free(pCountry);
			freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->countryArr[i] = pCountry;
	}
	pAgency->travelerSortOpt = eNotSorted;

	fscanf(fp, "%d\n", &pAgency->numOfReviews);
	pAgency->reviews = (Review*)calloc(pAgency->numOfReviews, sizeof(Review));
	if (!pAgency->reviews)
	{
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		CLOSE_RETURN_ZERO(fp);
	}
	
	for (int i = 0; i < pAgency->numOfReviews; i++)
	{
		if (!loadReviewFromTextFile(&pAgency->reviews[i], pAgency->allTravelerArr, pAgency->allTravelerCount, fp))
		{
			freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			CLOSE_RETURN_ZERO(fp);
		}
	}
	
	fscanf(fp, "%lf\n", &pAgency->finalScore);

	CLOSE_RETURN_ONE(fp);
}

int saveAgencyToBinaryFile(TravelAgency* pAgency, const char* fileName)
{
	FILE* fp;
	OPEN_FILE(fp, fileName, "wb", "Error open company file");

	if (!writeStringToFile(pAgency->name, fp, "Error write agency name\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	if (!writeIntToFile(pAgency->allTravelerCount, fp, "Error write traveler count\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		if (!saveTravelerToBinaryFile(pAgency->allTravelerArr[i], fp)) {
			CLOSE_RETURN_ZERO(fp);
		}
	}

	if (!writeIntToFile(pAgency->allAttCount, fp, "Error write attraction count\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		if (!saveAttractionToBinaryFile(pAgency->allAttArr[i], fp)) {
			CLOSE_RETURN_ZERO(fp);
		}
	}

	if (!writeIntToFile(pAgency->countryCount, fp, "Error write country count\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	for (int i = 0; i < pAgency->countryCount; i++)
	{
		if (!saveCountryToBinaryFile(pAgency->countryArr[i], fp)) {
			CLOSE_RETURN_ZERO(fp);
		}
	}

	if (!writeIntToFile(pAgency->numOfReviews, fp, "Error write reviews count\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	for (int i = 0; i < pAgency->numOfReviews; i++)
	{
		if (!saveReviewToBinaryFile(&pAgency->reviews[i], fp))
		{
			CLOSE_RETURN_ZERO(fp);
		}
	}

	if (!writeDoubleToFile(pAgency->finalScore, fp, "Error write final score\n")) {
		CLOSE_RETURN_ZERO(fp);
	}

	CLOSE_RETURN_ONE(fp);
}

int loadAgencyFromBinaryFile(TravelAgency* pAgency, const char* fileName)
{
	FILE* fp;
	OPEN_FILE(fp, fileName, "rb", "Error open company file");

	pAgency->name = readStringFromFile(fp, "Error reading agency name\n");
	if (!pAgency->name) {
		CLOSE_RETURN_ZERO(fp);
	}

	if (!readIntFromFile(&pAgency->allTravelerCount, fp, "Error reading traveler count\n"))
	{
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}
	pAgency->allTravelerArr = (Traveler**)calloc(pAgency->allTravelerCount, sizeof(Traveler*));
	Traveler* pTraveler;
	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		pTraveler = (Traveler*)calloc(1, sizeof(Traveler));
		if (!pTraveler)
		{
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		if (!loadTravelerFromBinaryFile(pTraveler, fp))
		{
			free(pTraveler);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->allTravelerArr[i] = pTraveler;
	}
	pAgency->travelerSortOpt = eNotSorted;

	if (!readIntFromFile(&pAgency->allAttCount, fp, "Error reading attraction count\n"))
	{
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}
	pAgency->allAttArr = (Attraction**)calloc(pAgency->allAttCount, sizeof(Attraction*));
	if (!pAgency->allAttArr)
	{
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}
	Attraction* pAtt;
	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		pAtt = (Attraction*)calloc(1, sizeof(Attraction));
		if (!pAtt)
		{
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		if (!loadAttractionFromBinaryFile(pAtt, fp))
		{
			free(pAtt);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->allAttArr[i] = pAtt;
	}

	if (!readIntFromFile(&pAgency->countryCount, fp, "Error reading country count\n"))
	{
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}
	pAgency->countryArr = (Country**)calloc(pAgency->countryCount, sizeof(Country*));
	if (!pAgency->countryArr)
	{
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}
	Country* pCountry;
	for (int i = 0; i < pAgency->countryCount; i++)
	{
		pCountry = (Country*)calloc(1, sizeof(Country));
		if (!pCountry)
		{
			free(pAgency->countryArr);
			freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		if (!loadCountryFromBinaryFile(pCountry, pAgency->allTravelerArr, pAgency->allTravelerCount, pAgency->allAttArr, pAgency->allAttCount, fp))
		{
			free(pCountry);
			free(pAgency->countryArr);
			freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
		pAgency->countryArr[i] = pCountry;
	}

	if (!readIntFromFile(&pAgency->numOfReviews, fp, "Error reading reviews count\n"))
	{
		free(pAgency->countryArr);
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}

	pAgency->reviews = (Review*)calloc(pAgency->numOfReviews, sizeof(Review));
	if (!pAgency->reviews)
	{
		free(pAgency->countryArr);
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}

	for (int i = 0; i < pAgency->numOfReviews; i++)
	{
		if (!loadReviewFromBinaryFile(&pAgency->reviews[i], pAgency->allTravelerArr, pAgency->allTravelerCount, fp))
		{
			free(pAgency->countryArr);
			freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
			free(pAgency->allAttArr);
			freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
			free(pAgency->allTravelerArr);
			free(pAgency->name);
			CLOSE_RETURN_ZERO(fp);
		}
	}

	if (!readDoubleFromFile(&pAgency->finalScore, fp, "Error reading final score\n"))
	{
		free(pAgency->countryArr);
		freeAttractionArr(pAgency->allAttArr, pAgency->allAttCount);
		free(pAgency->allAttArr);
		freeTravelerArr(pAgency->allTravelerArr, pAgency->allTravelerCount);
		free(pAgency->allTravelerArr);
		free(pAgency->name);
		CLOSE_RETURN_ZERO(fp);
	}

	CLOSE_RETURN_ONE(fp);
}