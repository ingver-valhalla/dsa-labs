// sorts.cpp

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>

typedef int Key;

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
			std::swap( arr[i-1], arr[i] );
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
		std::cerr << "MSRadixG: Error while allocating memory" 
		          << std::endl;
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
	if( word > type_size )
		return 0;
	for( int i = word; i != 1; i /= 2 )
		if( i % 2 ) {
			std::cerr << "word isn't a power of 2" << std::endl;
			return 0;
		}

	MSRadixG( arr, 0, size - 1, cmp, type_size/word - 1, word );
	delete[] aux;
	aux = NULL;
	/*
	 *std::cout << "///" << std::endl;
	 *std::cout << "Recursion depth = " << recursion_depth << std::endl;
	 *std::cout << "Memory cost for arrays: " 
	 *     << (memory_cost > 1024 ? memory_cost / 1024 : memory_cost )
	 *     << (memory_cost > 1024 ? "Mb" : "Kb") << std::endl;
	 */
	memory_cost = 0;
	recursion_depth = 0;
	return 1;
}
//==============================================================================
