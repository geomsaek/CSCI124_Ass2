/*****

Name:			Matthew Saliba
Subject:		CSCI124
Student Num:	3284165
Title:			Assignment 2
Lab:			A: 0830 - 1030
Description:	Contains functions used in generating the different menus of the program
Last Modified:	9th September 2013

******/

#include "import.h"
#include "validate.h"
#include "menus.h"
#include "header.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include<cstdlib>

using namespace std;

//function displays the options of the main menu

void displayMainMenu() {
	cout << endl;
	cout << "/**************** MAIN MENU *********************/" << '\n' << endl;
	cout << "Welcome to the main menu. Please choose one of the following options to begin" << endl;
	cout << "q - Quit the program" << endl;
	cout << "i - Import data from text file" << endl;
	cout << "e - export data to text file" << endl;
	cout << "s - Display the search menu" <<  endl;
	cout << "c - Validate Course Information" << endl;
	cout << "o - Options. Order Current Records" << '\n' << endl;
	cout << "Please enter your selection: ";
	return;
}


// function handles the selection from the user and calls the appropriate function
void actionSelect(struct student database[], char selection, int & enrolments, char signals[][13]){
	
	char fileNone[] = { "none" };
	bool NoConflict = false;
	
	// switch statement handles valid choices
	switch(selection){
		case 'q':
			cout<< endl;
			cout << "--- You Quit the program! ---" << endl;
			cout<< endl;
		break;
		
		case 'i':
			// calls the getName function that retrieves the values from the import file
			enrolments = getName(database, fileNone, false, signals, enrolments);
			// calculate the GPA values after the import is complete
			calculateGPA(database, enrolments);
			
			// if there are enrolments, then output a prompt explaining this and how many records were uploaded
			if(enrolments > 0){
				displayMainMenu();
				cout << "--------- IMPORT COMPLETE ---------" << endl;
				cout << "The total number of records that were imported was: " << enrolments << endl;
				cout << endl;
			}else {
			
				// if there are no enrolments, either the file was empty or an error occurred
				// message is displayed to inform the user
				cout << "There were no records to upload in the file that was selected or there was an error in the import" << endl;
				cout << "Please ensure that the correct format was followed" << endl;
			}
			cout << '\n' << "Please specify action from the main menu you would like to make: ";
		break;
		
		case 's':
			// displays the search menu options
			displaySearchMenu(database, enrolments, signals);
		break;
		
		case 'e':
			// prompts the user of when data is exported
			cout << "--------- EXPORT DATA ---------" << endl;
			exportData(database, enrolments, signals);
			displayMainMenu();
			cout << "EXPORT COMPLETE..." << endl;
			cout << endl;
			cout << '\n' << "Please specify action from the main menu you would like to make: ";
		break;
		
		case 'c':
			displayCVinfo(database, enrolments);
			displayMainMenu();
		break;
		
		case 'o':
			displayOptionMenu(database, enrolments);
			displayMainMenu();
		break;
	}
	
	
	return;
}

// function stores the indexes of records that contain mismatched inconsistent values for the course name
// and the course number
void displayCVinfo(struct student database[], int & enrolments){
	
	char userVals[3] = { 'i', 'm' };
	int matches[1000] = { 0 };
	int matchCount = 0;
	char userInput;
	bool matchFound = false;
	bool initialFind = false;
	int initial = enrolments;
	
	// if there are no enrolments, then output a warning message and return back to the main menu
	if(enrolments == 0){
		cout << endl;
		cout << "------ WARNING: There are currently no records in the database!! Please upload some first!! ------" << endl;
		return;
	}
	
	// if there are stored records in the database, then loop through the enrolments and find instances, where the course name
	// and course number are inconsistent and store these in an array
	// let the laster sentinel know that there is a match of inconsistent records via a boolean value
	
	for(int i = 0; i < enrolments; i++){
		for(int k = i; k < enrolments; k++){
			if(strcmp(database[i].single.courseName, database[k].single.courseName) > 0 && database[i].single.courseNum == database[k].single.courseNum){
				matches[matchCount] = k;
				matchFound = true;
				matchCount = matchCount + 1;
			}
			else if(strcmp(database[i].single.courseName, database[k].single.courseName) == 0 && database[i].single.courseNum != database[k].single.courseNum){
				matches[matchCount] = k;
				matchFound = true;
				matchCount = matchCount + 1;
			}
		}
		// bool triggers a calling function to output and display the found records which have this inconsistency
		// sentinel also receives the user's response as to whether they want to edit the set the course name and number
		// for all the found records
		if(matchFound){
			outDiscrep(database, matches, matchCount, i);
			userInput = getUserInput(2, userVals);
			if(userInput == 'm'){
				initialFind = true;
			}
			
			// function takes the user's input and handles the response
			manageCourseValid(database, enrolments, userInput, matches, matchCount, i);
			
			// indicating variables are then reset for the next iteration
			matchFound = false;
			matchCount = 0;
		}
	}
	
	// if the user has selected to modify the records the corresponding message is output
	if(initialFind){
		cout << "----- You have successfully modified the database -----" << endl;
	}
	else {
		// otherwise a message is output explaining that no action has been taken, or that there are no conflicts in the database
		cout << endl;
		cout << "----- There were no/ or more conflicting records! -----" << endl;
		cout << endl;
	}
	
	return;
}


