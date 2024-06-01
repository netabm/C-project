#ifndef _AGENCYFILE_
#define _AGENCYFILE_

#include "FileHelper.h"
#include "General.h"
#include "TravelAgency.h"

int saveAgencyToTextFile(TravelAgency* pAgency, const char* fileName);
int loadAgencyFromTextFile(TravelAgency* pAgency, const char* fileName);
int saveAgencyToBinaryFile(TravelAgency* pAgency, const char* fileName);
int loadAgencyFromBinaryFile(TravelAgency* pAgency, const char* fileName);

#endif