// search.cpp

#include <iostream>
#include <cmath>
#include "key.h"
#include "search.h"

int search_fib( Key * arr, int size, Key key, int jump )
{
	if( !arr || size <= 0 )
		return -1;

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
                
int search_seq( Key* arr, int size, Key key, int jump )
{
	if( !arr || size <= 0 )
		return -1;

	Key t = arr[size - 1];
	int i; arr[size - 1] = key;
	for( i = 0; arr[i] != key; ++i );
	arr[size - 1] = t;
	return (i < size && arr[size-1] == key) ? i : -1;
}

int search_inter( Key * arr, int size, Key key, int jump )
{
	if( !arr || size <= 0 )
		return -1;

	int low = 0;
	int high = size - 1;
	int mid;

	while( arr[high] != arr[low] && key >= arr[low] && key <= arr[high] ) {

		mid = low + (key.val() - arr[low].val()) * ((high - low) / (arr[high].val() - arr[low].val()));

		if( key > arr[mid] ) {
			low = mid + 1;
		}
		else if( key < arr[mid] ) {
			high = mid - 1;
		}
		else {
			return mid;
		}
		//std::cout << "low = " << low << ", mid = " << mid << ", high = " << high << std::endl;

	}

	if( key == arr[low] ) {
		return low;
	}

	return -1;

}

int search_jump( Key * arr, int size, Key key, int jump )
{
	if( !arr || size <= 0 || jump <= 0 )
		return -1;

	int pos = jump;

	for( ; pos < size && arr[pos] < key; pos += jump ) {
		//std::cout << "pos = " << pos << "; arr[pos] = " << arr[pos].val() << std::endl;
	}

	//std::cout << "****pos = " << pos << std::endl;
	if( pos - jump < size ) {
		int lim = pos + 1 < size ? pos + 1 : size;

		//std::cout << "finding element" << std::endl;
		//std::cout << "pos - jump = " << pos - jump
			  //<< "; lim = " << lim << std::endl;

		for( int i = pos - jump; i < lim; ++i ) {
			if( arr[i] == key ) {
				return i;
			}
		}
	}
	return -1;
}

int search_jump2( Key * arr, int size, Key key, int )
{
	if( !arr || size <= 0 ) 
		return -1;

	int jump = sqrt( size );
	int pos = jump;

	for( ; pos < size && arr[pos] < key; pos += jump );

	if( pos - jump >= size )
		return -1;

	return search_jump( arr + (pos - jump), jump+1, key, sqrt( jump+1 ) );
}
