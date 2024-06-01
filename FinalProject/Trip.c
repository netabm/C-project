#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trip.h"
#include "FileHelper.h"

int initTrip(Trip* pTrip, char* countryName)
{
	pTrip->serialNum = getNextSerialNum();
	pTrip->dest = countryName;
	if (!createFlights(pTrip))
		return 0;
	pTrip->travelerArr = NULL;
	pTrip->travelerCount = 0;
	if (L_init(&pTrip->attractionList) == False)
	{
		return 0;
	}
	pTrip->attractionCount = 0;
	pTrip->hardLevelFlag = False;
	pTrip->finalPrice = 0;
	return 1;
}

int getNextSerialNum()
{
	return nextSerialNum++;
}

int createFlights(Trip* pTrip)
{
	Date* start = (Date*)calloc(1, sizeof(Date));
	if (!start)
		return 0;
	Date* end = (Date*)calloc(1, sizeof(Date));
	if (!end)
	{
		free(start);
		return 0;
	}
	pTrip->flightStart = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightStart) {
		free(start);
		free(end);
		return 0;
	}
	pTrip->flightEnd = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightEnd) {
		free(start);
		free(end);
		freeFlight(pTrip->flightStart);
		return 0;
	}
	printf("\nOutbound flight:\n");
	initDate(start);
	if (!initFlightOutbound(pTrip->flightStart, pTrip->dest, start))
	{
		free(start);
		free(end);
		freeFlight(pTrip->flightStart);
		freeFlight(pTrip->flightEnd);
		return 0;
	}
	printf("\nInbound flight:\n");
	initDate(end);
	while (compareDates(start, end) != -1)
	{
		printf("Inbount flight must be after outbound flight. Please try again\n");
		initDate(end);
	}
	if (!initFlightInbound(pTrip->flightEnd, pTrip->dest, end))
	{
		free(start);
		free(end);
		freeFlight(pTrip->flightStart);
		freeFlight(pTrip->flightEnd);
		return 0;
	}
	return 1;
}

int addTraveler(Trip* pTrip, Traveler* pTraveler)
{
	if (pTrip->hardLevelFlag == True && pTraveler->ins.isExtreme == False)
		if (insufficientInsurance(pTraveler) == 0) {
			printf("Traveler Wasn't added\n"); // give option to upgrade insurance
			return 0;
		}

	if ((!addTicket(pTrip->flightStart, pTraveler, "outbound")) || (!addTicket(pTrip->flightEnd, pTraveler, "inbound"))) {
		printf("One of the flights is full, cannot add traveler to the trip.\n");
		//needs to free thins
		return 0;
	}

	pTrip->travelerArr = (Traveler**)realloc(pTrip->travelerArr, (pTrip->travelerCount + 1) * sizeof(Traveler*));
	if (!pTrip->travelerArr)
		return 0;
	pTrip->travelerArr[pTrip->travelerCount] = pTraveler;
	pTrip->travelerCount++;

	updateFinalPrice(pTrip);
	printf("%s was added to the trip successfully\n", pTraveler->name);
	return 1;
}

int insufficientInsurance(Traveler* pTraveler)
{
	int opt;
	printf("The traveler's insurance is basic, there are extreme attractions that he can't participate\n");
	printf("Enter 0 if you want to remove the traveler from the trip\n");
	printf("Enter 1 if you want to upgrade traveler's insurance\n");
	do
	{
		getInt(&opt, "your choise");
	} while (opt != 0 && opt != 1);
	if (opt == 0)
		return 0;
	if (opt == 1)
		upgradeInsurance(pTraveler);
	return 1;
}

