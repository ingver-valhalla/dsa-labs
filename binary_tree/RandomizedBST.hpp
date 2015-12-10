// RandomizedBST.hpp

#include <cstdlib>
#include <ctime>
#include <iostream>

template < typename Item >
RandomizedBST<Item>::RandomizedBST( )
	: root( NULL )
{
	std::srand( std::time( 0 ) );
}

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
	return root == NULL;
}

template < typename Item >
void RandomizedBST<Item>::insert( const Item & to_insert )
{
	if( !contains( to_insert ) )
		insert( &root, to_insert );
}

template < typename Item >
void RandomizedBST<Item>::insert( Node ** pnode, const Item & to_insert )
{
	// TODO

	Node * node = *pnode;
	
	if( node == NULL ) {
		std::cout << "inserting to leaf" << std::endl;
		insert_to_root( pnode, to_insert );
	}
	else if( std::rand() % 2 ) {
		std::cout << "inserting to root" << std::endl;
		insert_to_root( pnode, to_insert );
	}
	else if( to_insert < node->data ) {
		std::cout << "skipping " << node->data << std::endl;
		insert( &node->left, to_insert );
	}
	else {
		std::cout << "skipping " << node->data << std::endl;
		insert( &node->right, to_insert );
	}
}


template < typename Item >
void RandomizedBST<Item>::insert_to_root( Node ** pnode, const Item & to_insert )
{
	Node * node = *pnode;

	if( node == NULL ) {
		*pnode = new Node( to_insert, NULL, NULL );

		print( std::cout );
		std::cout << std::endl;
	}
	else if( to_insert < node->data ) {
		insert_to_root( &node->left, to_insert );
		rotate_right( pnode );

		print( std::cout );
		std::cout << std::endl;
	}
	else if( to_insert > node->data ) {
		insert_to_root( &node->right, to_insert );
		rotate_left( pnode );

		print( std::cout );
		std::cout << std::endl;
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
	else if( !node->left && !node->right ) {

		std::cout << "removing from leaf" << std::endl;
		delete node;
		*pnode = NULL;

		print( std::cout );
		std::cout << std::endl;
	}
	else {

		Node * min_node = node->right;

		if( min_node != NULL ) {

			std::cout << "removing min from right and merging" << std::endl;
			while( min_node->left != NULL )
				min_node = min_node->left;

			Item min = min_node->data;
			remove( pnode, min );
			*pnode = new Node( min, node->left, node->right );

			print( std::cout );
			std::cout << std::endl;
		}
		else {

			std::cout << "replacing with left child" << std::endl;
			*pnode = node->left;

			print( std::cout );
			std::cout << std::endl;
		}
		delete node;
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
	*pnode = NULL;
}

template < typename Item >
void RandomizedBST<Item>::print( std::ostream & os ) const
{
	os << "# ";
	print( root, os );
}

template < typename Item >
void RandomizedBST<Item>::print( Node * node, std::ostream & os ) const
{
	if( node == NULL ) {
		os << "* ";
		return;
	}

	os << node->data;
	os << " <";
	print( node->left, os );
	os << " >";
	print( node->right, os );
}


template < typename Item >
void RandomizedBST<Item>::rotate_left( Node ** pnode )
{
	Node * node = *pnode;

	Node * t = node->right;
	if( t != NULL ) {
		node->right = t->left;
		t->left = node;
		*pnode = t;
	}
}


template < typename Item >
void RandomizedBST<Item>::rotate_right( Node ** pnode )
{
	Node * node = *pnode;

	Node * t = node->left;

	if( t != NULL ) {
		node->left = t->right;
		t->right = node;
		*pnode = t;
	}
}