// function simply outputs the message of the conflicting course name and number records
void outDiscrep(struct student database[], int matches[], int matchCount, int currentRecord){
	
	cout << endl;
	cout << "The following records are conflicting with the current record: " << endl;
	cout << endl;
	cout << "Name: " << database[currentRecord].name << endl;
	cout << "Course Name: " << database[currentRecord].single.courseName << endl;
	cout << "Course Number: " << database[currentRecord].single.courseNum << endl;
	cout << endl;
	
	// loops through the array of marked records and outputs the details of these
	for(int i = 0; i < matchCount; i++){
		cout << "---- CONFLICT #" << i + 1 << " ----" << endl;
		outPartStruct(database, matches[i]);
		cout << endl;
	}
	cout << endl;
	cout << "Please specify the action you would like to make: (i)gnore, (m)erge content names: ";

}

// function outputs handles the response of the user as to what choice they would like to perform on the conflicting records
// and calls the appropriate function based on what is entered
void manageCourseValid(struct student database[], int & enrolments, char userInput, int matches[], int matchCount, int currentRecord){
	
	char courseName[100] = { '\0' };
	int courseNum = 0;
	char alphabetLower[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	char alphabetUpper[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	
	// switch statement handles the choice of the user, outputs the appropriate message and calls the
	// functions that are needed
	switch(userInput){
		case 'i':
			cout << "You have chosen to ignore the current conflict!" << endl;
			return;
		break;
		
		case 'm':
			 // case is indicative of instances where the user has selected to update the found records
			// case calls the appropriate functions to accept and validate the change course name and number
			
			 cout << endl;
			 cout << "Please enter the COURSE NUMBER that you would like to apply to all conflicting records: ";
			 courseNum = numberChange(0, 1000, 9999);
			 cin.clear();
			 cin.ignore(1000,'\n');
			 cout << "Please enter the COURSE NAME that you would like to apply to all conflicting records: ";
			 nameChange(database[currentRecord].single.courseName, false, alphabetUpper, alphabetLower, 100, 0, true, 7);
			 database[currentRecord].single.courseNum = courseNum;
			 
			 // function then displays a summary fo the records that were updated.
			 
			 cout << endl;
			 cout << "--- DISPLAYING THE UPDATED RECORD CHANGES ----" << endl;
			 cout << endl;
			 cout << "New Record Course Number: " << courseNum << endl;
			 cout << "New Record Course Name: " << database[currentRecord].single.courseName << endl;
			 cout << "Number of records updated: " << matchCount + 1 << endl;
			 cout << endl;
			 for(int i = 0;  i <= matchCount; i++){
			 	database[matches[i]].single.courseNum = courseNum;
			 	strcpy(database[matches[i]].single.courseName, database[currentRecord].single.courseName);
			 	cout << "---- RECORD #" << i + 1 << " ----" << endl;
			 	outPartStruct(database, i);
			 	cout << endl;
			 }
			return;
		break;
	}
	
	return;
}

// function displays the search key option menu and calls the appropriate functions to store the appropriate
// search keys
void displayOptionMenu(struct student database[], int & enrolments){

	char *keys[3][2];
	char userSelect[3][17] = { '\0' };
	int currentRow = 0;
	char validSelect[3][5] = { '\0' };
	char orderSelect[2][11] = { '\0' };
	
	int currentKeyCol = 0;
	
	strcpy(validSelect[0], "name");
	strcpy(validSelect[1], "id");
	strcpy(validSelect[2], "gpa");
	strcpy(orderSelect[0], "ascending");
	strcpy(orderSelect[1], "descending");
	
	char *vlidptr[3];
	vlidptr[0] = &validSelect[0][0];
	vlidptr[1] = &validSelect[1][0];
	vlidptr[2] = &validSelect[2][0];
	
	char *orderptr[2];
	orderptr[0] = &orderSelect[0][0];
	orderptr[1] = &orderSelect[1][0];
	
	// if there are no records in the database, return the user back to the main menu
	if(enrolments == 0){
		cout << endl;
		cout << "------ WARNING: There are currently no records in the database!! Please upload some first!! ------" << endl;
		return;
	}
	
	cout << "Choose the order of display according to the three sort keys from the three fields: Name, ID and GPA" << endl;
	cout << "Records will be displayed according to the sort key and order. The default option is shown below:";
	cout << endl;
	cout << "First Sort Key: ID , ascending, Second Sort Key: Name, ascending, Third Sort Key: GPA, descending" << endl;
	cout << endl;
	
	// prompt the user to enter their search key combinations
	// store these in a character array pointer
	
	cout << "Please enter your first sort key and order: ";
	validKey(userSelect[0], keys, currentRow, 17, vlidptr, orderptr);
	currentRow = currentRow + 1;
	
	cout << "Please enter your second sort key and order: ";
	validKey(userSelect[1], keys, currentRow, 17, vlidptr, orderptr);
	currentRow = currentRow + 1;
	
	cout << "Please enter your third sort key and order: ";
	validKey(userSelect[2], keys, currentRow, 17, vlidptr, orderptr);
	currentRow = currentRow + 1;
	cout << endl;
	
	// output to the user their search key selections
	for(int i = 0; i < 3; i++){
		cout << "------ HERE ARE THE ENTERED SORT KEYS. " << "#" << i + 1 << " ------" << endl;
		for(int k = 0; k < 2; k++){
			cout << keys[i][k] << endl;
		}
		cout << endl;
	}
	
	// call the sort function to search through the database using the supplied keys
	sort(keys, orderptr, vlidptr, database, enrolments);
	
	return;
}

// function handles the searching of database records based on the keys input by the user
// and stores them in the order based on the keys that are input by the user.
void sort(char * keys[][2], char *orderptr[], char * vlidptr[], struct student database[], int & enrolments){

	int*stdIndex;
	void * ptrOne;
	void * ptrTwo;
	stdIndex = new int[enrolments];
	int keySelect = 0;
	bool keyChange = true;
	int currentKeyRow = 0;
	bool orderChange = true;
	int orAsDe = 0;
	int (*cmp)(void*, void*, int);
	
	bool focus = false;
	int lookforJ = -1;

	// function populates the dynamic array with a list of indexes from one to the number of enrolments
	// that are currently in the database.
	// seeing as each record in the database is currently in the order from 0 to n-1, it makes sense to
	// the generated sort is accomplished simply by ordering the indexes of the records rather than moving the records themeselves
	populateIndex(stdIndex, enrolments);
	
	
	// the sorting process uses bubble sort to sort the records into the desired orders
	for(int i = enrolments-1; i > 0; i--){
		for(int j = 0; j < i; j++){
			
			// boolean is set to enter a switch statement
			//if the order of the sort needs to be changed from ascending to descending 
			// an integer pointer function is assigned according to the nature of the sort key
			// whether it needs to be set to ascending or descending
			if(orderChange){
				orAsDe = getKeyValue(keys, orderptr, 2, currentKeyRow, 1);
				switch(orAsDe){
		
					case 0:
						cmp = lessThan;
					break;
			
					case 1:
						cmp = greaterThan;
					break;
				}
				orderChange = false;
			}
			//boolean is set to enter a function call, which gets the current sort value
			// key of the first key to be used
			if(keyChange){
				keySelect = getKeyValue(keys, vlidptr, 3, currentKeyRow, 0);
				keyChange = false;
			}
			
			// calls a function to enter a switch statement, which will either assign a void pointer address
			// to the values to be used in a function, or compare two strings
			delegateContent(stdIndex, j, database, focus, lookforJ, currentKeyRow, orderChange, keyChange, ptrOne, ptrTwo, keySelect, orAsDe);
			
			// if the sort type is not a string, then compare the two ptr values
			// that is if they are either a float or integer
			if(keySelect != 0){
				
				// call a comparison pointer function to assess the comparison of two values
				if(cmp(ptrOne, ptrTwo,keySelect) == 0 && !focus){
					// swap the two index values if the statement of the comparison is true
					swapVals(stdIndex, j);
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
				}
				// the focus boolean lets the funciton know that there a duplicate has been detected and the
				// comparison of two values is to occur at a certain iteration
				else if(focus){
					// the value of this variable means that there is a duplicate that has previously
					// been encountered, so until the iteration has reached this value, it is not to be
					// compared and possibly swapped
					if(lookforJ >= 0 && lookforJ == j){
						if(cmp(ptrOne, ptrTwo,keySelect) == 0){
							swapVals(stdIndex, j);
						}
					}
					// reset the boolean and key values back to the original key and continue the sort
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
				
				}
				// if there is a duplicate in the values that are currently being sorted
				// set a true boolean for the function to ignore the usual comparison,
				// and change change the key values
				else if(cmp(ptrOne, ptrTwo,keySelect) == -1){
					focus = true;
					lookforJ = j;
					j = j -1;
					currentKeyRow = currentKeyRow + 1;
					orderChange = true;
					keyChange = true;
					
				}
			}
		}
	}
	// output the search result values
	cout << endl;
	cout << "--- HERE ARE YOUR SEARCH RESULTS ---" << endl;
	cout << endl;
	outSearchResults(stdIndex, enrolments, database);
	
	// delete the void array and set the pointers to NULL
	delete stdIndex;
	stdIndex = NULL;
	ptrOne = NULL;
	ptrTwo = NULL;
	
	return;
}

// function displays the search results generated by the search keys
void outSearchResults(int stdIndex[], int enrolments, struct student database[]){

	for(int e=0; e < enrolments; e++){
		cout << "Name: " << database[stdIndex[e]].name << endl;
		cout << "Student: " << database[stdIndex[e]].id << endl;
		cout << "GPA: " << database[stdIndex[e]].GPA << endl;
		cout << endl;
	}
	cout << endl;
	
	return;
}

// function populates an array of integers representing the indexes of the records
// that are currently stored in the database
void populateIndex(int stdIndex[], int enrolments){
	
	for(int x=0; x < enrolments; x++){
		stdIndex[x] = x;
	}
	return;
}

// function handles the comparing of database based on the current key value that is
// passed on through the sort.
void delegateContent(int stdIndex[], int & curPos, struct student database[], bool&focus, int& lookforJ, int &currentKeyRow, bool & orderChange, bool & keyChange, void*&ptrOne, void*&ptrTwo, int &keySelect, int & orAsDe){

	switch(keySelect){
		// if key value is 0 then the data to be sorted is a string
		case 0:
			if(strcmp(database[stdIndex[curPos]].name, database[stdIndex[curPos + 1]].name) == 0){
					focus = true;
					lookforJ = curPos;
					curPos = curPos -1;
					currentKeyRow = currentKeyRow + 1;
					orderChange = true;
					keyChange = true;
			}
			else if(orAsDe == 0){
				
				if(strcmp(database[stdIndex[curPos]].name, database[stdIndex[curPos + 1]].name) > 0 && !focus){
					swapVals(stdIndex, curPos);
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
					
				}else if(focus){
					if(lookforJ >= 0 && lookforJ == curPos){
						if(strcmp(database[stdIndex[curPos]].name, database[stdIndex[curPos + 1]].name) > 0){
							swapVals(stdIndex, curPos);
						}
					}
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
					
				}
			}
			else if(orAsDe == 1){
				if(strcmp(database[stdIndex[curPos]].name, database[stdIndex[curPos + 1]].name) < 0 && !focus){
					swapVals(stdIndex, curPos);
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
						
				}
				else if(focus){
					if(lookforJ >= 0 && lookforJ == curPos){
						if(strcmp(database[stdIndex[curPos]].name, database[stdIndex[curPos + 1]].name) < 0){
							swapVals(stdIndex, curPos);
						}
					}				
					resetBools(currentKeyRow, orderChange, keyChange,focus, lookforJ);
				}
			}
		break;
		
		// if the number to be sorted is a 1, then the data type is an integer
		// set the void pointers to the address of the current IDs of the two records
		case 1:
			ptrOne = &database[stdIndex[curPos]].id;
			ptrTwo = &database[stdIndex[curPos + 1]].id;
		break;
		
		// if the number to be sorted is a 2, then the data type is a float
		// set the void pointers to the address of the current GPAs of the two records
		case 2:
			ptrOne = &database[stdIndex[curPos]].GPA;
			ptrTwo = &database[stdIndex[curPos + 1]].GPA;
		break;
	}
	
	return;
}

// function swaps the values of an integer array
void swapVals(int stdIndex[], int curPos){

	int temp = stdIndex[curPos];
	stdIndex[curPos] = stdIndex[curPos+1];
	stdIndex[curPos+1] = temp;
	
	return;
}

// resets the boolean values to their default value
// this is usually completed after a match of a value
// and matched value has been found
void resetBools(int & currentKeyRow, bool& orderChange, bool&keyChange, bool & focus, int & lookforJ){
	
	// means that a duplicate has been found 
	// and therefore the key has been changed
	// in this case after the match has been found
	// the key has to be reset back to the first one
	if(currentKeyRow > 0){
		currentKeyRow = 0;
		orderChange = true;
		keyChange = true;
		
	// otherwise set the order and key boolean indicators
	// to null to avoid any unwanted key changes
	}else {
		orderChange = false;
		keyChange = false;
	}
	
	// reset the boolean indicators alerting the function
	// a duplicate has actually been found
	focus = false;
	lookforJ = -1;
	
	return;
}

// function accepts two void pointers and converts them to the appropriate
// datatype based on a passed value
// function compares two values that are less than each other
int lessThan(void * valOne, void *valTwo, int type){

	float gpaOne = 0;
	float gpaTwo = 0;
	
	int idOne = 0;
	int idTwo = 0;
	
	// switch statement determines the data type of the two ptrs
	switch(type){
		
		case 1:
			idOne = *((int*)valOne);
			idTwo = *((int*)valTwo);
			if(idOne == idTwo){
				return -1;
			}else {
				return(idOne < idTwo);
			}
		break;
		
		case 2:
			gpaOne = *((float*)valOne);
			gpaTwo = *((float*)valTwo);
			if(gpaOne == gpaTwo){
				return -1;
			}else {
				return(gpaOne < gpaTwo);
			}
		break;
	}
}

// function accepts two void pointers and converts them to the appropriate
// datatype based on a passed value
// function compares two values that are greater than each other
int greaterThan(void * valOne, void *valTwo, int type){

	float gpaOne = 0;
	float gpaTwo = 0;
	
	int idOne = 0;
	int idTwo = 0;
	
	// switch statement determines the data type of the two ptrs
	switch(type){
		
		case 1:
			idOne = *((int*)valOne);
			idTwo = *((int*)valTwo);
			if(idOne == idTwo){
				return -1;
			}else {
				return(idOne > idTwo);
			}
		break;
		
		case 2:
			gpaOne = *((float*)valOne);
			gpaTwo = *((float*)valTwo);
			if(gpaOne == gpaTwo){
				return -1;
			}else {
				return(gpaOne > gpaTwo);
			}
		break;
	}
}

// gets the value of the sortKey identifier and returns an index number relevant to the chosen key
int getKeyValue(char *keys[][2], char * idnptr[], int ptrLen, int currentRow, int currentPos){
	
	int current = 0;
	for(int i =0; i < ptrLen; i++){
		
		if(strcmp(keys[currentRow][currentPos], idnptr[i]) == 0){
			current = i;
		}
	}
	return current;
}


// takes a string from the user and calls on the appropriate functions to test whether the values
// are valid or not and calls another to store these indexes in a char array pointer
void validKey(char userSelect[], char *keys[][2], int currentRow, int maxChar, char *vlidptr[], char*orderptr[]){
	
	int length = 0;
	bool keyType = false;
	bool key = false;
	bool order = false;
	char reset[100] = { '\0' };
	
	// continue the input while the input for the keys is invalid
	while(keyType == false){
		
		cin.getline(userSelect, maxChar, '\n');
		length = strlen(userSelect);
		if(length >= 2){
			sortKeyPop(length, userSelect, keys, currentRow);
			key = testKeys(keys, 3, vlidptr, currentRow, 0);
			order = testKeys(keys, 2, orderptr, currentRow, 1);
		}
		
		// end the loop in the event a valid key and order return value
		// have been set to true
		if(order && key){
			keyType = true;
		
		// otherwise reset the values and output an error message
		}else {
			cout << endl;
			cout << "Please enter a valid sort key combination: ";
			cin.clear();
			keyType = false;
			order= false;
			key = false;
			strcpy(userSelect, reset);
		}
	}
	return;
}

// function takes input from the user and if valid, stores these values in a character
// array pointer
void sortKeyPop(int length, char userSelect[], char *keys[][2], int currentRow){

	int sort = 0;
	int letterCount = 0;
	int extraChar = 0;

	// enter a loop to run the length of the string
	for(int i = 0; i < length; i++){
	
		// convert the valus to lowercase
		userSelect[i] = tolower(userSelect[i]);
		
		// if the value is between a and b
		// count this
		if(userSelect[i] >= 'a' && userSelect[i] <= 'z'){
			letterCount = letterCount + 1;
		}
		// if the current increment is equal to the length -1
		// store the start of the index for the current string as a pointer
		// in the key array of pointers
		if(i == length -1){
			keys[currentRow][sort] = &userSelect[i - letterCount + 1];
		}
		// otherwise if the increment is below the end and the current value is a space
		// or a comma, place a nul character in the place
		else {
			if(userSelect[i] == ' ' || userSelect[i] == ','){
				extraChar = extraChar + 1;
				keys[currentRow][sort] = &userSelect[i - letterCount];
				userSelect[i] = '\0';
				letterCount = 0;
				if(sort == 0){
					sort = sort + 1;
				}
			}
		}
	}
	
	return;
}

// function returns a boolean confirming whether or not a valid key sort value has been entered
// otherwise returns false to indicate an invalid key has been entered
bool testKeys(char *keys[][2], int compareLength, char*pointerArray[], int currentRow, int column){

	bool match = false;
	
	// returns a 
	for(int k = 0; k < compareLength; k++){
		if(strcmp(pointerArray[k],keys[currentRow][column]) == 0){
			match = true;
		}
	}
	
	return match;
}

// displays the search menu options and calls the appropriate functions based on the user's input
void displaySearchMenu(struct student database[], int & enrolments, char signals[][13]){
	
	char searchInput;
	int length = 4;
	char validVals[4];
	validVals[0] = 'r'; 
	validVals[1] = 'n';
	validVals[2] = 's';
	validVals[3] = 'g';
	char alphabetLower[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	char alphabetUpper[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
	int returnType = 0;
	
	// while the user hasn't entered the r character to return to the main menu, stay in the current menu
	while(searchInput != 'r'){
	
		cout << '\n' << "/**************** SEARCH MENU *********************/" << endl;
		cout << '\n' << "Welcome to the search menu. Please select one of the following options" << endl;
		cout << endl;
		cout << "r - return to the main menu" << endl;
		cout << "n - Name Search. (Case Insensitive). Supports the wild card * feature. Any name that starts or ends with the letter";
		cout << "desired letter will be produced" << endl;
		cout << "s - Subject Search (Case sensitive). Option returns a list of students taking the subject matching the search criteria promote the user for year, session and code";
		cout << "for each input, the user can use the wild card *" << endl;
		cout << "g - Search by GPA, the input format is a symbol <, > or = followed by a floating point number. for =, any result within 1 point of the specified GPA is considered a match";
		cout << "That is, if the user want to search for students with GPA = 5.5, it will display all students whose GPA is from 4.5 to 6.5 (inclusive)" << endl;
		cout << endl;
		cout << "Please enter your selection: ";
		
		searchInput = getUserInput(length, validVals);
		
		switch(searchInput){
	
			case 'n':
			
				// call the itemSearch function that searches for database names
				returnType = itemSearch(database, enrolments, signals, alphabetLower, alphabetUpper, false);
				// if the returntype is not 0, meaning that they have not selected to return to the main menu
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
	
			case 's':
				// call the itemSearch function for subjects
				returnType = itemSearch(database, enrolments, signals, alphabetLower, alphabetUpper, true);
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
	
			case 'g':
				// call the sortGPA function to handle the search for GPAs
				returnType = sortGPA(database, enrolments, signals, alphabetLower, alphabetUpper);
				if(returnType == 0){
					searchInput = 'r';
				}
			break;
		}
		// if the user has entered the r character display the main menu list and return back to the main input area
		if(searchInput == 'r'){
			displayMainMenu();
		}
		cin.clear();
	}

	return;
}

// function displays the options available when a series of search results have been returned and allows the user to move through the
// resulting record list
int toggleSearchMenu(struct student database[], int found, int matches[], int & enrolments, char alphabetUpper[], char alphabetLower[]){

	char moveBetween;
	bool select = false;
	int mover = 0;
	bool initialDisplay =true;
	
	// if the menu hasnt been displayed before then output the full options available
	if(initialDisplay == true){
		cout << endl;
		cout << "There are a total of: " << found << " results. Please use the listed keys to perform the following actions:" << endl;
		cout << endl;
		cout << "r - return to the main menu" << endl;
		cout << "s - return to the search menu" << endl;
		cout << "n - Go to the next record" << endl;
		cout << "p - Go to the previous record" << endl;
		cout << "e - edit the current record" << endl;
		cout << "d - delete the current record" << endl;
		cout << endl;
	}
	while(select == false){
		// selections are kept in a while loop, with the choices selected in a switch statement
		// until the user selects a choice that returns to a different menu, or completes an action, they will remain in this menu
	
		cout << endl;
		cout << "----- CURRENT RECORD " << mover + 1 << " -----" << endl;
		outPartStruct(database, matches[mover]);
		cout << endl;
		cout << "Please select your option display menu option, (r)eturn to search menu, (s)earch menu return, ";
		cout << "(p)revious record, (n)ext record, (e)dit current record, (d)elete current record: " << endl;
		
		// get the users value
		cin >> moveBetween;
		switch(moveBetween){
			case 'r':
				// if its r return to the main menu, 0 will return back to the calling function which will return back to the main menu
				return  0;
			break;
	
			case 's':
				// return to the search function
				return - 1;
			break;
	
			case 'n':
				// display the next item in the search result list and output the details of the current record
				// turn of the initial display of the menu
				initialDisplay =false;
				
				// if the current record is not at the end, move through the list and display the details
				if(found > 1 && mover < found-1){
					// keeps track of where the user is within the search result list
					mover = mover + 1;
					cout << endl;
					cout << "Currently Displaying Record: " << mover + 1 << " of " << found << " records" << endl;
					cout << endl;
				}else {
					// if you cant move any further then output the message
					cout << "------- You can't move to the next record, because there aren't any more to see ------- " << endl;
				}
			break;
	
			case 'p':
				// turn of the initial display of the menu
				initialDisplay =false;
				
				// displays the previous item within the search result list and outputs the details of the record
				if(found > 1 && mover > 0){
					// keeps track of where the user is within the search result list
					mover = mover - 1;
					cout << "Currently Displaying Record: " << mover + 1 << " of " << found << " records" << endl;
					cout << endl;
				}else {
					// if the user reaches the end then display a message explaining this
					cout << "-------  You have reached the start of the selection and can't move back any further -------" << endl;
				}
			break;
	
			case 'e':
				// if the user decides to edit the current record, then listed function is called
				// with the current details to edit the value
				editCurrentRecord(database, matches, mover, alphabetUpper, alphabetLower);
				calculateGPA(database, enrolments);
				select = true;
			break;
	
			case 'd':
				// if delete the current record
				deleteRecord(enrolments, database, matches, mover);
				return 0;
				select = true;
			break;
			
			default:
				// if the user enters an invalid selection clear the error and display the list
				cout << endl;
				cout << "Please enter a valid selection!" << endl;
				cin.clear();
				cin.ignore(1000,'\n');
				cout << endl;
			break;
		}
	}
}

// function edits the current record using the passed details to identify the record to edit within the database
// function iterates through all aspects of a student record and allows a user to edit each value. the user cannot enter a null value for
// any of the selections, nor can they enter the same value
void editCurrentRecord(struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){

	bool value = false;
	char valArray[4] = { 'y','Y','n','N' };
	char yesNo;
	int sectionCount = 0;
	
	while(sectionCount < 5){
	
		// main section is controlled in a while loop which outputs a different message upon the
		// iteration of a counter. according to what iteration the counter is up to and what value the
		// user enters, the choice is validated and passed on to another function, which handles what
		// section of the student record the user is to update next
		
		switch(sectionCount){
			case 0:
				cout << endl;
				cout << "Do you want to edit the current name? Y/N?: " << endl;
			break;
			
			case 1:
				// need to look at the current ID as this isnt passing the test
				cout << endl;
				cout << "Do you want to edit the current ID? Y/N?: " << endl;
			break;
			
			case 2:
				cout << endl;
				cout << "Do you want to edit the current course number? Y/N? " << endl;
			break;
			
			case 3:
				cout << endl;
				cout << "Do you want to edit the current course name? Y/N?" << endl; 
			break;
			
			case 4:
				cout << endl;
				cout << "Do you want to edit the current subject list? Y/N?: " << endl;
			break;
		}
		
		// get the user input and validate it before
		yesNo = getUserInput(4, valArray);
		
		// call on the edit function to edit the section of the record
		delegateEdit(yesNo, sectionCount, database, matches, mover, alphabetUpper, alphabetLower);
		
		// clear the selection before the next prompt.
		yesNo = '\0';
		
		// increment to the next choice
		sectionCount = sectionCount + 1;
	}
		
	return;
}

// function controls what section of the record to edit according to what iteration the calling function, editCurrentRecord is up to
void delegateEdit(char yesNo, int sectionCount, struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){
	
	int IDNumber = 0;
	
	switch(sectionCount){
		case 0:
			// edit the current name and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new name you would like to change: ";
				nameChange(database[matches[mover]].name, false, alphabetUpper, alphabetLower, 30, 0, false, 1);
				cout << "--- You changed the name to: " << database[matches[mover]].name << " ---" <<endl;
				return;
			}else {
				return;
			}
		break;
		
		case 1:
			// edit the current ID and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new ID that you would like to change: ";
				IDNumber =  numberChange(database[matches[mover]].id, 1000000, 9999999);
				database[matches[mover]].id = IDNumber;
				cout << "--- You changed the id to: " << database[matches[mover]].id << " ---" <<endl;
				cout << endl;
				IDNumber = 0;
				return;
			}else {
				return;
			}
			
		break;
		
		case 2:
			// edit the current course number and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new course number that you would like to change: ";
				IDNumber =  numberChange(database[matches[mover]].single.courseNum, 1000, 9999);
				database[matches[mover]].single.courseNum = IDNumber;
				cout << "--- You changed the course number to: " << database[matches[mover]].single.courseNum << " ---" <<endl;
				cout << endl;
				IDNumber = 0;
				return;
			}else {
				return;
			}
		break;
		
		case 3:
			// edit the current course name and return back to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new course name you would like to change: ";
				nameChange(database[matches[mover]].single.courseName , false, alphabetUpper, alphabetLower, 100, 0, true, 7);
				cout << "--- You changed the course name to: " << database[matches[mover]].single.courseName << " ---" <<endl;
				cout << endl;
				return;
			}else {
				return;
			}
		break;
		
		case 4:
			// if the user wants to edit, call the subjectChange function to edit the values associated to the subjects of the current record
			if(yesNo == 'y' || yesNo == 'Y'){
				subjectChange(database, matches, mover, alphabetUpper, alphabetLower);
			}else {
				return;
			}
		break;
	}
}

