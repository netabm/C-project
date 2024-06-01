#ifndef _INSURANCE_
#define _INSURANCE_

#include "General.h"

#define BASIC 300
#define EXTREME 600

typedef unsigned char BYTE;

typedef enum {ePhone, eLuggage, eHealth, eNofOpt} additionOptions;
static const char* additionStr[eNofOpt] = { "Phone", "Luggage", "Health" };
static const int additionsPrice[eNofOpt] = {30, 50, 100};

typedef struct
{
	BOOL isExtreme;
	BOOL additions[eNofOpt];
	int finalPrice;
}Insurance;


void initInsurance(Insurance* pIns);
int getInsuranceType();
void addAdditions(Insurance* pIns);
void setFinalPrice(Insurance* pIns);
void upgradeToExtreme(Insurance* pIns);
void printInsurance(const Insurance* pIns);
int saveInsuranceToTextFile(const Insurance* pIns, FILE* fp);
int loadInsuranceFromTextFile(Insurance* pIns, FILE* fp);
int saveInsuranceToBinaryFileCompressed(const Insurance* pInsurance, FILE* fp);
int loadInsuranceFromBinaryFileCompressed(Insurance* pInsurance, FILE* fp);

#endif

