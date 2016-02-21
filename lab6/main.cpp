#include <iostream>
#include "btree.h"

using namespace std;

void print_btree( BTreeNode * root )
{
	BTreeNode * p;
	BTreeNode * c = root;

	// level 1
	//if( c->leaf )
		//cout << "L";
	for( int i = 0; i < c->n; ++i ) {
		cout << c->keys[i] << " ";
	}
	cout << endl;

	p = c;

	if( !p->leaf ) {
	// level 2
	for( int i = 0; i < p->n+1; ++i ) {
		c = p->children[i];
		//if( c->leaf )
			//cout << "L";
		cout << c->keys[0];
		for( int j = 1; j < c->n; ++j ) {
			cout << " " << c->keys[j];
		}
		cout << "|";
	}
	cout << endl;

	if( !p->children[0]->leaf ) {
	// level 3
	for( int i = 0; i < p->n+1; ++i ) {
		c = p->children[i];
		for( int j = 0; j < c->n+1; ++j ) {
			BTreeNode * c2 = c->children[j];
			//if( c2->leaf )
				//cout << "L";
			cout << c2->keys[0];
			for( int k = 1; k < c2->n; ++k ) {
				cout << " " << c2->keys[k];
			}
			cout << "|";
		}
		cout << "|";
	}
	cout << endl;

	// level 4
	if( !p->children[0]->children[0]->leaf ) {
	for( int i = 0; i < p->n+1; ++i ) {
		c = p->children[i];
		for( int j = 0; j < c->n+1; ++j ) {
			BTreeNode * c2 = c->children[j];
			for( int k = 0; k < c2->n+1; ++k ) {
				BTreeNode * c3 = c2->children[k];
				cout << c3->keys[0];
				for( int l = 1; l < c3->n; ++l ) {
					cout << " " << c3->keys[l];
				}
				cout << "|";
			}
			cout << "|";
		}
		cout << "|";
	}
	cout << endl;
	}
	}
	}
}

int main()
{
	BTreeNode * tr = new BTreeNode( 4 );

	for( int i = 0; i < 80; ++i ) {
		btree_insert( tr, i );
		//cout << "inserted " << i << endl;
		//print_btree( tr );
		//cout << "*****************" << endl;
	}

	cout << "tree height: " << btree_height( tr ) << endl;
	cout << "printing:" << endl;
	print_btree( tr );

	return 0;
}
