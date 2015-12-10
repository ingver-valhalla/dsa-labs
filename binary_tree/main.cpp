#include <iostream>
#include <cstdlib>
#include "RandomizedBST.h"

using namespace std;

//struct A {
	//int data;

	//A( int i ) : data( i ) {}
	//~A()
	//{
		//cout << "destroying Item(" << data << ")" << endl;
	//}
	//bool operator < ( const A & a ) const 
	//{
		//return data < a.data;
	//}
	//bool operator > ( const A & a ) const
	//{
		//return data > a.data;
	//}


	//friend ostream & operator << ( ostream &, A & );
//};

//ostream& operator << ( ostream & os, A & a )
//{
	//os << a.data;
//}

int main()
{
	srand( time( 0 ) );

	RandomizedBST<int> tr;

	for( int i = 0; i < 20; ++i )
		tr.insert( i );

	cout << endl;
	int to_delete;

	cout << "Which to delete? ";
	while( cin >> to_delete ) {

		tr.remove( to_delete );
		if( tr.is_empty() ) 
			break;
		cout << "Which to delete? ";
	}

	cout << endl;
	return 0;
}
