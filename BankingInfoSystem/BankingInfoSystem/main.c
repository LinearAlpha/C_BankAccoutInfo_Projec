/******************************************************************************
 * Project: FinalBankingSystem
 * File: Main.c
 * Authors: Ryan Smit, Alexandre Soper
 * Date: 11/26/2018
 * Description: This is the main source file for the banking system console program
 * This program allows a user to add bank accounts, archive withdraw and deposit
 actions, store the account data into text files, and retrieve/edit them upon any
 instance of reloading the program.
******************************************************************************/
//to avoid Visual Studio memory overflow warning
#define _CRT_SECURE_NO_WARNINGS
//preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finalProject.h"
#define ACCOUNT_LIMIT 100
//function prototypes
void addAccount(accountInfo accArr[], int* count);
void addActivity(accountInfo* acct, int count, int option);
void viewAccounts(accountInfo* acct, int count);
int getInputInt(int lowerBound, int upperBound);
int monthEndDateCalculate(int month, int year);
void displayIntro(void);

/******************************************************************************
* Function: main
* Description: Controls the general flow of the rest of the program
******************************************************************************/
int main()
{
    //declaration statements
    int userInput = -1,
		count = 0;
	accountInfo* acct = fileRead(count);
    //reads the information from existing files,
    //and saves it into the acct array.
    //int count = fileRead(acct);
    displayIntro();
    //displays the menu, and gets user input, until they are done
    do
    {
        printf("0 - Exit\n");
        printf("1 - Add Account\n");
        printf("2 - View Existing Accounts\n");
        printf("3 - Deposit\n");
        printf("4 - Withdraw\n");
        printf("> ");
        userInput = getInputInt(0, 4);
        switch (userInput)
        {
        case 0:
            break;
        case 1:
            addAccount(acct, &count);
            break;
        case 2:
            viewAccounts(acct, count);
            break;
        case 3:
            addActivity(acct, count, userInput);
            break;
        case 4:
            addActivity(acct, count, userInput);
            break;
        default:
            puts("Invalid input.\n");
            while (getchar() != '\n');
        }
    }
    while (userInput != 0);
}

/******************************************************************************
* Function: addAccount
* Description: This function receives user-input data, to add to the an array
*              of accounts
*Input: accountInfo - accArr[] is an array of account details, for several users
		int* count - a pointer to a variable holding the number of accounts
		             already in the array
******************************************************************************/
void addAccount(accountInfo accArr[], int* count)
{
    //declaration statements to represent the lower and upper limits for each
    //parameter in a date
    const int YEAR_LOWER_BOUND = 1900,
              YEAR_UPPER_BOUND = 2099,
              MONTH_LOWER_BOUND = 1,
              MONTH_UPPER_BOUND = 12,
              DAY_LOWER_BOUND = 1;
    int dayUpperBound;
    puts("");
    //gets the account name
    while (getchar() != '\n');
    printf("\nPlease enter the name of the client:\n> ");
    scanf("%99[^\n]s", accArr[*count].name);
    //gets the client's birthday
    printf("\nPlease enter the client's birth year (0 to exit):\n> ");
    accArr[*count].year = getInputInt(YEAR_LOWER_BOUND, YEAR_UPPER_BOUND);
    if (accArr[*count].year == 0)
        return;
    printf("\nPlease enter the client's birth month (number) (0 to exit):\n> ");
    accArr[*count].month = getInputInt(MONTH_LOWER_BOUND, MONTH_UPPER_BOUND);
    if (accArr[*count].month == 0)
        return;
    dayUpperBound = monthEndDateCalculate(accArr[*count].month,
                                          accArr[*count].year);
    printf("\nPlease enter the client's birth date (0 to exit):\n> ");
    accArr[*count].day = getInputInt(DAY_LOWER_BOUND, dayUpperBound);
    if (accArr[*count].day == 0)
        return;
    //gets the account interest rate
    printf("\nPlease enter the interest rate for this account (decimal)(0 to exit):\n>");
    while ((scanf("%lf", &accArr[*count].interest) == 0) ||
           accArr[*count].interest < 0)
    {
        printf("Error: Please enter a value greater than 0.00.\n> ");
        while (getchar() != '\n');
    }
    if (accArr[*count].interest == 0)
    {
        puts("");
        return;
    }
    //saves what has been entered to a new file
	accArr = fileWrite(accArr, *count);
    //increments number of accounts
    (*count)++;
    puts("");
}

