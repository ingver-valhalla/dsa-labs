// test_key.cpp -- a test for Key class

#include <cassert>
#include <iostream>
#include "key.h"

using namespace std;

int main()
{
	assert( Key().val() == 0 );
	assert( Key(1).val() == 1 );
	assert( Key(3) == Key(3) );
	assert( Key(4) != Key(5) );
	assert( Key(5) < Key(6) );
	assert( Key(7) > Key(6) );
	assert( Key::comparisons() == 4 );
	Key::reset_comps();
	assert( Key::comparisons() == 0 );
	Key a;
	a = 10;
	assert( Key::assignments() == 1 );
	Key::reset_assigns();
	assert( Key::assignments() == 0 );

	return 0;
}
