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
                            if (isInArray(ffSet[ntIdx].follow, grammar.rules[ruleIdx].alternatives[altIdx].symbols[symIdx + 1].value, ffSet[ntIdx].numFollow) != 1) {
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
                                        if (isInArray(ffSet[ntIdx].follow, ffSet[nextNt].first[m][g], ffSet[ntIdx].numFollow) != 1) {
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
                                    if (isInArray(ffSet[ntIdx].follow, followNt, ffSet[ntIdx].numFollow) != 1) {
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
                                                if (isInArray(ffSet[ntIdx].follow, ffSet[followNt].first[m][g], ffSet[ntIdx].numFollow) != 1) {
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
                                        if (isInArray(ffSet[ntIdx].follow, ffSet[ruleIdx].follow[f], ffSet[ntIdx].numFollow) != 1) {
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

void displayParseTable(Grammar grammar, ParseTable* table) {
    int tot = 0;
    for (int i = 0; i < grammar.numNonTerminals; i++) {
        for (int j = 0; j < grammar.numTerminals; j++) {
            if (table->entries[i][j].isError == 0) {
                tot++;
                printRule(grammar, table->entries[i][j].ruleIndex, table->entries[i][j].alternativeIndex);
            }
        }
    }
}

ParseTable* initializeParseTable(int numNonTerminals, int numTerminals) {
    ParseTable* table = (ParseTable*)malloc(sizeof(ParseTable));
    table->entries = (ParseTableEntry**)malloc(sizeof(ParseTableEntry*) * numNonTerminals);
    for (int i = 0; i < numNonTerminals; i++) {
        table->entries[i] = (ParseTableEntry*)malloc(sizeof(ParseTableEntry) * numTerminals);
    }
    for (int i = 0; i < numNonTerminals; i++) {
        for (int j = 0; j < numTerminals; j++) {
            table->entries[i][j].isError = 1;
            table->entries[i][j].ruleIndex = -1;
            table->entries[i][j].alternativeIndex = -1;
        }
    }
    return table;
}

void createParseTable(Grammar grammar, FirstFollowSet* firstFollow, ParseTable* table) {
    int index;
    for (int i = 0; i < grammar.totalRules; i++) {
        for (int j = 0; j < grammar.rules[i].numAlternatives; j++) {
            for (int terminal_num = 0; terminal_num < firstFollow[i].numFirst[j]; terminal_num++) {
                index = firstFollow[i].first[j][terminal_num];
                if (index != 0) {   
                    table->entries[i][index].isError = 0;
                    table->entries[i][index].ruleIndex = i;
                    table->entries[i][index].alternativeIndex = j;
                } else {
                    for (int follow_terminal_num = 0; follow_terminal_num < firstFollow[i].numFollow; follow_terminal_num++) {
                        index = firstFollow[i].follow[follow_terminal_num]; 
                        table->entries[i][index].isError = 0;
                        table->entries[i][index].ruleIndex = i;
                        table->entries[i][index].alternativeIndex = j;
                    }
                }
            }
        }
    }
    for (int i = 0; i < grammar.totalRules; i++) {
        for (int j = 0; j < grammar.rules[i].numAlternatives; j++) {
            for (int follow_terminal_num = 0; follow_terminal_num < firstFollow[i].numFollow; follow_terminal_num++) {
                index = firstFollow[i].follow[follow_terminal_num]; 
                if (table->entries[i][index].isError == 1) {   
                    table->entries[i][index].isError = 2;
                }
            }
        }
    }
}

// Define a function to check if a token is in the sync set.
int isSyncToken(tokenInfo token) {
    // You can compare the token's type to your sync tokens.
    // Adjust these based on your token enum values.
    if (token.tkn_name == TK_SEM ||
        token.tkn_name == TK_END ||
        token.tkn_name == TK_ENDIF ||
        token.tkn_name == TK_ELSE ||
        token.tkn_name == TK_SQL ||  // Example for a closing curly bracket or parenthesis
        token.tkn_name == TK_SQR)    // Example for a closing square bracket
    {
        return 1;
    }
    return 0;
}

tokenInfo skipToSync(twinBuffer *B, FILE *fp) {
    tokenInfo token = getNextToken(B, fp);
    
    while (!isSyncToken(token) && token.tkn_name != TK_EOF) {
        token = getNextToken(B, fp);
    }
    return token;
}


treeN parseSourceCode(char* sourceFile, Grammar grammar, ParseTable* table, int* errorFlag) { 
    printf("Parsing input source code...\n");
    FILE *fp = fopen(sourceFile, "r");
    
    initialize();
    // Create and initialize the twin buffer for the lexer.
    twinBuffer *B = initializeTwinBuffer();
    // Fill the first buffer.
    fp = getStream(B, fp, 0);
    
    // Use GrammarSymbol (defined in parserDef.h) for the stack.
    GrammarSymbol* stack = (GrammarSymbol*) malloc(sizeof(GrammarSymbol) * 2);
    // Push $ (terminal): type = 1, value = grammar.numTerminals - 1
    stack[0].symbolType = 1; 
    stack[0].value = grammar.numTerminals - 1; // '$'
    // Push start symbol: type = 0, value = 0
    stack[1].symbolType = 0; 
    stack[1].value = 0; // Start symbol 
    
    // pointers holds treeN* corresponding to the symbols on the stack.
    treeN** pointers = (treeN**) malloc(sizeof(treeN) * 2); 
    pointers[0] = NULL; 
    node rootNode = createEl(-1, -1, 0, 0,  0); 
    treeN root = createNode(rootNode, grammar); 
    pointers[1] = &root; 

    int stackLen = 2; 
    int stackPointer = 1; 
    // Get the first token using the twinBuffer.
    tokenInfo currToken = getNextToken(B, fp);
    while (currToken.tkn_name == TK_COMMENT) {
        currToken = getNextToken(B, fp);
    }

    while(1) { 
        if (currToken.tkn_name == ERROR) { 
            printf("Line No: %d - ERROR: Lexical error at %s \n", currToken.line, currToken.value.str); 
            *errorFlag = 1;
            currToken = getNextToken(B, fp);
            while (currToken.tkn_name == TK_COMMENT) {
                currToken = getNextToken(B, fp);
            }
            stackPointer--;
            continue;
        } 
        if (currToken.tkn_name == TK_EOF) { 
            if (stackPointer == 0) { 
                printf("Parsing completed.\n\n");
                break; 
            } 
            else { 
                int flag = 0; 
                while (stackPointer != 0) { 
                    if (stack[stackPointer].symbolType == 1) { 
                        printf("ERROR: Top element of stack is a terminal but EOF has been reached\n"); 
                        *errorFlag = 1;
                        flag = 1; 
                        break; 
                    } 
                    if(table->entries[stack[stackPointer].value][grammar.numTerminals - 1].isError == 1) { 
                        printf("ERROR: ParseTable[X, a] is blank\n"); 
                        *errorFlag = 1;
                        flag = 1; 
                        break;
                    } 
                    else { 
                        int ruleInd = table->entries[stack[stackPointer].value][grammar.numTerminals - 1].ruleIndex; 
                        int rhsInd = table->entries[stack[stackPointer].value][grammar.numTerminals - 1].alternativeIndex; 
                        if (grammar.rules[ruleInd].alternatives[rhsInd].symbols[0].symbolType == 1 && 
                            grammar.rules[ruleInd].alternatives[rhsInd].symbols[0].value == 0) { 
                            node currNode = createEl(-1, stack[stackPointer].value, 0, 1, 0); 
                            treeN* eps = (treeN*) malloc(sizeof(treeN)); 
                            *eps = createNode(currNode, grammar); 
                            addChild(pointers[stackPointer], eps); 
                            printf("Rule is epsilon, pop last non-terminal off the stack \n"); 
                            stackPointer--; 
                        } 
                        else { 
                            int parentSymbol = pointers[stackPointer]->elem.curr; 
                            treeN* parent = pointers[stackPointer]; 
                            for (int i = grammar.rules[ruleInd].alternatives[rhsInd].count - 1; i >= 0; i--) { 
                                if(stackPointer == stackLen) { 
                                    stack = (GrammarSymbol*) realloc(stack, sizeof(GrammarSymbol) * (stackLen + 1)); 
                                    pointers = (treeN**) realloc(pointers, sizeof(treeN) * (stackLen + 1)); 
                                    stackLen++; 
                                }
                                stack[stackPointer].symbolType = grammar.rules[ruleInd].alternatives[rhsInd].symbols[i].symbolType; 
                                stack[stackPointer].value = grammar.rules[ruleInd].alternatives[rhsInd].symbols[i].value; 
                    
                                node currNode;
                                if (stack[stackPointer].symbolType == 1) { 
                                    currNode = createEl(-1, parentSymbol, stack[stackPointer].value, 1, 0); 
                                } 
                                else { 
                                    currNode = createEl(-1, parentSymbol, stack[stackPointer].value, 0, 0); 
                                } 
                                treeN* new = (treeN*) malloc(sizeof(treeN)); 
                                *new = createNode(currNode, grammar); 
                                addChild(parent, new); 
                                pointers[stackPointer] = new; 
                    
                                if (i != 0) { 
                                    stackPointer++; 
                                } 
                            } 
                        } 
                    } 
                } 
                if (flag == 1) { 
                    break; 
                } 
                else { 
                    printf("Parsing completed.\n\n"); 
                    break; 
                }
            }
        }
        
        if (stackPointer == 0 && currToken.tkn_name != TK_EOF) { 
            printf("Line No: %d - ERROR: Start symbol popped from stack but end of file not reached \n", currToken.line); 
            *errorFlag = 1;
            break; 
        } 
        // printf("Current token: %s, Stack top: %s, Stack pointer: %d\n");
        // printf("Current token: %s, Stack top: %s, Stack pointer: %d\n",
        //     enumToString[currToken.tkn_name],
        //     (stackPointer >= 0
        //         ? (stack[stackPointer].symbolType == 1
        //                ? grammar.terminals[stack[stackPointer].value]
        //                : grammar.nonTerminals[stack[stackPointer].value])
        //         : "None"),
        //     stackPointer);
     
        int tokenID = findIndex(grammar.terminals, grammar.numTerminals, enumToString[currToken.tkn_name]); 
        if (stack[stackPointer].symbolType == 1 && stack[stackPointer].value == tokenID) { 
            // printf("Terminals on top of stack and at lookahead pointer (%s) match at line number %d\n", grammar.terminals[tokenID], currToken.line); 
            
            pointers[stackPointer]->elem.lineNo = currToken.line; 
            if (currToken.tkn_name == TK_NUM) { 
                pointers[stackPointer]->elem.lex.numVal = currToken.value.num; 
            } 
            else if (currToken.tkn_name == TK_RNUM) { 
                pointers[stackPointer]->elem.lex.rVal = currToken.value.rnum.v; 
            } 
            else { 
                pointers[stackPointer]->elem.lex.lexemeStr = currToken.value.str; 
            }
            
            stackPointer--; 
            currToken = getNextToken(B, fp);
            while (currToken.tkn_name == TK_COMMENT) {
                currToken = getNextToken(B, fp);
            }
        } 
        else if(stack[stackPointer].symbolType == 1 && stack[stackPointer].value != tokenID) { 
            // printf("Line No: %d - ERROR: Top of stack is %s and lookahead is %s : Terminals do not match\n", currToken.line, grammar.terminals[stack[stackPointer].value], grammar.terminals[tokenID]); 
            printf("Line %d Error: The token %s for lexeme %s does not match with the expected token %s\n",
                currToken.line,
                enumToString[currToken.tkn_name],
                currToken.value.str,
                grammar.terminals[stack[stackPointer].value]);
         
            *errorFlag = 1;
            currToken = getNextToken(B, fp);
            while (currToken.tkn_name == TK_COMMENT) {
                currToken = getNextToken(B, fp);
            }
        } 
        else if(stack[stackPointer].symbolType == 0 && table->entries[stack[stackPointer].value][tokenID].isError != 0) { 
            // printf("Line No: %d - ERROR: Top of stack is %s(X) and lookahead is %s(a) : ParseTable[X, a] is blank ERROR \n", currToken.line, grammar.nonTerminals[stack[stackPointer].value], grammar.terminals[tokenID]);      
            *errorFlag = 1;
            
            if(!isInArray(grammar.firstFollow[stack[stackPointer].value].follow, tokenID, grammar.firstFollow[stack[stackPointer].value].numFollow)){
                currToken = getNextToken(B, fp);
                while (currToken.tkn_name == TK_COMMENT) {
                    currToken = getNextToken(B, fp);
                }
                tokenID = currToken.tkn_name;
            }
            else{
                stackPointer--;   
            }
        } 
        else if (stack[stackPointer].symbolType == 0 && table->entries[stack[stackPointer].value][tokenID].isError == 0) { 
            int ruleInd = table->entries[stack[stackPointer].value][tokenID].ruleIndex; 
            int rhsInd = table->entries[stack[stackPointer].value][tokenID].alternativeIndex; 
            pointers[stackPointer]->elem.ruleNumber = getRuleNumberFromIndices(ruleInd, rhsInd, grammar);
            if (grammar.rules[ruleInd].alternatives[rhsInd].symbols[0].symbolType == 1 && grammar.rules[ruleInd].alternatives[rhsInd].symbols[0].value == 0) { 
                node currNode = createEl(-1, stack[stackPointer].value, 0, 1, 0); 
                treeN* eps = (treeN*) malloc(sizeof(treeN)); 
                *eps = createNode(currNode, grammar); 
                addChild(pointers[stackPointer], eps); 
                stackPointer--; 
            } 
            else { 
                int parentSymbol = pointers[stackPointer]->elem.curr; 
                treeN* parent = pointers[stackPointer]; 
                for (int i = grammar.rules[ruleInd].alternatives[rhsInd].count - 1; i >= 0; i--) { 
                    if(stackPointer == stackLen) { 
                        stack = (GrammarSymbol*) realloc(stack, sizeof(GrammarSymbol) * (stackLen + 1)); 
                        pointers = (treeN**) realloc(pointers, sizeof(treeN) * (stackLen + 1)); 
                        stackLen++; 
                    }
                    stack[stackPointer].symbolType = grammar.rules[ruleInd].alternatives[rhsInd].symbols[i].symbolType; 
                    stack[stackPointer].value = grammar.rules[ruleInd].alternatives[rhsInd].symbols[i].value; 
                    
                    node currNode;
                    if (stack[stackPointer].symbolType == 1) { 
                        currNode = createEl(-1, parentSymbol, stack[stackPointer].value, 1, 0); 
                    } 
                    else { 
                        currNode = createEl(-1, parentSymbol, stack[stackPointer].value, 0, 0); 
                    } 
                    treeN* new = (treeN*) malloc(sizeof(treeN)); 
                    *new = createNode(currNode, grammar); 
                    addChild(parent, new); 
                    pointers[stackPointer] = new; 
                    
                    if (i != 0) { 
                        stackPointer++; 
                    } 
                } 
            } 
        }
    } 
    clearTwinBuffer(B);
    return root; 
} 

void inOrderTraversal(treeN* node, Grammar grammar, int *count)
{   
    char *str;
    char *nt;
    char *terminal_tok;
    if(node->elem.isLeaf == 1){
        str = "yes";
        nt = "NOT_NON_TERMINAL";
        terminal_tok = grammar.terminals[node->elem.curr];
    }
    else{
        str = "no";
        nt = grammar.nonTerminals[node->elem.curr];
        terminal_tok = "NOT_TERMINAL";
    }
    char *eps_lex;
    if(strcmp(grammar.terminals[node->elem.curr], "eps") == 0){
        eps_lex = "eps";
    }
    else{
        eps_lex = node->elem.lex.lexemeStr;
    }
    if(node->numChild == 0){
        if(strcmp(grammar.terminals[node->elem.curr],"TK_NUM") == 0)
            printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",
                   node->elem.lex.numVal,
                   node->elem.lineNo,
                   terminal_tok,
                   node->elem.lex.numVal,
                   grammar.nonTerminals[node->elem.parentNodeSymbolID],
                   str,
                   nt);
        else if(strcmp(grammar.terminals[node->elem.curr],"TK_RNUM") == 0)
            printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",
                   node->elem.lex.rVal,
                   node->elem.lineNo,
                   terminal_tok,
                   node->elem.lex.rVal,
                   grammar.nonTerminals[node->elem.parentNodeSymbolID],
                   str,
                   nt);
        else {
            if(node->elem.parentNodeSymbolID != -1){
                printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",
                       eps_lex,
                       node->elem.lineNo,
                       terminal_tok,
                       grammar.nonTerminals[node->elem.parentNodeSymbolID],
                       str,
                       nt);
            }
            else{
                printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",
                       eps_lex,
                       node->elem.lineNo,
                       terminal_tok,
                       str,
                       nt);
            }
        } 
        (*count)++;     
        return;  
    }
    if(node->numChild == 1){
        inOrderTraversal(node->children[0], grammar, count);
        if(strcmp(grammar.terminals[node->elem.curr],"TK_NUM") == 0)
            printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",
                   node->elem.lex.numVal,
                   node->elem.lineNo,
                   terminal_tok,
                   node->elem.lex.numVal,
                   grammar.nonTerminals[node->elem.parentNodeSymbolID],
                   str,
                   nt);
        else if(strcmp(grammar.terminals[node->elem.curr],"TK_RNUM") == 0)
            printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",
                   node->elem.lex.rVal,
                   node->elem.lineNo,
                   terminal_tok,
                   node->elem.lex.rVal,
                   grammar.nonTerminals[node->elem.parentNodeSymbolID],
                   str,
                   nt);
        else{
            if(node->elem.parentNodeSymbolID != -1){
                printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",
                       eps_lex,
                       node->elem.lineNo,
                       terminal_tok,
                       grammar.nonTerminals[node->elem.parentNodeSymbolID],
                       str,
                       nt);
            }
            else{
                printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",
                       eps_lex,
                       node->elem.lineNo,
                       terminal_tok,
                       str,
                       nt);
            }
        }
        (*count)++;
        return;  
    }

    inOrderTraversal(node->children[node->numChild - 1], grammar, count);

    if(strcmp(grammar.terminals[node->elem.curr],"TK_NUM") == 0)
        printf("%d\t%d\t%s\t%d\t%s\t%s\t%s\n",
               node->elem.lex.numVal,
               node->elem.lineNo,
               terminal_tok,
               node->elem.lex.numVal,
               grammar.nonTerminals[node->elem.parentNodeSymbolID],
               str,
               nt);
    else if(strcmp(grammar.terminals[node->elem.curr],"TK_RNUM") == 0)
        printf("%f\t%d\t%s\t%f\t%s\t%s\t%s\n",
               node->elem.lex.rVal,
               node->elem.lineNo,
               terminal_tok,
               node->elem.lex.rVal,
               grammar.nonTerminals[node->elem.parentNodeSymbolID],
               str,
               nt);
    else{
        if(node->elem.parentNodeSymbolID != -1){
            printf("%s\t%d\t%s\tNOT_NUM\t%s\t%s\t%s\n",
                   eps_lex,
                   node->elem.lineNo,
                   terminal_tok,
                   grammar.nonTerminals[node->elem.parentNodeSymbolID],
                   str,
                   nt);
        }
        else{
            printf("%s\t%d\t%s\tNOT_NUM\t%s\tROOT\t%s\n",
                   eps_lex,
                   node->elem.lineNo,
                   terminal_tok,
                   str,
                   nt);
        }
    }

    for(int i = node->numChild - 2; i >= 0; i--){
        inOrderTraversal(node->children[i], grammar, count);
    }
    (*count)++;
    return;
}

