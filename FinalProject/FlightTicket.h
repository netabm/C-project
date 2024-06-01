#ifndef _FLIGHTTICKET_
#define _FLIGHTTICKET_

#include "Traveler.h"
#define SIZE 3
#define ROWS 10
#define COLS 6

typedef enum {eEconomy, eBusiness, eFirstClass, eNofTypes} classType;
static const char* classStr[eNofTypes] = { "Economy", "Business", "First class" };
static const int classPrice[eNofTypes] = { 300, 700, 1200 };

typedef struct
{
	char* name;
	int id;
	char seat[SIZE + 1];
	classType flightClass;
	int price;
}FlightTicket;

int initTicket(FlightTicket* pFTicket, BOOL(*seatsMat)[COLS], int countFlightTicket, Traveler* pTraveler);
int chooseSeat(FlightTicket* pFTicket, BOOL(*seatsMat)[COLS], int countFlightTicket);
int checkSeat(char* seat, BOOL(*seatsMat)[COLS]);
void chooseClass(FlightTicket* pFTicket);
void printFlightTicket(FlightTicket* pFTicket);
int saveFlightTicketToTextFile(const FlightTicket* pFTicket, FILE* fp);
int loadFlightTicketFromTextFile(FlightTicket* pFTicket, Traveler** travelerArr, int countTraveler, BOOL(*seatsMat)[COLS], FILE* fp);
int saveFlightTicketToBinaryFile(const FlightTicket* pFTicket, FILE* fp);
int loadFlightTicketFromBinaryFile(FlightTicket* pFTicket, Traveler** travelerArr, int countTraveler, BOOL(*seatsMat)[COLS], FILE* fp);

#endif