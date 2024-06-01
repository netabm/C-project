#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TravelAgency.h"
#include "FileHelper.h"

void initTravelAgency(TravelAgency* pAgency)
{
	pAgency->name = getStrExactName("Please enter travel agency name");
	pAgency->countryArr = NULL;
	pAgency->countryCount = 0;
	pAgency->allTravelerArr = NULL;
	pAgency->allTravelerCount = 0;
	pAgency->allAttArr = NULL;
	pAgency->allAttCount = 0;
	pAgency->travelerSortOpt = eNotSorted;
	pAgency->numOfReviews = 0;
	pAgency->finalScore = 0;
	pAgency->reviews = NULL;
}

int addCountry(TravelAgency* pAgency)
{
	Country* pCountry = (Country*)calloc(1, sizeof(Country));
	if (!pCountry)
		return 0;
	initCountry(pCountry);

	for (int i = 0; i < pAgency->countryCount; i++)
	{
		if (strcmp(pAgency->countryArr[i]->name, pCountry->name) == 0)
		{
			printf("Country with the same name already exists in the agency\n");
			return 0;
		}
	}

	pAgency->countryArr = (Country**)realloc(pAgency->countryArr, (pAgency->countryCount + 1) * sizeof(Country*));
	if (!pAgency->countryArr)
	{
		free(pCountry);
		return 0;
	}
	pAgency->countryArr[pAgency->countryCount] = pCountry;
	pAgency->countryCount++;
	return 1;
}

int addTravelerToAgency(TravelAgency* pAgency)
{
	Traveler* pTraveler = (Traveler*)calloc(1, sizeof(Traveler));
	if (!pTraveler)
		return 0;
	int id;
	getInt(&id, "id");

	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		if (pAgency->allTravelerArr[i]->id == id)
		{
			printf("Traveler with the same id already exists in the Agency\n");
			free(pTraveler);
			return 0;
		}
	}

	initTraveler(pTraveler, id);

	pAgency->allTravelerArr = (Traveler**)realloc(pAgency->allTravelerArr, (pAgency->allTravelerCount + 1) * sizeof(Traveler*));
	if (!pAgency->allTravelerArr)
	{
		free(pTraveler->name);
		free(pTraveler);
		return 0;
	}
	pAgency->allTravelerArr[pAgency->allTravelerCount] = pTraveler;
	pAgency->allTravelerCount++;
	return 1;
}

int addAttractionToAgency(TravelAgency* pAgency)
{
	Attraction* pAtt = (Attraction*)calloc(1, sizeof(Attraction));
	if (!pAtt)
		return 0;
	initAttraction(pAtt);

	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		if (strcmp(pAgency->allAttArr[i]->name, pAgency->name) == 0)
		{
			printf("Attraction with the same name already exists in the Agency\n");
			freeAttraction(pAtt);
			free(pAtt);
			return 0;
		}
	}

	pAgency->allAttArr = (Attraction**)realloc(pAgency->allAttArr, (pAgency->allAttCount + 1) * sizeof(Attraction*));
	if (!pAgency->allAttArr)
	{
		free(pAtt);
		return 0;
	}
	pAgency->allAttArr[pAgency->allAttCount] = pAtt;
	pAgency->allAttCount++;
	return 1;
}

Country* chooseCountry(const TravelAgency* pAgency, char* msg)
{
	if (pAgency->countryCount == 0)
	{
		printf("To %s you need to add countries to the agency\n", msg);
		return NULL;
	}
	if (pAgency->allTravelerCount == 0)
	{
		printf("To %s you need to add travelers to the agency\n", msg);
		return NULL;
	}
	int opt;
	printCountryArr(pAgency);
	printf("Please choose a country by it's number\n");
	scanf("%d", &opt);
	while (!checkValidIndex(opt, pAgency->countryCount))
	{
		scanf("%d", &opt);
	}
	printf("\n");
	return pAgency->countryArr[opt - 1];
}