int addAttraction(Trip* pTrip, Attraction* pAtt)
{
	int insuranceUpgradeCost = 0;

	if (pAtt->level == eHard)
		pTrip->hardLevelFlag = True;
	// Check if insurance upgrade is needed and calculate the cost
	for (int i = 0; i < pTrip->travelerCount; i++) {
		if (pTrip->hardLevelFlag == True && pTrip->travelerArr[i]->ins.isExtreme == False) {
			insuranceUpgradeCost += EXTREME - BASIC;
		}
	}

	// Calculate the total cost
	int totalCost = pTrip->finalPrice + pAtt->price + insuranceUpgradeCost;

	// Check if upgrading insurance is needed and ask the user
	if (insuranceUpgradeCost > 0) {
		printf("One or more of your travelers can't participate in this attraction because they don't have proper insurence.\n");
		printf("Do you want to upgrade travelers' insurance? (1 for Yes, 0 for No): ");
		int opt;
		scanf("%d", &opt);

		if (opt == 1)
		{
			// Upgrade insurance for travelers who need it
			for (int i = 0; i < pTrip->travelerCount; i++) {
				if (pTrip->hardLevelFlag == True && pTrip->travelerArr[i]->ins.isExtreme == False) {
					upgradeInsurance(pTrip->travelerArr[i]);
				}
			}
		}
		else
		{
			checkFlag(pTrip);
			printf("Attraction wasn't added\n");
			return 0;
		}
	}
	if (!L_insert(&pTrip->attractionList.head, pAtt))
		return 0;
	pTrip->attractionCount++;
	pTrip->finalPrice = totalCost;

	return 1;
}

void checkFlag(Trip* pTrip)
{
	NODE* pNode = &pTrip->attractionList.head;
	NODE* pCurr = pNode->next;

	while (pCurr != NULL)
	{
		Attraction* pAtt = (Attraction*)pCurr->key;
		if (pAtt->level == eHard)
		{
			pTrip->hardLevelFlag = True;
			return;
		}
		pCurr = pCurr->next;
	}
}

void updateFinalPrice(Trip* pTrip)
{
	pTrip->finalPrice = 0;

	// Add the price of each traveler's insurance to the final price
	for (int i = 0; i < pTrip->travelerCount; i++) {
		pTrip->finalPrice += pTrip->travelerArr[i]->ins.finalPrice;
	}

	// Add the price of each attraction to the final price
	NODE* pNode = pTrip->attractionList.head.next;
	while (pNode != NULL) {
		pTrip->finalPrice += (((Attraction*)pNode->key)->price * pTrip->travelerCount);
		pNode = pNode->next;
	}

	// Iterate over the flight ticket array of the outbound flight
	for (int i = 0; i < pTrip->flightStart->countFlightTickets; i++) {
		pTrip->finalPrice += pTrip->flightStart->flightTicketArr[i].price;
	}

	// Iterate over the flight ticket array of the inbound flight
	for (int i = 0; i < pTrip->flightEnd->countFlightTickets; i++) {
		pTrip->finalPrice += pTrip->flightEnd->flightTicketArr[i].price;
	}
}

int isDateInRange(const Trip* pTrip, const Date* pDate)
{
	if (compareDates(pDate, pTrip->flightStart->date) == -1)
		return 0;
	if (compareDates(pDate, pTrip->flightEnd->date) == 1)
		return 0;
	return 1;
}

void deleteTraveler(Trip* pTrip)
{
	int travelerIndex;
	printf("Choose traveler to delete\n");
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		printf("%d for %s, id: %d\n", i + 1, pTrip->travelerArr[i]->name, pTrip->travelerArr[i]->id);
	}
	scanf("%d", &travelerIndex);
	while (travelerIndex < 1 || travelerIndex > pTrip->travelerCount)
	{
		printf("Wrong input, please choose a number between 1 to %d\n", pTrip->travelerCount);
		scanf("%d", &travelerIndex);
	}

	Traveler* pTraveler = pTrip->travelerArr[travelerIndex - 1];
	deleteFlightTicket(pTrip->flightStart, pTraveler);
	deleteFlightTicket(pTrip->flightEnd, pTraveler);

	for (int i = travelerIndex - 1; i < pTrip->travelerCount - 1; i++)
	{
		pTrip->travelerArr[i] = pTrip->travelerArr[i + 1];
	}
	pTrip->travelerArr[pTrip->travelerCount - 1] = NULL;
	pTrip->travelerCount--;
	printf("Traveler was removed successfully\n");
	updateFinalPrice(pTrip);
}