void printParseTree(treeN* root, Grammar grammar, int *count){
    printf("Printing the parse tree inorder:\n");
    if(root == NULL){
         printf("ERROR: Parse Tree root is null");
         return;
    }
    inOrderTraversal(root, grammar, count);    
}

int getRuleNumberFromIndices(int ruleIndex, int alternativeIndex, Grammar grammar){
    int c = 0;
    for(int i = 0; i < grammar.totalRules; i++){
        for(int j = 0; j < grammar.rules[i].numAlternatives; j++){
            if(i < ruleIndex || (i == ruleIndex && j <= alternativeIndex))
                c++;
        }
    }
    return c;
}

// int main(int argc, char *argv[]) {
//     printf("Hello %d\n",argc);
//     if(argc < 3) {

//         printf("Usage: %s <sourcefile>\n", argv[0]);
//         return 1;
//     }

//     // Read the grammar from a file (assume grammar.txt contains your grammar)
//     Grammar grammar = readGrammar("grammar.txt");
//     printf("Hello\n");
    
//     // Compute the first-follow sets for the grammar.
//     grammar.firstFollow = computeFirstFollowSets(grammar);

//     // Initialize and create the parse table.
//     ParseTable* table = initializeParseTable(grammar.numNonTerminals, grammar.numTerminals);
//     createParseTable(grammar, grammar.firstFollow, table);

//     int errorFlag = 0;
//     // Call the parser on your test file t3.txt.
//     treeN parseTree = parseSourceCode(argv[1], grammar, table, &errorFlag);
    
//     if(errorFlag) {
//         printf("There were syntactic errors in the source code.\n");
//     } else {
//         printf("Parsing completed successfully.\n");
//     }

//     // Optionally, print the parse tree
//     int count = 0;
//     printParseTree(&parseTree, grammar, &count);

//     // Free any allocated resources here as needed...

//     return 0;
// }