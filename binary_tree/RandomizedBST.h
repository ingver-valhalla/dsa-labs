// RandomizedBST.h

#ifndef RANDOMIZED_BST_H
#define RANDOMIZED_BST_H

#include <ostream>
#include <stack>
#include <utility>
#include "AbstractBST.h"

template < typename Item >
class RandomizedBST : AbstractBST<Item> {

	class Node;

	public:

		class PreOrderIterator;

		RandomizedBST( );
		~RandomizedBST( );

		virtual bool contains( const Item & ) const;
		virtual bool is_empty( ) const;

		virtual void insert( const Item & );
		virtual void remove( const Item & );

		virtual const Item & get_min( ) const;
		virtual const Item & get_max( ) const;
		virtual void clear( );

		virtual PreOrderIterator traverse_pre_order( ) const;


		class PreOrderIterator {
			
			friend RandomizedBST;

			public: 
				PreOrderIterator( ) : ended( false ), level( 0 ) { }

				const Item & operator * ();
				const PreOrderIterator & operator ++ ( );
				const PreOrderIterator & operator ++ ( int );
				bool end( ) { return ended; }
				int get_level( ) { return level; }

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

			Node( const Item & d, Node * l, Node * r  )
				: data( d ), left( l ), right( r ) { }
		};

		Node * root;

		void insert( Node ** , const Item & );
		void insert_to_root( Node **, const Item & );

		void remove( Node ** , const Item & );

		void clear( Node ** );
		void print( Node *, std::ostream & ) const;

		void rotate_left( Node ** );
		void rotate_right( Node ** );
};


#include "RandomizedBST.hpp"

#endif // RANDOMIZED_BST_H