void deleteFlightTicket(Flight* pFlight, Traveler* pTraveler)
{
	for (int i = 0; i < pFlight->countFlightTickets; i++)
	{
		if (pFlight->flightTicketArr[i].id == pTraveler->id)
		{
			char* seat = pFlight->flightTicketArr[i].seat;
			int seatNum;
			char seatChar;
			sscanf(seat, "%d%c", &seatNum, &seatChar);
			int seatCols = seatChar - 'A' + 1;
			pFlight->seats[seatNum - 1][seatCols - 1] = False;

			//free(&pFlight->flightTicketArr[i]);
			for (int j = i; j < pFlight->countFlightTickets - 1; j++)
			{
				pFlight->flightTicketArr[j] = pFlight->flightTicketArr[j + 1];
			}
			//pFlight->flightTicketArr[pFlight->countFlightTickets - 1] = NULL;
			pFlight->countFlightTickets--;
			pFlight->flightTicketArr = (FlightTicket*)realloc(pFlight->flightTicketArr, pFlight->countFlightTickets * sizeof(FlightTicket));
			return;
		}
	}
}

int deleteAttraction(Trip* pTrip)
{
	Attraction temp;
	printf("Choose attraction to delete\n");
	L_print(&pTrip->attractionList, printAttraction); //might have a const problem
	temp.name = getStrExactName("Please enter attractions' name");
	if (!temp.name)
		return 0;

	NODE* pNode = &pTrip->attractionList.head;
	while (pNode->next != NULL) {
		if (compareAttractions(pNode->next->key, &temp) == 0) {
			break; //Found the node before the node to be deleted
		}
		pNode = pNode->next;
	}

	// Check if the node was found
	if (pNode->next == NULL) {
		printf("Attraction with this name was not found in the trip\n");
		freeAttraction(&temp);
		return 0;
	}
	
	L_delete(pNode, NULL); //make sure freeAtt is good
	
	pTrip->attractionCount--;
	freeAttraction(&temp);
	checkFlag(pTrip);
	printf("Attraction was removed successfully\n");
	updateFinalPrice(pTrip);
	return 1;
}

void freeTravelerArr(Traveler** arr, int size)
{
	generalArrayFunction(arr, size, sizeof(Traveler*), freeTravelerPtr);
}

void freeAttractionArr(Attraction** arr, int size)
{
	generalArrayFunction(arr, size, sizeof(Attraction*), freeAttractionPtr);
}

void freeTrip(Trip* pTrip)
{
	freeTravelerArr(pTrip->travelerArr, pTrip->travelerCount);
	free(pTrip->travelerArr);
	freeFlight(pTrip->flightStart);
	freeFlight(pTrip->flightEnd);
}

void printShortTrip(const Trip* pTrip)
{
	printf("Destination: %s\n", pTrip->dest);
	printf("Dates: ");
	printDate(pTrip->flightStart->date);
	printf(" - ");
	printDate(pTrip->flightEnd->date);
	printf("\nTotal Travelers: %d\n", pTrip->travelerCount);
	printf("Total Attractions: %d\n", pTrip->attractionCount);
}

void printTrip(const Trip* pTrip)
{
	printf("\n");
	printf("--------------------------------------------------\n");
	printf("                TRIP INFORMATION                  \n");
	printf("--------------------------------------------------\n");
	printf("Serial Number: %d\n", pTrip->serialNum);
	printf("Destination: %s\n", pTrip->dest);
	printf("Final Price: %d$\n", pTrip->finalPrice);

	printFlight(pTrip->flightStart, "OUTBOUND");

	printFlight(pTrip->flightEnd, "INBOUND");

	printf("--------------------------------------------------\n");
	printf("               TRAVELERS INFORMATION              \n");
	printf("--------------------------------------------------\n");
	printf("Total Travelers: %d\n", pTrip->travelerCount);
	printf("--------------------------------------------------\n");

	generalArrayFunction(pTrip->travelerArr, pTrip->travelerCount, sizeof(Traveler*), printTravelerPtr);

	printf("                ATTRACTIONS LIST                  \n");
	printf("--------------------------------------------------\n");
	printf("Total Attractions: %d\n", pTrip->attractionCount);
	printf("--------------------------------------------------\n");

	NODE* pNode = pTrip->attractionList.head.next;
	int i = 0;
	while (pNode != NULL) {
		i++;
		printf("%d) ", i);
		printAttraction((Attraction*)pNode->key);
		pNode = pNode->next;
	}

	printf("--------------------------------------------------\n");
}