/******************************************************************************
* Function: viewAccounts
* Description: This function displays all available accounts and lets the user
*              view an individual account's information, more verbosely
*Input: accountInfo* acct - an array of accounts
		int count - the number of available accounts
******************************************************************************/
void viewAccounts(accountInfo* acct, int count)
{
    //declare variable
    int userInput = 0;
    double balance = 0;
    //display accounts and allow user to keep viewing individual accounts
    //until they are done
    puts("");
    if (count == 0)
        puts("No accounts have been entered!\n");
    else
    //prints every account that currently exists
    {
        printf("Accounts:\n");
        for (int i = 0; i < count; i++)
            printf("\tAccount %d: %s\n", (i + 1), acct[i].name);
        puts("");
        do
        {
            //Allows user to choose account
            printf("%s", "Enter an account number to view its information,"
                   " or 0 to quit:\n");
            printf("> ");
            userInput = getInputInt(0, count);
            if (userInput != 0)
            {
                printf("\nAccount Credentials:\n");
                printf("\t%s\n", acct[userInput - 1].name);
                //displays account creation date
                printf("\tClient Birthday: %d/%d/%d\n", acct[userInput - 1].
                       month, acct[userInput - 1].day, acct[userInput - 1].year);
                //displays interest
                printf("\tInterest: %.2lf%%\n", acct[userInput - 1].interest);
                //displays deposits and withdrawals
                if (acct[userInput - 1].activityCount != 0)
                {
                    puts("");
                    printf("Account history:\n");
                    for (int i = 0; i < acct[userInput - 1].activityCount; i++)
                    {
                        if (acct[userInput - 1].activity[i] > 0)
                            printf("\tDeposit of $%.2lf\n", acct[userInput - 1].
                                activity[i]);
                        else
                            printf("\tWithdrawal of $%.2lf\n", (acct[userInput - 1].
                                activity[i] * -1));
                        balance += acct[userInput - 1].activity[i];
                    }
                }
                printf("Total account balance: $%.2f.\n", balance);
            }
        }
        while (userInput != 0);
        puts("");
    }
}

/******************************************************************************
* Function: addActivity
* Description: This function is for depositing and withdrawing to an account.
*Input: accountInfo* acct - an array of accounts
		int count - the number of accounts in the array
		int option - the indicator for depositing or withdrawing
******************************************************************************/
void addActivity(accountInfo* acct, int count, int option)
{
    //declare variables
    int accountNum;
    double transaction;
    int check;
    char tmp[10];
    if (count == 0)
        puts("No accounts have been entered!\n");
    else
    {
        //prints a list of accounts, and prompts the user to choose one
        puts("\nAccounts:");
        for (int i = 0; i < count; i++)
            printf("%d: %s\n", (i + 1), acct[i].name);
        puts("");
        printf("Please enter the account number (0 if you would like to cancel):\n> ");
        accountNum = getInputInt(0, count);
        if (accountNum != 0)
        {
            //make a deposit or withdrawal message
            option == 3 ? strcpy(tmp, "deposit"): strcpy(tmp, "withdraw");
            printf("%s %s %s", "Please enter how much you would like to", tmp,
                "(0 if you would like to choose another user):\n$");
            //validate input
            do
            {
                check = scanf("%lf", &transaction);
                while (getchar() != '\n');
                if (check == 0)
                {
                    puts("Please enter valid input");
                    printf("> ");
                }
            } while (check == 0);
            //add transaction amount to account history
            int arrIndex = accountNum - 1;
			option == 3? transaction: (transaction *= -1);
            acct[arrIndex].activity[acct[arrIndex].activityCount] = transaction;
            acct[arrIndex].activityCount++;
            //save transaction to a file
            fileWriteActivity(acct, arrIndex);
        }
    }
    puts("");
}

