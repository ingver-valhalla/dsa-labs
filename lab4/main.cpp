#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "string_search.h"

using namespace std;

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

/* return time in ms spent for searching */
double test_RK( const char * text, int text_size, int pattern_size )
{
	if( text_size < pattern_size ) {
		cout << "PATTERN IS LONGER THAN TEXT" << endl;
		return -1;
	}

	int offset = (text_size != pattern_size) ?
		(rand() % (text_size - pattern_size)) : 0;

	//cout << "offset is " << offset << endl;
	//cout << "pattern is: ";
	//for( int i = offset; i < offset + pattern_size; ++i ) {
		//cout << text[i];
	//}
	

	clock_t time1 = 0;
	clock_t time2 = 0;

	time1 = clock();
	if( -1 == substringRK( text, text_size, text + offset, pattern_size ) )
		return -1;
	time2 = clock();

	return 1000.0 * (time2 - time1) / CLOCKS_PER_SEC;
}

double test_strstr( const char * text, int text_size, int pattern_size )
{
	if( text_size < pattern_size ) {
		cout << "PATTERN IS LONGER THAN TEXT" << endl;
		return -1;
	}

	int offset = (text_size != pattern_size) ?
		(rand() % (text_size - pattern_size)) : 0;

	char * ptrn = new char[pattern_size+1]();
	strncpy( ptrn, text + offset, pattern_size );
	ptrn[pattern_size] = 0;

	clock_t time1 = 0;
	clock_t time2 = 0;

	time1 = clock();
	if( NULL == strstr( text, ptrn ) )
		return -1;
	time2 = clock();

	return 1000.0 * (time2 - time1) / CLOCKS_PER_SEC;
}

int main()
{
	const int TEXT_SIZE = (int)1e6;
	const int TESTS = 100;
	//const int ptrn_sizes[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000 };
	const int INIT_SIZE = TEXT_SIZE / 10;
	const int FINAL_SIZE = TEXT_SIZE;
	const int STEP_SIZE = INIT_SIZE;
	//const int SIZES_COUNT = sizeof(ptrn_sizes) / sizeof(ptrn_sizes[0]);
	

	//file for output
	fstream file( "out.txt", ios::out );

	{
		int degree = 0;
		int foo = INIT_SIZE;
		while( foo /= 10 )
			++degree;
		file << "degree: 10^" << degree << endl;
	}

	srand( time( NULL ) );

	char * text = new char[TEXT_SIZE+1];
	gen_text( text, TEXT_SIZE+1 );

	cout << "Text size: " << TEXT_SIZE << endl;
	cout << endl;
	//cout << "Text:" << endl;
	//cout << text << endl;

	cout << "Testing RK" << endl;
	file << "RK >>> " << endl;
	for( int i = INIT_SIZE; i <= FINAL_SIZE; i += STEP_SIZE ) {

		double total_time = 0;

		for( int j = 0; j < TESTS; ++j ) {
			
			double cur_time = test_RK( text, TEXT_SIZE, i );
			if( cur_time == -1 ) {
				total_time = -1;
				break;
			}
			else{
				total_time += cur_time;
			}
		}

		if( total_time == -1 ) {
			cout << "SUCCESSFUL SEARCH FAILED!!! CHECK ALGORITHMS" << endl;
			break;
		}
		else {
			double avg = total_time / TESTS;
			cout << "avg. time for pattern size " << i
			     << ": " << avg << endl;
			file << "(" << i / INIT_SIZE << ";" << avg << ")" << endl;
		}
	}

	cout << endl << "Testing strstr" << endl;
	file << endl << "strstr >>> " << endl;
	for( int i = INIT_SIZE; i <= FINAL_SIZE; i += STEP_SIZE ) {

		double total_time = 0;

		for( int j = 0; j < TESTS; ++j ) {
			
			double cur_time = test_strstr( text, TEXT_SIZE, i );
			if( cur_time == -1 ) {
				total_time = -1;
				break;
			}
			else{
				total_time += cur_time;
			}
		}

		if( total_time == -1 ) {
			cout << "SUCCESSFUL SEARCH FAILED!!! CHECK ALGORITHMS" << endl;
			break;
		}
		else {
			double avg = total_time / TESTS;
			cout << "avg. time for pattern size " << i
			     << ": " << avg << endl;
			file << "(" << i / INIT_SIZE << ";" << avg << ")" << endl;
		}
	}

	delete[] text;

	return 0;
}
