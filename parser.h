#pragma once
#include "parserDef.h"
// #include "utils.h"
// #include "lexer.h"
// #include "tree.h"

Grammar readGrammar(char* filename);
// void inOrderTraversal(treeN* node, Grammar grammar, int* count);
// void computeIndividualFirst(Grammar grammar, FirstFollowSet* firstFollow, int nonTerminalIndex);
// void computeIndividualFollow(Grammar grammar, FirstFollowSet* firstFollow, int nonTerminalIndex, int previousIndex);
// void displayParseTable(Grammar grammar, ParseTable* table);
// int getRuleNumberFromIndices(int ruleIndex, int alternativeIndex, Grammar grammar);
// ParseTable* initializeParseTable(int numNonTerminals, int numTerminals);
// treeN parseSourceCode(char* sourceFile, Grammar grammar, ParseTable* table, int* errorFlag);
// void printParseTree(treeN* root, Grammar grammar, int* count);
// void createParseTable(Grammar grammar, FirstFollowSet* firstFollow, ParseTable* table);
// FirstFollowSet* computeFirstFollowSets(Grammar grammar);
