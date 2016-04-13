// rand_bst.hpp

#ifndef RAND_BST_HPP
#define RAND_BST_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stack>

namespace BST {

template < typename Item >
class RandBST {

	class Node;

	public:

		class PreOrderIterator;

		RandBST( );
		RandBST( const RandBST & );
		~RandBST( );

		bool contains( const Item & ) const;
		bool is_empty( ) const;

		void insert( const Item & );
		void remove( const Item & );

		const Item & get_min( ) const;
		const Item & get_max( ) const;
		void clear( );

		PreOrderIterator traverse_pre_order( ) const;


		class PreOrderIterator {

			friend RandBST;

			public: 
				PreOrderIterator( ) : ended( false ), level( 0 ) { }

				const Item & operator * ();
				const PreOrderIterator & operator ++ ( );
				const PreOrderIterator & operator ++ ( int );
				bool end( ) { return ended; }
				int get_level( ) { return level; }
				int get_size() { return node->size; }

			private: 
				Node * node;
				std::stack< std::pair<Node *, int> > st;
				bool ended;
				int level;
		};


		void print( std::ostream & ) const;

	private:
		struct Node {

			Item data;
			Node * left;
			Node * right;
			int size;

			Node( const Item & d, Node * l, Node * r  )
				: data( d ), left( l ), right( r ), size( 1 ) { }
			Node( const Node & other ) 
				: data( other.data ),
				  size( other.size )
			{ 
				left = other.left ? new Node( *other.left ) : nullptr;
				right = other.right ? new Node( *other.right ) : nullptr;
			}
		};

		Node * root;

		void insert( Node ** , const Item & );
		void insert_to_root( Node **, const Item & );

		void remove( Node ** , const Item & );

		void clear( Node ** );
		void print( Node *, std::ostream & ) const;

		void rotate_left( Node ** );
		void rotate_right( Node ** );

		Node * join( Node *, Node * );

