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

void computeIndividualFirst(Grammar grammar, FirstFollowSet* firstFollow, int ntIndex) {
    int altCount = grammar.rules[ntIndex].numAlternatives;
    for (int altIndex = 0; altIndex < altCount; altIndex++) {
        if (grammar.rules[ntIndex].alternatives[altIndex].symbols[0].symbolType == 1) {
            if (firstFollow[ntIndex].numFirst[altIndex] == 0)
                firstFollow[ntIndex].first[altIndex] = (int*)malloc(sizeof(int));
            else
                firstFollow[ntIndex].first[altIndex] = (int*)realloc(firstFollow[ntIndex].first[altIndex], sizeof(int) * (firstFollow[ntIndex].numFirst[altIndex] + 1));
            firstFollow[ntIndex].first[altIndex][firstFollow[ntIndex].numFirst[altIndex]] = grammar.rules[ntIndex].alternatives[altIndex].symbols[0].value;
            firstFollow[ntIndex].numFirst[altIndex]++;
        }
    }
    for (int altIndex = 0; altIndex < altCount; altIndex++) {
        if (grammar.rules[ntIndex].alternatives[altIndex].symbols[0].symbolType == 0) {
            int subNtIndex = grammar.rules[ntIndex].alternatives[altIndex].symbols[0].value;
            if (firstFollow[subNtIndex].numFirst[0] == 0)
                computeIndividualFirst(grammar, firstFollow, subNtIndex);
            for (int altSub = 0; altSub < grammar.rules[subNtIndex].numAlternatives; altSub++) {
                for (int firstElemIndex = 0; firstElemIndex < firstFollow[subNtIndex].numFirst[altSub]; firstElemIndex++) {
                    if (firstFollow[subNtIndex].first[altSub][firstElemIndex] != 0) {
                        if (isInArray(firstFollow[ntIndex].first[altIndex], firstFollow[subNtIndex].first[altSub][firstElemIndex], firstFollow[ntIndex].numFirst[altIndex]) != 1) {
                            firstFollow[ntIndex].first[altIndex] = (int*)realloc(firstFollow[ntIndex].first[altIndex], sizeof(int) * (firstFollow[ntIndex].numFirst[altIndex] + 1));
                            firstFollow[ntIndex].first[altIndex][firstFollow[ntIndex].numFirst[altIndex]] = firstFollow[subNtIndex].first[altSub][firstElemIndex];
                            firstFollow[ntIndex].numFirst[altIndex]++;
                        }
                    }
                }
            }
            if (grammar.rules[subNtIndex].hasEpsilon == 1) {
                int symIndex = 0;
                while (symIndex < grammar.rules[ntIndex].alternatives[altIndex].count) {
                    int nextSymbolIndex = grammar.rules[ntIndex].alternatives[altIndex].symbols[symIndex].value;
                    if (nextSymbolIndex == ntIndex) {
                        symIndex++;
                        continue;
                    }
                    if (grammar.rules[ntIndex].alternatives[altIndex].symbols[symIndex].symbolType == 1) {
                        if (isInArray(firstFollow[ntIndex].first[altIndex], nextSymbolIndex, firstFollow[ntIndex].numFirst[altIndex]) != 1) {
                            firstFollow[ntIndex].first[altIndex] = (int*)realloc(firstFollow[ntIndex].first[altIndex], sizeof(int) * (firstFollow[ntIndex].numFirst[altIndex] + 1));
                            firstFollow[ntIndex].first[altIndex][firstFollow[ntIndex].numFirst[altIndex]] = nextSymbolIndex;
                            firstFollow[ntIndex].numFirst[altIndex]++;
                        }
                        break;
                    } else {
                        if (firstFollow[nextSymbolIndex].numFirst[0] == 0)
                            computeIndividualFirst(grammar, firstFollow, nextSymbolIndex);
                        for (int altSub2 = 0; altSub2 < grammar.rules[nextSymbolIndex].numAlternatives; altSub2++) {
                            for (int firstElemIndex2 = 0; firstElemIndex2 < firstFollow[nextSymbolIndex].numFirst[altSub2]; firstElemIndex2++) {
                                if (firstFollow[nextSymbolIndex].first[altSub2][firstElemIndex2] != 0) {
                                    if (isInArray(firstFollow[ntIndex].first[altIndex], firstFollow[nextSymbolIndex].first[altSub2][firstElemIndex2], firstFollow[ntIndex].numFirst[altIndex]) != 1) {
                                        firstFollow[ntIndex].first[altIndex] = (int*)realloc(firstFollow[ntIndex].first[altIndex], sizeof(int) * (firstFollow[ntIndex].numFirst[altIndex] + 1));
                                        firstFollow[ntIndex].first[altIndex][firstFollow[ntIndex].numFirst[altIndex]] = firstFollow[nextSymbolIndex].first[altSub2][firstElemIndex2];
                                        firstFollow[ntIndex].numFirst[altIndex]++;
                                    }
                                }
                            }
                        }
                        if (grammar.rules[nextSymbolIndex].hasEpsilon != 1)
                            break;
                    }
                    symIndex++;
                }
            }
        }
    }
}
void computeIndividualFollow(Grammar grammar, FirstFollowSet* ffSet, int ntIdx, int prevIdx) {
    if (ntIdx == 0) {
        ffSet[ntIdx].follow = (int*)malloc(sizeof(int));
        ffSet[ntIdx].follow[0] = grammar.numTerminals - 1;
        ffSet[ntIdx].numFollow = 1;
    }
    for (int ruleIdx = grammar.totalRules - 1; ruleIdx >= 0; ruleIdx--) {
        for (int altIdx = 0; altIdx < grammar.rules[ruleIdx].numAlternatives; altIdx++) {
            for (int symIdx = 0; symIdx < grammar.rules[ruleIdx].alternatives[altIdx].count; symIdx++) {
                if (symIdx == grammar.rules[ruleIdx].alternatives[altIdx].count - 1) {
                    if (grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx].symbolType == 0 &&
                        grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx].value == ntIdx) {
                        if (ruleIdx != ntIdx) {
                            if (prevIdx == ruleIdx)
                                break;
                            if (ffSet[ruleIdx].numFollow == 0)
                                computeIndividualFollow(grammar, ffSet, ruleIdx, ntIdx);
                            for (int f = 0; f < ffSet[ruleIdx].numFollow; f++) {
                                if (isInArray(ffSet[ntIdx].follow, ffSet[ruleIdx].follow[f], ffSet[ntIdx].numFollow) != 1) {
                                    ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                    ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = ffSet[ruleIdx].follow[f];
                                    ffSet[ntIdx].numFollow++;
                                }
                            }
                        }
                    }
                    continue;
                }
                if (grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx].symbolType == 0 &&
                    grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx].value == ntIdx) {
                    if (grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].symbolType == 1) {
                        if (ffSet[ntIdx].numFollow == 0) {
                            ffSet[ntIdx].follow = (int*)malloc(sizeof(int));
                            ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].value;
                            ffSet[ntIdx].numFollow++;
                        }
                        else {
                            if (isInArr(ffSet[ntIdx].follow, grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].value, ffSet[ntIdx].numFollow) != 1) {
                                ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].value;
                                ffSet[ntIdx].numFollow++;
                            }
                        }
                        continue;
                    }
                    else {
                        int nextNt = grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].value;
                        if (ffSet[nextNt].numFirst[0] == 0)
                            computeIndividualFirst(grammar, ffSet, nextNt);
                        for (int m = 0; m < grammar.rules[nextNt].numAlternatives; m++) {
                            for (int g = 0; g < ffSet[nextNt].numFirst[m]; g++) {
                                if (ffSet[nextNt].first[m][g] != 0) {
                                    if (ffSet[ntIdx].numFollow == 0) {
                                        ffSet[ntIdx].follow = (int*)malloc(sizeof(int));
                                        ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = ffSet[nextNt].first[m][g];
                                        ffSet[ntIdx].numFollow++;
                                    }
                                    else {
                                        if (isInArr(ffSet[ntIdx].follow, ffSet[nextNt].first[m][g], ffSet[ntIdx].numFollow) != 1) {
                                            ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                            ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = ffSet[nextNt].first[m][g];
                                            ffSet[ntIdx].numFollow++;
                                        }
                                    }
                                }
                            }
                        }
                        if (grammar.rules[nextNt].hasEpsilon != 1)
                            continue;
                        else {
                            int idx = symIdx + 2;
                            while (idx < grammar.rules[ruleIdx].alternatives[altIdx].count) {
                                int followNt = grammar.rules[ruleIdx].alternatives[altIdx].symbols[idx].value;
                                if (grammar.rules[ruleIdx].alternatives[altIdx].symbols[idx].symbolType == 1) {
                                    if (isInArr(ffSet[ntIdx].follow, followNt, ffSet[ntIdx].numFollow) != 1) {
                                        ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                        ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = followNt;
                                        ffSet[ntIdx].numFollow++;
                                    }
                                    break;
                                }
                                else {
                                    if (ffSet[followNt].numFirst[0] == 0)
                                        computeIndividualFirst(grammar, ffSet, followNt);
                                    for (int m = 0; m < grammar.rules[followNt].numAlternatives; m++) {
                                        for (int g = 0; g < ffSet[followNt].numFirst[m]; g++) {
                                            if (ffSet[followNt].first[m][g] != 0) {
                                                if (isInArr(ffSet[ntIdx].follow, ffSet[followNt].first[m][g], ffSet[ntIdx].numFollow) != 1) {
                                                    ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                                    ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = ffSet[followNt].first[m][g];
                                                    ffSet[ntIdx].numFollow++;
                                                }
                                            }
                                        }
                                    }
                                    if (grammar.rules[followNt].hasEpsilon != 1)
                                        break;
                                }
                                idx++;
                            }
                            if (idx == grammar.rules[ruleIdx].alternatives[altIdx].count) {
                                if (ruleIdx != ntIdx) {
                                    if (ffSet[ruleIdx].numFollow == 0)
                                        computeIndividualFollow(grammar, ffSet, ruleIdx, -1);
                                    for (int f = 0; f < ffSet[ruleIdx].numFollow; f++) {
                                        if (isInArr(ffSet[ntIdx].follow, ffSet[ruleIdx].follow[f], ffSet[ntIdx].numFollow) != 1) {
                                            ffSet[ntIdx].follow = (int*)realloc(ffSet[ntIdx].follow, sizeof(int) * (ffSet[ntIdx].numFollow + 1));
                                            ffSet[ntIdx].follow[ffSet[ntIdx].numFollow] = ffSet[ruleIdx].follow[f];
                                            ffSet[ntIdx].numFollow++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

FirstFollowSet* computeFirstFollowSets(Grammar grammar) {
    FirstFollowSet* sets = (FirstFollowSet*)malloc(sizeof(FirstFollowSet) * grammar.numNonTerminals);
    for (int i = 0; i < grammar.numNonTerminals; i++) {
        int altCount = grammar.rules[i].numAlternatives;
        sets[i].numFirst = (int*)malloc(sizeof(int) * altCount);
        for (int j = 0; j < altCount; j++) {
            sets[i].numFirst[j] = 0;
        }
        sets[i].first = (int**)malloc(sizeof(int*) * altCount);
        sets[i].numFollow = 0;
    }
    for (int i = grammar.numNonTerminals - 1; i >= 0; i--) {
        if (sets[i].numFirst[0] == 0)
            computeIndividualFirst(grammar, sets, i);
        if (sets[i].numFollow == 0)
            computeIndividualFollow(grammar, sets, i, -1);
    }
    return sets;
}
