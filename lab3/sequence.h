// sequence.h

#include "key.h"

int make_ord( Key * arr, int size, int min, int max );
int make_rand( Key * arr, int size, int min, int max );
int make_odd( Key * arr, int size, int min = 0, int max = 0 );
int make_sparse( Key * arr, int size, int min = 0, int max = 0 );


typedef int (*Make_seq_p)( Key*, int, int, int );
