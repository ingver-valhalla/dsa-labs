// search.cpp

#include "key.h"
#include "search.h"
#include <iostream>

int search_fib( Key * arr, int size, Key key )
{
	int index, f2 = 1, f1 = 1, t, high = size - 1;
	for( ; f1 <= size; f1 += f2, f2 = f1 - f2 );
	index = f2 - 1;
	f1 -= f2;
	f2 -= f1;
	while( f1 + f2 > 0 )
		if( key < arr[index] ) {
			index -= f2;
			t = f2;
			f2 = f1 - f2;
			f1 = t;
		}
		else if( key > arr[index] ) {
			index += f2;
			index = high > index ? index : high;
			f1 -= f2;
			f2 -= f1;
		}
		else
			return index;
	return -1;
				
}
                
int search_seq( Key* arr, int size, Key key )
{
	Key t = arr[size - 1];
	int i; arr[size - 1] = key;
	for( i = 0; arr[i] != key; ++i );
	arr[size - 1] = t;
	return i < size ? i : -1;
}

int search_inter( Key * arr, int size, Key key )
{
	int low = 0, high = size - 1,
	    pos = low + key.val() * (high - low) / (arr[high].val() - arr[low].val());
	for( ; low <= high && low <= pos && pos <= high
	     ; pos = low + arr[pos].val() * (high - low) / (arr[high].val() - arr[low].val()) )
		if( key < arr[pos] )
			high = pos - 1;
		else if( key > arr[pos] )
			low = pos + 1;
		else
			return pos;
	return -1;
}
