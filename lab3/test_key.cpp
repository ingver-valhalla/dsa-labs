// test_key.cpp -- a test for Key class

#include <cassert>
#include <iostream>
#include "key.h"

using namespace std;

int main()
{
	cout << "==============================" << endl;
	cout << "LAUNCHING TEST" << endl;
	cout << "==============================" << endl;
	assert( Key().val() == 0 );
	assert( Key(1).val() == 1 );

	assert( Key(3) == Key(3) );
	assert(! (Key(2) == Key(3)) );

	assert( Key(4) != Key(5) );
	assert(! (Key(3) != Key(3)) );

	assert( Key(5) < Key(6) );
	assert(! (Key(5) < Key(5)) );
	assert(! (Key(5) < Key(4)) );

	assert( Key(7) > Key(6) );
	assert(! (Key(7) > Key(7)) );
	assert(! (Key(7) > Key(8)) );

	assert( Key(10) >= Key(10) );
	assert( Key(10) >= Key(9) );
	assert(! (Key(10) >= Key(11)) );

	assert( Key(10) <= Key(10) );
	assert( Key(10) <= Key(11) );
	assert(! (Key(10) <= Key(9)) );

	assert( Key::comparisons() == 20 );
	Key::reset_comps();
	assert( Key::comparisons() == 0 );

	Key a;
	a = 10;
	assert( Key::assignments() == 1 );
	Key::reset_assigns();
	assert( Key::assignments() == 0 );

	cout << "==============================" << endl;
	std::cout << "TEST PASSED" << std::endl;
	cout << "==============================" << endl;
	return 0;
}
