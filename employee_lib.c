/*
Name: Gary Koers Jr.
Class : ECE359
Homework #4

// this file contains definitions of all declared variables and functions within employee.h
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "employee.h"
#include "string_module.h"


int GMExistsFlag = 0;
int mainmenuselection;
int searchselection;																		 //it is recommended to use char as opposed to int for getc()
int deleteselection;
int insertselection;
int currentscreen = 0;
int debugselection;
int DOB_PROBLEM = 1;																			 //flag used to trap user until valid DOB entered
int numberofemployees = 0;

bool firstemployeeflag = true;
bool GMflag = false;
char *back = "b\n";
char USERINPUT_PROBLEM = 0;
int SupervisorID_Problem = 0;
																										// BEGIN MainMenu()
int MainMenu(struct GroupList *head) {
	printf("Please choose an option:\nI:insert\nD:Delete\nS:Search\nR:String Operations\n");
	mainmenuselection = getchar();
	fseek(stdin, 0, SEEK_END);
	//if input is i, run insert, then start a new main
	if (mainmenuselection == 'i' || mainmenuselection == 'I')
	{
		currentscreen = 1;
		//Insert(&head);  
		return 1;
	}																//same for delete....
	if (mainmenuselection == 'd' || mainmenuselection == 'D')
	{
		currentscreen = 2;
		//Delete(&head);  
		return 2;
	}																//search...
	if (mainmenuselection == 's' || mainmenuselection == 'S')
	{
		currentscreen = 3;
		//Search(&head);	
		return 3;
	}																//debug...
	if (mainmenuselection == '?')
	{
		currentscreen = 4;
		//Debug();  
		return 4;
	}

	if (mainmenuselection == 'R' || mainmenuselection == 'r')
	{
		currentscreen = 5;
		//Search(&head);	
		return 5;
	}




	else { printf("Invalid Entry in Main Menu\n"); return 0; }
}



// BEGIN INSERT()



int Insert(struct GroupList *GroupHead) {												//this function will gather infromation using subfunctions (GetUserFirstName etc). 
																						//then pass to InsertNode() which handles the dirty work of node data creation and ptr
																						//declare temporary insert variables
	struct GroupList *grouphead;
	char temp_firstname[firstnamelength];
	char temp_lastname[lastnamelength];
	char temp_jobtitle[jobtitlelength];
	char insertselection = NULL;
	int temp_DOB[9];																			 //int arrays dont append a null poointer '\0'
	char temp_ID[5];		 //5 to acommodate \n
	char temp_SupID[5];
	char temp_DOB_string[11];
	int ID_Problem = 0;
	char ZeroArray[4] = { '0','0','0','0' };
	struct SearchResults TempSearchResults;
	printf("====================================================================");
	printf("\nINSERT:\n");
	printf("type b to go back\n\n");
	// Get Employee Information for new entry...
	printf("Enter First Name:\n");
	fseek(stdin, 0, SEEK_END);
	fgets(temp_firstname, sizeof(temp_firstname), stdin);
	fseek(stdin, 0, SEEK_END);

	if (strcmp(temp_firstname, back) == 0)																			//exit if "b entered" 
	{
		currentscreen = 0; return 0;
	}

	RemoveNewLineFromString(temp_firstname);															// remove new line from string used to pull off '\n'

	printf("Enter Last Name:\n");
	fseek(stdin, 0, SEEK_END);
	fgets(temp_lastname, sizeof(temp_lastname), stdin);
	fseek(stdin, 0, SEEK_END);

	if (strcmp(temp_lastname, back) == 0)																			//exit if "b entered" 
	{
		currentscreen = 0; return 0;
	}

	RemoveNewLineFromString(temp_lastname);

	printf("Enter Job Title:\n");																			//get jobtitle string
	fseek(stdin, 0, SEEK_END);
	fgets(temp_jobtitle, sizeof(temp_jobtitle), stdin);
	fseek(stdin, 0, SEEK_END);

	if (strcmp(temp_jobtitle, back) == 0)																		 //exit if "b entered 
	{
		currentscreen = 0; return 0;
	}

	RemoveNewLineFromString(temp_jobtitle);
	// get DOB
	DOB_PROBLEM = 1;																	// initialize with problem: we need a dob, lower when we get it, thus exiting while loop
	while (DOB_PROBLEM == 1)
	{

		printf("Enter DOB (MM/DD/YYYY):\n");
		fseek(stdin, 0, SEEK_END);
		fgets(temp_DOB_string, sizeof(temp_DOB_string), stdin);
		fseek(stdin, 0, SEEK_END);
		//convert DOBstring to int array with no '/'
		temp_DOB[0] = temp_DOB_string[0] - '0'; temp_DOB[1] = temp_DOB_string[1] - '0';
		temp_DOB[2] = temp_DOB_string[3] - '0';	temp_DOB[3] = temp_DOB_string[4] - '0';
		temp_DOB[4] = temp_DOB_string[6] - '0';	temp_DOB[5] = temp_DOB_string[7] - '0';
		temp_DOB[6] = temp_DOB_string[8] - '0';	temp_DOB[7] = temp_DOB_string[9] - '0';
		temp_DOB[8] = NULL;


		if (strcmp(temp_DOB_string, back) == 0)														      //exit if "b entered"
		{
			currentscreen = 0; return 0;
		}

		if (ValidateDOB(temp_DOB) == 0)																             //make sure DOB is valid
		{
			DOB_PROBLEM = 1; continue;
		}																  // if invalid, set problem flag, continue, starts a new while loop

		DOB_PROBLEM = 0;																					 //if here, we have a DOB, and its good
	}																									//end of while loop for DOB

	ID_Problem = 1;																							// get employee id. stuck in while loop till flag lowered.
	while (ID_Problem == 1) {
		printf("Enter Employee ID#:\n");
		fseek(stdin, 0, SEEK_END);
		fgets(temp_ID, 5, stdin);
		fseek(stdin, 0, SEEK_END);

		if (strcmp(temp_ID, back) == 0)																	   //exit if "b entered" 
		{
			currentscreen = 0; return 0;
		}

		if (ValidateID(temp_ID) == 0) { printf("Invalid ID (0 to 9 allowed)\n"); continue; }                 // validateID returns 1 if ok, 0 if fucked up
		else {

			RemoveNewLineFromString(temp_ID);																// verify that this employee doesnt already exist

			if ((CompareID(temp_ID, "0000") == 1))
			{
				printf("This ID# is not allowed..\n"); continue;
			}                                              // do not allow 0000 as ID

														   //SupervisorID_Problem = 0;

			TempSearchResults = FindEmployee(temp_ID, GroupHead);

			if (TempSearchResults.Found == 1) { printf("This employee already exists\n"); continue; }

			ID_Problem = 0;
		}



	}// end while loop to get employeeID

	SupervisorID_Problem = 1;
	while (SupervisorID_Problem == 1)
	{
		printf("Enter Employee's Immediate Supervisor ID#:\n");													// get supervisor id
		fseek(stdin, 0, SEEK_END);
		fgets(temp_SupID, 5, stdin);
		fseek(stdin, 0, SEEK_END);

		if (strcmp(temp_SupID, back) == 0)  //exit if "b entered" 
		{
			currentscreen = 0; return 0;
		}

		if (ValidateID(temp_SupID) == 0) { printf("Invalid ID (0 to 9 allowed)\n"); continue; }
		RemoveNewLineFromString(temp_SupID);																	  //pull off '\n' from hitting enter			

		if ((CompareID(temp_SupID, "0000") == 1) && (GMExistsFlag == 1))
		{
			printf("The General Manager already exists\n"); continue;
		}
		// GROUPHEAD IS CORRUPTED BEFORE THIS...............

		TempSearchResults = FindEmployee(temp_SupID, GroupHead);													//verify supervisor exists, result = 0 means doesnt exist



		if (TempSearchResults.Found == 0) { printf("This supervisor does not exist\n"); continue; }                 // if supervisor doesnt exist, stay in while loop (ask again)


		SupervisorID_Problem = 0;



	}///end of supervisor problem loop
	 //output information for verification
	printf("\n\n\nEmployee Information:\n");
	printf("First Name: "); printf("%s", temp_firstname); printf("\nLast Name: "); printf("%s", temp_lastname);
	printf("\nJob Title: %s ", temp_jobtitle);
	printf("\n");
	PrintDOB(temp_DOB);
	printf("\n");
	printf("Employee ID#: ");
	PrintEmpID(temp_ID);
	printf("\n");
	printf("Employee Supervisor ID#: ");
	PrintEmpID(temp_SupID);
	printf("\n");

	// Verify Employee Information is Correct, if not, start Insert() over again

	while (((insertselection == 'Y') || (insertselection == 'y') || (insertselection == 'N') || (insertselection == 'n')) == 0) {
		printf("\nIs this correct? (Y or N)\n");
		fseek(stdin, 0, SEEK_END);
		insertselection = getchar();
		printf("%c", insertselection);
		fseek(stdin, 0, SEEK_END);
	}// end while loop for y/n on is this ok?? employee info

	if (insertselection == 'y' || insertselection == 'Y')														// all is well, we will now create a new node from entered info.
	{
		currentscreen = 0; printf("\nEmployee Information Successfully Entered\n\n");
		if ((temp_SupID[0] == '0') && (temp_SupID[1] == '0') && (temp_SupID[2] == '0') && (temp_SupID[3] == '0')) { printf("This is the General Manager\n"); }
	}///end of while loop to verify employee information

	if (insertselection == 'n' || insertselection == 'N')
	{
		currentscreen = 1; printf("Employee Information Not Entered\n\n");  return 0;
	}			              //abort, return to insert menu for re-entry

	if ((temp_SupID[0] == '0') && (temp_SupID[1] == '0') && (temp_SupID[2] == '0') && (temp_SupID[3] == '0'))
	{
		GMExistsFlag = 1;      											//check if were creating the GM for the first time...
	}

	InsertNode(temp_firstname, temp_lastname, temp_jobtitle, temp_ID, temp_SupID, temp_DOB, GroupHead);					// actual new employee creation occurs here																							 //



}																									// end Insert()

																									//BEGIN Delete() (2) this is the Delete MENU

int Delete(struct GroupList *GroupHead) {


	struct Employee *EmployeePtr = NULL;
	struct Employee *EmployeeToReplaceSupervisor;
	struct SearchResults SearchResults;
	char ID[5];
	//int arrays dont append a null poointer '\0'
	char deleteselection;
	printf("====================================================================");
	printf("\nDELETE:\n");
	printf("1: Delete Employee by ID#:\n\n");
	printf("2: Delete Supervisor by ID#, (replace by alphabetically highest supervisee)\n"); printf("type b to go back\n");

	deleteselection = getchar();
	fseek(stdin, 0, SEEK_END);
	if ((deleteselection != '2') && (deleteselection != '1') && (deleteselection != 'b'))
	{
		printf("Invalid Selection\n"); currentscreen = 2; return 0;
	}
	if (deleteselection == 'b')																			//exit if "b entered" 
	{
		currentscreen = 0; return 0;
	}
	printf("Enter Employee ID#\n");
	fseek(stdin, 0, SEEK_END);
	fgets(ID, 5, stdin);
	fseek(stdin, 0, SEEK_END);

	if (ID[0] == 'b'&&ID[1] == '\n') { currentscreen = 0; return 0; }

	if (ValidateID(ID) == 0) { printf("Invalid ID, try again\n"); currentscreen = 2; return 0; };
	printf("You entered:"); PrintEmpID(ID); printf("\n");

	if (deleteselection == '1')																			//option "1 entered" 
	{
		SearchResults = FindEmployee(ID, GroupHead);

		if ((SearchResults.Found == NULL) || (SearchResults.Found == 0))
		{
			printf("No such employee exists\n"); currentscreen = 2; return 0;
		}
		if (SearchResults.EmployeePtr->SupID == "0000")
		{
			printf("This is the General Manager, please use option 2\n"); currentscreen = 2; return 0;
		}

		if (SearchResults.EmployeePtr->supervisee != NULL)
		{
			printf("This Employee is a supervisor, use option 2 to delete supervisors\n");
			currentscreen = 2; return 0;
		}

		DeleteEmployee(SearchResults.EmployeePtr, GroupHead);													// actual employee deletion occurs here


		printf("Employee Deleted\n"); getchar();
		currentscreen = 2; return 0;
	}

	if (deleteselection == '2') {																		//option "2 entered" 

		SearchResults = FindEmployee(ID, GroupHead);

		if ((SearchResults.Found == NULL) || (SearchResults.Found == 0))
		{
			printf("No such employee exists\n"); currentscreen = 2; return 0;
		}


		if ((SearchResults.EmployeePtr->supervisee == NULL) && (CompareID(SearchResults.EmployeePtr->SupID, "0000") == 0))
		{
			printf("This Employee is not a supervisor, use option 1 to delete employees\n");

			currentscreen = 2; return 0;
		}
		EmployeeToReplaceSupervisor = ChooseEmployeeAlphabetically(SearchResults.EmployeePtr->supervisee, GroupHead);// find employee to replace the supervisor
																													 //if (EmployeeToReplaceSupervisor == NULL) {}
		DeleteSupervisor(SearchResults.EmployeePtr, EmployeeToReplaceSupervisor, GroupHead);

		printf("Supervisor Deleted\n");
		fseek(stdin, 0, SEEK_END);																		//press key to continue
		getchar();
		fseek(stdin, 0, SEEK_END);

		currentscreen = 2; return 0;
	}

	return 0;

}


int Search(struct GroupList *GroupHead)																				// BEGIN Search()... (3) This is the Search MENU
{
	struct Employee *EmployeePtr = NULL;
	struct SearchResults SearchResults;
	char ID[5];
	//int arrays dont append a null poointer '\0'
	char searchselection;
	printf("====================================================================");
	printf("\nSEARCH:\n");
	printf("1: Search employee information by ID#:\n\n");
	printf("2: List an Employees group members (fellow supervisees)\n"); printf("type b to go back\n");

	searchselection = getchar();
	fseek(stdin, 0, SEEK_END);
	if ((searchselection != '2') && (searchselection != '1') && (searchselection != 'b'))
	{
		printf("Invalid Selection\n"); currentscreen = 3; return 0;
	}
	if (searchselection == 'b')																			//exit if "b entered" 
	{
		currentscreen = 0; return 0;
	}
	printf("Enter Employee ID#\n");
	fseek(stdin, 0, SEEK_END);
	fgets(ID, 5, stdin);
	fseek(stdin, 0, SEEK_END);

	if (ID[0] == 'b'&&ID[1] == '\n') { currentscreen = 0; return 0; }

	if (ValidateID(ID) == 0) { printf("Invalid ID, try again\n"); currentscreen = 3; return 0; };
	if (CompareID(ID, "0000")) { printf("Invalid ID, try again\n"); currentscreen = 3; return 0;; }
	printf("You entered:"); PrintEmpID(ID); printf("\n");

	if (searchselection == '1')																			//option "1 entered" 
	{
		SearchResults = FindEmployee(ID, GroupHead);

		if ((SearchResults.Found == NULL) || (SearchResults.Found == 0))
		{
			printf("No such employee exists\n"); currentscreen = 3; return 0;
		}

		PrintEmployeeInfo(&SearchResults.EmployeePtr);
		printf("Press any key to continue"); getchar();
		currentscreen = 3; return 0;
	}

	if (searchselection == '2') {																		//option "2 entered" 

		PrintEmployeeGroup(FindEmployee(ID, GroupHead).EmployeePtr);

		fseek(stdin, 0, SEEK_END);
		getchar();
		fseek(stdin, 0, SEEK_END);

		currentscreen = 3; return 0;
	}

	return 0;
}



void StringMenu()																										// BEGIN StringMenu()

{
	int j = -1;
	int i = -1;
	char input = '0';																									// I WAS CALLING A FUNCTION FROM STRING_MODULE_H 
	char *temp_S1;
	temp_S1 = (char *)malloc(100 * sizeof(char));
	char *temp_S2;
	temp_S2 = (char *)malloc(100 * sizeof(char));
	char *StringOutput;
	StringOutput = (char *)malloc(100 * sizeof(char));


																				// AND ALL VARIABLES WERE GETTING CORRUPTED
	//S = (char *)malloc(100 * sizeof(char));
	while ((input != 'b') && (input != 'B'))
	{
		//int j = -1;
		//int i = -1;
		printf("(Please limit strings to 100 characters or less)\n");
		printf("Enter S1: \n");
		//S = GetString();

		size_t string_length = 100;
		char S1[100];
		char S2[100];
		fseek(stdin, 0, SEEK_END);
		fgets(S1, string_length, stdin);
		fseek(stdin, 0, SEEK_END);

		printf("Enter S2: \n");
		fseek(stdin, 0, SEEK_END);
		fgets(S2, string_length, stdin);
		fseek(stdin, 0, SEEK_END);
		// strip strings of the \n at the end
		temp_S1 = strchr(S1, '\n');   //Get the pointer to char b
		if (temp_S1 != NULL)
		{
			*temp_S1 = '\0';
		}
		temp_S2 = strchr(S2, '\n');   //Get the pointer to char b
		if (temp_S2 != NULL)
		{
			*temp_S2 = '\0';
		}
		printf("---------------------------------------------------------------------------\n");
		printf("You entered\n");
		printf("S1: ");
		puts(S1);
		printf("S2: ");
		puts(S2);
		printf("What would you like to do with the string:\n");
		printf("(input is case sensitive)\n");
		printf("---------------------------------------------------------------------------\n");
		printf("L:	# of chars in string\n\n");
		printf("P:	position of beginning of first occurence of S2 within S1, or returns 0\n\n");
		printf("c:	concatenate S1 followed by S2\n\n");
		printf("C:	returns a substring starting at S1[i] of length j\n\n");
		printf("D:	delete a substring of S1 starting at S[i] and of length j\n\n");
		printf("I:	insert S1 INTO S2 at the S[i]\n\n");
		printf("B:	go back\n\n");

		fseek(stdin, 0, SEEK_END);
		input = getchar();
		fseek(stdin, 0, SEEK_END);

		if ((input == 'b') || (input == 'B'))
		{
			return;
		}
		if ((input == 'L') || (input == 'l'))
		{
			printf("The length of S1 is: %d\n", Length(S1));
			printf("The length of S2 is: %d\n", Length(S2));
			printf("S1 with strlen is: %d\n", strlen(S1));

			continue;

		}
		if ((input == 'P') || (input == 'p'))
		{
			printf("The Position of first occurence of S2 in S1 is %d\n", Pos(S2, S1));
			continue;
		}
		if (input == 'c')
		{
			printf("S1 appended with S2 is: ");
			puts(Concat(S1, S2));
			continue;
		}
		if ((input == 'C'))
		{
			int j = -1;
			int i = -1;
			printf("S1[i] is starting index, j is length of returned copy (index is 1 to n)\n");

	while ( (i < 0) || (i>strlen(S1)) )
			{
				printf("Enter i: ");
				fseek(stdin, 0, SEEK_END);
				i = getchar();
				i = i - '0' - 1;
				fseek(stdin, 0, SEEK_END);
			}
			printf("\nEnter j:");
			fseek(stdin, 0, SEEK_END);
			j = getchar();
			fseek(stdin, 0, SEEK_END);
			j = j - '0';
		
			printf("Your Copied String of length %d, starting at S1[%d]: ",i,j); 
			//StringOutput = _strdup((Copy(S1, i, j)));
			puts((Copy(S1, i, j)));			
		}
		if ((input == 'D') || (input == 'd'))
		{
			printf("Enter amount of string to delete (j):");
			
			fseek(stdin, 0, SEEK_END);
			j = getchar();
			fseek(stdin, 0, SEEK_END);
			j = j - '0';

			printf("\nEnter starting character of string (i) S[i]:");

			fseek(stdin, 0, SEEK_END);
			i = getchar();
			fseek(stdin, 0, SEEK_END);
			i = i - '0';

			printf("Your deleted string is:\n");
			DeleteString(S1, i, j);
			puts(S1);
		
			continue;
		}

		if ((input == 'I') || (input == 'i'))
		{
			printf("\nEnter starting character of S2 insert into S1 (i) S[i]:");

			fseek(stdin, 0, SEEK_END);
			i = getchar();
			fseek(stdin, 0, SEEK_END);
			i = i - '0';

			printf("Your new S1 with inserted S2 string is:\n");
			InsertString(S1, S2, i);
			printf("Your new S1, with S2 inserted as position S1[%d] is:\n", i);
			puts(S1);

			continue;
		}

	} // end while loop waiting for user to hit "back"

}








///BEGIN InsertNode()

// takes the employees supervisor ID, creates a new node in the supervisee group
//with the given passed information
void InsertNode(char *firstname, char *lastname, char *jobtitle, char *ID, char *SupID, int *DOB, struct GroupList *GroupHead)
{
	int i;
	struct SearchResults TempSearchResults;
	int AddToExistingListFlag = 0;
	int CreateNewListFlag = 0;

	struct GroupList *new_node, *walker, *walker2, *temp, *temp2, *head;												//create GroupList Nodes (the row pointers of a matrix)
	temp = NULL;
	head = GroupHead;

	struct Employee *new_emp_node, *emp_walker, *emp_temp, *emp_temp2, *emp_head;
	emp_temp = NULL;



	if (CompareID(SupID, "0000") == 1)															//special case of General Manager
	{																						// dont actually create a node, just populate the first, already existing node		
		head->EmployeePtr->firstname = _strdup(firstname);
		head->EmployeePtr->lastname = _strdup(lastname);
		head->EmployeePtr->jobtitle = _strdup(jobtitle);
		head->EmployeePtr->ID = _strdup(ID);
		head->EmployeePtr->SupID = _strdup(SupID);
		for (i = 0; i <= 8; i++)
		{
			head->EmployeePtr->DOB[i] = DOB[i];
		}
		head->EmployeePtr->supervisor = NULL;//head->EmployeePtr;																					//the only person withh NULL supervisor is GM
		head->EmployeePtr->supervisee = NULL;//head->EmployeePtr;
		printf("This is the General Manager\n");
		return;
	}
	//walk and look for pre-existing group with same supervisor								


	walker2 = GroupHead;																								// beginning of GroupList Row Ptr	
	while (walker2 != NULL) {
		if (CompareID((walker2->EmployeePtr->SupID), SupID) == 1)												// the while loop will return non-null *emp_temp with the employee in supervisee
		{																								//searching for a row where employee has same supervisor ID
			emp_temp = walker2->EmployeePtr;
			walker2 = NULL;
			AddToExistingListFlag = 1; continue;														 //set walker to null to get out of loop, set 
																										 //flag to add to existing LL of Supervisees
		}
		walker2 = walker2->prev;

	}//end of walk UP GroupList Nodes





	walker = GroupHead;																								//beginnging of GroupList Row Ptr
	while (walker != NULL) {																							// walk down.....	
		if (CompareID((walker->EmployeePtr->SupID), SupID))                                                        //if this emp. has same sup, put 
		{																										   ///new employee in this list...
			emp_temp = walker->EmployeePtr;																				//emp found...
			walker = NULL;
			AddToExistingListFlag = 1; continue;
		}																					  // the while loop will return non-null *temp with the employee in supervisee row
																							  //set walker to null to get out of loop, set flag to add to existing LL of Supervisees
		walker = walker->next;
	}//end of walk up GroupList Nodes




	if (AddToExistingListFlag == 1)																		//if here we need to utilize *temp, walk it right, append our new employee
	{

		while (emp_temp->next != NULL)																							//walk right to end
		{
			emp_temp = emp_temp->next;
		}
		//emp_temp now has the old last employee from supervisee ROW list

		new_emp_node = (struct Employee *)malloc(sizeof(struct Employee));						// set aside memory for your new employee
		emp_temp->next = new_emp_node;															// point the old end of list at your new employee
		new_emp_node->next = NULL;															    //point your new employees right side to NULL
		new_emp_node->prev = emp_temp;															///point your new employees left side to the (previous) last employee

		new_emp_node->firstname = _strdup(firstname);                                            //populate the new employees information fields
																								 //printf("firstname inside insert node: %s\n",new_emp_node->firstname);getchar();
		new_emp_node->lastname = _strdup(lastname);
		new_emp_node->jobtitle = _strdup(jobtitle);
		for (int i = 0; i <= 8; i++)
		{
			new_emp_node->DOB[i] = DOB[i];
		}

		new_emp_node->ID = _strdup(ID);
		new_emp_node->SupID = _strdup(SupID);
		new_emp_node->supervisor = emp_temp->supervisor;                                        //set new employees supervisor to same as fellow supervisee's
																								//this employee is already added to his supervisor supervisee list by the algorithm...
		AddToExistingListFlag = 0;																//lower flag, not sure if this is necessary
		return;																				//end of add employee to existing supervisee list. leave the function
	}


	// if we are here, we assume we need to create a new GroupList Node (ROW), and populate it with
	// our new employee. we will walk DOWN, then create GroupList Node, then Emoployee Node
	// if we are here we need to initialize this new employee as an entry point into his supervisors supervisee LL!

	head = GroupHead;
	walker = head;

	while (walker->next != NULL) {
		walker = walker->next;
	}
	new_node = (struct GroupList *)malloc(sizeof(struct GroupList));							// now were at the last GroupList node, so we create 
	walker->next = new_node;																					//a new GroupList Node, point walker's next to it
	new_node->next = NULL;
	new_node->prev = walker;

	new_emp_node = (struct Employee *)malloc(sizeof(struct Employee));								    // set aside memory for your new employee
	new_node->EmployeePtr = new_emp_node;																		// point the old end of list at your new employee
	new_emp_node->prev = NULL;																			//the leftmost employees "prev" does NOT point back to GroupList Row ptr
	new_emp_node->next = NULL;

	new_emp_node->firstname = _strdup(firstname);                                            //populate the new employees information fields
	new_emp_node->lastname = _strdup(lastname);
	new_emp_node->jobtitle = _strdup(jobtitle);
	new_emp_node->ID = _strdup(ID);
	new_emp_node->SupID = _strdup(SupID);
	for (i = 0; i <= 8; i++)
	{
		new_emp_node->DOB[i] = DOB[i];
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TempSearchResults = FindEmployee(SupID, GroupHead);	                                                                 // passing this as &GroupHead as error
	new_emp_node->supervisor = TempSearchResults.EmployeePtr;
	new_emp_node->supervisee = NULL;

	// make this new employees supervisor have this employee as an entry point into their .supervisee Linked List
	new_emp_node->supervisor->supervisee = new_emp_node;


}

//Begin DeleteEmployee(

void DeleteEmployee(struct Employee *EmployeePtr, struct GroupList *GroupHead)
{
	struct Employee *emp_temp, *emp_temp_left, *emp_temp_right;
	struct Employee *head;
	struct GroupList *GroupPtr, *GroupPtrToDelete, *GroupWalker, *GroupTemp, *Temp_GroupHead, *GroupPtrToAdjust;
	GroupPtr = GroupHead;
	Temp_GroupHead = GroupHead;
	GroupTemp = GroupHead;
	GroupWalker = GroupHead;
	GroupPtrToDelete = GroupHead;	// watch this...............
	GroupPtrToAdjust = GroupHead;
	head = EmployeePtr;
	emp_temp = head;
	emp_temp_left = head;
	emp_temp_right = head;

	if (((EmployeePtr->next) == NULL) && ((EmployeePtr->prev) == NULL))										// were deleting only employee in a group.... Find GroupList Ptr to delete
	{																										// by finding a group with the same manager
		while (GroupPtr != NULL) {
			if (GroupPtr->EmployeePtr->supervisor == NULL) { GroupPtr = GroupPtr->next; continue; }
			if (CompareID(GroupPtr->EmployeePtr->supervisor->ID, EmployeePtr->SupID))						// walk DOWN grouplist
			{
				GroupPtrToDelete = GroupPtr;
			}
			GroupPtr = GroupPtr->next;
		}
		/*
		while (GroupPtr != NULL) {
		if (CompareID(GroupPtr->EmployeePtr->supervisor->ID, EmployeePtr->SupID))						// walk DOWN grouplist
		{
		GroupPtrToDelete = GroupPtr;
		}
		GroupPtr = GroupPtr->next;
		}
		*/																									// delete the corresponding grouplist....
		DeleteGroupNode(GroupPtrToDelete, GroupHead);
		if (EmployeePtr->supervisor != NULL)
		{
			EmployeePtr->supervisor->supervisee = head;
			free(EmployeePtr);
		}																									// delete the lone employee....
		return;
	} // end delete of lone employee ...

	if ((head->prev != NULL) && (head->next != NULL))                             // this employee is neither on left or right, hes in middle
	{
		if ((EmployeePtr->supervisor->supervisee) == (EmployeePtr))				//...and hes the entry pt of supervisors supervisees
		{
			emp_temp_left = emp_temp_left->prev;
			emp_temp_right = emp_temp_right->next;
			EmployeePtr->supervisor->supervisee = emp_temp_left; free(emp_temp);
			emp_temp_left->next = emp_temp_right;
			emp_temp_right->prev = emp_temp_left;
			return;
		}


		else {
			emp_temp_left = emp_temp_left->prev;
			emp_temp_right = emp_temp_right->next;
			free(emp_temp);
			emp_temp_left->next = emp_temp_right;
			emp_temp_right->prev = emp_temp_left;
			return;
		}
	}

	if (head->prev == NULL) {													// delete a leftmost employee...


																				// deleting the leftmost employee required that you adjust the GrouplistPTR!
		while (GroupPtr != NULL) {
			if (GroupPtr->EmployeePtr->supervisor == NULL) { GroupPtr = GroupPtr->next; continue; }
			if (CompareID(GroupPtr->EmployeePtr->supervisor->ID, EmployeePtr->SupID))						// find grouplist pointer to adjust..
			{
				GroupPtrToAdjust = GroupPtr;
			}
			GroupPtr = GroupPtr->next;
		}



		if ((EmployeePtr->supervisor->supervisee) == (EmployeePtr))				//...and hes the entry pt of supervisors supervisees
		{
			head = head->next; EmployeePtr->supervisor->supervisee = head;
			GroupPtrToAdjust = head;
			free(emp_temp);
			emp_temp = head;
			head->prev = NULL;
			return;
		}

		else {
			head = head->next;
			GroupPtrToAdjust = head;
			free(emp_temp);
			emp_temp = head;
			head->prev = NULL;
			return;
		}		// replace the supervisors supervisee entry point 

	}

	if (head->next == NULL)                                                  // were deleting the rightmost employee
	{
		if ((EmployeePtr->supervisor->supervisee) == (EmployeePtr))			// and he is a group entry point for supervisors supervisees
		{
			head = head->prev; EmployeePtr->supervisor->supervisee = head;
			free(emp_temp);
			emp_temp = head;
			head->next = NULL;
			return;
		}

		else {
			head = head->prev;
			free(emp_temp);
			emp_temp = head;
			head->next = NULL;
			return;
		}




	}

} // END DeleteEmployee()													

  // BEGIN DeleteSupervisor()
