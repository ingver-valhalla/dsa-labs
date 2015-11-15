// test_sorts.cpp

#include "sorts.hpp"
#include <cstdlib>
#include <ctime>
#include <climits>
#include "../lab1/sequence.hpp"

using namespace std;

#define NUM_SEQ 4
int_seq seq_arr[NUM_SEQ] = { ord, rord, stepped, rand_seq };
const char * seq_str[NUM_SEQ] = { "ordered", "right-ordered",
                                  "stepped", "random"         };

//==============================================================================
// functors
//==============================================================================
class LessThan {
	int n;
public:
	LessThan() : n(0) {};
	void reset() { n = 0; }
	int amount() { return n; }

	template <typename T>
	bool operator() ( const T * a, const T * b )
	{
		++n;
		if( *a < *b )
			return true;
		return false;
	}
};

class MoreThan {
	int n;
public:
	MoreThan() : n(0) {};
	void reset() { n = 0; }
	int amount() { return n; }

	template <typename T>
	bool operator() ( const T * a, const T * b )
	{
		++n;
		if( *a > *b )
			return true;
		return false;
	}
};
//==============================================================================


//==============================================================================
// comparison function for testing standard qsort
//==============================================================================
static int comps;
int less_than( const void * a, const void * b )
{
	if( *(Key*)a < *(Key*)b ) {
		++comps;
		return -1;
	}
	else if( *(Key*)a == *(Key*)b ) {
		comps += 2;
		return 0;
	}
	comps += 2;
	return 1;
}
//==============================================================================


template <typename Key>
int is_sorted( Key * arr, int size )
{
	for( int i = 1; i < size; ++i )
	if( arr[i-1] > arr[i] )
		return 0;

	return 1;
}


template <typename Compare>
int sort_time_test( int min_arr_size, int max_arr_size, int size_step,
                    int (*sort)( Key *, int, Compare&, short ),
                    Compare & cmp, short word = 0 )
{
	typedef int (*sort_ptr)(Key*, int, Compare&, short);
	if( !sort || !size_step || min_arr_size > max_arr_size )
		return 0;
	clock_t clock1;
	clock_t clock2;
	Key * arr = NULL;
	int min_key = 0;
	int max_key = INT_MAX;
	int key_step = 1000;
	
	cmp.reset();

	for( int i = 0; i < NUM_SEQ; i++ ) {
		int_seq seq = seq_arr[i];
		/* 10 times for ordered sequence, taking the average */
		int repeats = (i == 0 && sort == (sort_ptr)sort_insert_sent) ? 500 : 1; 

		for( int k = 0; k < 79; ++k ) cout << '=';
		cout << endl;
		cout << "Sequence: " << seq_str[i] << endl;
		for( int k = 0; k < 30; ++k ) cout << '-';
		cout << endl;

		for( int size = min_arr_size; 
		     size <= max_arr_size; 
		     size += size_step )
		{
			int ret;
			arr = new Key[size];
			ret = seq( arr, size, min_key, max_key, key_step );
			if( !ret ) {
				cerr << "ERROR: sequence isn't created"
				     << endl;
				delete[] arr;
				arr = NULL;
				return 0;
			}
			clock1 = clock();
			for( int r = 0; r < repeats; ++r ) {
				ret = sort( arr, size, cmp, word );
			}
			clock2 = clock();
			if( !ret ) {
				cerr << "ERROR: failed sort" << endl;
				delete[] arr;
				arr = NULL;
				return 0;
			}
			if( !is_sorted( arr, size ) ) {
				cerr << "ERROR: result's not sorted"
				     << endl;
				delete[] arr;
				arr = NULL;
				return 0;
			}


			cout << "Sort time for " << size << " elements: "
			     << 1000.0 * (clock2 - clock1) / (CLOCKS_PER_SEC
			        * repeats)
			     << "ms; ";
			cout << "Comparisons: " << cmp.amount() / repeats << "; Repeats: " << repeats << endl;
			delete[] arr;
			arr = NULL;
			cmp.reset();
		}
		for( int k = 0; k < 79; ++k ) cout << '=';
		cout << endl;
	}
	return 1;
}

int qsort_time_test( int min_arr_size, int max_arr_size, int size_step,
                        int (*cmp)( const void *, const void * ))
{
	if( !size_step || min_arr_size > max_arr_size )
		return 0;
	clock_t clock1;
	clock_t clock2;
	Key * arr = NULL;
	int min_key = 0;
	int max_key = INT_MAX;
	int key_step = 1000;
	
	for( int i = 0; i < NUM_SEQ; i++ ) {
		int_seq seq = seq_arr[i];
		for( int k = 0; k < 79; ++k ) cout << '=';
		cout << endl;
		cout << "Sequence: " << seq_str[i] << endl;
		for( int k = 0; k < 30; ++k ) cout << '-';
		cout << endl;

		for( int size = min_arr_size; 
		     size <= max_arr_size; 
		     size += size_step )
		{
			int ret;
			arr = new Key[size];
			ret = seq( arr, size, min_key, max_key, key_step );
			if( !ret ) {
				cerr << "ERROR: sequence isn't created"
				     << endl;
				delete[] arr;
				arr = NULL;
				return 0;
			}
			clock1 = clock();
			qsort( arr, size, sizeof( *arr ), cmp );
			clock2 = clock();
			if( !is_sorted( arr, size ) ) {
				cerr << "ERROR: result's not sorted"
				     << endl;
				delete[] arr;
				arr = NULL;
				return 0;
			}


			cout << "Sort time for " << size << " elements: "
			     << 1000.0 * (clock2 - clock1) / CLOCKS_PER_SEC
			     << "ms; ";
			cout << "Comparisons: " << comps << endl;
			delete[] arr;
			arr = NULL;
			comps = 0;
		}
		for( int k = 0; k < 79; ++k ) cout << '=';
		cout << endl;
	}
	return 1;
}

#define SIZE (int)1000

int main( int argc, char ** argv )
{
	srand(time(0));
	LessThan less; // functor
	
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl;
	cout << "TESTING INSERTION SORT WITH SENTINEL" << endl;
	if( !sort_time_test( 5e3, 50e3, 5e3, sort_insert_sent, less ) )
	{
		cerr << "ERROR: Test failed" << endl;
		return 1;
	}
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl << endl;
	
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl;
	cout << "TESTING INSERTION SORT WITH BINARY SEARCH" << endl;
	if( !sort_time_test( 5e3, 50e3, 5e3, sort_insert_bin, less ) )
	{
		cerr << "ERROR: Test failed" << endl;
		return 1;
	}
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl;

	for( int i = 1; i <= 8; i *= 2 ) {
		cout << endl;
		for( int k = 0; k < 79; ++k ) cout << '#';
		cout << endl;
		cout << "TESTING RADIX MSD SORT; WORD: " << i << endl;
		if( !sort_time_test( 5e4, 50e4, 5e4, sort_radix_msd, less, i ) )
		{
			cerr << "ERROR: Test failed" << endl;
			return 1;
		}
		for( int k = 0; k < 79; ++k ) cout << '#';
		cout << endl;
	}

	cout << endl;
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl;
	cout << "TESTING C QSORT" << endl;
	if( !qsort_time_test( 5e4, 50e4, 5e4, less_than ) ) {
		cerr << "ERROR: Test failed" << endl;
		return 1;
	}
	for( int k = 0; k < 79; ++k ) cout << '#';
	cout << endl;

#ifdef _MSC_VER
	getchar();
#endif
	return 0;
}
