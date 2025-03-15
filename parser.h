/*
  Group Number 40
  ID  2022A7PS0144P					Name Prateek Parwani
  ID  2022A7PS0183P					Name Sarat Srinadhu
  ID  2022A7PS1178P					Name Sanjay Jampani 
  ID  2022A7PS1181P					Name Narasimha Naidu
*/

#pragma once
#include "parserDef.h"
#include "utils.h"
#include "lexer.h"
#include "tree.h"

Grammar readGrammar(char* filename);
void inOrderTraversal(treeN* node, Grammar grammar, int* count, FILE *outfile);
void computeIndividualFirst(Grammar grammar, FirstFollowSet* firstFollow, int nonTerminalIndex);
void computeIndividualFollow(Grammar grammar, FirstFollowSet* firstFollow, int nonTerminalIndex, int previousIndex);
void displayParseTable(Grammar grammar, ParseTable* table);
int getRuleNumberFromIndices(int ruleIndex, int alternativeIndex, Grammar grammar);
ParseTable* initializeParseTable(int numNonTerminals, int numTerminals);
treeN parseSourceCode(char* sourceFile, Grammar grammar, ParseTable* table, int* errorFlag);
void printParseTree(treeN* root, Grammar grammar, int* count , FILE *outfile);
void createParseTable(Grammar grammar, FirstFollowSet* firstFollow, ParseTable* table);
FirstFollowSet* computeFirstFollowSets(Grammar grammar);
