#include <cstring>
#include <cstdlib>
#include "text_gen.h"


const char * alphabet = "abcdefghijklmnopqrstuvwxyz ";


int gen_text( char * dest, int size )
{
	if( dest == NULL || size < 1 )
		return 0;

	int alphabet_size = strlen( alphabet );

	for( int i = 0; i < size - 1; ++i ) {
		dest[i] = alphabet[ rand() % alphabet_size ];
	}

	return 1;
}
