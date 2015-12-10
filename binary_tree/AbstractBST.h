// AbstractBST.h

#ifndef ABSTRACT_BST_H
#define ABSTRACT_BST_H

template < typename Item >
class AbstractBST {

	public:
		virtual bool contains( const Item & ) const = 0;
		virtual bool is_empty( ) const = 0;

		virtual void insert( const Item & ) = 0;
		virtual void remove( const Item & ) = 0;

		virtual const Item & get_min( ) const = 0;
		virtual const Item & get_max( ) const = 0;

		virtual ~AbstractBST( ) { }
}; 
#endif // ABSTRACT_BST_H

