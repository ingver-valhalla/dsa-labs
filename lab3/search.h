// search.h

#include "key.h"

int search_seq( Key * arr, int size, Key key );
int search_inter( Key * arr, int size, Key key );
int search_fib( Key * arr, int size, Key key );

typedef int (*Search_p)( Key*, int, Key );
