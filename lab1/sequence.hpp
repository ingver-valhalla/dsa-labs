int ord       ( int * arr, int size, int min,     int max,     int step = 0 );
int rord      ( int * arr, int size, int min,     int max,     int step = 0 );
int saw       ( int * arr, int size, int min,     int max,     int step = 0 );
int sinusoid  ( int * arr, int size, int min,     int max,     int step = 0 );
int stepped   ( int * arr, int size, int min,     int max,     int step = 0 );
int quazi_ord ( int * arr, int size, int min,     int max,     int step = 0 );
int rand_seq  ( int * arr, int size, int min = 0, int max = 0, int step = 0 );

int ord       ( double * arr, int size, double min,     double max,     int step = 0 );
int rord      ( double * arr, int size, double min,     double max,     int step = 0 );
int saw       ( double * arr, int size, double min,     double max,     int step = 0 );
int sinusoid  ( double * arr, int size, double min,     double max,     int step = 0 );
int stepped   ( double * arr, int size, double min,     double max,     int step = 0 );
int quazi_ord ( double * arr, int size, double min,     double max,     int step = 0 );
int rand_seq  ( double * arr, int size, double min = 0, double max = 0, int step = 0 );

void show_arr ( int * arr, int size );
void show_arr ( double * arr, int size );

typedef int (*int_seq)( int*, int, int, int, int );
typedef int (*double_seq)( double*, int, double, double, int);

enum fn_names { ORD, RORD, SAW, SIN, STEP, QUAZI, RAND };
extern const char * fn_str[];
