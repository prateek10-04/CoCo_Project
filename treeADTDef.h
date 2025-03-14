/*
    ID  2022A7PS0144P					Name Prateek Parwani
    ID  2022A7PS0183P					Name Sarat Srinadhu
    ID  2022A7PS1178P					Name Sanjay Jampani 
    ID  2022A7PS1181P					Name Narasimha Naidu
*/

typedef struct node { 
    int lineNo; 
    int parentNodeSymbolID; 
    int curr; 
    int isLeaf; 
    struct lex { 
        char* lexemeStr; 
        int numVal; 
        float rVal; 
    } lex; 
    int ruleNumber;    
} node; 

typedef struct treeNode {
    struct treeNode** children;
    struct node elem; 
    int numChild; 
} treeN; 
