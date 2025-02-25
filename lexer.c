#include "lexer.h"
#include "lexerDef.h"
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

// Global buffers and variables
char *active_buffer = NULL, *backup_buffer = NULL; // Active and secondary buffers (each of size BUFFER_SIZE)
int buffer_pos = 0;         // Current position in active_buffer
int current_line = 1;       // Tracks current line number in the source
FILE *source_file = NULL;   // Input file pointer
struct HashMap *keyword_table = NULL; // Hash table for keywords
size_t characters_read = 0; // Number of characters read in the current buffer


const char *getTokenType(TokenType type) { /* Returns a string representation for a given token type.*/
    switch (type) {
        case TK_ASSIGNOP:         return "TK_ASSIGNOP";
        case TK_COMMENT:          return "TK_COMMENT";
        case TK_FIELDID:          return "TK_FIELDID";
        case TK_ID:               return "TK_ID";
        case TK_NUM:              return "TK_NUM";
        case TK_RNUM:             return "TK_RNUM";
        case TK_FUNID:            return "TK_FUNID";
        case TK_RUID:             return "TK_RUID";
        case TK_WITH:             return "TK_WITH";
        case TK_PARAMETERS:       return "TK_PARAMETERS";
        case TK_END:              return "TK_END";
        case TK_WHILE:            return "TK_WHILE";
        case TK_UNION:            return "TK_UNION";
        case TK_ENDUNION:         return "TK_ENDUNION";
        case TK_DEFINETYPE:       return "TK_DEFINETYPE";
        case TK_AS:               return "TK_AS";
        case TK_TYPE:             return "TK_TYPE";
        case TK_MAIN:             return "TK_MAIN";
        case TK_GLOBAL:           return "TK_GLOBAL";
        case TK_PARAMETER:        return "TK_PARAMETER";
        case TK_LIST:             return "TK_LIST";
        case TK_SQL:              return "TK_SQL";
        case TK_SQR:              return "TK_SQR";
        case TK_INPUT:            return "TK_INPUT";
        case TK_OUTPUT:           return "TK_OUTPUT";
        case TK_INT:              return "TK_INT";
        case TK_REAL:             return "TK_REAL";
        case TK_COMMA:            return "TK_COMMA";
        case TK_SEM:              return "TK_SEM";
        case TK_COLON:            return "TK_COLON";
        case TK_DOT:              return "TK_DOT";
        case TK_ENDWHILE:         return "TK_ENDWHILE";
        case TK_OP:               return "TK_OP";
        case TK_CL:               return "TK_CL";
        case TK_IF:               return "TK_IF";
        case TK_THEN:             return "TK_THEN";
        case TK_ENDIF:            return "TK_ENDIF";
        case TK_READ:             return "TK_READ";
        case TK_WRITE:            return "TK_WRITE";
        case TK_RETURN:           return "TK_RETURN";
        case TK_PLUS:             return "TK_PLUS";
        case TK_MINUS:            return "TK_MINUS";
        case TK_MUL:              return "TK_MUL";
        case TK_DIV:              return "TK_DIV";
        case TK_CALL:             return "TK_CALL";
        case TK_RECORD:           return "TK_RECORD";
        case TK_ENDRECORD:        return "TK_ENDRECORD";
        case TK_ELSE:             return "TK_ELSE";
        case TK_AND:              return "TK_AND";
        case TK_OR:               return "TK_OR";
        case TK_NOT:              return "TK_NOT";
        case TK_LT:               return "TK_LT";
        case TK_LE:               return "TK_LE";
        case TK_EQ:               return "TK_EQ";
        case TK_GT:               return "TK_GT";
        case TK_GE:               return "TK_GE";
        case TK_NE:               return "TK_NE";
       /*case TK_ERROR_PATTERN:    return "TK_ERROR_PATTERN";
        case TK_ERROR_SYMBOL:     return "TK_ERROR_SYMBOL";
        case TK_ERROR_ASSIGNOP:   return "TK_ERROR_ASSIGNOP";
        case TK_ERROR_SIZE20:     return "TK_ERROR_SIZE20";
        case TK_ERROR_SIZE30:     return "TK_ERROR_SIZE30";
        case TK_DELIM:            return "TK_DELIM";*/
        default:                  return "Unknown Token";
    }
}

/*Computes a hash value for a given key (used in the keyword hash table).*/
int hashFunction(struct HashMap *table, char *key) {
    unsigned long hash = 0;
    int factor = 31;
    for (int i = 0; key[i] != '\\0'; i++) {
        hash = (hash * factor + (unsigned char) key[i]) % table->capacity;
    }
    return (int) hash;
}

/*Initializes the hash table structure for storing keywords.*/
void initializeHashTable(struct HashMap *table) {
    table->capacity = 100;
    table->size = 0;
    table->buckets = (struct HashNode **)malloc(sizeof(struct HashNode *) * table->capacity);
    for (int i = 0; i < table->capacity; i++) {
        table->buckets[i] = NULL;
    }
}

