#include <cstdio>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "tree.h"

FILE* fi = NULL;

int get_rand()
{
	return rand() % 1001 - 500;
}

void mul2( const pNode root )
{
	printf( "%d ", root->Data * 2 );
}

void print( const pNode root )
{
	printf( "%d ", root->Data );
}


#define TESTS 10
#define INIT 100
#define FINAL 1000
#define STEP 100

typedef void (*pIns)( pNode *, Item );
void test_rand_bst( pIns insert )
{
	pNode tr = NULL;

	printf( "Inserting random keys...\n\n" );
	fprintf( fi, "random keys...\n\n" );

	for( int num = INIT; num <= FINAL; num += STEP ) {

		int height_sum = 0;
		for( int k = 0; k < TESTS; ++k ) {

			for( int j = 0; j < num; ++j ) {
				insert( &tr, rand() );
			}
			int height = TreeHeight( tr );
			//printf( "height = %d\n", height );
			height_sum += height;
			FreeTree( &tr );
		}
		printf( "elements: %d, avg. tree height: %lf\n", num, 
		        (double)height_sum / TESTS );
		fprintf( fi, "(%d;%.2lf)\n", num, (double)height_sum / TESTS );
	}

	printf( "\n\n" );
	printf( "Inserting ordered keys...\n\n" );
	fprintf( fi, "\nordered keys...\n\n" );

	for( int num = INIT; num <= FINAL; num += STEP ) {

		int height_sum = 0;
		for( int k = 0; k < TESTS; ++k ) {

			for( int j = 0; j < num; ++j ) {
				insert( &tr, j );
			}
			int height = TreeHeight( tr );
			//printf( "height = %d\n", height );
			height_sum += height;
			FreeTree( &tr );
		}
		printf( "elements: %d, avg. tree height: %lf\n", num, 
		        (double)height_sum / TESTS );
		fprintf( fi, "(%d;%.2lf)\n", num, (double)height_sum / TESTS );
	}
}

int main()
{
	srand( time( NULL ) );

	fi = fopen( "out.txt", "w" );
	if( fi == NULL ) {
		fprintf( stderr, "Failed to open file\n" );
		exit( 1 );
	}

	pNode tr = NULL;
	for( int i = 0; i < 10; ++i ) {
		int num = get_rand();
		//printf( "Inserting %d\n", num );
		InsertRand( &tr, num);
		//PrintTree( tr );
		//getchar();
	}
	PrintTree( tr );
	printf( "CountSum:\n" );
	printf( "\nresult: %d\n", CountSum( tr, print ));

	printf( "tree height: %d\n", TreeHeight( tr ) );
	printf( "nodes count: %d\n", CountNodes( tr ) );

	//printf( "VisitPost: " );
	//VisitPost( tr, mul2 );
	//printf( "\n" );

	printf( "SumOfInner: %d\n", SumOfInner( tr ) );
	
	printf( "\n*******************\n" );
	printf( "Testing BST\n" );
	fprintf( fi, "Testing BST\n" );
	test_rand_bst( Insert );
	printf( "\n*******************\n" );
	printf( "Testing Randomized BST\n" );
	fprintf( fi, "\nTesting Randomized BST\n" );
	test_rand_bst( InsertRand );
	
	//getchar();
	FreeTree( &tr );

	fclose( fi );

	return 0;
}
