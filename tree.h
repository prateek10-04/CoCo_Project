/*
ID  2022A7PS0144P					Name Prateek Parwani
ID  2022A7PS0183P					Name Sarat Srinadhu
ID  2022A7PS1181P					Name Narasimha Naidu  
ID  2022A7PS1178P 					Name Sanjay Jampani
*/

#pragma once
#include "treeADTDef.h"
#include "parserDef.h"
node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode, int expansionRule); 
treeN createNode(node el, Grammar G);
void addChild(treeN* parent, treeN* child); 