void DeleteSupervisor(struct Employee *EmployeePtr, struct Employee *EmployeeToReplaceSupervisor, struct GroupList *GroupHead)
{
	struct Employee *Temp_Employee, *Temp_Walker, *EmployeeRight, *EmployeeLeft;
	struct GroupList *GroupWalker, *TempGroup;
	GroupWalker = GroupHead;
	TempGroup = GroupHead;

	EmployeeToReplaceSupervisor->SupID = _strdup(EmployeePtr->SupID);                                            // make the new supervisors manager the older supervisors manager...
	EmployeeToReplaceSupervisor->supervisor = EmployeePtr->supervisor;


	if ((EmployeePtr == GroupWalker->EmployeePtr) && (EmployeeToReplaceSupervisor == NULL))                   // were deleting the GM, GM is only employee.....
	{
		EmployeePtr->firstname = _strdup("0000");
		EmployeePtr->ID = _strdup("0000");// just cover up its ID....
		GMExistsFlag = 0;
		return;
	}
	if (EmployeePtr == NULL) { printf("This Employee is not a Supervisor..\n"); return; }                     // verify we are deleting a supervisor...

																											  // see if a grouplist is pointing at this supervisor, if so we need to point it at the new employee
	while (GroupWalker != NULL) {
		if (GroupWalker->EmployeePtr == EmployeePtr)
		{
			GroupWalker->EmployeePtr = EmployeeToReplaceSupervisor;

		}      // if a grouplist is pointing at the supervisor, point it at the replacement supervisor
		GroupWalker = GroupWalker->next;
	}


	if (EmployeeToReplaceSupervisor->next != NULL)
	{
		EmployeeToReplaceSupervisor->next->prev = EmployeeToReplaceSupervisor->prev;
	}																								//link all the supervisee buddies together before moving old supervisee up....
	if (EmployeeToReplaceSupervisor->prev != NULL)
	{
		EmployeeToReplaceSupervisor->prev->next = EmployeeToReplaceSupervisor->next;
	}





	Temp_Employee = EmployeePtr;                                          //point to the old supervisor, remove old supervisor from its Group
	Temp_Walker = EmployeePtr;
	//walk left
	if (EmployeePtr->prev != NULL)
	{
		EmployeePtr->prev->next = EmployeeToReplaceSupervisor;       // link our new supervisor into the old supervisors group, or put him between NULLS if lone
	}
	else { EmployeeToReplaceSupervisor->prev = NULL; }
	if (EmployeePtr->next != NULL)                                    // walk right
	{
		EmployeePtr->next->prev = EmployeeToReplaceSupervisor;
	}
	else { EmployeeToReplaceSupervisor->next = NULL; }


	EmployeeToReplaceSupervisor->SupID = _strdup(EmployeePtr->SupID);                                            // make the new supervisors manager the older supervisors manager...
	EmployeeToReplaceSupervisor->supervisor = EmployeePtr->supervisor;

	free(EmployeePtr);                                                          // free the old manager.....

	GroupWalker = GroupHead;
	while ((GroupWalker != NULL) && (GroupWalker->EmployeePtr != NULL)) {

		if (GroupWalker->EmployeePtr == EmployeeToReplaceSupervisor)		 // if a grouplist is pointing at the supervisee to replace supervisor, point it left, if left NULL, point it right
		{
			if (EmployeeToReplaceSupervisor->prev != NULL) { GroupWalker->EmployeePtr = EmployeeToReplaceSupervisor->prev; }
			if (EmployeeToReplaceSupervisor->next != NULL) { GroupWalker->EmployeePtr = EmployeeToReplaceSupervisor->next; }
			if ((EmployeeToReplaceSupervisor->next == NULL) && (EmployeeToReplaceSupervisor->next == NULL))
			{
				DeleteGroupNode(GroupWalker, GroupHead);
			}         // case of a lone supervisee, need to delete the node

		}

		GroupWalker = GroupWalker->next;
	}

	Temp_Employee = EmployeeToReplaceSupervisor;                                          //point to the old supervisor, remove it from its Group
	Temp_Walker = EmployeeToReplaceSupervisor;

	if (EmployeeToReplaceSupervisor->prev != NULL)
	{
		EmployeeToReplaceSupervisor->prev->next = EmployeeToReplaceSupervisor->next;
	}
	if (EmployeeToReplaceSupervisor->next != NULL)
	{
		EmployeeToReplaceSupervisor->next->prev = EmployeeToReplaceSupervisor->prev;
	}
	//free(Temp_Walker);

	Temp_Walker = EmployeeToReplaceSupervisor->next;													// here we now need to update all supervisee's SupID strings..
	while (Temp_Walker != NULL)																	//walk right... STARTING ONE TO THE RIGHT
	{
		Temp_Walker->SupID = _strdup(EmployeeToReplaceSupervisor->ID);
		Temp_Walker->supervisor = EmployeeToReplaceSupervisor;
		Temp_Walker = Temp_Walker->next;
	}

	Temp_Walker = EmployeeToReplaceSupervisor->prev;												// here we now need to update all supervisee's SupID strings..
	while (Temp_Walker != NULL)                                                               // walk left...STARTING ONE TO THE LEFT
	{
		Temp_Walker->SupID = _strdup(EmployeeToReplaceSupervisor->ID);
		Temp_Walker = Temp_Walker->prev;
	}



}






