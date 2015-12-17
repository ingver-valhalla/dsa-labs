#include <iostream>
#include <cstdlib>
#include "RandomizedBST.h"

using namespace std;

void print_indent( int ind, int width, ostream& os )
{
	os << "  ";
	for( int i = 0; i < ind; ++i ) {
		for( int j = 0; j < width + 1; ++j ) {
			os << " ";
		}
		os << "|";
	}
}

void print_int_tree( RandomizedBST<int> & tr, ostream & os )
{
	if( !tr.is_empty( ) ) {
		auto it = tr.traverse_pre_order( );

		int ind_width = 0;
		// calculating width of indent according to a number of
		// digits in max of tree
		for( int max = tr.get_max( ); max; max /= 10 )
			++ind_width;

		// item stores previous item
		int item;
		// level stores previous level in tree
		int level = it.get_level( );

		// printing a root
		os << "# " << *it << endl;

		item = *it;
		++it;

		// traverse
		for( ; !it.end( );
		       level = it.get_level( ), item = *it, ++it)
		{
			if( *it < item ) {

				print_indent( it.get_level( ), ind_width, os );
				os << "<" << *it << endl;
			}
			else if( level >= it.get_level( ) ) {

				print_indent( it.get_level( ), ind_width, os );
				os << endl;
				print_indent( it.get_level( ), ind_width, os );
				os << ">" << *it << endl;
			}
			else {

				print_indent( it.get_level( ), ind_width, os );
				os << "<*" << endl;
				print_indent( it.get_level( ), ind_width, os );
				os << ">" << *it << endl;
			}
		}
	}
}


int main()
{
	srand( time( NULL ) );
	RandomizedBST<int> tr;

	for( int i = 0; i < 40; ++i )
		tr.insert( rand( ) % 100 );

	cout << endl;

	tr.print( cout );
	cout << endl;

	print_int_tree( tr, cout );
	cout << endl;

	//for( auto it = tr.traverse_pre_order( );
		 //!it.end( );
		 //++it )
	//{
		//cout << *it << " <= level: " << it.get_level( ) << endl;
	//}
	//cout << endl;

	system("sleep 1");
	return 0;
}
