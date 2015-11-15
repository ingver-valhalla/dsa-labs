// sequence.cpp

#include <cstdlib>

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