// BEGIN FindEmployee();

struct SearchResults FindEmployee(char *ID, struct GroupList *GroupHead) {                 // for some reason need to pass a double pointer, this is the second passing of GroupHead
	struct SearchResults SearchResults;                                                     // that probably has something to do with it.....
	SearchResults.Found = 0;
	struct GroupList *walker, *head;
	struct Employee *emp_walker, *emp_head;
	head = GroupHead;                                                                                         //USED TO BE *Grouphead
	walker = head;
	emp_walker = walker->EmployeePtr;



	// nested while loop, walks vertically up all groups, walks horizontally through all employees
	while ((SearchResults.Found == 0) && (walker->prev != NULL)) {						 //check above grouplist...		
		emp_walker = walker->EmployeePtr;
		while (emp_walker != NULL) {
			if (CompareID((emp_walker->ID), ID) == 1)
			{
				SearchResults.EmployeePtr = emp_walker;
				SearchResults.Found = 1; return SearchResults;
			}
			emp_walker = emp_walker->prev;
		}/// end inner while loop
		walker = walker->prev;
	}// end outer while


	walker = head;
	emp_walker = walker->EmployeePtr;

	while ((SearchResults.Found == 0) && (walker != NULL)) {																//check below grouplist...
		emp_walker = walker->EmployeePtr;
		while (emp_walker != NULL) {

			if (CompareID((emp_walker->ID), ID) == 1)
			{
				SearchResults.EmployeePtr = emp_walker; SearchResults.Found = 1; return SearchResults;
			}

			emp_walker = emp_walker->next;                                                              ///THIS BREAKS SHIT
		}
		walker = walker->next;
	}