Trip* chooseTrip(const TravelAgency* pAgency, const Country* pCountry, char* msg)
{
	if (pCountry->tripCount == 0)
	{
		printf("To %s you need to add trips to the country\n", msg);
		return NULL;
	}
	printf("Choose wanted trip by it's serial number\n");
	int serNum;
	printTripArr(pCountry);
	scanf("%d", &serNum);

	while (1)
	{
		for (int i = 0; i < pCountry->tripCount; i++)
		{
			if (pCountry->tripArr[i]->serialNum == serNum)
				return pCountry->tripArr[i];
		}
		printf("Serial number doesn't exist please try again\n");
		scanf("%d", &serNum);
	}
	return NULL;
}

int createTrip(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "create trip");
	if (!pCountry)
		return 0;

	Trip* pTrip = (Trip*)calloc(1, sizeof(Trip));
	if (!pTrip)
		return 0;
	if (!initTrip(pTrip, pCountry->name)) {
		return 0;
	}
	printf("\n");
	if (!addTravelerToTrip(pAgency, pTrip))
		return 0;
	if (!addTripToCountry(pCountry, pTrip))
		return 0;
	if (!addAttractionToTrip(pAgency, pTrip))
		return 0;

	return 1;
}

void printCountryArr(const TravelAgency* pAgency)
{
	for (int i = 0; i < pAgency->countryCount; i++)
		printf("%d) %s\n", i + 1, pAgency->countryArr[i]->name);
}

void printTravelerArr(const TravelAgency* pAgency)
{
	printf("List of Travelers:\n");
	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		printf("%d) Name: %s, ID: %d\n", i + 1, pAgency->allTravelerArr[i]->name, pAgency->allTravelerArr[i]->id);
	}
}

void printAttractionArr(TravelAgency* pAgency)
{
	for (int i = 0; i < pAgency->allAttCount; i++)
	{
		printf("%d) ", i + 1);
		printAttraction(pAgency->allAttArr[i]);
	}
}

int addTravelerToTrip(TravelAgency* pAgency, Trip* pTrip)
{
	int diff = pAgency->allTravelerCount - pTrip->travelerCount;
	if (diff == 0)
	{
		printf("All of the travelers in the agency are already participating in this trip\n");
		return 0;
	}
	int amount;
	int travelerIndex;
	printf("How many travelers do you want to add to the trip? between 1 to %d\n", diff);
	scanf("%d", &amount);

	while (!checkValidIndex(amount, diff))
		scanf("%d", &amount);

	printTravelerArr(pAgency);
	Traveler* pTraveler;
	for (int i = 0; i < amount; i++)
	{
		printf("\nPlease enter the number of the traveler you want to add\n");
		scanf("%d", &travelerIndex);
		while (!checkValidIndex(travelerIndex, pAgency->allTravelerCount))
		{
			scanf("%d", &travelerIndex);
		}

		pTraveler = pAgency->allTravelerArr[travelerIndex - 1];
		if (!checkSameTraveler(pTraveler, pTrip))
		{
			printf("You already added this traveler, please choose a different one\n");
			i--; // Try again
		}
		else
			if (addTraveler(pTrip, pTraveler) == 0)
				return 0;
	}
	return 1;
}

int addTravelerToExistingTrip(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "add traveler");
	if (!pCountry)
		return 0;
	Trip* pTrip = chooseTrip(pAgency, pCountry, "add traveler");
	if (!pTrip)
		return 0;
	if (addTravelerToTrip(pAgency, pTrip) == 0)
		return 0;
	return 1;
}