		int get_size( Node * );
		void fix_size( Node * );
};

template < typename Item >
RandBST<Item>::RandBST( )
    : root( NULL )
{
	std::srand( std::time( 0 ) );
}

template < typename Item >
RandBST<Item>::RandBST( const RandBST & other )
{
	root = new Node( *other.root );
}

template < typename Item >
RandBST<Item>::~RandBST( )
{
	clear();
}


template < typename Item >
bool RandBST<Item>::contains( const Item & to_find ) const
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
bool RandBST<Item>::is_empty( ) const
{
	return root == NULL;
}

template < typename Item >
void RandBST<Item>::insert( const Item & to_insert )
{
	if( !contains( to_insert ) )
		insert( &root, to_insert );
}

template < typename Item >
void RandBST<Item>::insert( Node ** pnode, const Item & to_insert )
{
	Node * node = *pnode;

	if( node == NULL ) {
		//std::cout << "inserting to leaf" << std::endl;
		insert_to_root( pnode, to_insert );
	}
	else if( std::rand() < RAND_MAX / (node->size+1) ) {
		//std::cout << "inserting to root" << std::endl;
		insert_to_root( pnode, to_insert );
	}
	else if( to_insert < node->data ) {
		//std::cout << "skipping " << node->data << std::endl;
		insert( &node->left, to_insert );
		fix_size( node );
	}
	else {
		//std::cout << "skipping " << node->data << std::endl;
		insert( &node->right, to_insert );
		fix_size( node );
	}
}


template < typename Item >
void RandBST<Item>::insert_to_root( Node ** pnode, const Item & to_insert )
{
	Node * node = *pnode;

	if( node == NULL ) {
		*pnode = new Node( to_insert, NULL, NULL );

		//print( std::cout );
		//std::cout << std::endl;
	}
	else if( to_insert < node->data ) {
		insert_to_root( &node->left, to_insert );
		rotate_right( pnode );

		//print( std::cout );
		//std::cout << std::endl;
	}
	else if( to_insert > node->data ) {
		insert_to_root( &node->right, to_insert );
		rotate_left( pnode );

		//print( std::cout );
		//std::cout << std::endl;
	}
}


template < typename Item >
void RandBST<Item>::remove( const Item & to_remove )
{
	remove( &root, to_remove );
}


template < typename Item >
void RandBST<Item>::remove( Node ** pnode, const Item & to_remove )
{
	if( pnode == NULL )
		return;

	Node * node = *pnode;
	if( node == NULL ) {
		return;
	}
	if( to_remove < node->data ) {
		remove( &(node->left), to_remove );
		fix_size( node );
	}
	else if( to_remove > node->data ) {
		remove( &(node->right), to_remove );
		fix_size( node );
	}
	else {
		Node * t = join( node->left, node->right );
		*pnode = t;
		delete node;
	}
}

template < typename Item >
const Item & RandBST<Item>::get_min( ) const
{
	Node *p = root;
	Node *min = NULL;

	for( ; p; min = p, p = p->left );

	return min->data;
}

template < typename Item >
const Item & RandBST<Item>::get_max( ) const
{
	Node *p = root;
	Node *max = NULL;

	for( ; p; max = p, p = p->right );

	return max->data;
}

template < typename Item >
void RandBST<Item>::clear( )
{
	clear( &root );
}

template < typename Item >
void RandBST<Item>::clear( Node ** pnode )
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
void RandBST<Item>::print( std::ostream & os ) const
{
	os << "# ";
	print( root, os );
}

template < typename Item >
void RandBST<Item>::print( Node * node, std::ostream & os ) const
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
void RandBST<Item>::rotate_left( Node ** pnode )
{
	Node * node = *pnode;

	Node * t = node->right;

	if( t != NULL ) {
		node->right = t->left;
		t->left = node;
		//t->size = node->size;

		fix_size( node );
		fix_size( t );

		*pnode = t;
	}
}


template < typename Item >
void RandBST<Item>::rotate_right( Node ** pnode )
{
	Node * node = *pnode;

	Node * t = node->left;

	if( t != NULL ) {
		node->left = t->right;
		t->right = node;
		//t->size = node->size;

		fix_size( node );
		fix_size( t );

		*pnode = t;
	}
}


template < typename Item >
typename RandBST<Item>::Node *
RandBST<Item>::join( Node * a, Node * b )
{
	if( a == NULL ) return b;
	if( b == NULL ) return a;

	if( rand() / (RAND_MAX / (a->size + b->size) + 1) < a->size ) {

		a->right = join( a->right, b );
		fix_size( a );
		return a;
	}
	else {

		b->left = join( a, b->left );
		fix_size( b );
		return b;
	}
}


template < typename Item >
int RandBST<Item>::get_size( Node * node )
{
	return node ? node->size : 0;
}

template < typename Item >
void RandBST<Item>::fix_size( Node * node )
{
	node->size = get_size( node->left ) + get_size( node->right ) + 1;
}


template < typename Item >
typename RandBST<Item>::PreOrderIterator
RandBST<Item>::traverse_pre_order( ) const
{
	PreOrderIterator it;
	it.node = root;
	if( root == NULL ) {
		it.ended = true;
	}
	return it;
}

template < typename Item >
const Item &
RandBST<Item>::PreOrderIterator::operator * ( )
{
	return node->data;
}

template < typename Item >
const typename RandBST<Item>::PreOrderIterator &
RandBST<Item>::PreOrderIterator::operator ++ ( )
{
	if( !ended ) {

		if( node->left != NULL ) {
			st.push( std::make_pair( node->right, level+1 ) );
			node = node->left;
			++level;
		}
		else if( node->right != NULL ) {
			node = node->right;
			++level;
		}
		else {

			while( !st.empty( ) && st.top( ).first == NULL ) {
				level = st.top( ).second;
				st.pop( );
			}

			if( !st.empty( ) ) {
				node = st.top( ).first;
				level = st.top( ).second;
				st.pop( );
			}
			else {
				ended = true;
			}
		}
	}
	return *this;
}

template < typename Item >
const typename RandBST<Item>::PreOrderIterator &
RandBST<Item>::PreOrderIterator::operator ++ ( int )
{
	return operator ++ ( );
}

} // namespace BST

#endif // RAND_BST_HPP
