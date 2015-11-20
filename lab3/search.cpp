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
	//std::cout << "search_inter" << std::endl;
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
