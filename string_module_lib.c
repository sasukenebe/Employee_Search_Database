/*
Gary Koers Jr.
ECE 359
HW4

This is the string module, it defines all the basic string functions, these can all be called in employee.c in the main menu
in the main menu hit R, and the options will be made clear, please put in reasonable numbers, ideally between 1-10 for now, as
i have not fully debugged
*/

#include "string_module.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



int Length(char *S)                  // number of characters in a string S
{
	int i=0;
	while(S[i]!=NULL)
	{
		i++;
	}
	return i;
}		
																																// BEGIN Pos()
int Pos(char *Substring, char *S)     //position of beginning of first occurence of a subtring within S, or returns 0
{
	int i = 0;
	int j = 0;
	int temp_marker = 0;
	char *char_marker=NULL;
	if ((S[0] == NULL) && (Substring[0] == NULL))				// special case, both strings are NULL
	{
		printf("Both strings are NULL\n");
		return 0;
	}

	while ((S[i] != NULL)&&(j!=strlen(Substring)))											// loop until at end of S1
	{
		if ((S[i] == Substring[j]) && (j == 0))						// first match
		{
			temp_marker = i;
			i++;
			j++;
			continue;
		}

		if ((S[i] == Substring[j]) && (j != 0))						// j-th match match
		{
			j++;
			i++;
			continue;
		}
		if (S[i] != Substring[j])										// no match
		{
			j = 0;
			i++;
			temp_marker = 0;
		}

	} // end while loop


	if (j == strlen(Substring))            // if the last characters
	{
		return temp_marker+1;
	}
	else return 0;
}
char *Concat(char *S1, char *S2)      // concatenate S1 followed by S2
{
	int i = 0; int j = 0;
	while (S1[i] != NULL)
	{
		i++;
	}
	while (S2[j]!=NULL)
	{
		S1[i] = S2[j];
		i++;
		j++;
	}
	S1[i] = S2[j];					// append the null terminator
	return S1;
}

char *Copy(char *S, int i, int j)     // returns a substring starting at S[i] of length j
{
	char *ReturnString;
	ReturnString = (char *)malloc(100 * sizeof(char));
	int k = 0;
	for (k = 0; k != j; k++)
	{
		if (S[i + k]==NULL)          // we hit the null terminator of S before getting to j, so just end the 
		{

			break;
		}
		ReturnString[k]=S[i + k];
	}
	ReturnString[k] = NULL;// on j-th iteration, we need to put the null terminator (this is because of strlen starting at 0, and k starting at 0)
	return ReturnString;
}

void DeleteString(char *S,int i, int j)    // delete a substring of S starting at S[i] and of length j
{
	int k = 0;
	
	if ((i + j) > strlen(S))
	{
		S[i] = NULL;
		return;
	}

	for (k = 0; S[i + j + k] != NULL; k++)          
	{
		S[i+k] = S[i + j + k];
	}

	S[i + k] = NULL;
}

void InsertString(char *S1,char *S2, int i) // insert S1 INTO S2 at the S[i]
{

	int k = 0;
	int j = 0;
	char *Temp_S1;
	Temp_S1 = (char *)malloc((100)*sizeof(char));

	for (k = 0; k < i;k++)
	{
		Temp_S1[k] = S1[k];
	}

	for (k = 0; S2[k] != NULL; k++)
	{
		Temp_S1[i + k] = S2[k];
	}

	for (k = 0; S1[i + k] != NULL; k++)
	{
		Temp_S1[i+strlen(S2)+ k] = S1[i+k];
	}
	Temp_S1[i+strlen(S2)+ k] = NULL;

	for (k = 0; Temp_S1[k] != NULL;k++)
	{
		S1[k] = Temp_S1[k];
	}
	S1[k] = NULL;

}

 char *GetString()
{
	size_t string_length = STRING_LENGTH;
	char S[STRING_LENGTH];
	fseek(stdin, 0, SEEK_END);
	fgets(S,string_length, stdin);
	fseek(stdin, 0, SEEK_END);
	return S;
}

 void PrintString(char *S)
{
	int i = 0;
	while (S[i] != NULL)
		printf("%c",S[i]);
	i++;
 
 }