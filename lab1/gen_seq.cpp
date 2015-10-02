#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <climits>
#include <cfloat>
#include "sequence.hpp"

using namespace std;

int_seq iseq_arr[7] = { ord, rord, saw, sinusoid, 
	stepped, quazi_ord, rand_seq };
double_seq dseq_arr[7] = { ord, rord, saw, sinusoid, 
	stepped, quazi_ord, rand_seq };

int main()
{
	int    * iarr  = NULL;
	double * darr  = NULL;
	int imin       = 0;
	int imax       = INT_MAX;
	double dmin    = 0;
	double dmax    = DBL_MAX;
	clock_t time1  = 0;
	clock_t time2  = 0;
	clock_t total1 = 0;
	clock_t total2 = 0;
	int size      = 0;
	cout << "***************************\n";
	cout << "Testing functions for INTS:\n";
	total1 = clock();
	for( int i = 0; i < 7; ++i ) {
		for( int j = 0; j < 10; ++j ) {
			size = (j+1) * 5 * 1e5;
			iarr = new int[size];
			time1 = clock();
			iseq_arr[i]( iarr, size, imin, imax, 100 );
			time2 = clock();
			delete[] iarr;

			cout << "Invoked " << fn_str[i] << "() for " 
			     << size << " of ints; time passed: "
			     << 1000.0*(time2-time1)/CLOCKS_PER_SEC
			     << "ms \n";
			
		}
		cout << endl;
	}
	cout << endl;
	cout << "******************************\n";
	cout << "Testing functions for DOUBLES:\n";
	for( int i = 0; i < 7; ++i ) {
		for( int j = 0; j < 10; ++j ) {
			size = (j+1) * 5 * 1e5;
			darr = new double[size];
			time1 = clock();
			dseq_arr[i]( darr, size, dmin, dmax, 100 );
			time2 = clock();
			delete[] darr;

			cout << "Invoked " << fn_str[i] << "() for " 
			     << size << " of doubles; time passed: "
			     << 1000.0*(time2-time1)/CLOCKS_PER_SEC
			     << "ms \n";
		}
		cout << endl;
	}
	total2 = clock();
	cout << "Total time: " 
	     << (double)(total2-total1)/CLOCKS_PER_SEC << "s\n";
#ifdef _MSC_VER
	getchar();
#endif
	return 0;
}
