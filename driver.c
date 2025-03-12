#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLEANED_FILE "cleanedFile.txt"

void displayImplementationStatus() {
    printf("\nImplementation Status:\n");
    printf("(a) FIRST and FOLLOW set automated\n");
    printf("(b) Lexical analyzer module developed\n");
    printf("(c) Both lexical and syntax analysis modules implemented\n");
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }
    
    char *sourceFile = argv[1];
    char *parseTreeFile = argv[2];

    displayImplementationStatus();

    // Read the grammar from file (ensure grammar.txt exists in your working directory)
    char grammarFileName[] = "grammar.txt";
    Grammar grammar = readGrammar(grammarFileName);
    
    // Initialize required components
    initialize();  // Initialization for lexer/parser modules (if required)
    FirstFollowSet* ffSet = computeFirstFollowSets(grammar);
    ParseTable* parseTable = initializeParseTable(grammar.numNonTerminals, grammar.numTerminals);
    createParseTable(grammar, ffSet, parseTable);

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
        }
        else if(choice == 2) {
            printf("Performing lexical analysis and printing tokens...\n");
            prettyPrint(sourceFile);
        }
        else if(choice == 3) {
            printf("Performing syntax analysis...\n");
            // First, remove comments from the source code.
            removeComments(sourceFile, CLEANED_FILE);
            
            // Invoke both the lexer and parser to print lexical and syntactic errors on the console.
            int errorFlag = 0;
            treeN parseTree = parseSourceCode(CLEANED_FILE, grammar, parseTable, &errorFlag);
            
            if(errorFlag)
                printf("There were syntactic/lexical errors in the source code (see above for details).\n");
            else
                printf("Parsing completed successfully.\n");
            
            // Write the parse tree to the output file.
            // We use freopen to redirect stdout temporarily so that the parse tree is written into the file.
            if (freopen(parseTreeFile, "w", stdout) == NULL) {
                perror("Error redirecting stdout to output file");
            } else {
                int count = 0;
                printParseTree(&parseTree, grammar, &count);
                fflush(stdout);
                // Restore stdout to console.
                // On Windows, "CON" refers to the console; on Unix systems, use "/dev/tty" if needed.
                freopen("CON", "w", stdout);
            }
            printf("Parse tree written to %s\n", parseTreeFile);
        }
        else if(choice == 4) {
            printf("Measuring execution time for parsing...\n");
            // Remove comments and then measure the time taken to parse the cleaned file.
            removeComments(sourceFile, CLEANED_FILE);
            int errorFlag = 0;
            clock_t start_time = clock();
            treeN parseTree = parseSourceCode(CLEANED_FILE, grammar, parseTable, &errorFlag);
            clock_t end_time = clock();
            double total_CPU_time = (double)(end_time - start_time);
            double total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU time: %lf\n", total_CPU_time);
            printf("Total CPU time in seconds: %lf\n", total_CPU_time_in_seconds);
        }
        else {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    
    return 0;
}