// function handles the editing of the subject values for the amount of enrolled subjects for a particular record
void subjectChange(struct student database[], int matches[], int mover, char alphabetUpper[], char alphabetLower[]){
	
	bool value = false;
	int subInfoCount = 0;
	int numOfSubs = database[matches[mover]].enrolled;
	char valArray[4] = { 'y','Y','n','N' };
	char yesNo;
	int subjectResponse = 0;
	bool markStatus = false;
	
	for(int i  = 0; i < numOfSubs; i++){
		// controlled in an inner while loop for all sections of each subject
		// the outer for loop makes this process run for the amount of enrolments for the particular record being edited
		while(subInfoCount < 6){
			// switch case handles the current iteration of the subject attribute list and outputs the message to edit
			switch(subInfoCount){
				case 0:
					cout << "Do you want to edit the current subject year? Y/N?: ";
				break;
				
				case 1:
					cout << "Do you want to edit the current subject session? Y/N?: ";
				break;
				
				case 2:
					cout << "Do you want to edit the current subject code? Y/N?: ";
				break;
				
				case 3:
					cout << "Do you want to edit the current subject credit point value? Y/N?: ";
				break;
				
				case 4:
					cout << "Do you want to edit the current subject mark? Y/N?: ";
				break;
				
				case 5:
					// if the user says no to entering a subject mark, and the value of the subject is -1
					// then that means the student has a current undefined status and the user has the option
					// to edit the status value
					// otherwise this option is unavailable
					if(subjectResponse != 1){
						cout << "Do you want to edit current subject status? Y/N?: ";
					}else {
						markStatus = true;
					}
				break;
			}
			// if the markstatus is false that is, there is a subject mark then call the next function to edit
			if(markStatus == false){
				yesNo = getUserInput(4, valArray);
				subjectResponse = subjectToEdit(yesNo, database, matches, mover, i, subInfoCount, alphabetUpper, alphabetLower);
				yesNo = '\0';
			}
			subInfoCount = subInfoCount + 1;
		}
		// reset values for next iteration
		subInfoCount = 0;
		markStatus = false;
	}
	
	return;
}


