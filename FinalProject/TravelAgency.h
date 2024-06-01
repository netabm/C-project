#ifndef _TRAVELAGENCY_
#define _TRAVELAGENCY_

#include "Country.h"
#include "Traveler.h"

typedef enum {
	eAge, eName, eInsPrice, eNotSorted, eNofSortType
} eSortType;

static const char* SortTypeStr[eNofSortType] = { "Age", "Name", "Insurance price", "Not sorted" };

typedef enum {
	eHotels, eFood, eBuses, eTreatment, eTourGuide, eNofReviewTypes
} eReviews;

static const char* ReviewTypeStr[eNofReviewTypes] = { "Hotels", "Food", "Buses","Treatment","Tour Guide" };

typedef struct {
	Traveler* travelerPtr;  //Pointer to the traveler who left the review
	int ratings[eNofReviewTypes];
	char comment[MAX_STR_LEN];
} Review;

typedef struct
{
	char* name;
	Country** countryArr;
	int countryCount;
	Traveler** allTravelerArr;
	int allTravelerCount;
	Attraction** allAttArr;
	int allAttCount;
	eSortType	travelerSortOpt;
	int numOfReviews;
	double finalScore;
	Review* reviews;
}TravelAgency;

void initTravelAgency(TravelAgency* pAgency);
int addCountry(TravelAgency* pAgency);
int addTravelerToAgency(TravelAgency* pAgency);
int addAttractionToAgency(TravelAgency* pAgency);
Country* chooseCountry(const TravelAgency* pAgency, char* msg);
Trip* chooseTrip(const TravelAgency* pAgency, const Country* pCountry, char* msg);
int createTrip(TravelAgency* pAgency);
void printCountryArr(const TravelAgency* pAgency);
void printTravelerArr(const TravelAgency* pAgency);
void printAttractionArr(TravelAgency* pAgency);
int addTravelerToTrip(TravelAgency* pAgency, Trip* pTrip);
int addTravelerToExistingTrip(TravelAgency* pAgency);
int addAttractionToTrip(TravelAgency* pAgency, Trip* pTrip);
int addAttractionToExistingTrip(TravelAgency* pAgency);
int checkSameTraveler(const Traveler* pTraveler, const Trip* pTrip);
int checkValidIndex(int index, int size);
int checkSameAttraction(Attraction* pAtt, const Trip* pTrip);
int deleteTravelerFromTrip(TravelAgency* pAgency);
int deleteAttractionFromTrip(TravelAgency* pAgency);
void sortTravelerArr(TravelAgency* pAgency);
void findTraveler(const TravelAgency* pAgency);
int joinATrip(TravelAgency* pAgency);
void addReview(TravelAgency* pAgency);
void printReviewsArr(const TravelAgency* pAgency);
void printReview(const Review review);
int selectTripToPrint(TravelAgency* pAgency);
void printTravelAgency(TravelAgency* pAgency);
int saveReviewToTextFile(const Review* pReview, FILE* fp);
int loadReviewFromTextFile(Review* pReview, Traveler** allTravelerArr, int countTraveler, FILE* fp);
int saveReviewToBinaryFile(const Review* pReview, FILE* fp);
int loadReviewFromBinaryFile(Review* pReview, Traveler** allTravelerArr, int countTraveler, FILE* fp);

#endif
