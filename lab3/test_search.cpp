// test_search.cpp

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

#include "key.h"
#include "search.h"
#include "sequence.h"

using namespace std;

void show_arr( Key* arr, int size );

int successful_search( Search_p search, int jump, Make_seq_p make_seq, int size_of_seq, int num_of_tests )
{
	Key::reset_comps();
	Key::reset_assigns();

	Key *arr = NULL;
	arr = new Key[size_of_seq];
	if( !make_seq( arr, size_of_seq, 0, size_of_seq ) ) {
		cerr << "successful_search: make_ord couldn't make a sequence"
		     << endl;
		delete[] arr;
		return -1;
	}

	cout << "After creation: comps = " << Key::comparisons()
	     << " assigns = " << Key::assignments() << endl;
	//show_arr( arr, size_of_seq );
	Key::reset_comps();
	Key::reset_assigns();

	if( make_seq == make_rand ) {
		sort( arr, arr + size_of_seq );
		cout << "After sort: comps = " << Key::comparisons()
	     	     << " assigns = " << Key::assignments() << endl;
	}

	Key::reset_comps();
	Key::reset_assigns();

	//show_arr( arr, size_of_seq );

	for( int i = 0; i < num_of_tests; ++i ) {
		
		int index = rand() % size_of_seq;
		int found_index = -1;
		//cout << "test #" << i << " | index: " << index 
		     //<< " arr[index] = " << arr[index].val() << endl;

		if( (found_index = search( arr, size_of_seq, arr[index], jump )) == -1 ) {
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

int failed_search( Search_p search, int jump, int size_of_seq, int num_of_tests)
{
	Key::reset_comps();
	Key::reset_assigns();

	Key *arr = NULL;
	arr = new Key[size_of_seq];
	if( !make_odd( arr, size_of_seq, 0, size_of_seq ) ) {
		cerr << "successful_search: make_ord couldn't make a sequence"
		     << endl;
		delete[] arr;
		return -1;
	}

	cout << "After creation: comps = " << Key::comparisons()
	     << " assigns = " << Key::assignments() << endl;
	//show_arr( arr, size_of_seq );
	Key::reset_comps();
	Key::reset_assigns();

	for( int i = 0; i < num_of_tests; ++i ) {
		
		Key to_find = rand() * 2;
		int found_index = -1;
		//cout << "test #" << i << " arr[index] = " << to_find.val() << endl;

		if( (found_index = search( arr, size_of_seq, to_find, jump )) != -1 ) {
			cerr << endl
			     << "failed_search: element somehow found. Check the algorithm!"
			     << " index : " << found_index
			     << endl;
			delete[] arr;
			return -1;
		}
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

#define MIN_SIZE (int)1e4
#define MAX_SIZE (MIN_SIZE * 10)
#define SIZE_STEP MIN_SIZE
#define TESTS 100 

	Search_p search = search_jump2;

	cout << "testing jump search" << endl;
	cout << "FAILED SEARCH >>>>" << endl;
	cout << "===============================================================================" << endl;
	//cout << "ordered sequence" << endl;
	for( int i = 0, size = MIN_SIZE;
	     size <= MAX_SIZE;
	     ++i, size += SIZE_STEP )
	{
		int jump = sqrt(size);
		//cout << "jump = " << jump << endl;
		cout << i+1 << ". size = " << size << endl;
		if( failed_search( search, jump, size, TESTS ) == -1 ) {
			cout << "Quiting..." << endl;
#ifdef _MSC_VER
			getchar();
#endif
			exit( 1 );
		}
		cout << endl;
	}

	/*
	cout << "testing 2 level jump search" << endl;
	cout << "SUCCESSFUL SEARCH >>>>" << endl;
	cout << "===============================================================================" << endl;
	cout << "ordered sequence" << endl;
	for( int i = 0, size = MIN_SIZE;
	     size <= MAX_SIZE;
	     ++i, size += SIZE_STEP )
	{
		int jump = sqrt(size);
		//cout << "jump = " << jump << endl;
		cout << i+1 << ". size = " << size << endl;
		if( successful_search( search, jump, make_ord, size, TESTS ) == -1 ) {
			cout << "Quiting..." << endl;
#ifdef _MSC_VER
			getchar();
#endif
			exit( 1 );
		}
		cout << endl;
	}
	cout << "===============================================================================" << endl;
	cout << "random sequence" << endl;
	for( int i = 0, size = MIN_SIZE;
		 size <= MAX_SIZE;
		 ++i, size += SIZE_STEP )
	{
		int jump = sqrt(size);
		cout << "jump = " << jump << endl;
		cout << i+1 << ". size = " << size << endl;
		if( failed_search( search, jump, size, TESTS ) == -1 ) {
			cout << "Quiting..." << endl;
#ifdef _MSC_VER
			getchar();
#endif
			exit( 1 );
		}
		cout << endl;
	}

	cout << "===============================================================================" << endl;
	cout << "sparse sequence" << endl;
	for( int i = 0, size = MIN_SIZE;
		 size <= MAX_SIZE;
		 ++i, size += SIZE_STEP )
	{
		int jump = sqrt(size);
		cout << "jump = " << jump << endl;
		cout << i+1 << ". size = " << size << endl;
		if( failed_search( search, jump, size, TESTS ) == -1 ) {
			cout << "Quiting..." << endl;
#ifdef _MSC_VER
			getchar();
#endif
			exit( 1 );
		}
		cout << endl;
	}
	

	Key arr[100];
	make_ord( arr, 100, 0, 100 );
	show_arr( arr, 100 );

#ifdef _MSC_VER
	getchar();
#endif

*/

	return 0;
}
