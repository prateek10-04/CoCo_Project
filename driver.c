#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLEANED_FILE "cleanedFile.txt"

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }
    
    char *sourceFile = argv[1];
    char *parseTreeFile = argv[2];

    // Read the grammar from file
    char grammarFileName[] = "grammar.txt";
    printf("Reading grammar file: %s\n", grammarFileName);
    Grammar grammar = readGrammar(grammarFileName);
    printf("Grammar file read successfully!\n");

    // Initialize required components
    printf("Initializing lexer/parser modules...\n");
    initialize();
    printf("Lexer/parser modules initialized! (Lexical analyzer developed)\n");

    printf("Computing FIRST and FOLLOW sets...\n");
    FirstFollowSet* ffSet = computeFirstFollowSets(grammar);
    printf("FIRST and FOLLOW sets computed! (Syntax analysis module implemented)\n");

    printf("Initializing Parse Table...\n");
    ParseTable* parseTable = initializeParseTable(grammar.numNonTerminals, grammar.numTerminals);
    printf("Parse Table initialized!\n");

    printf("Creating Parse Table...\n");
    createParseTable(grammar, ffSet, parseTable);
    printf("Parse Table created! (Lexical & syntax analysis implemented)\n");

    int choice;
    while(1) {
        printf("\nSelect an option:\n");
        printf("0: Exit\n");
        printf("1: Remove Comments\n");
        printf("2: Print Token List\n");
        printf("3: Parse & Print Errors (Parse tree output to file)\n");
        printf("4: Execution Time\n");
        printf("Enter choice: ");
        
        if(scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid option.\n");
            while(getchar() != '\n'); // Clear input buffer
            continue;
        }

        if(choice == 0) {
            printf("Exiting program...\n");
            break;
        }
        else if(choice == 1) {
            printf("Removing comments from source code...\n");
            removeComments(sourceFile, CLEANED_FILE);
            printf("Comments removed. Output written to %s\n", CLEANED_FILE);
            printf("(Lexical analyzer module developed)\n");
        }
        else if(choice == 2) {
            printf("Performing lexical analysis and printing tokens...\n");
            prettyPrint(sourceFile);
            printf("(Lexical analyzer module successfully executed)\n");
        }
        else if(choice == 3) {
            printf("Performing syntax analysis...\n");
            removeComments(sourceFile, CLEANED_FILE);
            
            int errorFlag = 0;
            treeN parseTree = parseSourceCode(CLEANED_FILE, grammar, parseTable, &errorFlag);
            
            if(errorFlag)
                printf("There were syntactic/lexical errors in the source code (see above for details).\n");
            else
                printf("Parsing completed successfully! (Syntax analysis module successfully executed)\n");

            if (freopen(parseTreeFile, "w", stdout) == NULL) {
                perror("Error redirecting stdout to output file");
            } else {
                int count = 0;
                printParseTree(&parseTree, grammar, &count);
                fflush(stdout);
                freopen("CON", "w", stdout);
            }
            printf("Parse tree written to %s\n", parseTreeFile);
        }
        else if(choice == 4) {
            printf("Measuring execution time for parsing...\n");
            removeComments(sourceFile, CLEANED_FILE);
            int errorFlag = 0;
            clock_t start_time = clock();
            treeN parseTree = parseSourceCode(CLEANED_FILE, grammar, parseTable, &errorFlag);
            (void) parseTree;
            clock_t end_time = clock();
            double total_CPU_time = (double)(end_time - start_time);
            double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU time: %lf\n", total_CPU_time);
            printf("Total CPU time in seconds: %lf\n", total_CPU_time_in_seconds);
            printf("(Execution time measurement completed successfully)\n");
        }
        else {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    
    return 0;
}
