#ifndef _DATE_
#define _DATE_

#include "General.h"
#define MINYEAR 1903 //First flight in history
#define MAXYEAR 2025

typedef struct
{
	int			hour;
	int			minute;
}Time;

typedef struct
{
	int			day;
	int			month;
	int			year;
	Time		theTime;
}Date;

void initDate(Date* pDate);
void initDateNoTime(Date* pDate);
int checkDate(char* date, Date* pDate);
int checkTime(char* date, Date* pDate);
int compareDates(const Date* pDate1, const Date* pDate2);
void printDate(const Date* pDate);
int saveDateToTextFile(const Date* pDate, FILE* fp);
int loadDateFromTextFile(Date* pDate, FILE* fp);
int saveDateToBinaryFile(const Date* pDate, FILE* fp);
int loadDateFromBinaryFile(Date* pDate, FILE* fp);

#endif
