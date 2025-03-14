/*
  ID  2022A7PS0144P					Name Prateek Parwani
  ID  2022A7PS0183P					Name Sarat Srinadhu
  ID  2022A7PS1178P					Name Sanjay Jampani 
  ID  2022A7PS1181P					Name Narasimha Naidu
*/

#pragma once
#include <stdio.h>
#include <string.h>
#include "parserDef.h"

int findIndex(char** array, int size, const char* target);
void printArray(char** array, int size);
int isInArray(int* array, int num, int size);
void printRule(Grammar grammarObj, int ruleIndex, int alternativeIndex);
void prettyPrintGrammar(Grammar grammarObj);