	SearchResults.EmployeePtr = NULL;
	return SearchResults;
}

// BEGIN Debug()

void Debug() {
	char *chararray[] = { "abx", "abz", "abc", "abb", "abx", "b", "w", "x", "p", "z",NULL };
	char *chartoprint;
	int count = 0;
	chartoprint = chararray[0];          // you will initialize whatever employeee is first on your walking list

	while (chararray[count] != NULL)
	{
		printf("%s", chararray[count]);
		count++;
	}

	int i = 0;
	while (chararray[i] != NULL)
	{
		if (strcmp(chartoprint, chararray[i]) > 0)
		{

			chartoprint = _strdup(chararray[i]);
		}
		i++;
	}
	printf("\n");
	printf("%s", chartoprint);

	currentscreen = 4; getchar(); fseek(stdin, 0, SEEK_END);



}//end debug

																														// BEGIN RemoveNewLineFromString()
void RemoveNewLineFromString(char *InputString)
{
	
	 char *temp = strchr(InputString, '\n');   //Get the pointer to char b
	if (temp == NULL) { return; }
	*temp = '\0';

}
//BEGIN PrintDOB()....

//prints out an int array MMDDYYYY and puts slashes inbetween
void PrintDOB(int temp_DOB[]) {
	int j;
	for (j = 0; j <= 7; j++)								//sizeof does not work when passing to functions 
	{
		if ((j == 2) || (j == 4))                 //put dashes at indices 2 and 5
		{
			printf("/"); printf("%d", temp_DOB[j]);
		}
		else {
			printf("%d", temp_DOB[j]);
		}
	}

}

