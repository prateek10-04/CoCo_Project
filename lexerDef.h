/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#ifndef LEXER_DEF
#define LEXER_DEF
#define BUFFER_SIZE 4096
#define MAX_LEXEME 400
// #include "lookuptable.h"
#include<stdio.h>
#include<stdlib.h>
typedef struct twinBuffer twinBuffer;



struct twinBuffer{
    char *buffer;
    int forward;
    int lexemeBegin;
    int pos;    // position at which character should be added to lexeme
    char *lexeme;
};
int lexError, lineNo;
/*
0- no error
1- unknown symbol
2- unknown pattern
3- length exceeded  
*/
typedef enum token_name{
    // tokens list
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    ERROR,   // special token when error is encountered
    TK_EOF,
}token_name;

static const char * enumToString[]={"TK_ASSIGNOP","TK_COMMENT","TK_FIELDID","TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RUID","TK_WITH","TK_PARAMETERS","TK_END",
"TK_WHILE","TK_UNION","TK_ENDUNION","TK_DEFINETYPE","TK_AS","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR","TK_INPUT","TK_OUTPUT","TK_INT","TK_REAL",
"TK_COMMA","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV",
"TK_CALL","TK_RECORD","TK_ENDRECORD","TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE","ERROR","TK_EOF"};
typedef struct TOKEN{
    int line;
    token_name tkn_name;
    struct value{
        int num;        // numeric value of int
        struct rnum{
            char *rep;  // string form of reals
            float v;    // numeric value of int
        }rnum;
        char * str; // lexeme, identifiers
    }value;
}tokenInfo;
twinBuffer *twin_buffer;    // buffer the lexer
#endif

/*
To Do:

1. Hash Table/ Symbol Table
2. getnextToken



*/