int addAttractionToTrip(TravelAgency* pAgency, Trip* pTrip)
{
	if (pAgency->allAttCount == 0)
		return 0;
	int diff = pAgency->allAttCount - pTrip->attractionCount;
	if (diff == 0)
	{
		printf("All of the attractions in the agency are already in this trip\n");
		return 0;
	}
	int amount;
	printf("\nHow many attractions do you want to add to the trip? between 1 to %d\n", diff);
	scanf("%d", &amount);
	while (!checkValidIndex(amount, diff))
		scanf("%d", &amount);

	printAttractionArr(pAgency);
	Attraction* pAtt;
	int attIndex;
	for (int i = 0; i < amount; i++)
	{
		printf("Please enter the number of the attraction you want to add\n");
		scanf("%d", &attIndex);
		while (!checkValidIndex(attIndex, pAgency->allAttCount))
			scanf("%d", &attIndex);

		pAtt = pAgency->allAttArr[attIndex - 1];
		if (!checkSameAttraction(pAtt, pTrip))
		{
			printf("This attraction already exists in the trip, please choose a different one\n");
			i--; //Try again
		}
		else
			if (addAttraction(pTrip, pAtt) == 0)
				return 0;
	}
	return 1;
}

int addAttractionToExistingTrip(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "add attraction");
	if (!pCountry)
		return 0;
	Trip* pTrip = chooseTrip(pAgency, pCountry, "add attraction");
	if (!pTrip)
		return 0;
	if (!addAttractionToTrip(pAgency, pTrip))
		return 0;
	return 1;
}

int checkSameTraveler(const Traveler* pTraveler, const Trip* pTrip)
{
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		if (pTrip->travelerArr[i] == pTraveler)
			return 0;
	}
	return 1;
}

int checkValidIndex(int index, int size)
{
	if (index < 1 || index > size) {
		printf("Index not in range, please try again\n");
		return 0;
	}
	return 1;
}

int checkSameAttraction(Attraction* pAtt, const Trip* pTrip)
{
	if (pTrip->attractionCount == 0)
		return 1;
	NODE* pNode = pTrip->attractionList.head.next;
	NODE* foundNode = L_find(pNode, pAtt, compareAttractions);
	if (foundNode != NULL)
		return 0;
	return 1;
}

int addAttractionToCountry(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "add attraction to country");
	if (!pCountry)
		return 0;
	return 1;
}

int deleteTravelerFromTrip(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "delete traveler from trip");
	if (!pCountry)
		return 0;
	Trip* pTrip = chooseTrip(pAgency, pCountry, "delete traveler from trip");
	if (!pTrip)
		return 0;
	deleteTraveler(pTrip);
	return 1;
}

int deleteAttractionFromTrip(TravelAgency* pAgency)
{
	Country* pCountry = chooseCountry(pAgency, "delete attraction from trip");
	if (!pCountry)
		return 0;
	Trip* pTrip = chooseTrip(pAgency, pCountry, "delete attraction from trip");
	if (!pTrip)
		return 0;
	if (!deleteAttraction(pTrip))
		return 0;
	return 1;
}

void sortTravelerArr(TravelAgency* pAgency)
{
	int opt;
	printf("Based on what field do you want to sort?\n");
	do
	{
		printf("Enter 1 for %s\n", SortTypeStr[eAge]);
		printf("Enter 2 for %s\n", SortTypeStr[eName]);
		printf("Enter 3 for %s\n", SortTypeStr[eInsPrice]);
		scanf("%d", &opt);
		switch (opt)
		{
		case 1:
			qsort(pAgency->allTravelerArr, pAgency->allTravelerCount, sizeof(Traveler*), compareTravelerByAge);
			pAgency->travelerSortOpt = eAge;
			break;
		case 2:
			qsort(pAgency->allTravelerArr, pAgency->allTravelerCount, sizeof(Traveler*), compareTravelerByName);
			pAgency->travelerSortOpt = eName;
			break;
		case 3:
			qsort(pAgency->allTravelerArr, pAgency->allTravelerCount, sizeof(Traveler*), compareTravelerByInsPrice);
			pAgency->travelerSortOpt = eInsPrice;
			break;
		}
	} while (opt < 1 || opt > 3);

	for (int i = 0; i < pAgency->allTravelerCount; i++)
	{
		printf("%d) Name: %s, ID: %d, ", i + 1, pAgency->allTravelerArr[i]->name, pAgency->allTravelerArr[i]->id);
		printf("Age: %d, Insurance price: %d\n", pAgency->allTravelerArr[i]->age, pAgency->allTravelerArr[i]->ins.finalPrice);
	}
}

