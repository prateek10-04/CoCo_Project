/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
#include "lookuptable.h"
#include "lexerDef.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int dfa_state;
token_name actualToken(token_name expected, char *lexeme);
int range_match(char a, char start, char end);
char* accept(bool isLastUseful,twinBuffer *B);
char * reject(twinBuffer *B);
void populateToken(tokenInfo *TOK, token_name t, char * lexeme, int lineNo);
twinBuffer *initializeTwinBuffer();
char next_char(twinBuffer *B, FILE *fp, bool isComment, bool isLengthExceeded);
void reset(int amt,twinBuffer *B);
int char_match(char a, char b);
FILE *getStream(twinBuffer *B, FILE *fp, int bufferNumber);
tokenInfo getNextToken(twinBuffer *B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
void clearTwinBuffer(twinBuffer *B_ref);
void removeComments(char *testcaseFile, char *cleanFile);
void prettyPrint(char *testcaseFile); 
