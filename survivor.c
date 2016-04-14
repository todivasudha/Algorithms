#include <stdio.h>
#include <math.h>

int survivor(int n, int flag, int j, int d)
{
	int i;
	if(flag == 1)
		d += pow(2, j-1);
	else if(flag == 2)
		d -= pow(2, j-1);
	if(n==1)
	{
			return (int)(pow(2, j) - d);
	}
	if(flag == 0)
	{
		for(i = 1; i <= n/2; i++)
			printf("%d ", i*2);
		if(n%2 !=0)	
			printf("1 ");
		j = 1;
		if(n%2 == 0)
			return survivor(n/2, 1, j, d);
		else
			return survivor(n/2, 2, j, d);
	}
	for(i = 1; i <= n/2; i++)
	{
		printf("%d ", (int)(pow(2, j)*2*i - d));
	}
	if(n%2 != 0)
	{
		printf("%d ", (int)(pow(2, j)*1 - d));
		j++;
		return survivor(n/2, 2, j, d);
	}
	else
	{
		j++;
		return survivor(n/2, 1, j, d);
	}
}

int main()
{
	int n, v, j = 0, d = 0, m, l;
	printf("\nEnter n : ");
	scanf("%d", &n);
	printf("The elimination order : ");
	v = survivor(n, 0, j, d);
	printf("\nThe survivor is : %d\n", v);
	m = log(n)/log(2);
	l = n - pow(2, m);
	v = 2*l + 1;
	printf("\nUsing the formula V(2^m + l) = 2*l + 1, the survivor is:  %d\n", v);
    return 0;
}
	 

