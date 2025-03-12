/*
ID  2022A7PS0144P					Name Prateek Parwani
ID  2022A7PS0183P					Name Sarat Srinadhu
ID  2022A7PS1181P					Name Narasimha Naidu  
ID  2022A7PS1178P 					Name Sanjay Jampani
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
    int ruleNumber;     // This variable stores the rule number used for expansion, 0 means that it is a terminal
} node; 

typedef struct treeNode {
    struct treeNode** children;
    struct node elem; 
    int numChild; 
} treeN; 
