#include <iostream>

using namespace std;

int main( int argc, char ** argv )
{
	int i = 0;
	double y;
	double min;
	double max;
	if( cin >> y ) {
		min = y;
		max = y;
		cout << i << "," << y << endl;
		++i;
	}
	else {
		cerr << "ERROR\n";
		return 1;
	}
	while( cin >> y ) {
		if( y < min ) 
			min = y;
		if( y > max )
			max = y;
		cout << i << "," << y << endl;
		i++;
	}
	cout << "\n min: " << min;
	cout << "\n max: " << max << endl;

	return 0;
}

