/*
    Group Number 40
    ID  2022A7PS0144P					Name Prateek Parwani
    ID  2022A7PS0183P					Name Sarat Srinadhu
    ID  2022A7PS1178P					Name Sanjay Jampani 
    ID  2022A7PS1181P					Name Narasimha Naidu
*/

#ifndef LEXER_DEF
#define LEXER_DEF
#define BUFFER_SIZE 4096
#define MAX_LEXEME 400
#include<stdio.h>
#include<stdlib.h>
typedef struct twinBuffer twinBuffer;



struct twinBuffer{
    char *buffer;
    int forward;
    int lexemeBegin;
    int pos;   
    char *lexeme;
};
int lexError, lineNo;

typedef enum token_name{
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
    ERROR,   
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
        int num;        
        struct rnum{
            char *rep;  
            float v;    
        }rnum;
        char * str;
    }value;
}tokenInfo;
twinBuffer *twin_buffer;    
#endif
