// sorts.cpp

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstring>
#include <climits>
#include <unistd.h>
#include "../lab1/sequence.hpp"

using namespace std;

typedef unsigned int uint;
typedef int Key;

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
		return 1;
	return 0;
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
int sort_radix_msd( Key * arr, int size, Compare & cmp, int word )
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
	cout << "Recursion depth = " << recursion_depth << endl;
	cout << "Stack using for arrays: " 
	     << (memory_cost > 1024 ? memory_cost / 1024 : memory_cost )
	     << (memory_cost > 1024 ? "Mb" : "Kb") << endl;
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
double sort_time_test( int (*sort)( Key *, int, Compare&, int ),
                       Key * arr, int size, Compare & cmp, int word = 0 )
{
	clock_t clock1;
	clock_t clock2;

	clock1 = clock();
	assert( sort( arr, size, cmp, word ) );
	clock2 = clock();
	
	return 1000.0 * (clock2 - clock1) / CLOCKS_PER_SEC;

}

double qsort_time_test( Key * arr, int size, 
                        int (*cmp)( const void *, const void * ))
{
	clock_t clock1;
	clock_t clock2;

	clock1 = clock();
	qsort( arr, size, sizeof(*arr), cmp );
	clock2 = clock();
	
	return 1000.0 * (clock2 - clock1) / CLOCKS_PER_SEC;
}

#define SIZE (int)100

int main( int argc, char ** argv )
{
	srand(time(0));
	Key * arr = new Key[SIZE];

	LessThan less; // functor

	assert( rand_seq( (int *)arr, SIZE, 0, INT_MAX) );
	cout << "Sort time for " << SIZE << " ints: " 
	     << sort_time_test( sort_radix_msd, arr, SIZE, less, 8 )
	     << "ms"  << endl;
	cout << "Comparisons: " << less.amount() << endl;
	cout << (is_sorted( arr, SIZE ) ? "Sorted" : "NOT sorted") << endl;
	show_arr( (int *)arr, SIZE );
	

	/*
	 *cout << "\nTesting C qsort" << endl;
	 *assert( rand_seq( arr, SIZE, 0, 9999) );
	 *cout << (is_sorted( arr, SIZE ) ? "Sorted" : "NOT sorted") << endl;
	 *cout << "C qsort time for " << SIZE << " ints: " 
	 *     << qsort_time_test( arr, SIZE, less_than )
	 *     << endl;
	 *cout << "Comparisons: " << comps << endl;
	 */

#ifdef _MSC_VER
	getchar();
#endif
	return 0;
}
