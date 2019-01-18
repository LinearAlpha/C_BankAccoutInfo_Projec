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
#define NAME_CHARACTER_LIMIT  100


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

accountInfo* fileWrite(accountInfo* acct, int count);
void fileWriteActivity(accountInfo* acct, int accountChoice);
accountInfo* fileRead(int* i);
#endif // FINALPROJECT_H_INCLUDEDG
