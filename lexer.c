/*
ID  2022A7PS0183P					Name Sarat Srinadhu
ID  2022A7PS0144P					Name Prateek Parwani
ID  2022A7PS1181P					Name Narasimha Naidu  
ID  2022A7PS1178P 					Name Sanjay Jampani
*/

#include "lexer.h"
#include "lexerDef.h"
#include <fcntl.h>
#include <ctype.h>

/*
Tokens are present between lexemeBegin and forward (inclusive)
*/

/*
    checks if two characters match
*/
int char_match(char a, char b){
    return a == b;
}

/*
    Function which checks whether an identifier is a keyword
*/
token_name actualToken(token_name expected, char *lexeme){
    int j = search(lexeme);
    if(j==-1)return expected;
   
    else{
        return lookup_table[j].tkn;
    }
} 

/*
    Function checks whether a character lies in a range
*/
int range_match(char a, char start, char end) {
    return (a >= start && a <= end);
}

/*
    This function is used when a token is valid.
    The way forward, lexemeBegin, and the strings are returned depends on whether the last character of twin_buffer->lexeme is useful or not
    In all of the cases accept(false,B) is preceded by reset(1,B)
*/

char* accept(bool isLastUseful,twinBuffer *B) {
    char *str = NULL;
    int l = 0;
    
    if (isLastUseful) {
        B->lexeme[B->pos] = '\0';
        l = B->pos + 1;
        str = (char *)malloc(sizeof(char) * l);
        for (int i = 0; i < l; i++) {
            str[i] = B->lexeme[i];
        }
    } else {
        if (B->pos != 0) {
            B->lexeme[B->pos - 1] = '\0';
            l = B->pos;
            str = (char *)malloc(sizeof(char) * l);
            for (int i = 0; i < l; i++) {
                str[i] = B->lexeme[i];
            }
        }
    }
    
    dfa_state = 0; 
    
    B->lexemeBegin = B->forward + 1;
    
    memset(B->lexeme, 0, MAX_LEXEME);
    B->pos = 0;
    
    return str;
}


/*
    This function is used to return erroneous lexemes. reset(1,B) is called before reject(B) to reset t,Bhe forward 
    pointer for tokenizing other tokens.
*/

char* reject(twinBuffer *B) {
    char *str = NULL;
    int l = 0;
    
    if (B->pos != 0) {
        B->lexeme[B->pos - 1] = '\0';
        l = B->pos;
        str = (char *)malloc(sizeof(char) * l);
        for (int i = 0; i < l; i++) {
            str[i] = B->lexeme[i];
        }
    }
    
    dfa_state = 0;
    B->lexemeBegin = B->forward + 1;
    
    memset(B->lexeme, 0, MAX_LEXEME);
    B->pos = 0;
    
    return str;
}

/*
    This function populates tokens which are returned by getNextToken(FILE *fp)
*/
void populateToken(tokenInfo *TOK, token_name t, char * lexeme, int lineNo){
    TOK->line=lineNo;
    TOK->tkn_name=t;
    if (t == TK_NUM){
        TOK->value.num = atoi(lexeme);
    }
    else if (t == TK_RNUM){
        TOK->value.rnum.v=atof(lexeme);
        TOK->value.rnum.rep=(char *)malloc(sizeof(char)*strlen(lexeme));
         for(int i=0;i<strlen(lexeme);i++)
            TOK->value.rnum.rep[i] = *(lexeme +i);       
    }
    else{
        TOK->value.str=(char *)malloc(sizeof(char)*strlen(lexeme));

        for(int i=0;i<strlen(lexeme);i++)
            TOK->value.str[i] = *(lexeme +i);
        TOK->value.str[strlen(lexeme)]='\0';
    }

}   

/*
    This function initializes the twin buffer. The EOF characters are added at index BUFFER_SIZE - 1 and 2*BUFFER_SIZE - 1, to work as sentinels 
    for performing twin buffer operations
*/

