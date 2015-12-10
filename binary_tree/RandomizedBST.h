// RandomizedBST.h

#ifndef RANDOMIZED_BST_H
#define RANDOMIZED_BST_H

#include <ostream>
#include "AbstractBST.h"


template < typename Item >
class RandomizedBST : AbstractBST<Item> {

	public:
		RandomizedBST( );
		~RandomizedBST( );

		virtual bool contains( const Item & ) const;
		virtual bool is_empty( ) const;

		virtual void insert( const Item & );
		virtual void remove( const Item & );

		virtual const Item & get_min( ) const;
		virtual const Item & get_max( ) const;
		virtual void clear( );

		void print( std::ostream & ) const;

		//temp
		void lrotate_by_val( const Item & val )
		{
			Node ** p = &root;

			while( *p != NULL ) {

				if( (*p)->data > val ) {
					p = &(*p)->left;
				}
				else if( (*p)->data < val ) {
					p = &(*p)->right;
				}
				else {
					break;
				}
			}

			if( *p != NULL ) {
				rotate_left( p );
			}
		}

		void rrotate_by_val( const Item & val )
		{
			Node ** p = &root;

			while( *p != NULL ) {

				if( (*p)->data > val ) {
					p = &(*p)->left;
				}
				else if( (*p)->data < val ) {
					p = &(*p)->right;
				}
				else {
					break;
				}
			}

			if( *p != NULL ) {
				rotate_right( p );
			}
		}

	private:
		struct Node {

			Item data;
			Node * left;
			Node * right;

			Node( const Item & d, Node * l, Node * r  )
				: data( d ), left( l ), right( r ) { }
		};

		Node * root;

		void insert( Node ** , const Item & );
		void insert_to_root( Node **, const Item & );

		void remove( Node ** , const Item & );

		// temp
		void clear( Node ** );
		void print( Node *, std::ostream & ) const;

		void rotate_left( Node ** );
		void rotate_right( Node ** );
};


#include "RandomizedBST.hpp"

#endif // RANDOMIZED_BST_H