/*---------------------------------------------------------------------------
 Function: insertKeyword
 Purpose : Inserts a key-token pair into the hash table.
---------------------------------------------------------------------------*/
void insertKeyword(struct HashMap *table, char *key, TokenType token) {
    int index = hashFunction(table, key);
    struct HashNode *newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
    newNode->key = strdup(key);
    newNode->token = token;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    table->size++;
}



void populateKeywordTable(struct HashMap *table) {
    insertKeyword(table, "with", TK_WITH);
    insertKeyword(table, "parameters", TK_PARAMETERS);
    insertKeyword(table, "end", TK_END);
    insertKeyword(table, "while", TK_WHILE);
    insertKeyword(table, "union", TK_UNION);
    insertKeyword(table, "endunion", TK_ENDUNION);
    insertKeyword(table, "definetype", TK_DEFINETYPE);
    insertKeyword(table, "as", TK_AS);
    insertKeyword(table, "type", TK_TYPE);
    insertKeyword(table, "_main", TK_MAIN);
    insertKeyword(table, "global", TK_GLOBAL);
    insertKeyword(table, "parameter", TK_PARAMETER);
    insertKeyword(table, "list", TK_LIST);
    insertKeyword(table, "input", TK_INPUT);
    insertKeyword(table, "output", TK_OUTPUT);
    insertKeyword(table, "int", TK_INT);
    insertKeyword(table, "real", TK_REAL);
    insertKeyword(table, "endwhile", TK_ENDWHILE);
    insertKeyword(table, "if", TK_IF);
    insertKeyword(table, "then", TK_THEN);
    insertKeyword(table, "endif", TK_ENDIF);
    insertKeyword(table, "read", TK_READ);
    insertKeyword(table, "write", TK_WRITE);
    insertKeyword(table, "return", TK_RETURN);
    insertKeyword(table, "call", TK_CALL);
    insertKeyword(table, "record", TK_RECORD);
    insertKeyword(table, "endrecord", TK_ENDRECORD);
    insertKeyword(table, "else", TK_ELSE);
}

/*---------------------------------------------------------------------------
 Function: lookupKeyword
 Purpose : Searches for a key in the hash table; returns the token if found.
---------------------------------------------------------------------------*/
TokenType lookupKeyword(struct HashMap *table, char *key) {
    int index = hashFunction(table, key);
    struct HashNode *curr = table->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            return curr->token;
        }
        curr = curr->next;
    }
    return TK_ERROR_PATTERN; // Indicates key not found
}
/*
/*---------------------------------------------------------------------------
 Function: loadStream
 Purpose : Loads a new chunk of characters (of size BUFFER_SIZE) from the input file into the active buffer.
---------------------------------------------------------------------------*/
/*FILE *loadStream(FILE *fp, int alreadyFilled) {
    free(backup_buffer);
    backup_buffer = (char *)malloc(BUFFER_SIZE);
    // Copy leftover characters (if any)
    for (int i = 0; i < alreadyFilled; i++) {
        backup_buffer[i] = active_buffer[buffer_pos++];
    }
    // Fill the rest of the buffer from file
    size_t bytesRead = fread(backup_buffer + alreadyFilled, 1, BUFFER_SIZE - alreadyFilled, fp);
    characters_read = bytesRead + alreadyFilled;
    if (characters_read < BUFFER_SIZE) {
        char *temp = (char *)malloc(characters_read);
        memcpy(temp, backup_buffer, characters_read);
        free(backup_buffer);
        backup_buffer = temp;
    }
    // Swap buffers for continuous reading
    char *temp = active_buffer;
    active_buffer = backup_buffer;
    backup_buffer = temp;
    buffer_pos = 0;
    return fp;
} */

/*---------------------------------------------------------------------------
 Function: monitorBufferBounds
 Purpose : Checks if the current buffer has been exhausted and loads new data if necessary.
---------------------------------------------------------------------------*/
void monitorBufferBounds(void) {
    if (buffer_pos >= BUFFER_SIZE) {
        source_file = loadStream(source_file, 0);
    }
}