twinBuffer *initializeTwinBuffer(){
    twinBuffer *B = (twinBuffer *)malloc(sizeof(twinBuffer));
    B->buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE * 2);
    B->lexeme = (char *)malloc(sizeof(char) * MAX_LEXEME);
    memset(B->buffer, 0, BUFFER_SIZE * 2);
    B->lexemeBegin = 0;
    B->forward = -1;
    B->pos = 0;
    lineNo = 1;
    B->buffer[BUFFER_SIZE - 1] = EOF;
    B->buffer[2 * BUFFER_SIZE - 1] = EOF;
    return B;
}
/*
    This funcion frees the memory allocated to the twin buffer, and sets it to NULL.
*/

void clearTwinBuffer(twinBuffer *B_ref){
    if(B_ref) {
        free(B_ref->lexeme);
        free(B_ref->buffer);
        free(B_ref);
        B_ref = NULL;
    }
}
/*
    This function adds characters to twin_buffer->lexeme if it isn't a part of comment and if the length isnt exceeded. Here the working of twin_buffer is implemented.
    twin_buffer->forward is always incremented first.
    Also updates the position of pos to which new character is added.
    Returns the character which is added to twin_buffer->lexeme
*/

char next_char(twinBuffer *B, FILE *fp, bool isComment, bool isLengthExceeded){
    B->forward++;
    if(B->buffer[B->forward] == EOF){
        if(B->forward == BUFFER_SIZE - 1){
            fp = getStream(B, fp, 1);
            B->forward = BUFFER_SIZE;
        }
        else if(B->forward == 2 * BUFFER_SIZE - 1){
            fp = getStream(B, fp, 0);
            B->forward = 0;
        }
    }
    if(!isComment && !isLengthExceeded){
        B->lexeme[B->pos] = B->buffer[B->forward];
        B->pos++;
    }
    return B->buffer[B->forward];
}


/*  bufferNumer=0 implies first buffer to be filled
    bufferNumber=1 implies second buffer to be filled
*/

FILE *getStream(twinBuffer *B, FILE *fp, int bufferNumber){
    int x;
    if(bufferNumber == 0) {
        x = fread(&B->buffer[0], 1, BUFFER_SIZE - 1, fp);
        if(x != BUFFER_SIZE - 1)
            B->buffer[x] = EOF;
    }
    else {
        x = fread(&B->buffer[BUFFER_SIZE], 1, BUFFER_SIZE - 1, fp);
        if(x != BUFFER_SIZE - 1)
            B->buffer[BUFFER_SIZE + x] = EOF;
    }
    return fp;
}

/*
    updates twin_buffer->forward pointer
*/
void reset(int amt,twinBuffer *B) {
    B->forward-=amt;
}


