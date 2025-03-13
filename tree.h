/*
ID  2019A7PS0064P					Name Siddharth Sharma
ID  2019A7PS0062P					Name Atharva Chandak
ID  2019A7PS0133P					Name Archit Bhatnagar 
ID  2019A7PS0554P					Name Suchismita Tripathy
ID  2019A7PS1139P 					Name Srujan Deolasee
*/

#pragma once
#include "treeADTDef.h"
#include "parserDef.h"
node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode, int expansionRule); 
treeN createNode(node el, Grammar G);
void addChild(treeN* parent, treeN* child); 