int saveTripToTextFile(const Trip* pTrip, FILE* fp)
{
	fprintf(fp, "%d\n", pTrip->serialNum);

	fprintf(fp, "%d\n", pTrip->travelerCount);
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		fprintf(fp, "%d\n", pTrip->travelerArr[i]->id);
	}

	saveFlightToTextFile(pTrip->flightStart, fp);
	saveFlightToTextFile(pTrip->flightEnd, fp);

	fprintf(fp, "%d\n", pTrip->attractionCount);
	if (pTrip->attractionCount > 0)
	{
		NODE* pN = pTrip->attractionList.head.next;

		Attraction* pAtt;
		while (pN != NULL)
		{
			pAtt = (Attraction*)pN->key;
			fprintf(fp, "%s\n", pAtt->name);
			pN = pN->next;
		}
	}

	fprintf(fp, "%d\n", pTrip->hardLevelFlag);
	fprintf(fp, "%d\n", pTrip->finalPrice);
	return 1;
}

int loadTripFromTextFile(Trip* pTrip, char* countryName, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp)
{
	getNextSerialNum();
	fscanf(fp, "%d\n", &pTrip->serialNum);
	pTrip->dest = countryName;

	fscanf(fp, "%d\n", &pTrip->travelerCount);
	pTrip->travelerArr = (Traveler**)calloc(pTrip->travelerCount, sizeof(Traveler*));
	if (!pTrip->travelerArr)
		return 0;

	int id;
	//Find the the traveler in agency by his id
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		fscanf(fp, "%d\n", &id);
		for (int j = 0; j < countTraveler; j++)
		{
			if (travelerArr[j]->id == id)
			{
				pTrip->travelerArr[i] = travelerArr[j];
				break;
			}
		}
	}

	pTrip->flightStart = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightStart)
		return 0;
	loadFlightOutboundFromTextFile(pTrip->flightStart, countryName, pTrip->travelerArr, pTrip->travelerCount, fp);
	pTrip->flightEnd = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightEnd)
		return 0;
	loadFlightInboundFromTextFile(pTrip->flightEnd, countryName, pTrip->travelerArr, pTrip->travelerCount, fp);

	fscanf(fp, "%d\n", &pTrip->attractionCount);
	if (L_init(&pTrip->attractionList) == False)
		return 0;

	char* attName;
	char temp[MAX_STR_LEN];
	for (int i = 0; i < pTrip->attractionCount; i++)
	{
		myGets(temp, MAX_STR_LEN, fp);
		attName = getDynStr(temp);
		for (int j = 0; j < countAtt; j++)
		{
			if (strcmp(attArr[j]->name, attName) == 0)
			{
				L_insert(&pTrip->attractionList.head, attArr[j]);
				break;
			}
		}
		free(attName);
	}

	fscanf(fp, "%d\n", &pTrip->hardLevelFlag);
	fscanf(fp, "%d\n", &pTrip->finalPrice);

	return 1;
}