void PrintEmpID(char *ID) {
	for (int i = 0; i < 4; i++)
	{
		printf("%c", ID[i]);
	}
}


int ValidateDOB(int *temp_DOB) {

	if (temp_DOB[0] > 1 || temp_DOB[1] < 0)  //check ten months
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[1] > 9 || temp_DOB[1] < 0)  //check month singles
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[2] > 3 || temp_DOB[2] <0)  //check tens days
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[3] > 9 || temp_DOB[3] <0)  //check singles days
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[4] > 9 || temp_DOB[4] <0)  //check years 1
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[4] > 9 || temp_DOB[4] <0)  //check years 2
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[4] > 9 || temp_DOB[4] <0)  //check years 3
	{
		printf("Invalid DOB\n"); return 0;
	}
	if (temp_DOB[4] > 9 || temp_DOB[4] <0)  //check years 4
	{
		printf("Invalid DOB\n"); return 0;
	}

	if ((temp_DOB[0] == 1) && (temp_DOB[1]>2 || temp_DOB[1] < 0))
	{
		printf("Invalid DOB\n");
	}
}

int ValidateID(char *ID) {																								// validateID returns 1 if ok, 0 if fucked up
	for (int i = 0; i < 4; i++)
	{
		if (ID[i] > '9' || ID[i] < '0') { return 0; }
	}
	{ return 1; }
}


