// tree.h
#ifndef TREE_H
#define TREE_H
#include <cstdio>

typedef struct Node * pNode;
typedef void (*pFunction) (const pNode);
typedef int Item;

struct Node {
	pNode Left;
	pNode Right;
    Item Data;
    int Size;
};

pNode NewNode( Item D );
void VisitPre( pNode root, pFunction Function );
void VisitPost( pNode root, pFunction Function );
void Insert( pNode * proot, Item D );
pNode Find( pNode root, pNode * parent, int * LR, Item D );
pNode FindMinRight( pNode root, pNode * parent );
void DeleteNode( pNode * ROOT, Item D );
void RotateRight( pNode * pLev0 );
void RotateLeft( pNode * pLev0 );
int InsertRoot( pNode * proot, Item D );
void PrintTree( pNode root );
int TreeHeight( pNode root );
int CountNodes( pNode root );
void InsertRand( pNode * root, Item D );
int get_size( pNode root );
void fix_size( pNode root );
int CountSum( pNode root, pFunction print );
int SumOfInner( pNode root );
void FreeTree( pNode * proot );

#endif // TREE_H