int saveTripToBinaryFile(const Trip* pTrip, FILE* fp)
{
	if (!writeIntToFile(pTrip->serialNum, fp, "Error write trip serial number\n"))
		return 0;
	if (!writeIntToFile(pTrip->travelerCount, fp, "Error write traveler count\n"))
		return 0;
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		if (!writeIntToFile(pTrip->travelerArr[i]->id, fp, "Error write traveler\n"))
			return 0;
	}

	if (!saveFlightToBinaryFile(pTrip->flightStart, fp))
		return 0;
	if (!saveFlightToBinaryFile(pTrip->flightEnd, fp))
		return 0;
	if (!writeIntToFile(pTrip->attractionCount, fp, "Error write attraction\n"))
		return 0;
	if (pTrip->attractionCount > 0)
	{
		NODE* pN = pTrip->attractionList.head.next;
		Attraction* pAtt;
		while (pN != NULL)
		{
			pAtt = (Attraction*)pN->key;
			if (!writeStringToFile(pAtt->name, fp, "Error write attraction name\n"))
				return 0;
			pN = pN->next;
		}
	}
	if (!writeIntToFile(pTrip->hardLevelFlag, fp, "Error write level flag\n"))
		return 0;
	if (!writeIntToFile(pTrip->finalPrice, fp, "Error write final price\n"))
		return 0;

	return 1;
}

int loadTripFromBinaryFile(Trip* pTrip, char* countryName, Traveler** travelerArr, int countTraveler, Attraction** attArr, int countAtt, FILE* fp)
{
	getNextSerialNum();
	if (!readIntFromFile(&pTrip->serialNum, fp, "Error reading trip serial number\n"))
		return 0;
	pTrip->dest = countryName;
	if (!readIntFromFile(&pTrip->travelerCount, fp, "Error reading traveler count\n"))
		return 0;

	pTrip->travelerArr = (Traveler**)calloc(pTrip->travelerCount, sizeof(Traveler*));
	if (!pTrip->travelerArr)
		return 0;

	int id;
	for (int i = 0; i < pTrip->travelerCount; i++)
	{
		if (!readIntFromFile(&id, fp, "Error reading traveler id\n"))
			return 0;
		for (int j = 0; j < countTraveler; j++)
		{
			if (travelerArr[j]->id == id)
			{
				pTrip->travelerArr[i] = travelerArr[j];
				break;
			}
		}
	}

	pTrip->flightStart = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightStart)
		return 0;
	if (!loadFlightOutboundFromBinaryFile(pTrip->flightStart, countryName, pTrip->travelerArr, pTrip->travelerCount, fp))
	{
		free(pTrip->flightStart);
		return 0;
	}
	pTrip->flightEnd = (Flight*)calloc(1, sizeof(Flight));
	if (!pTrip->flightEnd)
	{
		free(pTrip->flightStart);
		return 0;
	}
	if (!loadFlightinboundFromBinaryFile(pTrip->flightEnd, countryName, pTrip->travelerArr, pTrip->travelerCount, fp))
	{
		free(pTrip->flightStart);
		free(pTrip->flightEnd);
		return 0;
	}

	if (!readIntFromFile(&pTrip->attractionCount, fp, "Error reading attraction count\n"))
	{
		free(pTrip->flightStart);
		free(pTrip->flightEnd);
		return 0;
	}
	if (L_init(&pTrip->attractionList) == False)
	{
		free(pTrip->flightStart);
		free(pTrip->flightEnd);
		return 0;
	}

	char* attName;
	for (int i = 0; i < pTrip->attractionCount; i++)
	{
		attName = readStringFromFile(fp, "Error reading attraction name\n");
		for (int j = 0; j < countAtt; j++)
		{
			if (strcmp(attArr[j]->name, attName) == 0)
			{
				L_insert(&pTrip->attractionList.head, attArr[j]);
				break;
			}
		}
		free(attName);
	}

	int temp;
	if (!readIntFromFile(&temp, fp, "Error reading level flag\n"))
	{
		free(pTrip->flightStart);
		free(pTrip->flightEnd);
		return 0;
	}
	pTrip->hardLevelFlag = temp;
	if (!readIntFromFile(&temp, fp, "Error reading final price\n"))
	{
		free(pTrip->flightStart);
		free(pTrip->flightEnd);
		return 0;
	}
	pTrip->finalPrice = temp;

	return 1;
}