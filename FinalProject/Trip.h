#ifndef _TRIP_
#define _TRIP_

#include "Traveler.h"
#include "Attraction.h"
#include "Flight.h"
#include "list.h"

#define HARD 2

static int nextSerialNum = 1;

typedef struct
{
	int serialNum;
	char* dest;
	Traveler** travelerArr;
	int travelerCount;
	Flight* flightStart;
	Flight* flightEnd;
	LIST attractionList;
	int attractionCount;
	BOOL hardLevelFlag;
	int finalPrice;
}Trip;

int initTrip(Trip* pTrip, char* countryName);
int getNextSerialNum();
int createFlights(Trip* pTrip);
int addTraveler(Trip* pTrip, Traveler* pTraveler);
int insufficientInsurance(Traveler* pTraveler);
int addAttraction(Trip* pTrip, Attraction* pAtt);
void checkFlag(Trip* pTrip);
void updateFinalPrice(Trip* pTrip);
int isDateInRange(const Trip* pTrip, const Date* pDate);
void deleteTraveler(Trip* pTrip);
void deleteFlightTicket(Flight* pFlight, Traveler* pTraveler);
int deleteAttraction(Trip* pTrip);
void freeTravelerArr(Traveler** arr, int size);
void freeAttractionArr(Attraction** arr, int size);
void freeTrip(Trip* pTrip);
void printShortTrip(const Trip* pTrip);
void printTrip(const Trip* pTrip);
int saveTripToTextFile(const Trip* pTrip, FILE* fp);
int loadTripFromTextFile(Trip* pTrip, char* countryName, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp);
int saveTripToBinaryFile(const Trip* pTrip, FILE* fp);
int loadTripFromBinaryFile(Trip* pTrip, char* countryName, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp);

#endif
