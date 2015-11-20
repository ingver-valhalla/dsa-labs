// test_search.cpp

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "key.h"
#include "search.h"
#include "sequence.h"

using namespace std;

void show_arr( Key* arr, int size );

int successful_search( Search_p search, int size_of_seq, int num_of_tests )
{
	Key::reset_comps();
	Key::reset_assigns();

	Key *arr = NULL;
	arr = new Key[size_of_seq];
	if( !make_rand( arr, size_of_seq, 0, size_of_seq ) ) {
		cerr << "successful_search: make_ord couldn't make a sequence"
		     << endl;
		delete[] arr;
		return -1;
	}

	sort( arr, arr + size_of_seq );

	cout << "After sort: comps = " << Key::comparisons()
	     << " assigns = " << Key::assignments() << endl;

	Key::reset_comps();
	Key::reset_assigns();

	//show_arr( arr, size_of_seq );

	for( int i = 0; i < num_of_tests; ++i ) {
		
		int index = rand() % size_of_seq;
		int found_index = -1;
		//cout << "test #" << i << " | index: " << index 
		     //<< " arr[index] = " << arr[index].val() << endl;

		if( (found_index = search( arr, size_of_seq, arr[index] )) == -1 ) {
			cerr << endl
			     << "successful_search: didn't find element. Check the algorithm!"
			     << endl;
			delete[] arr;
			return -1;
		}

		//cout << "FOUND! arr[" << found_index << "] = " << arr[found_index].val() << endl;
	}       

	cout << "Average number of comparisons: "
	     << Key::comparisons() / (double)num_of_tests
	     << endl;

	delete[] arr;
	Key::reset_comps();
	Key::reset_assigns();

	return 1;
}

void show_arr( Key* arr, int size )
{
	for( int i = 0; i < size; ++i )
		cout << arr[i].val() << ' ';
	cout << endl;
}



int main()
{
	srand( time(NULL) );

#define MIN_SIZE 1e5
#define MAX_SIZE 1e6
#define SIZE_STEP 1e5
#define TESTS 100

	for( int i = 0, size = MIN_SIZE;
	     size <= MAX_SIZE;
	     ++i, size += SIZE_STEP )
	{
		cout << i+1 << ". size = " << size << endl;
		if( successful_search( search_fib, size, TESTS ) == -1 ) {
			cout << "Quiting..." << endl;
			exit( 1 );
		}
		cout << endl;
	}

	//Key arr[100];
	//make_ord( arr, 100, 0, 100 );
	//show_arr( arr, 100 );

#ifdef __MSC_VER
	getchar();
#endif

	return 0;
}
