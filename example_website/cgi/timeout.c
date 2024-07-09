#include<unistd.h>
#include<stdio.h>
int main(void)
{
	write(1, "Something is printed\n",21);
	sleep(50);
	printf("The script finished execution\n");
	return(0);
}
