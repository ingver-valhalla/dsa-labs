// RandomizedBST.hpp

#include <cstdlib>
#include <iostream>

template < typename Item >
RandomizedBST<Item>::RandomizedBST( )
	: root( NULL ) { }

template < typename Item >
RandomizedBST<Item>::~RandomizedBST( )
{
	clear();
}


template < typename Item >
bool RandomizedBST<Item>::contains( const Item & to_find ) const
{
	Node *p = root;

	while( p ) {

		if( to_find < p->data ) {
			p = p->left;
		}
		else if(  to_find > p->data ) {
			p = p->right;
		}
		else {
			return true;
		}
	}

	return false;
}

template < typename Item >
bool RandomizedBST<Item>::is_empty( ) const
{
	return !root;
}

template < typename Item >
void RandomizedBST<Item>::insert( const Item & to_insert )
{
	insert( &root, to_insert );
	//std::cout << "root = " << root << std::endl;
}

template < typename Item >
void RandomizedBST<Item>::insert( Node ** pnode, const Item & to_insert )
{
	// TODO
	// TESTING
	Node * node = *pnode;
	//std::cout << "&root = " << &root << " pnode = " << pnode << std::endl;
	if( node == NULL ) {
		//std::cout << "creating new node" << std::endl;
		*pnode = new Node( to_insert, NULL, NULL );
	}
	else if( to_insert < node->data ) {
		insert( &node->left, to_insert );
		//std::cout << "node->left = " << node->left << std::endl;
	}
	else if( to_insert > node->data ) {
		insert( &node->right, to_insert );
		//std::cout << "node->right = " << node->right << std::endl;
	}
}

template < typename Item >
void RandomizedBST<Item>::remove( const Item & to_remove )
{
	remove( &root, to_remove );
}

template < typename Item >
void RandomizedBST<Item>::remove( Node ** pnode, const Item & to_remove )
{
	// TODO
	// TESTING
	Node * node = *pnode;
	if( node == NULL ) {
		return;
	}
	else if( to_remove < node->data ) {
		remove( &node->left, to_remove );
	}
	else if( to_remove > node->data ) {
		remove( &node->right, to_remove );
	}
	// deleting only leafs
	else if( !node->left && !node->right ) {
		delete node;
		node = NULL;
	}
}

template < typename Item >
const Item & RandomizedBST<Item>::get_min( ) const
{
	Node *p = root;
	Node *min = NULL;

	for( ; p; min = p, p = p->left );

	return min->data;
}

template < typename Item >
const Item & RandomizedBST<Item>::get_max( ) const
{
	Node *p = root;
	Node *max = NULL;

	for( ; p; max = p, p = p->right );

	return max->data;
}

template < typename Item >
void RandomizedBST<Item>::clear( )
{
	clear( &root );
}

template < typename Item >
void RandomizedBST<Item>::clear( Node ** pnode )
{
	Node * node = *pnode;

	if( node == NULL ) {
		return;
	}
	if( node->left )
		clear( &node->left );

	if( node->right )
		clear( &node->right );

	delete node;
	node = NULL;
}

template < typename Item >
void RandomizedBST<Item>::print( std::ostream & os ) const
{
	print( root, os );
	os << std::endl;
}

template < typename Item >
void RandomizedBST<Item>::print( Node * node, std::ostream & os ) const
{
	if( node == NULL ) {
		os << "* ";
		return;
	}

	os << node->data;
	os << " l";
	print( node->left, os );
	os << " r";
	print( node->right, os );
}