void findTraveler(const TravelAgency* pAgency)
{
	int(*compare)(const void* t1, const void* t2) = NULL;
	Traveler t = { 0 };
	Traveler* pTraveler = &t;
	switch (pAgency->travelerSortOpt)
	{
	case eAge:
		printf("Enter age:\t");
		scanf("%d", &pTraveler->age);
		compare = compareTravelerByAge;
		break;
	case eName:
		pTraveler->name = getStrExactName("Enter name:");
		compare = compareTravelerByName;
		break;
	case eInsPrice:
		printf("Enter insurance price:\t");
		scanf("%d", &pTraveler->ins.finalPrice);
		compare = compareTravelerByInsPrice;
		break;
	}

	if (compare != NULL)
	{
		Traveler** pT = bsearch(&pTraveler, pAgency->allTravelerArr, pAgency->allTravelerCount, sizeof(Traveler*), compare);
		if (pT == NULL)
			printf("Traveler was not found\n");
		else
		{
			printf("Traveler was found, ");
			printTraveler(*pT);
		}
	}
	else {
		printf("The search cannot be performed, array not sorted\n");
	}
}

int joinATrip(TravelAgency* pAgency)
{
	if (!addTravelerToAgency(pAgency))
		return 0;
	Traveler* pTraveler = pAgency->allTravelerArr[pAgency->allTravelerCount - 1];
	printf("Please enter a date to find trips within its range:\n");
	Date pDate;
	initDateNoTime(&pDate);
	int arrSize = 0;
	Trip** tempTripArr = NULL;
	for (int i = 0; i < pAgency->countryCount; i++)
	{
		if (!findTripByDate(pAgency->countryArr[i], &pDate, &tempTripArr, &arrSize))
		{
			printf("Error in allocation\n");
			return 0;
		}
	}
	if (arrSize == 0)
	{
		printf("Sorry, there are no trips in range of the date you entered\n");
		return 0;
	}
	for (int i = 0; i < arrSize; i++)
	{
		printf("\nTrip number %d:\n", i + 1);
		printShortTrip(tempTripArr[i]);
	}
	int choice;
	printf("Choose the number of trip you want to join to\n");
	scanf("%d", &choice);
	while (!checkValidIndex(choice, arrSize))
		scanf("%d", &choice);
	if (!addTraveler(tempTripArr[choice - 1], pTraveler))
		return 0;
	free(tempTripArr);
	return 1;
}

