#include <iostream>
#include <cstdlib>
#include "rand_bst.hpp"

using namespace std;
using namespace BST;

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

void print_int_tree( RandBST<int> & tr, ostream & os )
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

		// printing a root
		os << "# " << *it; 
		os << "(" << it.get_size() << ")";
		os << endl;

		item = *it;
		++it;

		// traverse
		for( ; it != tr.end( ); item = *it, ++it)
		{
			if( *it < item ) {

				print_indent( it.get_level( ), ind_width, os );
				os << "<" << *it;
				cout << "(" << it.get_size() << ")";
				cout << endl;
			}
			else {

				print_indent( it.get_level( ), ind_width, os );
				os << "<*" << endl;
				print_indent( it.get_level( ), ind_width, os );
				os << endl;
				print_indent( it.get_level( ), ind_width, os );
				os << ">" << *it;
				cout << "(" << it.get_size() << ")";
				cout << endl;
			}
		}
	}
}


int int_tree_height( const RandBST<int> & tr )
{
	auto it = tr.traverse_pre_order();
	int height = 0;

	while( it != tr.end() ) {
		if( height < it.get_level() ) {
			height = it.get_level();
		}
		++it;
	}
	return height;
}


int main()
{
	srand( time( NULL ) );
	RandBST<int> tr;

	for( int i = 0; i < 10; ++i ) {
		int to_insert = rand() % 100;
		while( tr.contains( to_insert ) )
			to_insert = rand() % 100;

		//cout << "inserting " << to_insert << endl;
		tr.insert( to_insert );
		//print_int_tree( tr, cout );
	}
	
	print_int_tree( tr, cout );
	cout << "height = " << int_tree_height( tr ) << endl;
	cout << "\n\n";

	cout << "copying...\n";
	auto copy = RandBST<int>(tr);
	cout << "the copy:\n";
	print_int_tree( tr, cout );
	cout << "\n";

	// ranged-for loop test
	for(auto item : tr) {
		cout << item << " ";
	}
	cout << endl;

	//cout << "DELETE" << endl;
	//for( int i = 0; i < 40; ++i ) {
		//int to_delete = rand() % 100;
		//while( !tr.contains( to_delete ) ) {
			//to_delete = rand() % 100;
		//}

		//cout << "removing " << to_delete << endl;
		//tr.remove( to_delete );
		////print_int_tree( tr, cout  );
		//cout << endl;
		//cout << "height = " << int_tree_height( tr ) << endl;
	//}

	//while( 1 ) {
		//int to_insert;
		//cout << "INSERT: ";
		//while( cin >> to_insert ) {

			//cout << endl;
			//tr.insert( to_insert );
			//print_int_tree( tr, cout );
			//cout << endl;
			//cout << "INSERT: ";
		//}
		//cin.clear();
		//cout << endl;
		
		//int to_delete;
		//cout << "DEL: ";
		//while( cin >> to_delete ) {

			//cout << endl;
			//tr.remove( to_delete );
			//print_int_tree( tr, cout );
			//cout << endl;
			//cout << "DEL: ";
		//}
		//cin.clear();
		//cout << endl;
	//}

	//system("sleep 1");
	return 0;
}
