#include <stdio.h>
#include "Insurance.h"
#include "FileHelper.h"

void initInsurance(Insurance* pIns)
{
	if (getInsuranceType() == 0)
		pIns->isExtreme = 0;
	else
		pIns->isExtreme = 1;
	addAdditions(pIns);
	setFinalPrice(pIns);
}

int getInsuranceType()
{
	int opt;
	do
	{
		printf("For basic insurance enter 0, for extreme insurance enter 1\n");
		scanf("%d", &opt);
	} while (opt != 0 && opt != 1);
	return opt;
}

void addAdditions(Insurance* pIns)
{
	int opt;
	for (int i = 0; i < eNofOpt; i++)
	{
		printf("Are you interested in %s insurance that costs %d$? (1 for Yes, 0 for No): ",
			additionStr[i], additionsPrice[i]);
		scanf("%d", &opt);
		printf("\n");
		while (opt != 0 && opt != 1)
		{
			printf("Please enter 1 or 0\n");
			scanf("%d", &opt);
		}
		if (opt == 1)
			pIns->additions[i] = True;
	}
}

void setFinalPrice(Insurance* pIns)
{
	int sumPrice = 0;
	if (pIns->isExtreme == True)
		sumPrice += EXTREME;
	else
		sumPrice += BASIC;
	for (int i = 0; i < eNofOpt; i++)
	{
		if (pIns->additions[i] == True)
			sumPrice += additionsPrice[i];
	}
	pIns->finalPrice = sumPrice;
}

void upgradeToExtreme(Insurance* pIns)
{
	pIns->isExtreme = True;
	pIns->finalPrice += EXTREME - BASIC;
}

void printInsurance(const Insurance* pIns)
{
	if (pIns->isExtreme == True)
		printf("Insurance type is extreme\n");
	else
		printf("Insurance type is basic\n");
	for (int i = 0; i < eNofOpt; i++)
	{
		if (pIns->additions[i] == True)
			printf("Additional %s insurance\n", additionStr[i]);
	}
	printf("Final price of the insurance is: %d$\n", pIns->finalPrice);
}

int saveInsuranceToTextFile(const Insurance* pIns, FILE* fp)
{
	fprintf(fp, "%d\n", pIns->isExtreme);
	for (int i = 0; i < eNofOpt; i++)
	{
		fprintf(fp, "%d\n", pIns->additions[i]);
	}
	fprintf(fp, "%d\n", pIns->finalPrice);
	return 1;
}

int loadInsuranceFromTextFile(Insurance* pIns, FILE* fp)
{
	fscanf(fp, "%d\n", &pIns->isExtreme);
	for (int i = 0; i < eNofOpt; i++)
	{
		fscanf(fp, "%d\n", &pIns->additions[i]);
	}
	fscanf(fp, "%d\n", &pIns->finalPrice);
	return 1;
}

int saveInsuranceToBinaryFileCompressed(const Insurance* pInsurance, FILE* fp)
{
	BYTE data[3] = { 0 };

	// Encode isExtreme (bit 7)
	data[0] |= (pInsurance->isExtreme ? 1 : 0) << 7;

	// Encode additions (bits 0 to 2)
	for (int i = 0; i < eNofOpt; i++) {
		if (pInsurance->additions[i]) {
			data[0] |= 1 << i;
		}
	}

	// Encode finalPrice (bits 8 to 22)
	data[1] |= (pInsurance->finalPrice >> 7) & 0xFF;   // Last 8 bits of finalPrice
	data[2] |= (pInsurance->finalPrice >> 8) & 0x03; // First 2 bits from finalPrice


	// Write compressed data to file
	if (fwrite(data, sizeof(BYTE), 3, fp) != 3) {
		fclose(fp);
		return 0;
	}

	return 1;
}

int loadInsuranceFromBinaryFileCompressed(Insurance* pInsurance, FILE* fp)
{
	BYTE data[3];
	if (fread(data, sizeof(BYTE), 3, fp) != 3) {
		fclose(fp);
		return 0;
	}

	// Decode isExtreme (bit 7)
	pInsurance->isExtreme = (data[0] >> 7) & 0x01;

	// Decode additions (bits 0 to 2)
	for (int i = 0; i < eNofOpt; i++) {
		pInsurance->additions[i] = (data[0] >> i) & 0x01;
	}

	// Decode finalPrice (bits 8 to 17)
	pInsurance->finalPrice = ((data[1] & 0xFF) << 7) | ((data[2] & 0x03) << 8);
	
	return 1;
}