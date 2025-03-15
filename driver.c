/*
    Group Number 40
    ID  2022A7PS0144P					Name Prateek Parwani
    ID  2022A7PS0183P					Name Sarat Srinadhu
    ID  2022A7PS1178P					Name Sanjay Jampani 
    ID  2022A7PS1181P					Name Narasimha Naidu
*/

#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLEANED_FILE "cleanedFile.txt"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }

    char *sourceFile = argv[1];
    char *parseTreeFile = argv[2];

    
    printf("Reading grammar file...\n");
    Grammar grammar = readGrammar("grammar.txt");
    printf("Grammar file read successfully!\n");

    
    initialize();
    printf("Lexer/parser modules initialized.\n");

    
    FirstFollowSet *ffSet = computeFirstFollowSets(grammar);
    printf("Computed FIRST and FOLLOW sets.\n");

    
    ParseTable *parseTable = initializeParseTable(grammar.numNonTerminals, grammar.numTerminals);
    createParseTable(grammar, ffSet, parseTable);
    printf("Parse table created successfully.\n");

    int choice;
    while (1) {
        printf("\nSelect an option:\n");
        printf("0: Exit\n");
        printf("1: Remove Comments\n");
        printf("2: Print Token List\n");
        printf("3: Parse & Print Errors (Parse tree output to file)\n");
        printf("4: Execution Time\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid option.\n");
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 0) {
            printf("Exiting program...\n");
            break;
        } else if (choice == 1) {
            removeComments(sourceFile, CLEANED_FILE);
            printf("Comments removed and saved to %s\n", CLEANED_FILE);
        } else if (choice == 2) {
            prettyPrint(sourceFile);
            printf("Lexical analysis completed.\n");
        } else if (choice == 3) {
            removeComments(sourceFile, CLEANED_FILE);
            int errorFlag = 0;
            treeN parseTree = parseSourceCode(CLEANED_FILE, grammar, parseTable, &errorFlag);

            if (errorFlag) {
                printf("Errors encountered in syntax/lexical analysis.\n");
            } else {
                printf("Parsing completed successfully.\n");
            }

            FILE *outFile = fopen(parseTreeFile, "w");
            if (!outFile) {
                perror("Error opening output file");
            } else {
                int count = 0;
                printParseTree(&parseTree, grammar, &count, outFile);
                fclose(outFile);
                printf("Parse tree written to %s\n", parseTreeFile);
            }
        } else if (choice == 4) {
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
        } else {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    return 0;
}
