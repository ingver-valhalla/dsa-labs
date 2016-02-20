#include "tree.h"
#include <cstdlib>

pNode NewNode( Item D)
{
	pNode pN = new Node;
	pN->Left = NULL;
	pN->Right = NULL;
	pN->Data = D;
	pN->Size = 1;

	return pN;
}


void VisitPre( pNode root, pFunction Function )
{
	if( root ) {
		Function( root );
		VisitPre( root->Left, Function );
		VisitPre( root->Right, Function );
	}
}


void VisitPost( pNode root, pFunction Function )
{
	if( root ) {
		VisitPost( root->Left, Function );
		VisitPost( root->Right, Function );
		Function( root );
	}
}


void Insert( pNode * proot, Item D )
{
#define root (*proot)
	if( !root )
		root = NewNode( D );
	else if ( D < root->Data )
		Insert( &(root->Left), D );
	else if( D > root->Data )
		Insert( &(root->Right ), D );
#undef root
}


pNode Find( pNode root, pNode * parent, int * LR, Item D )
{
	*parent = NULL;
	*LR = 0;
	while( root ) {
		if( D < root->Data ) {
			*parent = root;
			*LR = 0;
			root = root->Left;
		}
		else if( D > root->Data ) {
			*parent = root;
			*LR = 1;
			root = root->Right;
		}
		else 
			return root;
	}
	return NULL;
}


pNode FindMinRight( pNode root, pNode * parent )
{
	while( root->Left ) {
		*parent = root;
		root = root->Left;
	}
	return root;
}


void Delete( pNode root )
{
	if( root )
		delete root;
}


void DeleteNode( pNode * ROOT, Item D )
{
	int LR = 0;
	pNode parent = NULL, root = Find( *ROOT, &parent, &LR, D );
	if( root ) {
		if( root->Left && root->Right ) {
			pNode MinRightParent = root;
			pNode MinRight = FindMinRight( root->Right, &MinRightParent );
			MinRight->Left = root->Left;
			MinRightParent->Left = MinRight->Right;
			MinRight->Right = root->Right;

			if( parent )
				if( LR )
					parent->Right = MinRight;
				else
					parent->Left = MinRight;
			else
				*ROOT = MinRight;
		}
		else if( parent ) {
			if( LR )
				parent->Right = root->Right ? root->Right : root->Left;
			else
				parent->Left = root->Right ? root->Right : root->Left;
		}
		else {
			*ROOT = root->Right ? root->Right : root->Left;
		}

		Delete( root );
	}
}


void RotateRight( pNode * pLev0 )
{
#define Lev0 (*pLev0)
	pNode Lev1 = Lev0->Left;
	Lev0->Left = Lev1->Right;
	Lev1->Right = Lev0;
	Lev0 = Lev1;
	fix_size( Lev0->Right );
	fix_size( Lev0 );
#undef Lev0
}


void RotateLeft( pNode * pLev0 )
{
#define Lev0 (*pLev0)
	pNode Lev1 = Lev0->Right; Lev0->Right = Lev1->Left;
	Lev1->Left = Lev0;Lev0 = Lev1;
	fix_size( Lev0->Left );
	fix_size( Lev0 );
#undef Lev0
}

void PrintTree1( pNode root, int left )
{
	static int level = 1;
	if( root == NULL )
	return;


	for( int i = 0; i < level; ++i ) {
		printf("   |");
	}

	if( left )
		printf( "<" );
	else
		printf( ">" );

	printf("%d(%d)\n", root->Data, root->Size );
	++level;
	PrintTree1( root->Left, 1 );
	PrintTree1( root->Right, 0 );
	--level;
}

void PrintTree( pNode root )
{
	printf("%d(%d)\n", root->Data, root->Size);
	PrintTree1( root->Left, 1 );
	PrintTree1( root->Right, 0 );
}

int TreeHeight( pNode root )
{
	if( root == NULL ) {
		return 0;
	}
	else {
		int lh = TreeHeight( root->Left );
		int rh = TreeHeight( root->Right );
		return 1 + ((lh > rh) ? lh : rh);
	}
}


int CountNodes( pNode root )
{
	if( root == NULL ) {
		return 0;
	}
	else {
		return 1 + CountNodes( root->Left ) + CountNodes( root->Right );
	}
}

int get_size( pNode root )
{
	return root ? root->Size : 0;
}
void fix_size( pNode root )
{
	root->Size = get_size( root->Left ) + get_size( root->Right ) + 1;
}

int InsertRoot( pNode * proot, Item D )
{
#define root (*proot)
	if( !root ) {
		root = NewNode( D );
		return 1;
	}
	else if( D < root->Data ) {
		if( InsertRoot( &(root->Left), D ) ) {
			RotateRight( &(root) );
			return 1;
		}
		return 0;
	}
	else if( D > root->Data ) {
		if( InsertRoot( &(root->Right), D ) ) {
			RotateLeft( &(root) );
			return 1;
		}
		return 0;
	}
	return 0;
#undef root
}

void InsertRand( pNode * proot, Item D )
{
#define root (*proot)
	if( root == NULL ) {
		root = NewNode( D );
	}
	else if ( D == root->Data ) {
		return;
	}
	else if( rand() < RAND_MAX / root->Size ) {
		InsertRoot( proot, D );
	}
	else if( D < root->Data ) {
		InsertRand( &(root->Left), D );
		fix_size( root );
	}
	else {
		InsertRand( &(root->Right), D );
		fix_size( root );
	}
#undef root
}


int is_even( int n )
{
	return n % 2 == 0;
}

int CountSum( pNode root, pFunction print )
{
	if( root == NULL ) {    
		return 0;
	}
	else if( root->Size - 1 <= 4
	         && is_even( root->Data )
	         && root->Data < 0 )
	{
		print( root );
		return root->Data
		       + CountSum( root->Left, print )
		       + CountSum( root->Right, print );
	}
	else {
		return CountSum( root->Left, print )
		       + CountSum( root->Right, print );
	}
	return 0;
}


int SumOfInner1( pNode root )
{
	if( root == NULL
	    || (root->Left == NULL && root->Right == NULL) )
	{
		return 0;
	}
	else
		return SumOfInner1( root->Left ) 
		       + SumOfInner1( root->Right )
		       + root->Data;
}

int SumOfInner( pNode root )
{
	return SumOfInner1( root->Left ) + SumOfInner1( root->Right );
}

void FreeTree( pNode * proot )
{
#define root (*proot)
	if( proot == NULL || root == NULL ) {
		return;
	}

	FreeTree( &(root->Left) );
	FreeTree( &(root->Right) );
	delete root;
	root = NULL;
#undef root
}
