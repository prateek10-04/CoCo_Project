#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "utils.h"

// Helper function to print FIRST and FOLLOW sets for each non-terminal.
void printFirstFollow(Grammar grammar, FirstFollowSet* ff) {
    for (int i = 0; i < grammar.numNonTerminals; i++) {
        printf("Non-Terminal: %s\n", grammar.nonTerminals[i]);
        
        // Print FIRST set (each non-terminal may have FIRST sets per alternative)
        printf("  FIRST: ");
        for (int alt = 0; alt < grammar.rules[i].numAlternatives; alt++) {
            printf("{ ");
            for (int j = 0; j < ff[i].numFirst[alt]; j++) {
                int termIndex = ff[i].first[alt][j];
                printf("%s ", grammar.terminals[termIndex]);
            }
            printf("} ");
        }
        printf("\n");
        
        // Print FOLLOW set (union of follow symbols)
        printf("  FOLLOW: { ");
        for (int j = 0; j < ff[i].numFollow; j++) {
            printf("%s ", grammar.terminals[ff[i].follow[j]]);
        }
        printf("}\n\n");
    }
}

int main() {
    char grammarFilename[] = "grammar.txt";
    
    // Read the grammar from the file.
    Grammar grammarObj = readGrammar(grammarFilename);
    
    // Print basic grammar information.
    printf("Total Grammar Rules: %d\n\n", grammarObj.totalRules);
    
    printf("Non-Terminals (%d):\n", grammarObj.numNonTerminals);
    for (int i = 0; i < grammarObj.numNonTerminals; i++) {
        printf("  %s\n", grammarObj.nonTerminals[i]);
    }
    printf("\n");
    
    printf("Terminals (%d):\n", grammarObj.numTerminals);
    for (int i = 0; i < grammarObj.numTerminals; i++) {
        printf("  %s\n", grammarObj.terminals[i]);
    }
    printf("\n");
    
    // Test printing a single rule (all alternatives).
    printRule(grammarObj, 0, -1);
    
    // Test detailed grammar printing.
    prettyPrintGrammar(grammarObj);
    
    // Compute FIRST and FOLLOW sets.
    FirstFollowSet* ff = computeFirstFollowSets(grammarObj);
    
    // Print the computed FIRST and FOLLOW sets.
    printFirstFollow(grammarObj, ff);
    
    // (Optionally free allocated memory here)
    
    return 0;
}
