// lexerDef.h
#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

// Enum representing all possible token types in the language
typedef enum {
    TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID,
    TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS,
    TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT,
    TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF,
    TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
    TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ,
    TK_GT, TK_GE, TK_NE, TK_ERROR_PATTERN, TK_ERROR_SYMBOL, TK_ERROR_ASSIGNOP,
    TK_ERROR_SIZE20, TK_ERROR_SIZE30, TK_DELIM
} TokenType;

// Structure representing a token with associated metadata
typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} TokenInfo;

// Structure for a linked list node in the keyword hash table
struct HashNode {
    char *key;
    TokenType token;
    struct HashNode *next;
};

// Hash table for efficient keyword recognition
struct HashMap {
    int size, capacity;
    struct HashNode **buckets;
};

// Buffer and stream-related global variables
extern char *active_buffer, *backup_buffer;
extern int buffer_pos, buffer_limit, current_line;
extern FILE *source_file;
extern struct HashMap *keyword_table;
extern size_t characters_read;

#endif // LEXERDEF_H
