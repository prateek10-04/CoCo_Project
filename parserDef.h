#pragma once
#include <stdio.h>

typedef struct GrammarSymbol {
    int symbolType; 
    int value;
} GrammarSymbol;

typedef struct ProductionAlternative {
    GrammarSymbol* symbols;
    int count;
} ProductionAlternative;

typedef struct GrammarRule {
    int leftHandSide;
    ProductionAlternative* alternatives;
    int numAlternatives;
    int ruleNumber;
    int hasEpsilon;
} GrammarRule;

typedef struct FirstFollowSet {
    int* numFirst;
    int numFollow;
    int** first;
    int* follow;
} FirstFollowSet;

typedef struct Grammar {
    GrammarRule* rules;
    int totalRules;
    char** nonTerminals;
    char** terminals;
    int numNonTerminals;
    int numTerminals;
    FirstFollowSet* firstFollow;
} Grammar;

typedef struct ParseTableEntry {
    int ruleIndex;
    int alternativeIndex;
    int isError;
} ParseTableEntry;

typedef struct ParseTable {
    ParseTableEntry **entries;
} ParseTable;