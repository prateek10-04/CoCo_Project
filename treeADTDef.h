/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
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
