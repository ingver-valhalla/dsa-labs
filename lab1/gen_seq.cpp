#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

int ord       ( int * arr, int size, int min,     int max,     int step = 0 );
int rord      ( int * arr, int size, int min,     int max,     int step = 0 );
int saw       ( int * arr, int size, int min,     int max,     int step = 0 );
int sinusoid  ( int * arr, int size, int min,     int max,     int step = 0 );
int stepped   ( int * arr, int size, int min,     int max,     int step = 0 );
int quazi_ord ( int * arr, int size, int min,     int max,     int step = 0 );
int rand_seq  ( int * arr, int size, int min = 0, int max = 0, int step = 0 );
void show_arr ( int * arr, int size );

int ord       ( double * arr, int size, double min,     double max,     int step = 0 );
int rord      ( double * arr, int size, double min,     double max,     int step = 0 );
int saw       ( double * arr, int size, double min,     double max,     int step = 0 );
int sinusoid  ( double * arr, int size, double min,     double max,     int step = 0 );
int stepped   ( double * arr, int size, double min,     double max,     int step = 0 );
int quazi_ord ( double * arr, int size, double min,     double max,     int step = 0 );
int rand_seq  ( double * arr, int size, double min = 0, double max = 0, int step = 0 );
void show_arr ( double * arr, int size );

typedef int (*int_seq)( int*, int, int, int, int );
typedef int (*double_seq)( double*, int, double, double, int);

enum fn_names { ORD, RORD, SAW, SIN, STEP, QUAZI, RAND };
const char * fn_str[] = { "ord", "rord", "saw", "sin", "step", "quazi", "rand" };
int_seq iseq_arr[7] = { ord, rord, saw, sinusoid, 
	stepped, quazi_ord, rand_seq };
double_seq dseq_arr[7] = { ord, rord, saw, sinusoid, 
	stepped, quazi_ord, rand_seq };

#define SIZE 113

int main()
{
	int iarr[SIZE] = { 0 };
	double darr[SIZE] = { 0 };
	srand( time( 0 ) );
	
	if( stepped( darr, SIZE, 3, 203, 11 ) )
		show_arr( darr, SIZE );
	else
		cout << "error\n";
	
	return 0;
}

// functions for integers
int ord( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) {
		arr[i] = min + i * (double)(max - min) / (size - 1) + 0.5;
	}
	return 1;
}

int rord( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) {
		arr[size-1 - i] = min + i * (double)(max - min) 
			/ (size - 1) + 0.5;
	}
	return 1;
}

int saw( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	int intervals = size / step + (size%step ? 1 : 0);
	// Generating first interval
	ord( arr, step, min, max );
	int i;
	for( i = 1; i < intervals - 1; ++i ) {
		//memcpy( arr + i*step, arr, step * sizeof(double) );
		copy( arr, arr + step, arr + i*step ); 
	}
	//memcpy( arr + i*step, arr, (size%step) * sizeof(double) );
	copy( arr, arr + (size%step), arr + i*step ); 
	return 1;
}
int sinusoid( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	int intervals = size / step + (size%step ? 1 : 0);
	// Generating first interval
	
	int i;
	for( i = 0; i < step; ++i ) {
		double rad_step = 2 * M_PI / step;
		arr[i] = sin( i*rad_step ) * (double)(max-min)/2 
			+ (double)(max+min)/2 + 0.5;
	}
	for( i = 1; i < intervals - 1; ++i ) {
		//memcpy( arr + i*step, arr, step * sizeof(double) );
		copy( arr, arr + step, arr + i*step ); 
	}
	//memcpy( arr + i*step, arr, (size%step) * sizeof(double) );
	copy( arr, arr + (size%step), arr + i*step ); 
	return 1;
}

