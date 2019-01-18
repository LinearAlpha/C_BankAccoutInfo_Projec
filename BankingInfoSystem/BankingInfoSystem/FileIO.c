/******************************************************************************
* Project: Final_Banking System
* File: fileIO.c
* Authors: Yuhuan Ma, Minpyo Kim
* Date: 11/25/2018
* Description: The fileIO.c file has several functions that receive the
*			   information from main.c, and then stores it in
*			   "Account[number].txt" in the accounts folder.
******************************************************************************/
//to avoid Visual Studio memory overflow warning
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"finalProject.h"
//defines directory and file name as FILENAME, for ease of use
#define FILENAME "accounts\\account%d.txt"

accountInfo fileRead(int* counter)
{
	accountInfo* acct = (accountInfo*)malloc(1 * sizeof(accountInfo));
	//creating file pointer name as fh
	double temDeWit;
	char temName[1000];
	FILE* fh;
	//read files until it returns NULL
	do
	{
		char fileNameWNum[50];
		sprintf(fileNameWNum, FILENAME, (*counter + 1));
		//opens "accounts.txt"
		fh = fopen(fileNameWNum, "r");
		if (fh != NULL)
		{
			do
			{
				int i = 0;
				acct[*counter].activityCount = 0;
				fscanf(fh, "%99[^\n]s", temName);
				strcpy(acct[*counter].name, temName);
				fscanf(fh, "%d%*c%d%*c%d", &acct[*counter].month,
					&acct[*counter].day, &acct[*count].year);
				fscanf(fh, "%lf", &acct[*count].interest);
				//acct[*count].activity = malloc(1 * sizeof(double));
				while (fscanf(fh, "%lf", &temDeWit) != EOF)
				{
					acct[*count].activity = realloc(acct[*count].activity,
						sizeof(double) * (i + 2));
					acct[*count].activity[i] = temDeWit;
					acct[*count].activityCount = ++i;
				}
			} while (fscanf(fh, "%99[^\n]s", temName) != EOF);
			//closes "account[number].txt"
			fclose(fh);
			(*count)++;
			acct = (accountInfo*)realloc(acct, sizeof(accountInfo) * (*count + 2));
		}
	} while (fh != NULL);
	return acct;
}














/******************************************************************************
* Function: fileWrite
* Description: This function receives the account's information from the
*			   an other function, and created file to save account information
*Input: accountInfo - acct[] is the Student array of student's informations
*		int - count is the amount of the accounts are in the system
******************************************************************************/
void fileWrite(accountInfo* acct, int count)
{
    //creates file pointer name as fh
	FILE* fh;
	//declaration statements
	char fileNameTmp[50];
	sprintf(fileNameTmp, FILENAME, count + 1);
	fh = fopen(fileNameTmp, "w");
	if (fh == NULL)
	{
		puts("Error: unable to open file");
	}
	else
	{
		acct = (accountInfo*)realloc(acct, sizeof(accountInfo) * (count + 2));
		acct[count].activity = (double*)malloc(1 * sizeof(double));
		acct[count].activityCount = 0;
		fprintf(fh, "%s\n", acct[count].name);
		fprintf(fh, "%d/%d/%d\n", acct[count].month, acct[count].day,
          acct[count].year);
		fprintf(fh, "%lf\n", acct[count].interest);
		//closes "account[number].txt"
		fclose(fh);
	}
	return acct;
}

/******************************************************************************
* Function: fileWriteActivity
* Description: This function receives transaction information, and appends the
*			   transaction information, to the corresponding account file.
*Input: accountInfo - acct[] is an array of account information
*       int - account refers to the number of the account that we wish to
*                     write to.
******************************************************************************/
void fileWriteActivity(accountInfo* acct, int account)
{
    //creates file pointer name as fh
    FILE* fh;
    //declaration statements
	char fileNameTmp[30];
	sprintf(fileNameTmp, FILENAME, account + 1);
	fh = fopen(fileNameTmp, "a");
	if (fh == NULL)
	{
		printf("%s %s\n\n\n", "Error: unable to create account folder.",
         "Please create accounts folder, within the program folder.");
	}
	else
	{
		int i = acct[account].activityCount - 1;
		//if the user is withdrawing, "-" will be prepended
		//to the respective line
		fprintf(fh, "%+.2lf\n", acct[account].activity[i]);
		//closes "account[number].txt"
		fclose(fh);
		acct[account].activity = (double*)realloc(acct[account].activity,
            sizeof(double) * (i + 2));
	}
}

