/*
ID  2022A7PS0144P					Name Prateek Parwani
ID  2022A7PS0183P					Name Sarat Srinadhu
ID  2022A7PS1181P					Name Narasimha Naidu  
ID  2022A7PS1178P 					Name Sanjay Jampani
*/

#ifndef LOOKUPTABLEH
#define LOOKUPTABLEH
#include "lookuptableDef.h"
#include<stdio.h>
// polynomial rolling function to be used
int binExpo(int a, int b);
int kmult(int a, int b);
int hash(char *lexeme, int i);
void initialize();
int h1(char *lexeme);
int h2(int h1_hashvalue);
int search(char *lexeme);
int insert(char *lexeme, token_name tkn);



#endif
