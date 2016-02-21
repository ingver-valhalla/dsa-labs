// btree.cpp

#include <cstdlib>
#include "btree.h"

BTreeNode * btree_search( BTreeNode * node, Key k, int * index )
{
	int i = 0;

	while( i < node->n && k > node->keys[i] )
		++i;

	if( i < node->n && k == node->keys[i] ) {
		*index = i;
		return node;
	}
	else if( node->leaf ) {
		return NULL;
	}
	else {
		return btree_search(node->children[i], k, index );
	}
}

void btree_split_child( BTreeNode * node, int index )
{
	int d = node->min_degree;
	BTreeNode * sibling = new BTreeNode( d );
	BTreeNode * child = node->children[index];
	sibling->leaf = child->leaf;
	sibling->n = d - 1;

	for( int j = 0; j < d - 1; ++j ) {
		sibling->keys[j] = child->keys[j+d];
	}

	if( !child->leaf ) {
		for( int j = 0; j < d; ++j ) {
			sibling->children[j] = child->children[j+d];
		}
	}

	child->n = d - 1;

	for( int j = node->n; j >= index+1; --j ) {
		node->children[j+1] = node->children[j];
	}

	node->children[index+1] = sibling;

	for( int j = node->n - 1; j > index; --j ) {
		node->keys[j+1] = node->keys[j];
	}

	node->keys[index] = child->keys[d-1];
	++node->n;
}

void btree_insert_nonfull( BTreeNode * node, Key k )
{
	int i = node->n - 1;
	if( node->leaf ) {
		for( ; i >= 0 && k < node->keys[i]; --i ) {
			node->keys[i+1] = node->keys[i];
		}
		node->keys[i+1] = k;
		++node->n;
	}
	else {
		for( ; i >= 0 && k < node->keys[i]; --i );
		++i;
		if( node->children[i]->n == 2*node->min_degree - 1 ) {
			btree_split_child( node, i );
			if( k > node->keys[i] )
				++i;
		}
		btree_insert_nonfull( node->children[i], k );
	}
}

void btree_insert( BTreeNode *& root, Key k )
{
	BTreeNode * r = root;
	BTreeNode * s;
	int d = root->min_degree;

	if( r->n == 2*d - 1 ) {
		s = new BTreeNode( d );
		root = s;
		s->leaf = false;
		s->n = 0;
		s->children[0] = r;
		btree_split_child( s, 0 );
		btree_insert_nonfull( s, k );
	}
	else {
		btree_insert_nonfull( r, k );
	}
}

int btree_height( BTreeNode * root )
{
	int i = 1;
	while( !root->leaf ) {
		root = root->children[0];
		++i;
	}

	return i;
}
