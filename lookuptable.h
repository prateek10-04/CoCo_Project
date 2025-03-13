/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
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