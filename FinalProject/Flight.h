#ifndef _FLIGHT_
#define _FLIGHT_

#include "FlightTicket.h"
#include "Date.h"

static char* home = "Israel";

typedef struct
{
	char* originCountry;
	char* destCountry;
	Date* date;
	FlightTicket* flightTicketArr;
	int countFlightTickets;
	BOOL seats[ROWS][COLS];
}Flight;

int initFlightOutbound(Flight* pFlight, char* destC, Date* pDate);
int initFlightInbound(Flight* pFlight, char* originC, Date* pDate);
void initSeatsMat(Flight* pFlight);
int addTicket(Flight* pFlight, Traveler* pTraveler, char* msg);
void displayAvailableSeats(Flight* pFlight);
void freeFlight(Flight* pFlight);
void printFlight(Flight* pFlight, char* msg);
void printFlightTicketArr(FlightTicket* flightTicketArr, int count);
int saveFlightToTextFile(const Flight* pFlight, FILE* fp);
int loadFlightOutboundFromTextFile(Flight* pFlight, char* destC, Traveler** travelerArr, int countTraveler, FILE* fp);
int loadFlightInboundFromTextFile(Flight* pFlight, char* originC, Traveler** travelerArr, int countTraveler, FILE* fp);
int loadRestOfFlightTextFile(Flight* pFlight, Traveler** travelerArr, int countTraveler, FILE* fp);
int saveFlightToBinaryFile(const Flight* pFlight, FILE* fp);
int loadFlightOutboundFromBinaryFile(Flight* pFlight, char* destC, Traveler** travelerArr, int countTraveler, FILE* fp);
int loadFlightinboundFromBinaryFile(Flight* pFlight, char* originC, Traveler** travelerArr, int countTraveler, FILE* fp);
int loadRestOfFlightBinaryFile(Flight* pFlight, Traveler** travelerArr, int countTraveler, FILE* fp);

#endif
