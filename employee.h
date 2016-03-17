
/*
Name: Gary Koers Jr.
Class : ECE359
Homework #4

this file contains extern declarations of all functions and variables that will be defined in employee_lib.c 
and will be used in the main function (employee.c)
*/
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define firstnamelength 256
#define lastnamelength 256
#define jobtitlelength 256
#define useridlength 4
#define supervisoridlength 4

/*
// Variable declarations
extern int GMExistsFlag = 0;
extern int mainmenuselection;
extern int searchselection;																		 //it is recommended to use char as opposed to int for getc()
extern int deleteselection;
extern int insertselection;
extern int currentscreen = 0;
extern int debugselection;
extern int DOB_PROBLEM = 1;																			 //flag used to trap user until valid DOB entered
extern int numberofemployees = 0;
extern int i;
extern bool firstemployeeflag = true;
extern bool GMflag = false;
extern char *back = "b\n";
extern char USERINPUT_PROBLEM = 0;
extern int SupervisorID_Problem = 0;
*/





// Function Declarations


extern int Insert(struct GroupList *GroupHead);																					//were at least defined as existing, actual code is after main
extern void InsertNodeToExistingGroup(char firstname, char lastname, char jobtitle, char ID, char SupID, int DOB, struct LinkedList *LinkedListStructPtr);
extern void PrintDOB(int temp_DOB[]);
extern void RemoveNewLineFromString();
extern void PrintEmpID(char ID[4]);
extern void InsertNode(char *firstname, char *lastname, char *jobtitle, char *ID, char *SupID, int *DOB, struct GroupList *GroupHead);
extern int  ValidateDOB(int *DOB);
extern char *GetUserFirstName();																  //this is a function that returns a character pointer
extern char *GetUserLastName();
extern char *GetUserJobTitle();
extern char *GetUserID();
extern int *GetUserDOB();																			//this is a function that returns an int pointer
extern int ValidateID(char *ID);
extern struct SearchResults FindEmployee(char *ID, struct GroupList *GroupHead);
extern void FreeList(struct GroupList *GroupHead);

extern void Debug();
extern int MainMenu(struct GroupList *head);
extern int Search();

extern void StringMenu();


extern int CompareID(char *ID1, char *ID2);
extern void StringCopy(char *s1, char *s2, int sizeofstring);
extern void PrintEmployeeInfo(struct Employee *EmployeePtr);
extern void PrintEmployeeGroup(struct Employee *EmployeePtr);
extern void DeleteEmployee(struct Employee *EmployeePtr, struct GroupList *GroupHead);
extern void DeleteGroupNode(struct GroupList *GroupPtrToDelete, struct GroupList *GroupHead);
extern void DeleteSupervisor(struct Employee *EmployeePtr, struct Grouplist *GroupHead);
extern struct Employee *ChooseEmployeeAlphabetically(struct Employee *EmployeePtr, struct GroupList *GroupHead);
extern void DeleteSupervisor(struct Employee *EmployeePtr, struct Employee *EmployeeToReplaceSupervisor, struct GroupList *GroupHead);

// End function declarations

// struct definitions
 struct SearchResults {                                                                          // struct that returns 1 if found, 0 if not found, returns a pointer pointing to employee
	int Found;
	struct Employee *EmployeePtr;
};

//the head pointer will point to the frist in the GroupPointer list.
 typedef struct LinkedList {																	//if a new supervisor
	int EmployeeCount;
	struct GroupList *head;
};

//"Employee" is now a keyword referring to an employee struct
 typedef struct Employee {
	//had to change these from array to pointer, otherwise they couldnt be modified	
	char *firstname;
	char *lastname;
	char *jobtitle;
	char *ID;
	char *SupID;
	int *DOB[8];
	int count;																				//if employee is 5th entered in system, count =5

	struct Employee *supervisor; //struct node *supervisor;
	struct Employee *supervisee;//struct node *supervisees;
	struct Employee *prev;
	struct Employee *next;
};



typedef struct GroupList {
	struct Employee *EmployeePtr;
	struct GroupList *prev;
	struct GroupList *next;

};

#endif
