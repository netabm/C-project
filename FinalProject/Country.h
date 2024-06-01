#ifndef _COUNTRY_
#define _COUNTRY_

#include "Attraction.h"
#include "Trip.h"

typedef struct
{
	char* name;
	Trip** tripArr;
	int tripCount;
}Country;

void initCountry(Country* pCountry);
int addTripToCountry(Country* pCountry, Trip* pTrip);
int findTripByDate(Country* pCountry, Date* pDate, Trip*** tempTripArr, int* arrSize);
void printTripArr(const Country* pCountry);
void printCountry(const Country* pCountry);
int saveCountryToTextFile(const Country* pCountry, FILE* fp);
int loadCountryFromTextFile(Country* pCountry, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp);
int saveCountryToBinaryFile(const Country* pCountry, FILE* fp);
int loadCountryFromBinaryFile(Country* pCountry, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp);

#endif
