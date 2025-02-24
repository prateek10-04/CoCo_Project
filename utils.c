#include "utils.h"

int findIndex(char** array, int size, char* target) {
    for (int i = 0; i < size; i++) {
        if (strcmp(target, array[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void printArray(char** array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", array[i]);
    }
}

int isInArray(int* array, int num, int size) {
    for (int i = 0; i < size; i++) {
        if (num == array[i]) {
            return 1;
        }
    }
    return 0;
}

void printRule(Grammar grammarObj, int ruleIndex, int alternativeIndex) {
    printf("\n\nPrinting rule number: %d\n", grammarObj.rules[ruleIndex].ruleNumber);
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
        printf("\n\n\n");
    } else {
        for (int sym = 0; sym < grammarObj.rules[ruleIndex].alternatives[alternativeIndex].count; sym++) {
            if (grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].symbolType == 0)
                printf("%s ", grammarObj.nonTerminals[grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].value]);
            else
                printf("%s ", grammarObj.terminals[grammarObj.rules[ruleIndex].alternatives[alternativeIndex].symbols[sym].value]);
        }
        printf("\n\n\n");
    }
}

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
        printf("\n\n");
    }
}
