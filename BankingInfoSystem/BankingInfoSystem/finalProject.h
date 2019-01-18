/******************************************************************************
 * Project: Final_Banking System
 * File: finalProject.h
 * Authors: Yuhuan Ma, Minpyo Kim
 * Date: 11/25/2018
 * Description: The finalProject.h contains the accountInfo struct and the
 *              function prototypes that main file can access and use them
******************************************************************************/
#ifndef FINALPROJECT_H_INCLUDED
#define FINALPROJECT_H_INCLUDED
#define NAME_CHARACTER_LIMIT  10000


//struct that maintains data detailing to an increasing list of bank users
typedef struct accountInfo
{
    char name[NAME_CHARACTER_LIMIT];
    int year,
        month,
        day;
	double interest;
    int activityCount;
	double* activity;
} accountInfo;

accountInfo fileRead(int* counter);


void fileWrite(accountInfo* acct, int count);
void fileWriteActivity(accountInfo* acct, int accountChoice);

#endif // FINALPROJECT_H_INCLUDEDG