int stepped( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	
	int intervals = size / step + (size%step ? 1 : 0);
	double interval_range = (double)(max - min) / intervals;
	int deviation_limit = interval_range / 5.0 + 0.5;
	int median = 0;
	int i = 0;
	for( ; i < intervals - 1; ++i ) {
		median = min + i*interval_range + interval_range / 2.0 + 0.5;
		for( int j = 0; j < step; ++j ) {
			arr[i*step + j] = median + ((rand()%2) ? 1 : -1) 
				* rand()%deviation_limit;
		}
	}
	median = min + i*interval_range + interval_range / 2.0 + 0.5;
	for( int j = 0; j < size - i*step; ++j ) {
		arr[i*step + j] = median + ((rand()%2) ? 1 : -1) 
			* rand()%deviation_limit;
	}
	
	return 1;
}
int quazi_ord( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	
	for( int i = 0; i < size; ++i ) {
		arr[i] = min + i * (double)(max - min) / (size - 1) + 0.5; 
		bool apply_deviant = rand() % 2;
		if( apply_deviant ) {
			int deviant = rand() % (max-min)/10;
			int sign = (rand() % 2) ? 1 : -1;
			arr[i] += sign * apply_deviant * deviant;
		}
	}
	return 1;
}
int rand_seq( int * arr, int size, int min, int max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) 
		arr[i] = rand() % (max-min+1) + min;
	return 1;
}

void show_arr( int * arr, int size )
{
	for( int i = 0; i < size; ++i ) {
		printf( "%d ", arr[i]);
	}
	cout << endl;
}

// functions for double
int ord( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) {
		arr[i] = min + i * (max - min) / (size - 1);
	}
	return 1;
}

int rord( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max )
		return 0;
	for( int i = 0; i < size; ++i ) {
		arr[size-1 - i] = min + i * (max - min) / (size - 1);
	}
	return 1;
}

int saw( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	int intervals = size / step + (size%step ? 1 : 0);
	// Generating first interval
	ord( arr, step, min, max );
	int i;
	for( i = 1; i < intervals - 1; ++i ) {
		//memcpy( arr + i*step, arr, step * sizeof(double) );
		copy( arr, arr + step, arr + i*step ); 
	}
	//memcpy( arr + i*step, arr, (size%step) * sizeof(double) );
	copy( arr, arr + (size%step), arr + i*step ); 
	return 1;
}
int sinusoid( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	int intervals = size / step + (size%step ? 1 : 0);
	// Generating first interval
	
	int i;
	for( i = 0; i < step; ++i ) {
		double rad_step = 2 * M_PI / step;
		arr[i] = sin( i*rad_step ) * (max-min)/2 
			+ (max+min)/2;
	}
	for( i = 1; i < intervals - 1; ++i ) {
		//memcpy( arr + i*step, arr, step * sizeof(double) );
		copy( arr, arr + step, arr + i*step ); 
	}
	//memcpy( arr + i*step, arr, (size%step) * sizeof(double) );
	copy( arr, arr + (size%step), arr + i*step ); 

	return 1;
}
int stepped( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	
	int intervals = size / step + (size%step ? 1 : 0);
	double interval_range = (max - min) / intervals;
	int deviation_limit = interval_range / 5.0 + 0.5;
	double median = 0;
	int i = 0;
	for( ; i < intervals - 1; ++i ) {
		median = min + i*interval_range + interval_range / 2.0;
		for( int j = 0; j < step; ++j ) {
			arr[i*step + j] = median + ((rand()%2) ? 1 : -1) 
				* rand()%deviation_limit;
		}
	}
	median = min + i*interval_range + interval_range / 2.0;
	for( int j = 0; j < size - i*step; ++j ) {
		arr[i*step + j] = median + ((rand()%2) ? 1 : -1) 
			* rand()%deviation_limit;
	}
	
	return 1;
}
int quazi_ord( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <= 0 || min > max || !step )
		return 0;
	
	for( int i = 0; i < size; ++i ) {
		arr[i] = min + i * (max - min) / (size - 1) + 0.5; 
		bool apply_deviant = rand() % 2;
		if( apply_deviant ) {
			int deviant = rand() % (int)(max-min)/10;
			int sign = (rand() % 2) ? 1 : -1;
			arr[i] += sign * apply_deviant * deviant;
		}
	}
	return 1;
}
int rand_seq( double * arr, int size, double min, double max, int step )
{
	if( !arr || size <=0 || min > max )
		return 0;
	for( int i = 0; i < size; i++ ) {
		arr[i] = min + (max - min) * rand() / RAND_MAX;
	}

	return 1;
}

void show_arr( double * arr, int size )
{
	for( int i = 0; i < size; ++i ) {
		printf( "%lf ", arr[i]);
	}
	cout << endl;
}
