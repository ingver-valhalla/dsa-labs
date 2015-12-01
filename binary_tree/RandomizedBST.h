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
		void remove( Node ** , const Item & );
		// temp
		void clear( Node ** );
		void print( Node *, std::ostream & ) const;
};


#include "RandomizedBST.hpp"

#endif // RANDOMIZED_BST_H
