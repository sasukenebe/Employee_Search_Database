#ifndef STRING_MODULE_H
#define STRING_MODULE_H


#define STRING_LENGTH 100

extern int Length(char *S);                   // number of characters in a string S
extern int Pos(char *Substring, char *S);     //position of beginning of first occurence of a subtring within S, or returns 0
extern char *Concat(char *S1, char *S2);      // concatenate S1 followed by S2
extern char *Copy(char *S, int i, int j);     // returns a substring starting at S[i] of length j
extern void DeleteString(char *S, int i, int j);    // delete a substring of S starting at S[i] and of length j
extern void InsertString(char *S1, char *S2, int i); // insert S1 INTO S2 at the S[i]
extern char *GetString();
extern void PrintString(char *S);




#endif