void FreeList(struct GroupList *GroupHead)
{
	struct GroupList *head, *groupwalker, *group_temp;
	head = GroupHead;										//define and extract the head ptr from your LinkedListStruct
	groupwalker = GroupHead;

	struct Employee *emp_head, *walker, *emp_walker;


	while (groupwalker != NULL)
	{
		emp_walker = groupwalker->EmployeePtr;
		emp_head = emp_walker;
		while (emp_walker != NULL)
		{
			emp_head = emp_head->next;
			free(emp_walker);
			emp_walker = emp_head;
		}

		head = head->next;
		free(groupwalker);
		groupwalker = head;
	}
}

// BEGIN CompareID()
int CompareID(char *ID1, char *ID2)
{
	if ((ID1 == NULL)) { return 1; }
	for (int k = 0; k <= 4; k++)																		// compare (lengthofID -2) bc '\n' and 'NULL'
	{
		if (ID1[k] != ID2[k]) { return 0; }
	}
	return 1;
}
// BEGIN PrintEmployeeInfo()
void PrintEmployeeInfo(struct Employee **EmployeePtr)
{
	char *temp_DOB_string;
	int temp_DOB[9];

	struct Employee *employeeptr;
	employeeptr = *EmployeePtr;
	printf("First Name:"); puts(employeeptr->firstname);
	printf("Last Name:"); puts(employeeptr->lastname);
	printf("Job Title: %s\n", employeeptr->jobtitle); //puts(employeeptr->jobtitle);
	printf("DOB: "); PrintDOB(employeeptr->DOB);
	printf("\nID: "); PrintEmpID(employeeptr->ID);
	printf("\nSupervisor ID: "); PrintEmpID(employeeptr->SupID); printf("\n");
}
// BEGIN PrintEmployeeGroup()
// This function takes an employee ID, prints all workers under same supervisor
// by walking to the left and right of the employee
void PrintEmployeeGroup(struct Employee *EmployeePtr)
{
	struct Employee *emp_walker;
	emp_walker = EmployeePtr;
	if (EmployeePtr == NULL) { printf("Employee does not exist\n"); return; }
	if ((EmployeePtr->next == NULL) && (EmployeePtr->prev == NULL))
		// first see if the employee is alone
	{
		printf("This employee has no co-workers\n"); return;
	}


	//walk left, print employee info
	while (emp_walker != NULL) {
		printf("Name:"); fputs(emp_walker->firstname, stdout); fputs(emp_walker->lastname, stdout);
		printf("\nID: "); PrintEmpID(emp_walker->ID); printf("\n");
		emp_walker = emp_walker->next;
	}/// end inner while loop

	emp_walker = EmployeePtr;														       // walk right, print employee info
	while (emp_walker != NULL) {

		printf("Name:"); fputs(emp_walker->firstname, stdout); fputs(emp_walker->lastname, stdout);
		printf("\nID: "); PrintEmpID(emp_walker->ID); printf("\n");

		emp_walker = emp_walker->next;
	}/// end inner while loop


}
/*
void DeleteNode(struct Employee *EmployeePtr,struct GroupList *GroupPtr)
{
if (EmployeePtr != NULL)                                       // we have an Employee Node to Delete
{

}

if (GroupPtr != NULL)
{

}

}
*/
// BEGIN DeleteGroupNode()

