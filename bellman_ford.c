#include <stdio.h>
#include <float.h>
#include <math.h>

int bellmanFord(float X[30][30], int s, int n)
{
	int i, u, v;
	float dist[30];
	for (i = 0; i < n; ++i)
	{
		if(i == s)
			dist[i] = 0;
		else
			dist[i] = FLT_MAX;
	}
	for (i = 0; i < (n-1); ++i)
	{
		for (u = 0; u < n; ++u)
		{
			for (v = 0; v < n; ++v)
			{
				if(dist[v] > dist[u] + X[u][v])
				{
					dist[v] = dist[u] + X[u][v];
				}
			}
		}
	}
	for (u = 0; u < n; ++u)
	{
		for (v = 0; v < n; ++v)
		{
			if(dist[v] > dist[u] + X[u][v])
			{
				return 1;
			}	
		}
	}
	return 0;
}

int main()
{
	const int n = 30;
	float X[n][n];
	int i, j, flag = 0;
	printf("\nEnter the conversion table : ");
	for (i = 0; i < n; ++i)
	{
		printf("\nEnter conversion for %dth currency : ", (i+1));
		for (j = 0; j < n; ++j)
		{
			scanf("%f", &X[i][j]);
			X[i][j] = -1*log(X[i][j]/100);
		}
	}
	flag = bellmanFord(X, 0, n);
	if(flag)
		printf("\nThe conversion table represents an opportunity of making a profit through cyclic conversions.");
	else
		printf("\nThe conversion table does not represent an opportunity to make profit.");
	return;
}