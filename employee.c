/*
Name: Gary Koers Jr.
Class : ECE359
Homework #4

Description: This program has an edited FindEmployee() , it has an additional menu page to do string functions the option is R
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "employee.h"
#include "string_module.h"

//fgets() will always append the newline (enter) to your string, made a function to remove this. also need to keep this in mind
//when declaring string sizes. Dynamic string sizes are impossible, cannot be made with global variables etc...
//dynamic strings CAN be made using malloc, then editing the memory

/*
Program will not allow you to put in an employee if the employeee's supervisor does not exist, this solves a logistical problem that would require you to keep entering supervisors up to
the general manager for your entry point employee in order to have sufficient information

Program creates a new head pointer for each "group" of supervisee's under a supervisor


//the leftmost employees "prev" does NOT point back to GroupList Row ptr

*/




int main() {
	int currentscreen = 0;

	struct Employee *EmployeeHead;
	EmployeeHead = (struct Employee *)malloc(sizeof(struct Employee));

	struct GroupList *GroupHead;																	//Group List is a linked list where each link points to the beginning of a group
	GroupHead = (struct GroupList *)malloc(sizeof(struct GroupList));

	GroupHead->next = NULL; GroupHead->prev = NULL; GroupHead->EmployeePtr = EmployeeHead;
	GroupHead->EmployeePtr->firstname = NULL;                                                          // create a never deleted GM node at the beginning
	GroupHead->EmployeePtr->lastname = NULL;
	GroupHead->EmployeePtr->jobtitle = _strdup("General Manager");
	GroupHead->EmployeePtr->ID = _strdup("0000");
	GroupHead->EmployeePtr->SupID = _strdup("0000");
	GroupHead->EmployeePtr->supervisee = NULL;
	GroupHead->EmployeePtr->supervisor = NULL;
	GroupHead->EmployeePtr->next = NULL;
	GroupHead->EmployeePtr->prev = NULL;

	while (1) {																							// infinite screen selection loop
		currentscreen = MainMenu(GroupHead);
		if (currentscreen == 0) { MainMenu(GroupHead); }
		if (currentscreen == 1) { Insert(GroupHead); }
		if (currentscreen == 2) { Delete(GroupHead); }
		if (currentscreen == 3) { Search(GroupHead); }
		if (currentscreen == 4) { Debug(GroupHead); }
		if (currentscreen == 5) { StringMenu(); }														// this part of program is for part 2 of hw 4, the string module
	}// end screen selection while loop

	FreeList(GroupHead);
}


