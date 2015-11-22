// search.h

#include "key.h"

int search_seq( Key * arr, int size, Key key, int jump = 0 );
int search_inter( Key * arr, int size, Key key, int jump = 0 );
int search_fib( Key * arr, int size, Key key, int jump = 0 );
int search_jump( Key * arr, int size, Key key, int jump = 0 );

typedef int (*Search_p)( Key*, int, Key, int );
