// sequence.cpp

#include <cstdlib>
#include <climits>

#include "sequence.h"

int make_ord( Key * arr, int size, int min, int max )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) {
		arr[i] = (int)(min + i * (double)(max - min) / (size - 1) + 0.5);
		
	}
	return 1;
}
	
int make_rand( Key * arr, int size, int min, int max )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) 
		arr[i] = (int)(rand() % (max-min+1) + min);
	return 1;
}

int make_odd( Key * arr, int size, int min, int max )
{
	if( !arr || size <= 0 )
		return 0;
	for( int i = 0; i < size; ++i ) 
		arr[i] = i * 2 + 1;
	return 1;
}

int make_sparse( Key * arr, int size, int min, int max )
{
	if( !arr || size <= 0 )
		return 0;

	int step = INT_MAX/ size;

	arr[0] = 0;
	for( int i = 0; i < size - 1; ++i ) 
		arr[i] = arr[i-1].val() + step;
	arr[size-1] = INT_MAX;

	return 1;
}
