#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FlightTicket.h"
#include "FileHelper.h"

int initTicket(FlightTicket* pFTicket, BOOL(*seatsMat)[COLS], int countFlightTicket, Traveler* pTraveler)
{
	pFTicket->name = pTraveler->name;
	pFTicket->id = pTraveler->id;
	if (!chooseSeat(pFTicket, seatsMat, countFlightTicket))
		return 0;
	chooseClass(pFTicket);
	return 1;
}

int chooseSeat(FlightTicket* pFTicket, BOOL(*seatsMat)[COLS], int countFlightTicket)
{
	char tempSeat[MAX_STR_LEN];
	int ok;

	if (countFlightTicket == (ROWS * COLS)) {
		return 0; // This flight is full, cannot enter new seat.
	}
	do {
		printf("Please choose seat between 01A-10F\n");
		myGets(tempSeat, MAX_STR_LEN, stdin);
		ok = checkSeat(tempSeat, seatsMat);
		if (ok == 0)
			printf("The seat is not valid\n");
		if (ok == -1)
			printf("The seat is taken\n");

	} while (ok != 1);
	strcpy(pFTicket->seat, tempSeat);
	return 1;
}

int checkSeat(char* seat, BOOL(*seatsMat)[COLS])
{
	int seatNum;
	char seatChar;
	if (strlen(seat) != 3)
		return 0;
	sscanf(seat, "%d%c", &seatNum, &seatChar);
	if (seatNum < 0 || seatNum > 10)
		return 0;
	if (seatChar < 'A' || seatChar > 'F')
		return 0;

	int seatCols = seatChar - 'A' + 1;
	if (seatsMat[seatNum - 1][seatCols - 1] == True)
		return -1;
	seatsMat[seatNum - 1][seatCols - 1] = True;

	return 1;
}

void chooseClass(FlightTicket* pFTicket)
{
	int opt;
	do
	{
		printf("\nPlease choose the desired class:\n");
		for (int i = 1; i <= eNofTypes; i++)
			printf("%d for %s, price: %d$\n", i, classStr[i - 1], classPrice[i - 1]);
		scanf("%d", &opt);

		// Check if the entered value is valid
		if (opt < 1 || opt > eNofTypes) {
			printf("Invalid choice. Please enter a number between 1 and %d.\n", eNofTypes);
		}
	} while (opt < 1 || opt > eNofTypes);

	pFTicket->flightClass = opt - 1;
	pFTicket->price = classPrice[opt - 1];
}

void printFlightTicket(FlightTicket* pFTicket)
{
	printf("--------------------------------------------------\n");
	printf("|                  FLIGHT TICKET                  |\n");
	printf("--------------------------------------------------\n");
	printf("| Passenger's Name: %-30s |\n", pFTicket->name);
	printf("| Passenger's Id: %-30d |\n", pFTicket->id);
	printf("| Class:            %-30s |\n", classStr[pFTicket->flightClass]);
	printf("| Seat:             %-30s |\n", pFTicket->seat);
	printf("| Price:            %6d$                    |\n", pFTicket->price);
	printf("--------------------------------------------------\n");
}

int saveFlightTicketToTextFile(const FlightTicket* pFTicket, FILE* fp)
{
	fprintf(fp, "%d\n", pFTicket->id);
	fprintf(fp, "%s\n", pFTicket->seat);
	fprintf(fp, "%d\n", pFTicket->flightClass);
	fprintf(fp, "%d\n", pFTicket->price);
	return 1;
}

int loadFlightTicketFromTextFile(FlightTicket* pFTicket, Traveler** travelerArr, int countTraveler, BOOL(*seatsMat)[COLS], FILE* fp)
{
	int id;
	fscanf(fp, "%d\n", &id);

	for (int i = 0; i < countTraveler; i++)
	{
		if (travelerArr[i]->id == id)
		{
			pFTicket->name = travelerArr[i]->name;
			pFTicket->id = travelerArr[i]->id;
			break;
		}
	}

	char temp[MAX_STR_LEN];
	myGets(temp, MAX_STR_LEN, fp);
	strcpy(pFTicket->seat, temp);

	int seatNum;
	char seatChar;
	sscanf(pFTicket->seat, "%d%c", &seatNum, &seatChar);
	int seatCols = seatChar - 'A' + 1;
	seatsMat[seatNum - 1][seatCols - 1] = True;

	fscanf(fp, "%d\n", &pFTicket->flightClass);
	fscanf(fp, "%d\n", &pFTicket->price);

	return 1;
}

int saveFlightTicketToBinaryFile(const FlightTicket* pFTicket, FILE* fp)
{
	if (!writeIntToFile(pFTicket->id, fp, "Error write traveler id\n"))
		return 0;
	if(!writeCharsToFile(pFTicket->seat, SIZE + 1, fp, "Error write flight ticket seat\n"))
		return 0;
	if (!writeIntToFile(pFTicket->flightClass, fp, "Error write flight ticket class\n"))
		return 0;
	if (!writeIntToFile(pFTicket->price, fp, "Error write flight ticket price\n"))
		return 0;
	return 1;
}

int loadFlightTicketFromBinaryFile(FlightTicket* pFTicket, Traveler** travelerArr, int countTraveler, BOOL(*seatsMat)[COLS], FILE* fp)
{
	int id;
	if (!readIntFromFile(&id, fp, "Error reading traveler id\n"))
		return 0;
	for (int i = 0; i < countTraveler; i++)
	{
		if (travelerArr[i]->id == id)
		{
			pFTicket->name = travelerArr[i]->name;
			pFTicket->id = travelerArr[i]->id;
			break;
		}
	}
	
	readCharsFromFile(pFTicket->seat, SIZE + 1, fp, "Error reading seat\n");
	int seatNum;
	char seatChar;
	sscanf(pFTicket->seat, "%d%c", &seatNum, &seatChar);
	int seatCols = seatChar - 'A' + 1;
	seatsMat[seatNum - 1][seatCols - 1] = True;

	int temp;
	if (!readIntFromFile(&temp, fp, "Error reading flight ticket class\n"))
		return 0;
	pFTicket->flightClass = temp;
	if (!readIntFromFile(&pFTicket->price, fp, "Error reading flight ticket price\n"))
		return 0;
	return 1;
}