void addReview(TravelAgency* pAgency) {
	int travelerId;
	printf("Please enter your ID: ");
	scanf("%d", &travelerId);

	Traveler* traveler = NULL;
	for (int i = 0; i < pAgency->allTravelerCount; i++) {
		if (pAgency->allTravelerArr[i]->id == travelerId) {
			traveler = pAgency->allTravelerArr[i];
			break;
		}
	}

	if (traveler == NULL) {
		printf("Traveler with the ID %d was not found in the agency.\n", travelerId);
		return;
	}

	pAgency->reviews = (Review*)realloc(pAgency->reviews, (pAgency->numOfReviews + 1) * sizeof(Review));
	if (!pAgency->reviews) { //Allocation failed
		printf("Memory allocation failed for adding the review.\n");
		return;
	}

	//Get user ratings for different aspects
	Review* newReview = &(pAgency->reviews[pAgency->numOfReviews]);
	newReview->travelerPtr = traveler;  // Save pointer to the traveler

	printf("Please rate the following aspects from 1-5:\n");
	for (int i = 0; i < eNofReviewTypes; i++) {
		printf("Enter rating for %s (1-5): ", ReviewTypeStr[i]);
		scanf("%d", &newReview->ratings[i]);
		//Validate input
		while (newReview->ratings[i] < 1 || newReview->ratings[i] > 5) {
			printf("Review must be in the range of 1-5.\n");
			printf("Enter rating for %s (1-5): ", ReviewTypeStr[i]);
			scanf("%d", &newReview->ratings[i]);
		}
	}

	//Ask if the user wants to leave a comment
	int wantComment;
	do {
		printf("Do you want to leave a comment? (1 for yes, 0 for no): ");
		scanf("%d", &wantComment);
		// Check if the input is valid
		if (wantComment != 0 && wantComment != 1) {
			printf("Invalid input. Please enter 1 for yes or 0 for no.\n");
		}
	} while (wantComment != 0 && wantComment != 1);

	//Add comment if requested
	if (wantComment == 1) {
		char* tempComment = getStrExactName("Enter your comment (up to 254 characters):");
		strcpy(newReview->comment, tempComment);
		free(tempComment); //Free dynamically allocated memory
	}
	else {
		//Empty comment
		strcpy(newReview->comment, "No comment");
	}

	//Calculate the average score for the review
	int totalScore = 0;
	for (int i = 0; i < eNofReviewTypes; i++) {
		totalScore += newReview->ratings[i];
	}
	double averageScore = (double)totalScore / eNofReviewTypes;

	//Update the current final score and number of reviews
	pAgency->finalScore = (pAgency->finalScore * pAgency->numOfReviews + averageScore) / (pAgency->numOfReviews + 1);
	pAgency->numOfReviews++;

	printf("Thank you for your review!\n");
	return;
}

void printReviewsArr(const TravelAgency* pAgency)
{
	printf("--------------------------------------------------\n");
	printf("                   ALL REVIEWS                    \n");
	printf("--------------------------------------------------\n");
	for (int i = 0; i < pAgency->numOfReviews; i++)
	{
		printf("%d) ", i + 1);
		printReview(pAgency->reviews[i]);
	}

	//Print final score
	printf("--------------------------------------------------\n");
	printf(" Final Score of All Reviews: %.2f\n", pAgency->finalScore);
	printf("--------------------------------------------------\n");
}

void printReview(const Review review)
{
	//Print traveler ID and name
	if (review.travelerPtr != NULL)
	{
		printf("\n--------------------------------------------------\n");
		printf("Traveler ID: %d\n", review.travelerPtr->id);
		printf("Traveler Name: %s\n", review.travelerPtr->name);
	}
	else
	{
		printf("Traveler ID: Not available\n");
		printf("Traveler Name: Not available\n");
	}

	//Print review details
	printf("Ratings:\n");
	for (int i = 0; i < eNofReviewTypes; i++)
	{
		printf("  %s: %d\n", ReviewTypeStr[i], review.ratings[i]);
	}
	if (strcmp(review.comment, "No comment\n") != 0)
		printf("Comment: %s\n", review.comment);
}


int selectTripToPrint(TravelAgency* pAgency)
{
	// Display all countries and their trips
	Country* pCountry = chooseCountry(pAgency, "print trip");
	if (!pCountry)
		return 0;
	Trip* pTrip = chooseTrip(pAgency, pCountry, "print trip");
	if (!pTrip)
		return 0;
	printTrip(pTrip);
	return 1;
}

