// sorts.cpp

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <cctype>
#include "../lab1/sequence.hpp"

using namespace std;

typedef unsigned int uint;
typedef int Key;

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
// function for testing standard qsort
//==============================================================================
static int comps;
int less_than( const void * a, const void * b )
{
	++comps;
	if( *(Key*)a < *(Key*)b )
		return 0;
	return 1;
}
//==============================================================================


//==============================================================================
// Insertion sort with sentinel 
//==============================================================================
template <typename Compare>
int sort_insert_sent( Key * arr, int size, Compare & cmp, short word = 0 )
{
	if( !arr || size <= 0 ) 
		return 0;
	int i, j;
	Key t;
	for( i = size - 1; i > 0; --i ) {
		if( !cmp( &arr[i-1], &arr[i] ) )
			swap( arr[i-1], arr[i] );
	}
	for( i = 2; i < size; ++i ) {
		t = arr[i];
		for( j = i; t < arr[j-1]; --j ) {
			arr[j] = arr[j-1];
		}
		arr[j] = t;
	}
	cmp.amount();
	return 1;
}
//==============================================================================


//==============================================================================
// Binary insertion sort
//==============================================================================
template <typename Compare>
int sort_insert_bin( Key * arr, int size, Compare & cmp, short word = 0 )
{
	if( !arr || size <= 0 )
		return 0;
	int i, j, low, high, pos;
	Key t;
	for( i = 1; i < size; ++i ) {
		t = arr[i];
		for( low = 0, high = i - 1, pos = (low + high) >> 1;
		     low <= high; pos = (low + high) >> 1 )
		{
			if( cmp( &t, &arr[pos]) )
				high = pos - 1;
			else
				low = pos + 1;
		}
		for( j = i; j > low; --j ) {
			arr[j] = arr[j-1];
		}
		arr[low] = t;
	}
	
	return 1;
}
//==============================================================================


//==============================================================================
// Radix Sort (Most Significant Digit)
//==============================================================================

// Macros for extracting a digit from an integer
#define digit(A,R,W) (((Key)A >> (R*W)) & ~(~0 << W))

Key * aux = NULL;
int recursion_depth = 0;
int memory_cost = 0;

template <typename Compare>
void MSRadixG( Key * arr, int low, int high, Compare & cmp, short r, short word )
{
	++recursion_depth;
	if( r < 0 ) 
		return;
	
	if( high - low <= 10 ) { 
		sort_insert_bin( arr+low, high-low+1, cmp );
		return;
	}
	
	int arr_size = pow( 2, word ) + 1;
	int * cnt = new int[arr_size]();
	if( cnt == NULL ) {
		cerr << "MSRadixG: Error while allocating memory" << endl;
		exit( EXIT_FAILURE );
	}
	memory_cost += arr_size + sizeof( int );

	int i;
	for( i = low; i <= high; ++i ) 
		cnt[digit(arr[i],r,word)+1]++;
	for( i = 1; i < arr_size; i++ )
		cnt[i] += cnt[i-1];
	for( i = low; i <= high; ++i )
		aux[cnt[digit(arr[i],r,word)]++] = arr[i];
	for( i = low; i <= high; ++i )
		arr[i] = aux[i-low];
	MSRadixG( arr, low, low + cnt[0] - 1, cmp, r - 1, word );
	for( i = 0; i < arr_size - 1; ++i )
		MSRadixG( arr, low + cnt[i], low + cnt[i+1] - 1,
		          cmp, r - 1, word );
	delete[] cnt;
	cnt = NULL;
}

template <typename Compare>
int sort_radix_msd( Key * arr, int size, Compare & cmp, short word )
{
	aux = new Key[size];
	int type_size = sizeof( *arr ) * 8;
	double int_part;
	double fract_part = modf( log2( word ), &int_part );
	if( fract_part != 0 || word > type_size )
		return 0;
	MSRadixG( arr, 0, size - 1, cmp, type_size/word - 1, word );
	delete[] aux;
	aux = NULL;
	cout << "///" << endl;
	cout << "Recursion depth = " << recursion_depth << endl;
	cout << "Memory cost for arrays: " 
	     << (memory_cost > 1024 ? memory_cost / 1024 : memory_cost )
	     << (memory_cost > 1024 ? "Mb" : "Kb") << endl;
	memory_cost = 0;
	recursion_depth = 0;
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
			ret = sort( arr, size, cmp, word );
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
			     << 1000.0 * (clock2 - clock1) / CLOCKS_PER_SEC
			     << "ms; ";
			cout << "Comparisons: " << cmp.amount() << endl;
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

	for( int i = 1; i < 16; i *= 2 ) {
		cout << endl;
		for( int k = 0; k < 79; ++k ) cout << '#';
		cout << endl;
		cout << "TESTING RADIX MSD SORT; WORD: " << i << endl;
		if( !sort_time_test( 5e3, 50e3, 5e3, sort_radix_msd, less, i ) )
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
