#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Date.h"
#include "FileHelper.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV1 '/' //Special character for date format
#define SPECIAL_TAV2 ':' //Special character for time format

void initDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1; //Flag for input validation
	do {
		printf("Enter flight date dd%cmm%cyyyy. min year: %d max year: %d\t",
			SPECIAL_TAV1, SPECIAL_TAV1, MINYEAR, MAXYEAR);
		myGets(date, MAX_STR_LEN, stdin);
		ok = checkDate(date, pDate); //Check input
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
	do {
		printf("Enter time hh%cmm\t", SPECIAL_TAV2);
		myGets(date, MAX_STR_LEN, stdin);
		ok = checkTime(date, pDate); //Check input
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}

void initDateNoTime(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1; //Flag for input validation
	do {
		printf("Enter date dd%cmm%cyyyy. min year: %d max year: %d\t",
			SPECIAL_TAV1, SPECIAL_TAV1, MINYEAR, MAXYEAR);
		myGets(date, MAX_STR_LEN, stdin);
		ok = checkDate(date, pDate); //Check input
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}

int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 10) //Check if input length is valid
		return 0;
	if ((date[2] != SPECIAL_TAV1) || (date[5] != SPECIAL_TAV1)) //Check if input format is correct
		return 0;
	sscanf(date, "%d%*c%d%*c%d", &day, &month, &year);

	if (year < 1903 || year > 2025) //Check valid range
		return 0;

	if (day < 1 || month < 1 || month > 12) //Check valid range
		return 0;

	if (day > DAY_MONTHS[month - 1]) //Check valid range
		return 0;

	//Set day, month, year in Date structure
	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

int checkTime(char* date, Date* pDate)
{
	int hour, minute;
	if (strlen(date) != 5) //Check if input length is valid
		return 0;
	if (date[2] != SPECIAL_TAV2) //Check if input format is correct
		return 0;
	sscanf(date, "%d%*c%d", &hour, &minute);
	if (hour < 0 || hour > 23 || minute < 0 || minute > 59) //Check valid range
		return 0;

	//Set hour and minute in Time structure
	pDate->theTime.hour = hour;
	pDate->theTime.minute = minute;

	return 1;
}

int compareDates(const Date* pDate1, const Date* pDate2)
{
	if (pDate1->year < pDate2->year) //Compare years
		return -1;
	else if (pDate1->year > pDate2->year)
		return 1;
	
	if (pDate1->month < pDate2->month) //Compare months
		return -1;
	else if (pDate1->month > pDate2->month)
		return 1;

	if (pDate1->day < pDate2->day) //Compare days
		return -1;
	else if (pDate1->day > pDate2->day)
		return 1;

	return 0;
}

void printDate(const Date* pDate)
{
	printf("%02d/%02d/%d", pDate->day, pDate->month, pDate->year);
}

int saveDateToTextFile(const Date* pDate, FILE* fp)
{
	fprintf(fp, "%d\n", pDate->day);
	fprintf(fp, "%d\n", pDate->month);
	fprintf(fp, "%d\n", pDate->year);
	fprintf(fp, "%d\n", pDate->theTime.hour);
	fprintf(fp, "%d\n", pDate->theTime.minute);
	return 1;
}

int loadDateFromTextFile(Date* pDate, FILE* fp)
{
	fscanf(fp, "%d\n", &pDate->day);
	fscanf(fp, "%d\n", &pDate->month);
	fscanf(fp, "%d\n", &pDate->year);
	fscanf(fp, "%d\n", &pDate->theTime.hour);
	fscanf(fp, "%d\n", &pDate->theTime.minute);
	return 1;
}

int saveDateToBinaryFile(const Date* pDate, FILE* fp)
{
	if (fwrite(pDate, sizeof(Date), 1, fp) != 1)
	{
		printf("Error write date\n");
		return 0;
	}
	return 1;
}

int loadDateFromBinaryFile(Date* pDate, FILE* fp)
{
	if (fread(pDate, sizeof(Date), 1, fp) != 1)
	{
		printf("Error reading date\n");
		return 0;
	}
	return 1;
}
