#include <stdio.h>
#include <stdlib.h>
#include "TravelAgency.h"
#include "TravelAgencyFile.h"

#define EXIT 0
#define AGENCY_TFILE_NAME "travel_agency.txt"
#define AGENCY_BFILE_NAME "travel_agency.bin"

int displayMenu();
void loadSystem(TravelAgency* agency);

void printPicture1Main()
{
	printf("   __  _\n");
	printf("  /  \\` |\n");
	printf("  \\__/`!\n");
	printf("  / ,' `-.__________________\n");
	printf(" '-'\\_____                LI`-.\n");
	printf("    <____()-=O=O=O=O=O=[]====--)\n");
	printf("      `.___ ,-----,_______...-'\n");
	printf("           /    .'\n");
	printf("          /   .'\n");
	printf("         /  .'\n");
	printf("        `-'\n");
}

void printPicture2Main()
{
	printf("          |\n");
	printf("        \\ _ /\n");
	printf("      -= (_) =-\n");
	printf("        /   \\         _\\/_\n");
	printf("          |           //o\\  _\\/_\n");
	printf("   _____ _ __ __ ____ _ | __/o\\\\ _\n");
	printf(" =-=-_-__=_-= _=_=-=_,-'|\"'\"\"-|-,_\n");
	printf("  =- _=-=- -_=-=_,-\"          |\n");
	printf(" =- =- -=.--\"\n");
}

int main()
{
	printPicture1Main();
	printPicture2Main();

	TravelAgency agency;
	loadSystem(&agency);

	int option;
	int stop = 0;
	do
	{
		printf("\n\n%s ", agency.name);
		option = displayMenu();
		switch (option)
		{
		case 1:
			if (!addCountry(&agency))
				printf("Failed adding country to agency\n");
			break;

		case 2:
			if (!addTravelerToAgency(&agency))
				printf("Failed adding traveler to agency\n");
			break;

		case 3:
			if (!addAttractionToAgency(&agency))
				printf("Failed adding attraction to agency\n");
			break;

		case 4:
			if (!createTrip(&agency))
				printf("Failed creating trip\n");
			break;

		case 5:
			if (!addAttractionToExistingTrip(&agency))
				printf("Failed adding attraction to an existing trip\n");
			break;

		case 6:
			if (!addTravelerToExistingTrip(&agency))
				printf("Failed adding traveler to an existing trip\n");
			break;

		case 7:
			if (!deleteTravelerFromTrip(&agency))
				printf("Failed deleting traveler from trip\n");
			break;

		case 8:
			if (!deleteAttractionFromTrip(&agency))
				printf("Failed deleting attraction from trip\n");
			break;

		case 9:
			sortTravelerArr(&agency);
			break;

		case 10:
			findTraveler(&agency);
			break;

		case 11:
			if (!joinATrip(&agency))
				printf("Failed adding traveler to trip\n");
			break;

		case 12:
			addReview(&agency);
			break;

		case 13:
			printReviewsArr(&agency);
			break;

		case 14:
			if (!selectTripToPrint(&agency))
				printf("Failed printing selected trip\n");
			break;

		case 15:
			printTravelAgency(&agency);
			break;

		case EXIT:
			printf("Thank you for choosing %s travel agency!\n", agency.name);
			stop = 1;
			break;

		default:
			printf("Wrong option please try again\n");
			break;
		}
	} while (!stop);

	saveAgencyToTextFile(&agency, AGENCY_TFILE_NAME);
	saveAgencyToBinaryFile(&agency, AGENCY_BFILE_NAME);

	return 1;
}

int displayMenu()
{
	int option;
	printf("Travel Agency Management System\n");
	printf("Please choose one of the following options\n");
	printf("--------------------------------\n");
	printf("1 - Add a country\n");
	printf("2 - Add a traveler to agency\n");
	printf("3 - Add an attraction to agency\n");
	printf("4 - Create a trip\n");
	printf("5 - Add attraction to trip\n");
	printf("6 - Add traveler to trip\n");
	printf("7 - Delete traveler from trip\n");
	printf("8 - Delete attraction from trip\n");
	printf("9 - Sort travelers\n");
	printf("10 - Search traveler\n");
	printf("11 - Join a trip in range of date\n");
	printf("12 - Add a review\n");
	printf("13 - Show all reviews\n");
	printf("14 - Print trip\n");
	printf("15 - Print Travel Agency\n");
	printf("0 - Exit\n");
	printf("--------------------------------\n");
	scanf("%d", &option);
	return option;
}

void loadSystem(TravelAgency* agency)
{
	int loadOpt;
	printf("To load system, press 0 for text file or 1 for binary file\n");
	scanf("%d", &loadOpt);
	while (loadOpt < 0 || loadOpt >> 1)
	{
		printf("Please choose 0 or 1\n");
		scanf("%d", &loadOpt);
	}
	if (loadOpt == 0)
	{
		if (!loadAgencyFromTextFile(agency, AGENCY_TFILE_NAME))
			initTravelAgency(agency);
	}

	else if (loadOpt == 1)
	{
		if (!loadAgencyFromBinaryFile(agency, AGENCY_BFILE_NAME))
			initTravelAgency(agency);
	}
}