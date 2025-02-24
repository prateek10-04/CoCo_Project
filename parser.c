#include "parser.h"
#include <stdlib.h>
#include <string.h>

char* terminalSymbols[] = {
    "eps", "TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RUID", "TK_WITH",
    "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN",
    "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_INT", "TK_REAL", "TK_COMMA",
    "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE", "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE",
    "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND",
    "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "$"
};

int totalTerminals = 59;

Grammar readGrammar(char* filename) {
    Grammar grammarObj;
    char** nonTerminalList = (char**)malloc(sizeof(char*) * 1);
    nonTerminalList[0] = "program";
    int totalNonTerminals = 1;
    FILE* filePtr = fopen(filename, "r");
    int ruleCounter = 0;
    char line[300];
    while (fgets(line, sizeof(line), filePtr)) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\r' || line[i] == '\n') {
                line[i] = '\0';
            }
        }
        char* currentToken = strtok(line, " ");
        if (ruleCounter != 0 && currentToken != NULL) {
            int length = strlen(currentToken);
            char* lhsName = (char*)malloc(sizeof(char) * (length - 1));
            strncpy(lhsName, currentToken + 1, length - 2);
            lhsName[length - 2] = '\0';
            nonTerminalList = realloc(nonTerminalList, sizeof(char*) * (totalNonTerminals + 1));
            nonTerminalList[totalNonTerminals] = lhsName;
            totalNonTerminals++;
        }
        ruleCounter++;
    }
    fclose(filePtr);
    grammarObj.terminals = terminalSymbols;
    grammarObj.nonTerminals = nonTerminalList;
    grammarObj.totalRules = ruleCounter;
    grammarObj.numNonTerminals = totalNonTerminals;
    grammarObj.numTerminals = totalTerminals;
    grammarObj.rules = (GrammarRule*)malloc(sizeof(GrammarRule) * ruleCounter);
    filePtr = fopen(filename, "r");
    ruleCounter = 0;
    while (fgets(line, sizeof(line), filePtr)) {
        int alternativeCount = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '|') {
                alternativeCount++;
            }
            if (line[i] == '\r' || line[i] == '\n') {
                line[i] = '\0';
            }
        }
        char* currentToken = strtok(line, " ");
        int length = strlen(currentToken);
        char* lhsName = (char*)malloc(sizeof(char) * (length - 1));
        strncpy(lhsName, currentToken + 1, length - 2);
        lhsName[length - 2] = '\0';
        GrammarRule currentRule;
        currentRule.leftHandSide = findIndex(nonTerminalList, totalNonTerminals, lhsName);
        currentToken = strtok(NULL, " ");
        currentToken = strtok(NULL, " ");
        currentRule.numAlternatives = alternativeCount + 1;
        int alternativeIndex = 0;
        currentRule.alternatives = (ProductionAlternative*)malloc(sizeof(ProductionAlternative) * (alternativeCount + 1));
        currentRule.hasEpsilon = 0;
        int symbolCount = 0;
        GrammarSymbol* symbolList = NULL;
        while (currentToken != NULL) {
            if (currentToken[0] == '|') {
                currentRule.alternatives[alternativeIndex].count = symbolCount;
                currentRule.alternatives[alternativeIndex].symbols = symbolList;
                symbolCount = 0;
                alternativeIndex++;
                currentToken = strtok(NULL, " ");
                continue;
            }
            if (currentToken[0] == '<' || currentToken[0] == 'T' || strcmp(currentToken, "∈") == 0) {
                symbolCount++;
                if (symbolCount == 1) {
                    symbolList = (GrammarSymbol*)malloc(sizeof(GrammarSymbol));
                } else {
                    symbolList = realloc(symbolList, sizeof(GrammarSymbol) * symbolCount);
                }
                if (currentToken[0] == '<') {
                    int len = strlen(currentToken);
                    char* symbolName = (char*)malloc(sizeof(char) * (len - 1));
                    strncpy(symbolName, currentToken + 1, len - 2);
                    symbolName[len - 2] = '\0';
                    int index = findIndex(nonTerminalList, totalNonTerminals, symbolName);
                    symbolList[symbolCount - 1].symbolType = 0;
                    symbolList[symbolCount - 1].value = index;
                } else if (currentToken[0] == 'T') {
                    int index = findIndex(terminalSymbols, totalTerminals, currentToken);
                    symbolList[symbolCount - 1].symbolType = 1;
                    symbolList[symbolCount - 1].value = index;
                } else if (strcmp(currentToken, "∈") == 0) {
                    int index = findIndex(terminalSymbols, totalTerminals, "eps");
                    symbolList[symbolCount - 1].symbolType = 1;
                    symbolList[symbolCount - 1].value = index;
                    currentRule.hasEpsilon = 1;
                }
            }
            currentToken = strtok(NULL, " ");
        }
        currentRule.alternatives[alternativeIndex].count = symbolCount;
        currentRule.alternatives[alternativeIndex].symbols = symbolList;
        currentRule.ruleNumber = ruleCounter + 1;
        grammarObj.rules[ruleCounter] = currentRule;
        ruleCounter++;
    }
    fclose(filePtr);
    return grammarObj;
}