/*
    This function returns the next token (which can be ERROR)
    that lies between twin_buffer->lexemeBegin and twin_buffer->forward
*/
tokenInfo getNextToken(twinBuffer *B, FILE *fp){
    char c;
    lexError = 0; 
    tokenInfo *TOK = (tokenInfo *)malloc(sizeof(tokenInfo));

    while(1){
        switch(dfa_state){
            case 0: {
                c = next_char(B,fp,false,false);

                if(char_match(c,'<')){
                    dfa_state = 1;
                }
                else if(char_match(c,'\n')||char_match(c,'\t')||char_match(c,'\f')||char_match(c,'\v')||char_match(c,'\r')){
                    dfa_state = 7;
                    if(char_match(c,'\n'))lineNo++;
                }
                else if(char_match(c,'>')){
                    dfa_state = 9;
                }
                else if(char_match(c,'=')){
                    dfa_state = 12;
                }
                else if(char_match(c,'!')){
                    dfa_state = 14;
                }
                else if(range_match(c,'b','d')){
                    dfa_state = 16;
                }
                else if(char_match(c,'a') || range_match(c,'e','z')) {
                    dfa_state = 20;
                }
                else if(char_match(c,'_')) {
                    // printf("Hello4\n");
                    dfa_state = 22;
                }
                else if(char_match(c,'#')) {
                    dfa_state = 26;
                }
                else if(range_match(c,'0','9')) {
                    dfa_state = 29;
                }
                else if(char_match(c,'%')) {
                    dfa_state = 58;
                }
                else if(char_match(c,':')) {
                    dfa_state = 57;
                }
                else if(char_match(c,',')) {
                    dfa_state = 56;
                }
                else if(char_match(c,'[')) {
                    dfa_state = 55;
                }
                else if(char_match(c,']')) {
                    dfa_state = 54;
                }
                else if(char_match(c,'(')) {
                    dfa_state = 53;
                }
                else if(char_match(c,')')) {
                    dfa_state = 52;
                }
                else if(char_match(c,';')) {
                    dfa_state = 51;
                }
                else if(char_match(c,'/')) {
                    dfa_state = 50;
                }
                else if(char_match(c,'*')) {
                    dfa_state = 49;
                }
                else if(char_match(c,'~')) {
                    dfa_state = 48;
                }
                else if(char_match(c,'-')) {
                    dfa_state = 47;
                }
                else if(char_match(c,'+')) {
                    dfa_state = 46;
                }
                else if(char_match(c,'@')) {
                    dfa_state = 43;
                }
                else if(char_match(c,'.')) {
                    dfa_state = 42;
                }
                else if(char_match(c,'&')) {
                    dfa_state = 39;
                }
                else if(char_match(c,'\n')|| char_match(c,' ') || char_match(c,'\f') || char_match(c,'\r') || char_match(c,'\t') || char_match(c,'\v')) {
                    dfa_state = 7;
                }


             
                else if(char_match(c,EOF) || c==0) {  
                    char *lex = accept(true,B);
                    populateToken(TOK,TK_EOF,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                else if(range_match(c,'A','Z')){    
                    dfa_state=59;

                }
                else {
                    printf("Line No: %d - ERROR: Unknown symbol <%c>\n" ,lineNo,c);
                    char *str = accept(true,B);
                    populateToken(TOK,ERROR,str,lineNo);
                    lexError = 1;    
                    dfa_state = 0;  
                    return getNextToken(B, fp);  
                }
                break;
            }

            
            case 1: {
                c = next_char(B,fp,false,false);
                if(c == '-') {
                    dfa_state = 2;
                }
                else if(c == '=') {
                    dfa_state = 6;
                }
                else {
                    dfa_state = 5;
                }
                break;
            }
            case 2: {
                c = next_char(B,fp,false,false);
                if(c == '-') {
                    dfa_state = 3;
                }
                else {
                    reset(1,B);
                    char* pattern = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                }
                break;
            }
            case 3: {
                c = next_char(B,fp,false,false);
                if(c == '-') {
                    dfa_state = 4;
                }
                else {
                    reset(1,B);
                    char* pattern = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError=2;
                    dfa_state=0;
                }
                break;
            }
            case 4: {
                char *str = accept(true,B);
                populateToken(TOK,TK_ASSIGNOP,str,lineNo);
                return *TOK;
                break;
            }   
            case 5: {
                reset(1,B);
                char *str = accept(false,B);
                populateToken(TOK,TK_LT,str,lineNo);
                return *TOK;
                break;
            }    
            case 6: {
                char *str = accept(true,B);
                populateToken(TOK,TK_LE,str,lineNo);
                return *TOK;
                break;
            }

            case 7: {
                c = next_char(B,fp,false,false);
                if(c == '\n'||c == '\r'||c == '\t'||c == '\f'||c == '\v') {
                    dfa_state = 7;
                    if(char_match(c,'\n'))lineNo++;
                }
                else dfa_state =8;
                break;
            }
            case 8: {
                reset(1,B); 
                accept(false,B);
                break;
            }
            case 9: {
                c = next_char(B,fp,false,false);
                if(c == '=') {
                    dfa_state = 10;
                }
                else {
                    dfa_state = 11;
                }
                break;
            }
            case 10: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_GE,lex,lineNo);
                return *TOK;
                break;
            }
            case 11: {
                reset(1,B);
                char *lex=accept(false,B);
                populateToken(TOK,TK_GT,lex,lineNo);
                return *TOK;
                break;
            }
            case 12: {
                c = next_char(B,fp,false,false);
                if(c == '=') {
                    dfa_state = 13;
                }
                else {
                    reset(1,B);
                    char *pattern = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,pattern);
                    populateToken(TOK,ERROR,pattern,lineNo);
                    free(pattern);
                    lexError=2;
                    dfa_state = 0;
                    return getNextToken(B, fp);
                }
                break;
            }
            case 13: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_EQ,lex,lineNo);
                return *TOK;
                break;
            }

            case 14: {
                c = next_char(B,fp,false,false);
                if(c == '=') {
                    dfa_state = 15;
                }
                else {
                    reset(1,B);
                    char* pattern = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,pattern);
                    populateToken(TOK,ERROR,pattern,lineNo);
                    free(pattern);
                    lexError = 2;
                    dfa_state = 0;
                }
                break;
            }
            case 15: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_NE,lex,lineNo);
                return *TOK;
                break;
            }

             case 16: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'2','7')) {
                    dfa_state = 17;
                }
                else if(range_match(c,'a','z')) {
                    dfa_state = 20;
                }
                else {
                    dfa_state = 21;
                }
                break;
            }
            case 17: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'b','d')){
                    if(strlen(B->lexeme)<21)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }

                if(range_match(c,'2','7'))
                    dfa_state = 18;
                else {
                    dfa_state = 19;
                }   
                break;
            }
            case 18: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'2','7')){
                    if(strlen(B->lexeme)<21)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }

                if(!range_match(c,'2','7') || strlen(B->lexeme)>21) {
                    dfa_state = 19;
                }
                break;
            }
            case 19: {     
                reset(1,B);
                char * lex = accept(false,B);
                if(strlen(lex) < 2) {
                    printf("Line No %d: Error :Variable Identifier is smaller than the prescribed length of 2 characters\n" , lineNo);
                    lexError=3;
                    populateToken(TOK,ERROR,lex,lineNo);
                }
                else if(strlen(lex) > 20) {
                    printf("Line No %d: Error :Variable Identifier is longer than the prescribed length of 20 characters\n" ,lineNo);
                    lexError = 3;
                    populateToken(TOK,ERROR, lex, lineNo);
                }
                else {
                    populateToken(TOK,actualToken(TK_ID, lex),lex,lineNo);
                }
                return *TOK;
                break;
            }
        
            case 20: { 
                c = next_char(B,fp,false,false);
                while(range_match(c,'a','z')){
                    if(strlen(B->lexeme)<21)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }
                dfa_state = 21;
                break;
            }
            case 21: {
                reset(1,B);
                char* lex = accept(false,B);
                    populateToken(TOK,actualToken(TK_FIELDID, lex),lex,lineNo);
                dfa_state=0;
                return *TOK;
                break;
            }

             case 22: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'a','z') || range_match(c,'A','Z')) {
                    dfa_state = 23;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    free(lex);
                    lexError = 2;
                    populateToken(TOK, ERROR, lex, lineNo);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 23: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'a','z') || range_match(c,'A','Z')){
                    if (strlen(B->lexeme)<31)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }
                if(range_match(c,'0','9')) {
                    dfa_state = 24;
                }
                else {
                    dfa_state = 25;
                }
                break;
            }
            case 24: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'0','9')){
                    if(strlen(B->lexeme)<31)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }
                dfa_state = 25;

                break;
            }
            case 25: {              
                reset(1,B);
                char *lex = accept(false,B);
                if(strlen(lex) > 30) {
                    printf("Line No %d: Error :Function Identifier is longer than the prescribed length of 30 characters\n" ,lineNo);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                }
                else {
                    populateToken(TOK,actualToken(TK_FUNID,lex),lex,lineNo);
                }
                dfa_state=0;
                return *TOK;
                break;
            }
            case 26: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'a','z')) {
                    dfa_state = 27;
                }
                else {
                    reset(1,B);
                    char* pattern = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                    populateToken(TOK,ERROR,pattern,lineNo);
                    dfa_state = 0;
                    return getNextToken(B, fp);
                }
                break;
            }
            case 27: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'a','z')){
                    if(strlen(B->lexeme)<21)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }

                dfa_state = 28;
                break;
            }
            case 28: {
                    reset(1,B);
                    char* lex = accept(false,B);
                    populateToken(TOK, TK_RUID, lex,lineNo);
                return *TOK;
                dfa_state = 0;
                break;
            }
            
             case 29: {
                c = next_char(B,fp,false,false);
                while(range_match(c,'0','9')){
                    if(strlen(B->lexeme)<10)
                        c = next_char(B,fp,false,false);
                    else
                        c = next_char(B,fp,false,true);
                }
                if(c == '.') {
                    dfa_state = 31;
                }
                else {
                    dfa_state = 30;
                }
                break;
            }
            case 30: {
                if(strlen(B->lexeme)>10){
                    lexError = 3;
                    reset(1,B);
                    char *lex = reject(B);
                    printf("Line No %d : Error : the integer %s is out of range\n" ,lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);                  
                }
                else{
                    reset(1,B);
                    char * lex = accept(false,B);
                    populateToken(TOK,TK_NUM,lex,lineNo);
                }
                return *TOK;
                break;
            }
            case 31: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 32;
                }
                else {
                    reset(1,B);
                    char *lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo, lex);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);   
                }
                break;
            }
            case 32: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 34;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 33: {
                reset(1,B);
                char* lex = accept(false,B);
                populateToken(TOK,TK_RNUM,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 34: {
                c = next_char(B,fp,false,false);
                if(char_match(c,'E')){
                    if(char_match(c,'e'))B->lexeme[B->pos]='E';
                    dfa_state = 35;
                }
                else{
                    dfa_state = 33;
                }
                break;
            }
            case 35: {
                c = next_char(B,fp,false,false);
                if(char_match(c, '+') || char_match(c, '-')){
                    dfa_state = 36;
                }
                else if (range_match(c,'0','9')){
                    dfa_state = 37;
                }
                else{
                    reset(1,B);
                    char *lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 36: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 37;
                }
                else{
                    reset(1,B);
                    char *lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n",lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    lexError = 2;
                    return getNextToken(B, fp);

                }
                break;
            }
            case 37: {
                c = next_char(B,fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 38;
                }
                else{
                    reset(1,B);
                    char *lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n",lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    lexError = 2;
                    return getNextToken(B, fp);
                }
                break;
            }
            case 38: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_RNUM,lex,lineNo);
                return *TOK;
                break;
            }
            case 39: {
                c = next_char(B,fp,false,false);
                if(c == '&') {
                    dfa_state = 40;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError= 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 40: {
                c = next_char(B,fp,false,false);
                if(c == '&') {
                    dfa_state = 41;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 41: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_AND,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 42: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_DOT,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 43: {
                c = next_char(B,fp,false,false);
                if(c == '@') {
                    dfa_state = 44;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return getNextToken(B, fp);
                }
                break;
            }
            case 44: {
                c = next_char(B,fp,false,false);
                if(c == '@') {
                    dfa_state = 45;
                }
                else {
                    reset(1,B);
                    char* lex = reject(B);
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    return getNextToken(B, fp);    
                }
                break;
            }

            case 45: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_OR,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 46: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_PLUS,lex,lineNo);
                return *TOK;
                break;
            }
            
            case 47: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_MINUS,lex,lineNo);
                return *TOK;
                break;
            }

            case 48: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_NOT,lex,lineNo);
                return *TOK;
                break;
            }

            case 49: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_MUL,lex,lineNo);
                return *TOK;
                break;
            }
            case 50: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_DIV,lex,lineNo);
                return *TOK;
                break;
            }
            case 51: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_SEM,lex,lineNo);
                return *TOK;
                break;
            }
            case 52: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_CL,lex,lineNo);
                return *TOK;
                break;
            }
            case 53: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_OP,lex,lineNo);
                return *TOK;
                break;
            }
            case 54: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_SQR,lex,lineNo);
                return *TOK;
                break;
            }
            case 55: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_SQL,lex,lineNo);
                return *TOK;
                break;
            }
           
            case 56: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_COMMA,lex,lineNo);
                return *TOK;
                break;
            }
            
            case 57: {
                char* lex = accept(true,B);
                populateToken(TOK,TK_COLON,lex,lineNo);
                return *TOK;
                break;
            }     
          
            case 58: {
                int commentLine = lineNo;  
                c = next_char(B, fp, true, false);
                while(c != '\n' && c != EOF) {
                    c = next_char(B, fp, true, false);
                }
                if(c == '\n') {
                    lineNo++; 
                }
                char *str = accept(true, B);
                populateToken(TOK, TK_COMMENT, str, commentLine);
                return *TOK;
            }

            
            

            case 59: { 
                c = next_char(B,fp,false,false);
                while(range_match(c,'A','Z') || range_match(c,'a','z')) {
                    c = next_char(B,fp,false,false);
                }
                reset(1,B);
                char *lex=accept(false,B);
                token_name t = actualToken(ERROR,lex);
                if(t==ERROR){
                    printf("Line No: %d - ERROR: Unknown pattern <%s>\n",lineNo,lex);
                }
                populateToken(TOK,t,lex,lineNo);
                return getNextToken(B, fp);
                break;
            }
        }   
    }
}
void removeComments(char *testcaseFile, char *cleanFile) {
    FILE *test_fptr = fopen(testcaseFile, "r");
    if (!test_fptr) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    
    FILE *fptr = fopen(cleanFile, "w");
    if (!fptr) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    
    char line[1024];
    while (fgets(line, sizeof(line), test_fptr)) {
        line[strcspn(line, "\n")] = '\0';
        
        char *ptr = line;
        while (*ptr && isspace((unsigned char)*ptr))
            ptr++;
        
        if (*ptr == '%')
            continue;
        
        char *commentPos = strchr(line, '%');
        if (commentPos) {
            *commentPos = '\0';
        }
        
        int len = strlen(line);
        while (len > 0 && isspace((unsigned char)line[len - 1])) {
            line[len - 1] = '\0';
            len--;
        }
        
        if (strlen(line) > 0)
            fprintf(fptr, "%s\n", line);
    }
    
    fclose(fptr);
    fclose(test_fptr);
}

/*
    This function invokes the lexer and prints all the lexemes and the corresponding tokens (and errors if present).
*/
void prettyPrint(char *testcaseFile){
    FILE *f = fopen(testcaseFile,"r");
    printf("\n--- PRETTY PRINT LEXER ---\n");
    twinBuffer *B = initializeTwinBuffer();
    f = getStream(B, f, 0);
    tokenInfo t = getNextToken(B, f);

    while(t.tkn_name!=TK_EOF){
        if(t.tkn_name==TK_NUM)
            printf("Line no. %-5d  Lexeme %-30d Token %-5s\n",t.line,t.value.num, enumToString[t.tkn_name]);
        else if(t.tkn_name==TK_RNUM)
            printf("Line no. %-5d  Lexeme %-30s Token %-5s\n",t.line,t.value.rnum.rep, enumToString[t.tkn_name]);
        else
        {
            printf("Line no. %-5d  Lexeme %-30s Token %-5s\n",t.line,t.value.str, enumToString[t.tkn_name],t.tkn_name);      
        }
        t = getNextToken(B, f);
    }
    clearTwinBuffer(B);
    fclose(f);
    f=NULL;
}