/******************************************************************************
Function: getInputInt()
Description: A general purpose function that validates an integer input, and
             will continue prompting the user to retry until a valid one
             is given.
Input:
    * int lowerBound: An int that represents the lower limit of a prospective
                      integer input. (0 is accounted for, first, as an exit
                      condition)
    * int upperBound: An int that represents the upper limit of a prospective
                      integer input.
Output:
    * int userInput: An int that represents a user input, now fully validated
                     for higher procedural usage.
******************************************************************************/
int getInputInt(int lowerBound, int upperBound)
{
    //declaration statements
    int userInput;
    int validInput = 0;
    //loop receives user input, then checks if it is a valid integer
    //afterwards tests for 0, and whether it fits lower and upper bounds
    do
    {
        validInput = scanf("%d", &userInput);
        if (validInput == 0)
        {
            //clears input pipe, only if invalid input
            while (getchar() != '\n');
            printf("\nError: You did not enter a valid integer. "
                   "Please try again\n> ");
        }
        else
        {
            //returns 0, if 0 is input
            if (userInput == 0)
                return userInput;
            if (userInput < lowerBound || userInput > upperBound)
            {
                printf("Error: Please enter an integer between %d and %d ",
                    lowerBound, upperBound);
                printf("(You entered %d)\n>", userInput);
            }
        }
    }
    while (validInput == 0 || (userInput < lowerBound || userInput > upperBound));
    return userInput;
}

/******************************************************************************
Function: monthEndDateCalculate
Description: Calculates the amount of days, or end date of a given month
Input:
    * int month: An int that represents a desired month to test for
    * int year: An int that represents a desired year, in case of a leap year
                for February
Output:
    * int dayCount: An int that represents the amount of days calculated for
                    a desired month
******************************************************************************/
int monthEndDateCalculate(int month, int year)
{
    //const declaration statements for various daycount factors
    const int FEBRUARY = 2,
              LEAP = 4,
              FEB_DAYCOUNT = 29,
              LEAP_DAYCOUNT = 28,
              ODD_DAYCOUNT = 31,
              EVEN_DAYCOUNT = 30,
              OFFSET_REMOVE = 7;
    //declaration of return variable
    int dayCount;
    //tests for February, alone
    if (month == FEBRUARY)
    {
        //if the year is divisible by 4, then it outputs a leap day count
        if ((year % LEAP) == 0)
            dayCount = FEB_DAYCOUNT;
        else
            dayCount = LEAP_DAYCOUNT;
    }
    //Removes day offset for months after July
    if (month > OFFSET_REMOVE)
        month -= OFFSET_REMOVE;
    //April, June, September, November
    if ((month % 2) == 0)
        dayCount = EVEN_DAYCOUNT;
    //January, March, May, July, August, October, December
    else
        dayCount = ODD_DAYCOUNT;
    return dayCount;
}

/******************************************************************************
Function: displayIntro()
Description: Prints a block statement, intended for use at the launch of the
             program, and only then. Provides a warm welcome, and brief
             instructions
******************************************************************************/
void displayIntro(void)
{
    printf("\t+----------------------------------------------------------------+\n");
    printf("\t|                                                                |\n");
    printf("\t|                      BANK TELLING PROGRAM                      |\n");
    printf("\t|                            v. 1.5                              |\n");
    printf("\t|              Written by Minpyo Kim, Ryan Smit,                 |\n");
    printf("\t|               Yuhuan Ma, and Alexandre Soper                   |\n");
    printf("\t|                          (c. 2018)                             |\n");
    printf("\t|                                                                |\n");
    printf("\t|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("\t|                                                                |\n");
    printf("\t|          ~ Welcome to the bank telling program! ~              |\n");
    printf("\t|           This console program will allow you to:              |\n");
    printf("\t|         * Add bank accounts for clients                        |\n");
    printf("\t|         * Record transactions made by these clients            |\n");
    printf("\t|         * View their account information                       |\n");
    printf("\t|         * And view their account history                       |\n");
    printf("\t|                                                                |\n");
    printf("\t|   All user information is saved in the \"Accounts\" directory    |\n");
    //DO NOT REMOVE THOSE TWO SPACES BEFORE THE NEWLINE AND PIPELINE CHARACTERS,
    //AT THE END OF THAT LAST LINE. THOSE ARE INTENTIONALLY KEPT IN!
    //DO NOT REMOVE THE BACKSLASHES EITHER (this is a comment reminder, to self)
    printf("\t|          Enter 0 at any time to go back one menu               |\n");
    printf("\t|         Or 0 on the main menu to exit the program              |\n");
    printf("\t|    We hope that you have a pleasant time with our program!     |\n");
    printf("\t|                                                                |\n");
    printf("\t+----------------------------------------------------------------+\n\n");
}
