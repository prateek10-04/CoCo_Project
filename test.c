#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main() {
    // The grammar file should contain productions in the expected format.
    // For example, it might be named "grammar.txt" and include productions like:
    //
    // <program> ===> <otherFunctions> <mainFunction>
    // <mainFunction> ===> TK_MAIN <stmts> TK_END
    // ...
    //
    // Ensure that the grammar file is formatted as expected.
    char grammarFilename[] = "grammar.txt";
    
    // Read the grammar from file
    Grammar grammarObj = readGrammar(grammarFilename);

    // Print the total number of rules
    printf("Total Grammar Rules: %d\n", grammarObj.totalRules);

    // Print the list of non-terminals
    printf("Non-terminals (%d):\n", grammarObj.numNonTerminals);
    for (int i = 0; i < grammarObj.numNonTerminals; i++) {
        printf("  %s\n", grammarObj.nonTerminals[i]);
    }

    // Print the list of terminals
    printf("Terminals (%d):\n", grammarObj.numTerminals);
    for (int i = 0; i < grammarObj.numTerminals; i++) {
        printf("  %s\n", grammarObj.terminals[i]);
    }
    
    // Additional testing could involve computing first/follow sets, creating a parse table,
    // and parsing a sample source file if those functions are implemented.
    // For example:
    // FirstFollowSet* ff = computeFirstFollowSets(grammarObj);
    // ParseTable* table = initializeParseTable(grammarObj.numNonTerminals, grammarObj.numTerminals);
    // createParseTable(grammarObj, ff, table);
    // treeN parseTree = parseSourceCode("testSource.txt", grammarObj, table, &errorFlag);
    //
    // Free allocated memory as needed.
    
    return 0;
}