void DeleteGroupNode(struct GroupList *GroupPtrToDelete, struct GroupList *GroupHead)
{
	struct Employee *emp_temp;
	struct Employee *head;
	struct GroupList *GroupPtr, *GroupWalker, *GroupTemp, *Temp_GroupHead;
	GroupPtr = GroupHead;
	Temp_GroupHead = GroupHead;



	Temp_GroupHead = GroupPtrToDelete;
	GroupWalker = GroupPtrToDelete;
	GroupTemp = GroupPtrToDelete;

	// delete the Bottom GroupList Node....
	if (GroupPtrToDelete->next == NULL)
	{
		Temp_GroupHead = Temp_GroupHead->prev;
		free(GroupPtrToDelete);
		if (Temp_GroupHead != NULL)
		{
			Temp_GroupHead->next = NULL;
		}
		return;
	}
	// delete a node that is not top or bottom ......
	if ((GroupPtrToDelete->next != NULL) && (GroupPtrToDelete->prev != NULL))
	{
		GroupWalker = GroupWalker->next;																// walk away from GroupPtrToDelete
		GroupWalker->prev = GroupPtrToDelete->prev;														// make this walker your replacement of GroupPtrToDelete by linking it
		GroupWalker->next = GroupPtrToDelete->next;														 // free it
		free(GroupPtrToDelete);
		return;
	}
}
// BEGIN ChooseEmployeeAlphabetically()
struct Employee *ChooseEmployeeAlphabetically(struct Employee *EmployeePtr, struct GroupList *GroupHead) {
	struct Employee *Temp_Employee, *EmployeeToReplaceSupervisor;
	Temp_Employee = EmployeePtr;

