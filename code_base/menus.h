/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 2
Lab:			A: 0830 - 1030
Description:	Contains function prototypes used in generating the different menus of the program
Last Modified:	9th September 2013

******/

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include<cstdlib>

using namespace std;

//function displays the options of the main menu
void displayMainMenu();

// function handles the selection from the user and calls the appropriate function
void actionSelect(struct student database[], char, int &, char[][13]);

// display the course validation menu conflicts, returns false boolean if no records exist in database
void displayCVinfo(struct student database[], int & enrolments);

// output error match in records
void outDiscrep(struct student database[], int[], int, int);

// manipulates the records according the the users input
void manageCourseValid(struct student database[], int &, char, int[], int, int);

// display the option menu and receive the selection for sort
void displayOptionMenu(struct student database[], int &);

// function handles the searching of database records based on the keys input by the user
// and stores them in the order based on the keys that are input by the user.
void sort(char * keys[][2], char *orderptr[], char * vlidptr[], struct student database[], int & enrolments);

// function displays the search results generated by the search keys
void outSearchResults(int[], int, struct student database[]);

// function populates an array of integers representing the indexes of the records
// that are currently stored in the database
void populateIndex(int stdIndex[], int enrolments);

// function handles the comparing of database based on the current key value that is
// passed on through the sort.
void delegateContent(int[], int &, struct student database[], bool&, int&, int &, bool &, bool &, void*&, void*&, int&, int&);

// function swaps the values of an integer array
void swapVals(int[], int);

// resets the boolean values to their default value
// this is usually completed after a match of a value
// and matched value has been found
void resetBools(int & , bool&, bool&, bool & , int &);

// function accepts two void pointers and converts them to the appropriate
// datatype based on a passed value
// function compares two values that are less than each other
int lessThan(void*valOne, void*valTwo, int);

// function accepts two void pointers and converts them to the appropriate
// datatype based on a passed value
// function compares two values that are greater than each others
int greaterThan(void*valOne, void*valTwo, int);

// gets the value of the sortKey identifier and returns an index number relevant to the chosen key
int getKeyValue(char *keys[][2], char * idnptr[], int ptrLen, int currentRow, int currentPos);

// takes a string from the user and calls on the appropriate functions to test whether the values
// are valid or not and calls another to store these indexes in a char array pointer
void validKey(char [], char *[][2], int, int, char *[], char*[]);

// function takes input from the user and if valid, stores these values in a character
// array pointer
void sortKeyPop(int, char[], char *[][2], int);

// function returns a boolean confirming whether or not a valid key sort value has been entered
// otherwise returns false to indicate an invalid key has been entered
bool testKeys(char *[][2], int, char*[], int, int);

// displays the search menu options and calls the appropriate functions based on the user's input
void displaySearchMenu(struct student database[], int & enrolments, char signals[][13]);

// function displays the options available when a series of search results have been returned and allows the user to move through the
// resulting record list
int toggleSearchMenu(struct student database[], int, int [], int &, char [], char[]);

// function edits the current record using the passed details to identify the record to edit within the database
// function iterates through all aspects of a student record and allows a user to edit each value. the user cannot enter a null value for
// any of the selections, nor can they enter the same value
void editCurrentRecord(struct student database[], int[], int, char[], char[]);

// function controls what section of the record to edit according to what iteration the calling function, editCurrentRecord is up to
void delegateEdit(char, int, struct student database[], int [], int, char [], char []);

// function handles the editing of the subject values for the amount of enrolled subjects for a particular record
void subjectChange(struct student database[], int [], int, char[], char[]);

// function handles the editing of each subject value for a particular subject record belonging to a student
int subjectToEdit(char, struct student database[], int[], int, int, int, char[], char[]);
