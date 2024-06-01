#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "FileHelper.h"

int initFlightOutbound(Flight* pFlight, char* destC, Date* pDate)
{
	pFlight->originCountry = home;
	pFlight->destCountry = destC;

	pFlight->date = pDate;
	pFlight->flightTicketArr = NULL;
	pFlight->countFlightTickets = 0;
	initSeatsMat(pFlight);
	return 1;
}

int initFlightInbound(Flight* pFlight, char* originC, Date* pDate)
{
	pFlight->destCountry = home;
	pFlight->originCountry = originC;

	pFlight->date = pDate;
	pFlight->flightTicketArr = NULL;
	pFlight->countFlightTickets = 0;
	initSeatsMat(pFlight);
	return 1;
}

void initSeatsMat(Flight* pFlight)
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			pFlight->seats[i][j] = False;
}

int addTicket(Flight* pFlight, Traveler* pTraveler, char* msg)
{
	pFlight->flightTicketArr = (FlightTicket*)realloc(pFlight->flightTicketArr, (pFlight->countFlightTickets + 1) * sizeof(FlightTicket));
	if (!pFlight->flightTicketArr)
		return 0;

	printf("\nFor %s flight:\n", msg);
	displayAvailableSeats(pFlight);
	if (!initTicket(&pFlight->flightTicketArr[pFlight->countFlightTickets], pFlight->seats, pFlight->countFlightTickets, pTraveler))
	{
		printf("There is no place in the flight, ticket wasn't added.\n");
		free(&pFlight->flightTicketArr[pFlight->countFlightTickets]);
		return 0;
	}

	pFlight->countFlightTickets++;
	return 1;
}

void displayAvailableSeats(Flight* pFlight)
{
	printf("Available seats:\n");
	BOOL found = False;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (pFlight->seats[i][j] == False)
			{

				printf("%02d%c   ", i + 1, 'A' + j);
				found = True;
			}
			else
				printf("      ");
		}
		printf("\n");
	}
	if (!found)
		printf("No available seats\n");
}

void freeFlight(Flight* pFlight)
{
	free(pFlight->flightTicketArr);
}

void printFlight(Flight* pFlight, char* msg)
{
	printf("--------------------------------------------------\n");
	printf("             %s FLIGHT INFORMATION             \n", msg);
	printf("--------------------------------------------------\n");
	printf("Origin Country: %s\n", pFlight->originCountry);
	printf("Destination Country: %s\n", pFlight->destCountry);
	printf("Date: %02d/%02d/%d\n", pFlight->date->day, pFlight->date->month, pFlight->date->year);
	//printf("--------------------------------------------------\n");
}

void printFlightTicketArr(FlightTicket* flightTicketArr, int count)
{
	printf("--------------------------------------------------\n");
	printf("              FLIGHT TICKET INFORMATION           \n");
	printf("--------------------------------------------------\n");

	if (count == 0)
	{
		printf("No flight tickets available.\n");
	}
	else
	{
		printf("Flight Tickets:\n");
		for (int i = 0; i < count; i++) {
			printf("Ticket %d:\n", i + 1);
			printFlightTicket(&flightTicketArr[i]);
			printf("------------------------------------\n");
		}
	}
}

int saveFlightToTextFile(const Flight* pFlight, FILE* fp)
{
	saveDateToTextFile(pFlight->date, fp);

	fprintf(fp, "%d\n", pFlight->countFlightTickets);
	for (int i = 0; i < pFlight->countFlightTickets; i++)
	{
		saveFlightTicketToTextFile(&pFlight->flightTicketArr[i], fp);
	}

	return 1;
}

int loadFlightOutboundFromTextFile(Flight* pFlight, char* destC, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->originCountry = home;
	pFlight->destCountry = destC;

	loadRestOfFlightTextFile(pFlight, travelerArr, countTraveler, fp);
	
	return 1;
}

int loadFlightInboundFromTextFile(Flight* pFlight, char* originC, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->originCountry = originC;
	pFlight->destCountry = home;

	loadRestOfFlightTextFile(pFlight, travelerArr, countTraveler, fp);

	return 1;
}

int loadRestOfFlightTextFile(Flight* pFlight, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->date = (Date*)calloc(1, sizeof(Date));
	if (!pFlight->date)
		return 0;
	loadDateFromTextFile(pFlight->date, fp);

	fscanf(fp, "%d\n", &pFlight->countFlightTickets);
	pFlight->flightTicketArr = (FlightTicket*)calloc(pFlight->countFlightTickets, sizeof(FlightTicket));
	if (!pFlight->flightTicketArr)
		return 0;

	initSeatsMat(pFlight);
	for (int i = 0; i < pFlight->countFlightTickets; i++)
		loadFlightTicketFromTextFile(&pFlight->flightTicketArr[i], travelerArr, countTraveler, pFlight->seats, fp);

	return 1;
}

int saveFlightToBinaryFile(const Flight* pFlight, FILE* fp)
{
	if (!saveDateToBinaryFile(pFlight->date, fp))
		return 0;
	if (!writeIntToFile(pFlight->countFlightTickets, fp, "Error write flight ticket count\n"))
		return 0;
	for (int i = 0; i < pFlight->countFlightTickets; i++)
	{
		if (!saveFlightTicketToBinaryFile(&pFlight->flightTicketArr[i], fp))
			return 0;
	}
	return 1;
}

int loadFlightOutboundFromBinaryFile(Flight* pFlight, char* destC, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->originCountry = home;
	pFlight->destCountry = destC;

	loadRestOfFlightBinaryFile(pFlight, travelerArr, countTraveler, fp);
	return 1;
}

int loadFlightinboundFromBinaryFile(Flight* pFlight, char* originC, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->originCountry = originC;
	pFlight->destCountry = home;

	loadRestOfFlightBinaryFile(pFlight, travelerArr, countTraveler, fp);
	return 1;
}

int loadRestOfFlightBinaryFile(Flight* pFlight, Traveler** travelerArr, int countTraveler, FILE* fp)
{
	pFlight->date = (Date*)calloc(1, sizeof(Date));
	if (!pFlight->date)
		return 0;
	if (!loadDateFromBinaryFile(pFlight->date, fp))
	{
		free(pFlight->date);
		return 0;
	}
	if (!readIntFromFile(&pFlight->countFlightTickets, fp, "Error reading flight ticket count\n"))
	{
		free(pFlight->date);
		return 0;
	}

	pFlight->flightTicketArr = (FlightTicket*)calloc(pFlight->countFlightTickets, sizeof(FlightTicket));
	if (!pFlight->flightTicketArr)
	{
		free(pFlight->date);
		return 0;
	}
	initSeatsMat(pFlight);
	for (int i = 0; i < pFlight->countFlightTickets; i++)
	{
		if (!loadFlightTicketFromBinaryFile(&pFlight->flightTicketArr[i], travelerArr, countTraveler, pFlight->seats, fp))
		{
			free(pFlight->flightTicketArr);
			free(pFlight->date);
			return 0;
		}
	}

	return 1;
}