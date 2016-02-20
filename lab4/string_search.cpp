#include <cstdio>
#include "string_search.h"

#define S 119
#define D 256

int substringRK( const char * text, int N, const char * ptrn, int M )
{
	//using namespace std;

	//printf("Searching that:\n");
	//const char * p = ptrn;
	//while( p < ptrn + M ) {
		//putchar(*p);
		//++p;
	//}
	//putchar('\n');

	int h = 1, phash = 0, thash = 0;
	
	for( int i = 0; i < M - 1; ++i ) {
		h = (D * h) % S;
	}
	//printf( "h = %d\n", h );

	for( int i = 0; i < M; ++i ) {

		phash = (D * phash + ptrn[i]) % S;
		thash = (D * thash + text[i]) % S;
	}
	//printf( "hash for pattern %*.*s: %d\n", M, M, ptrn, phash );
	//printf( "hash for part of text %*.*s: %d\n", M, M, text, thash );

	for( int i = 0; i < N - M + 1; ++i ) {
		
		if( phash == thash ) {

			int j;
			for( j = 0; j < N && j < M && ptrn[j] == text[i+j]; ++j);
			
			if( j == M ) 
				return i;
		}

		thash = (D * (thash - h * text[i]) + text[i + M]) % S;
		thash = (thash < 0) ? thash + S : thash;
		//printf( "hash for part of text %*.*s: %d\n", M, M, text + i + 1, thash );
	}

	printf("Didn't find anything\n" );	
	return -1;
}

#undef S
#undef D
