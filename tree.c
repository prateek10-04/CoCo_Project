/*
    ID  2022A7PS0144P					Name Prateek Parwani
    ID  2022A7PS0183P					Name Sarat Srinadhu
    ID  2022A7PS1178P					Name Sanjay Jampani 
    ID  2022A7PS1181P					Name Narasimha Naidu
*/

#include "tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode, int expansionRule) { 
    node newElement; 
    newElement.lineNo = lineNo; 
    newElement.parentNodeSymbolID = parentSymbolID; 
    newElement.curr = symbolID; 
    newElement.isLeaf = isLeafNode; 
    newElement.ruleNumber = expansionRule;
    if (isLeafNode == 0) { 
        newElement.lex.lexemeStr = "____"; 
    } 
    return newElement; 
}

treeN createNode(node el, Grammar G) {
    treeN newNode; 

    newNode.children = NULL; 
    newNode.numChild = 0; 
    newNode.elem.curr = el.curr; 
    newNode.elem.isLeaf = el.isLeaf; 
    if (strcmp(G.terminals[el.curr], "TK_NUM") == 0)
        newNode.elem.lex.numVal = el.lex.numVal;
    else if (strcmp(G.terminals[el.curr], "TK_RNUM") == 0)
        newNode.elem.lex.rVal = el.lex.rVal;
    else {
        newNode.elem.lex.lexemeStr = el.lex.lexemeStr;
    }
    newNode.elem.lineNo = el.lineNo; 
    newNode.elem.parentNodeSymbolID = el.parentNodeSymbolID;
    newNode.elem.ruleNumber = el.ruleNumber;
    return newNode;
}

void addChild(treeN* parent, treeN* child) {
    if (parent->numChild != 0) { 
        parent->children = (treeN**) realloc(parent->children, sizeof(treeN*) * (parent->numChild + 1)); 
        parent->children[parent->numChild] = child; 
        parent->numChild += 1; 
    } 
    else { 
        parent->children = (treeN**) malloc(sizeof(treeN*)); 
        parent->children[0] = child; 
        parent->numChild = 1;
    }       
}
