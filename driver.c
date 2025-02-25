#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void displayImplementationStatus() {
    printf("\nImplementation Status:\n");
    printf("(a) FIRST and FOLLOW set automated\n");
    printf("(b) Lexical analyzer module developed\n");
    printf("(c) Both lexical and syntax analysis modules implemented\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }
    
    char *sourceFile = argv[1];
    char *parseTreeFile = argv[2];
    int choice;
    
    displayImplementationStatus();
    
    while (1) {
        printf("\nSelect an option:\n");
        printf("0: Exit\n1: Remove Comments\n2: Print Token List\n3: Parse & Print Errors\n4: Execution Time\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid option.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        
        switch (choice) {
            case 0:
                printf("Exiting program...\n");
                return 0;
            
            case 1:
                printf("Removing comments from source code...\n");
                removeComments(sourceFile);
                break;
            
            case 2:
                printf("Performing lexical analysis and printing tokens...\n");
                printLexemes(sourceFile);
                break;
            
            case 3:
                printf("Performing syntax analysis...\n");
                parseInput(sourceFile, parseTreeFile);
                break;
            
            case 4: {
                printf("Measuring execution time...\n");
                clock_t start_time, end_time;
                double total_CPU_time, total_CPU_time_in_seconds;
                
                start_time = clock();
                parseInput(sourceFile, parseTreeFile);
                end_time = clock();
                
                total_CPU_time = (double)(end_time - start_time);
                total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
                
                printf("Total CPU time: %lf\n", total_CPU_time);
                printf("Total CPU time in seconds: %lf\n", total_CPU_time_in_seconds);
                break;
            }
            
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }
    
    return 0;
}
