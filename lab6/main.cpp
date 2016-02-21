#include <iostream>
#include <cstdlib>
#include <ctime>
#include "btree.h"


using namespace std;

void print_btreenode( BTreeNode * node, int level )
{
	if( node == NULL ) {
		cout << "node is NULL";
		return;
	}
	cout << "[" << node->keys[0];
	for( int i = 1; i < node->n; ++i ) {
		cout << " " << node->keys[i];
	}
	cout << "]";
}

void print_btree1( BTreeNode * root, int level )
{
	for( int i = 0; i < level; ++i ) {
		cout << "  ";
	}
	if( root == NULL ) {
		cout << "root is NULL" << endl;
		return;
	}
	print_btreenode( root, level );
	cout << endl;
	
	if( !root->leaf ) {
		for(int i = 0; i < root->n + 1; ++i ) {
			print_btree1( root->children[i], level+1 );
		}
	}

}

void print_btree( BTreeNode * root )
{
	print_btree1( root, 0 );
}

int main()
{
	srand( time( NULL ) );
	BTreeNode * tr = new BTreeNode( 2 );

	for( int i = 1; i < 40; ++i ) {
		int n = rand()%100 + 1;
		btree_insert( tr, n );
		cout << "inserted " << n << endl;
		print_btree( tr );
		cout << "*****************" << endl;
	}

	cout << "tree height: " << btree_height( tr ) << endl;
	cout << "printing:" << endl;
	print_btree( tr );

	return 0;
}
