#include		<stdio.h>
#include		<stdlib.h>
#include		<time.h>
#include		<unistd.h>


int main(int argc, char *argv[])
{
	int interval;
	
	srand( (unsigned)time( NULL ) );
	interval = int(rand()*4.0/(RAND_MAX+1.0));

	printf( "interval = %d\n", interval);

	sleep(interval);

	return interval-1;
}
