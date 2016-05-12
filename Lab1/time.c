#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


clock_t begin;
clock_t end;

void  Begin_clock()
{
	begin = clock();

}

void Eng_clock()
{
	end = clock();
}

void Calculate_Time()
{
	double time_spent;
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf(stderr, "Time is : %f\n", time_spent);
}