	if (EmployeePtr == NULL) { return NULL; }
	int count = 0;
	char *EmployeeToReplaceSupervisor_String = _strdup(EmployeePtr->lastname);

	EmployeeToReplaceSupervisor = EmployeePtr;          // you will initialize whatever employeee is first on your walking list

	Temp_Employee = Temp_Employee->prev;													// start one left of employee, walk left...
	while (Temp_Employee != NULL)
	{
		if (strcmp(EmployeeToReplaceSupervisor->lastname, Temp_Employee->lastname) > 0)
		{
			EmployeeToReplaceSupervisor_String = _strdup(Temp_Employee->lastname);
			EmployeeToReplaceSupervisor = Temp_Employee;
		}

		if (strcmp(EmployeeToReplaceSupervisor_String, Temp_Employee->lastname) == 0)               // Last names are same, check first names now...
		{
			if (strcmp(EmployeeToReplaceSupervisor->firstname, Temp_Employee->firstname) > 0)
			{
				EmployeeToReplaceSupervisor_String = _strdup(Temp_Employee->lastname);
				EmployeeToReplaceSupervisor = Temp_Employee;                                 //
			}
		}


		Temp_Employee = Temp_Employee->prev;
	}                                                                                        // end while loop walk left

	Temp_Employee = EmployeePtr;
	Temp_Employee = Temp_Employee->next;													// start one RIGHT of employee, walk right...
	while (Temp_Employee != NULL)
	{
		if (strcmp(EmployeeToReplaceSupervisor->lastname, Temp_Employee->lastname) > 0)
		{
			EmployeeToReplaceSupervisor_String = _strdup(Temp_Employee->lastname);
			EmployeeToReplaceSupervisor = Temp_Employee;
		}

		if (strcmp(EmployeeToReplaceSupervisor->lastname, Temp_Employee->lastname) == 0)               // Last names are same, check first names now...
		{
			if (strcmp(EmployeeToReplaceSupervisor->firstname, Temp_Employee->firstname) > 0)
			{
				EmployeeToReplaceSupervisor_String = _strdup(Temp_Employee->lastname);
				EmployeeToReplaceSupervisor = Temp_Employee;                                 //
			}
		}


		Temp_Employee = Temp_Employee->next;
	}                                                                                        // end while loop walk left


	printf("\n");
	printf("The Following Empployee will replace this manager: ");
	printf("First Name: %s Last Name:%s", EmployeeToReplaceSupervisor->firstname, EmployeeToReplaceSupervisor->lastname);
	return EmployeeToReplaceSupervisor;
	//currentscreen = 4;
	//getchar(); fseek(stdin, 0, SEEK_END);

}//end ChooseEmployeeAlphabetically