// function handles the editing of each subject value for a particular subject record belonging to a student
int subjectToEdit(char yesNo, struct student database[], int matches[], int mover, int currentSubject, int section, char alphabetUpper[], char alphabetLower[]){
	
	int numberUpdate = 0;
	bool validSingle = false;
	char choice;
	char sessionVals[3];
	sessionVals[0] = 'S';
	sessionVals[1] = 'W';
	sessionVals[2] = 'A';
	char subStatus[2];
	subStatus[0] = 'e';
	subStatus[1] = 'w';
	
	// select the case, that is the section of the current subject record you want to edit
	// and output the changed value if the user decides to change the value
	switch(section){
		case 0:
			// edit the subject year and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the subject year you wish to update: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].year, 2012, 3000);
				database[matches[mover]].person[currentSubject].year = numberUpdate;
				cout << "---- You have changed the subject year to: " << database[matches[mover]].person[currentSubject].year << " ---" <<endl;
				cout << endl;
				numberUpdate = 0;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 1:
			// edit the session value for the subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				while(validSingle == false){
					cout << "Please enter the session you would like to update: ";
					cin >> choice;
					validSingle = sessionCheck(choice, sessionVals, 3);
					if(validSingle == false){
						cout << "You did not enter a correct value. Please make sure your values are upper case." << endl;
						cout << endl;
					}
				}
				validSingle = false;
				database[matches[mover]].person[currentSubject].session = choice;
				cout << "---- You have changed the session value of the subject to: " << database[matches[mover]].person[currentSubject].session << " ---" <<endl;
				cout << endl;
				choice = '\0';
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 2:
			// edit the subject code for the subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject code you would like to update: ";
				nameChange(database[matches[mover]].person[currentSubject].code, true, alphabetUpper, alphabetLower, 9, 3, false, 0);
				cout << "--- You changed subject " << currentSubject << "'s code to: " << database[matches[mover]].person[currentSubject].code << " ---" <<endl;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 3:
			// edit the credit point value for the current subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject credit point value: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].credit, 5, 20);
				database[matches[mover]].person[currentSubject].credit = numberUpdate;
				numberUpdate = 0;
				cout << "---- You have changed the credit point value of the subject to: " << database[matches[mover]].person[currentSubject].credit << " ----" << endl;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 4:
			// edit the subject mark for the current subject and return to the calling function, otherwise just return
			if(yesNo == 'y' || yesNo == 'Y'){
				cout << "Please enter the new subject mark for this subject: ";
				numberUpdate = numberChange(database[matches[mover]].person[currentSubject].subjectMark, 0, 100);
				database[matches[mover]].person[currentSubject].subjectMark = numberUpdate;
				cout << "---- You have changed the current mark for this subject to: " << database[matches[mover]].person[currentSubject].subjectMark << " ----" << endl;
				numberUpdate = 0;
				cout << endl;
				return 1;
			}else {
				return 0;
			}
		break;
		
		case 5:
			// edit the subject status for the subject and return to the calling function, otherwise just return
			// if there a -1 then change the value and disallow the option to edit the subjects status, otherwise
			// if the user says no and the value indicates that there is a substatus value then the user must enter the substatus value
			// if the user enters a subject mark and says yes to the subject status, then output an error
			if(yesNo == 'y' || yesNo == 'Y'){	
				if(database[matches[mover]].person[currentSubject].subjectMark == -1){
					while(validSingle == false){
						cout << "Please enter the subject status you would like to update: ";
						cin >> choice;
						validSingle = sessionCheck(choice, subStatus, 2);
						if(validSingle == false){
							cout << "You did not enter a correct value. Please make sure your values are either e or w." << endl;
							cout << endl;
						}
					}
					validSingle = false;
					database[matches[mover]].person[currentSubject].subStatus = choice;
					cout << "---- You have changed the current session status for this subject to: " << database[matches[mover]].person[currentSubject].subStatus << " ----" << endl;
					choice = '\0';
					cout << endl;
				}else {
					cout << "There already is a subject mark. You can't add a subject status also!" << endl;
					cout << endl;
					return 0;
				}
				return 1;
			}else {
				return 0;
			}
		break;
	}
}