/*---------------------------------------------------------------------------
 Function: retrieveNextToken
 Purpose : Scans the active buffer and returns the next token.
 Note    : This is a simplified example that handles whitespace and a couple of token patterns.
---------------------------------------------------------------------------*/
/* TokenInfo retrieveNextToken(void) {
    TokenInfo token;
    token.lexeme = NULL;
    token.line = current_line;
    token.type = TK_ERROR_SYMBOL;  // Default to error
    
    // Skip over whitespace and update line numbers
    while (active_buffer[buffer_pos] == ' ' || active_buffer[buffer_pos] == '\\t' || active_buffer[buffer_pos] == '\\n') {
        if (active_buffer[buffer_pos] == '\\n')
            current_line++;
        buffer_pos++;
        monitorBufferBounds();
    }
    
    int start = buffer_pos;
    char ch = active_buffer[buffer_pos];
    
    // Check for end-of-file condition
    if (buffer_pos >= characters_read) {
        token.lexeme = strdup("EOF");
        token.type = TK_DELIM;
        token.line = current_line;
        return token;
    }
    
    // Example: Tokenizing '@' as part of a logical OR operator
    if (ch == '@') {
        buffer_pos++;
        monitorBufferBounds();
        if (active_buffer[buffer_pos] == '@') {
            buffer_pos++;
            monitorBufferBounds();
            if (active_buffer[buffer_pos] == '@') {
                buffer_pos++;
                token.lexeme = strdup(\"@@@\");
                token.type = TK_OR;
                token.line = current_line;
                return token;
            } else {
                token.lexeme = strdup(\"@@\");
                token.type = TK_ERROR_PATTERN;
                token.line = current_line;
                return token;
            }
        } else {
            token.lexeme = strdup(\"@\");
            token.type = TK_ERROR_SYMBOL;
            token.line = current_line;
            return token;
        }
    }
    
    // Example: Tokenizing '&' for logical AND
    if (ch == '&') {
        buffer_pos++;
        monitorBufferBounds();
        if (active_buffer[buffer_pos] == '&') {
            buffer_pos++;
            monitorBufferBounds();
            if (active_buffer[buffer_pos] == '&') {
                buffer_pos++;
                token.lexeme = strdup(\"&&&\");
                token.type = TK_AND;
                token.line = current_line;
                return token;
            } else {
                token.lexeme = strdup(\"&&\");
                token.type = TK_ERROR_PATTERN;
                token.line = current_line;
                return token;
            }
        } else {
            token.lexeme = strdup(\"&\");
            token.type = TK_ERROR_SYMBOL;
            token.line = current_line;
            return token;
        }
    }
    
    // Fallback: Consume one character and mark it as an error token
    buffer_pos++;
    monitorBufferBounds();
    int len = buffer_pos - start;
    token.lexeme = (char *)malloc(len + 1);
    strncpy(token.lexeme, active_buffer + start, len);
    token.lexeme[len] = '\\0';
    token.type = TK_ERROR_SYMBOL;
    token.line = current_line;
    return token;
}

/*---------------------------------------------------------------------------
 Function: getFileByteSize
 Purpose : Returns the size (in bytes) of the given file.
---------------------------------------------------------------------------*/
/* long getFileByteSize(FILE *fp) {
    if (!fp) {
        perror("File pointer is NULL");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

/*---------------------------------------------------------------------------
 Function: displayFileContents
 Purpose : Prints the entire contents of the given file to the console.
---------------------------------------------------------------------------*/
/* void displayFileContents(FILE *fp) {
    if (!fp) {
        perror("Failed to open file");
        return;
    }
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
}

/*---------------------------------------------------------------------------
 Function: stripComments
 Purpose : Removes comments (lines beginning with '%') from the input file and prints the clean file.
---------------------------------------------------------------------------*/
/* void stripComments(char *inputFile) {
    FILE *inFile = fopen(inputFile, "r");
    if (!inFile) {
        perror("Error opening input file");
        return;
    }
    FILE *outFile = fopen("cleanFile.txt", "w");
    if (!outFile) {
        perror("Error creating clean file");
        fclose(inFile);
        return;
    }
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), inFile)) {
        char *commentPos = strchr(line, '%');
        if (commentPos) {
            *commentPos = '\\0';
        }
        fprintf(outFile, "%s\n", line);
    }
    fclose(inFile);
    fclose(outFile);
    // Display the cleaned file contents
    outFile = fopen("cleanFile.txt", "r");
    displayFileContents(outFile);
    fclose(outFile);
}

/*---------------------------------------------------------------------------
 Function: printTokenStream
 Purpose : Performs lexical analysis on the given file and prints tokens with lexemes and line numbers.
---------------------------------------------------------------------------*/
void printTokenStream(char *fileName) {
    source_file = fopen(fileName, "r");
    if (!source_file) {
        perror("Unable to open source file");
        return;
    }
    active_buffer = (char *)malloc(BUFFER_SIZE);
    backup_buffer = (char *)malloc(BUFFER_SIZE);
    size_t bytes = fread(active_buffer, 1, BUFFER_SIZE, source_file);
    characters_read = bytes;
    
    keyword_table = (struct HashMap *)malloc(sizeof(struct HashMap));
    initializeHashTable(keyword_table);
    populateKeywordTable(keyword_table);
    
    current_line = 1;
    FILE *tokenOut = fopen("lexemeToParser.txt", "w");
    if (!tokenOut) {
        perror("Error opening token output file");
        return;
    }
    
    TokenInfo token;
    while (buffer_pos < characters_read) {
        token = retrieveNextToken();
        if (token.type == TK_DELIM) continue;
        printf("Line: %-4d | Lexeme: %-25s | Token: %-20s\n",
               token.line, token.lexeme, getTokenTypeString(token.type));
        fprintf(tokenOut, "%s,%s,%d\n", token.lexeme, getTokenTypeString(token.type), token.line);
        free(token.lexeme);
    }
    
    fclose(tokenOut);
    fclose(source_file);
}
    
