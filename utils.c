/*
    ID  2022A7PS0144P					Name Prateek Parwani
    ID  2022A7PS0183P					Name Sarat Srinadhu
    ID  2022A7PS1178P					Name Sanjay Jampani 
    ID  2022A7PS1181P					Name Narasimha Naidu
*/

#include "utils.h"


// Finds the index of a target string in an array, returns index or -1 if not found.
int findIndex(char** array, int size,const char* target) {
    for (int i = 0; i < size; i++) {
        if (strcmp(target, array[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// Prints all elements of a string array.
void printArray(char** array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", array[i]);
    }
}

// Checks if a number exists in an integer array, returns 1 if found, else 0.
int isInArray(int* array, int num, int size) {
    for (int i = 0; i < size; i++) {
        if (num == array[i]) {
            return 1;
        }
    }
    return 0;
}

// Prints a specific grammar rule, supporting all or a specific alternative.
void printRule(Grammar grammarObj, int ruleIndex, int alternativeIndex) {
    printf("\nPrinting rule number: %d\n", grammarObj.rules[ruleIndex].ruleNumber);
    printf("LHS: %s\n", grammarObj.nonTerminals[grammarObj.rules[ruleIndex].leftHandSide]);
    printf("RHS: ");
    if (alternativeIndex == -1) {
        for (int alt = 0; alt < grammarObj.rules[ruleIndex].numAlternatives; alt++) {
            for (int sym = 0; sym < grammarObj.rules[ruleIndex].alternatives[alt].count; sym++) {
                if (grammarObj.rules[ruleIndex].alternatives[alt].symbols[sym].symbolType == 0)
                    printf("%s ", grammarObj.nonTerminals[grammarObj.rules[ruleIndex].alternatives[alt].symbols[sym].value]);
                else
                    printf("%s ", grammarObj.terminals[grammarObj.rules[ruleIndex].alternatives[alt].symbols[sym].value]);
            }
            if (alt < grammarObj.rules[ruleIndex].numAlternatives - 1)
                printf("| ");
        }
        printf("\n");
    } else {
        for (int sym = 0; sym < grammarObj.rules[ruleIndex].alternatives[alternativeIndex].count; sym++) {
            if (grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].symbolType == 0)
                printf("%s ", grammarObj.nonTerminals[grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].value]);
            else
                printf("%s ", grammarObj.terminals[grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].value]);
        }
        printf("\n");
    }
}

// Prints the entire grammar in a structured format.
void prettyPrintGrammar(Grammar grammarObj) {
    for (int i = 0; i < grammarObj.totalRules; i++) {
        printf("Rule Number: %d ", i);
        for (int alt = 0; alt < grammarObj.rules[i].numAlternatives; alt++) {
            printf("\n\tRHS Alternative Index: %d ", alt);
            for (int sym = 0; sym < grammarObj.rules[i].alternatives[alt].count; sym++) {
                printf("\n\t\tSymbol Type: %d, Symbol ID: %d",
                       grammarObj.rules[i].alternatives[alt].symbols[sym].symbolType,
                       grammarObj.rules[i].alternatives[alt].symbols[sym].value);
            }
        }
        printf("\n");
    }
}