void printTravelAgency(TravelAgency* pAgency)
{
	printf("\n");
	printf("__________________________________________________\n");
	printf("              TRAVEL AGENCY INFORMATION           \n");
	printf("__________________________________________________\n");
	printf("Name:                  %s\n", pAgency->name);
	printf("Number of Reviews:     %d\n", pAgency->numOfReviews);
	printf("Final Score:           %.2f\n", pAgency->finalScore);
	printf("Number of Travelers:   %d\n", pAgency->allTravelerCount);
	printf("Number of Attractions: %d\n", pAgency->allAttCount);
	printf("Number of Countries:   %d\n", pAgency->countryCount);
	printf("\n");
	printf("__________________________________________________\n");
	printf("               TRAVELERS INFORMATION              \n");
	printf("__________________________________________________\n");
	printTravelerArr(pAgency);
	printf("\n");
	printf("--------------------------------------------------\n");
	printf("               COUNTRY INFORMATION                \n");
	printf("--------------------------------------------------\n");
	for (int i = 0; i < pAgency->countryCount; i++)
	{
		printf("%s: %d trips\n", pAgency->countryArr[i]->name, pAgency->countryArr[i]->tripCount);
	}
	printf("\n");
	printf("--------------------------------------------------\n");
	printf("              ATTRACTION INFORMATION              \n");
	printf("--------------------------------------------------\n");
	printAttractionArr(pAgency);
	printf("\n");
}

int saveReviewToTextFile(const Review* pReview, FILE* fp)
{
	fprintf(fp, "%d\n", pReview->travelerPtr->id); //Write traveler ID

	//Write ratings
	for (int i = 0; i < eNofReviewTypes; i++) 
	{
		fprintf(fp, "%d ", pReview->ratings[i]);
	}
	fprintf(fp, "\n");

	fprintf(fp, "%s\n", pReview->comment); //Write comment
	return 1;
}

int loadReviewFromTextFile(Review* pReview, Traveler** allTravelerArr, int countTraveler, FILE* fp)
{
	int id;
	fscanf(fp, "%d\n", &id);

	//Find the the traveler in agency by his id
	for (int i = 0; i < countTraveler; i++)
	{
		if (allTravelerArr[i]->id == id)
		{
			pReview->travelerPtr = allTravelerArr[i];
			break;
		}
	}

	for (int i = 0; i < eNofReviewTypes - 1; i++)
	{
		if (fscanf(fp, "%d", &(pReview->ratings[i])) != 1)
			return 0;
	}

	fscanf(fp, "%d\n", &pReview->ratings[eNofReviewTypes - 1]);
	
	if (fgets(pReview->comment, MAX_STR_LEN, fp) == NULL)
		return 0;

	return 1;
}

int saveReviewToBinaryFile(const Review* pReview, FILE* fp)
{
	if (!writeIntToFile(pReview->travelerPtr->id, fp, "Error write id\n")) //Write traveler ID
		return 0;

	for (int i = 0; i < eNofReviewTypes; i++)
	{
		if (!writeIntToFile(pReview->ratings[i], fp, "Error write rating\n"))
			return 0;
	}

	if (!writeStringToFile(pReview->comment, fp, "Error write comment\n"))
		return 0;

	return 1;
}

int loadReviewFromBinaryFile(Review* pReview, Traveler** allTravelerArr, int countTraveler, FILE* fp)
{
	int id;
	if (!readIntFromFile(&id, fp, "Error reading id\n"))
		return 0;

	//Find the the traveler in agency by his id
	for (int i = 0; i < countTraveler; i++)
	{
		if (allTravelerArr[i]->id == id)
		{
			pReview->travelerPtr = allTravelerArr[i];
			break;
		}
	}

	for (int i = 0; i < eNofReviewTypes; i++)
	{
		if (!readIntFromFile(&(pReview->ratings[i]), fp, "Error reading rating\n"))
			return 0;
	}

	char* comment = readStringFromFile(fp, "Error reading comment\n");
	if (!comment)
		return 0; //Error comment reading fail
	
	strncpy(pReview->comment, comment, MAX_STR_LEN - 1); //Copy comment to Review struct
	pReview->comment[MAX_STR_LEN - 1] = '\0'; //Put null termination
	free(comment);
	return 1;
}





