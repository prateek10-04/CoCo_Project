// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

// Functions for token processing
const char *getTokenTypeString(TokenType type);

// Hash table functions
void initializeHashTable(struct HashMap *table);
void insertKeyword(struct HashMap *table, char *key, TokenType token);
TokenType lookupKeyword(struct HashMap *table, char *key);
int hashFunction(struct HashMap *table, char *key);

// File buffer management
FILE *loadStream(FILE *fp, int bufferSection);
void monitorBufferBounds(void);
TokenInfo retrieveNextToken(void);

// Comment handling
void stripComments(char *inputFile);
long getFileByteSize(FILE *fp);
void displayFileContents(FILE *fp);
void printTokenStream(char *fileName);

#endif // LEXER_H
