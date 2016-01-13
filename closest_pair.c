#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

struct point
{
	int x;
	int y;
};

struct pair
{
	struct point p1, p2;
	float dist;
};

void quicksort(struct point *A, int left, int right, int flag){
    int min = (left+right)/2;
    int i = left;
    int j = right;
    struct point temp;
    struct point pivot = A[min];

    while(left<j || i<right)
    {	
    	if(flag)
    	{
       	 while(A[i].y<pivot.y)
       	 i++;
       	 while(A[j].y>pivot.y)
         j--;
     	}
     	else
     	{
     	 while(A[i].x<pivot.x)
       	 i++;
       	 while(A[j].x>pivot.x)
         j--;
     	}

        if(i<=j){
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
            i++;
            j--;
        }
        else{
            if(left<j)
                quicksort(A, left, j,flag);
            if(i<right)
                quicksort(A,i,right,flag);
            return;
        }
    }
}

struct pair direct(struct point *A, int j1, int j2)
{
  int i, j;
  float d;
  struct pair min;
  min.dist = FLT_MAX;
  for (i = j1; i <=j2 ; ++i)
  {
  	for (j = i+1; j <=j2 ; ++j)
  	{
  		d = sqrt((A[i].x - A[j].x)*(A[i].x - A[j].x) + (A[i].y - A[j].y)*(A[i].y - A[j].y));
  		if(d<min.dist)
  		{
  		  	min.dist = d;
  		  	min.p1 = A[i];
  		  	min.p2 = A[j];
  		}
  	}
  }
  return min;
}

struct pair strip(struct point *P, struct pair sd, int n)
{
	int i, j;
	float min;
	for (i = 0; i < n; ++i)
	{
		for (j = i+1; j <n,(P[j].x - P[i].x) < sd.dist ; ++j)
		{
			min = sqrt((P[i].x - P[j].x)*(P[i].x - P[j].x) + (P[i].y - P[j].y)*(P[i].y - P[j].y));
			if(min < sd.dist)
			{
				sd.dist = min;
				sd.p1 = P[i];
				sd.p2 = P[j];
			}
		}
	}
	return sd;
}

struct pair shortestDist(struct point *A,struct point *Ax, int n, int j1, int j2)
{
	int k, l, mid;
	struct pair sd;
	float t;
	struct point *P;
	if((j2 - j1) <= 2)
	{
		sd = direct(A, j1, j2);
		return sd;
	}
	P = (struct point*)malloc(n*sizeof(struct point));
	mid = (j1 + j2)/2;
	struct pair a = shortestDist(A, Ax, n, j1, mid);
	struct pair b = shortestDist(A, Ax, n, mid+1, j2);
	if(a.dist<b.dist)
		sd = a;
	else
		sd = b;
	int c = 0;
    for (k = 0; k < n; ++k)
    {
    	if((Ax[k].y>= A[mid].y - sd.dist) && (Ax[k].y<= A[mid].y + sd.dist))
    	{
    		P[c++] = Ax[k];
    	}
    }
    sd = strip(P, sd, c);
	return sd;

}

int main()
{
	struct point *A, *Ax;
	int n, i;
	printf("\nEnter the no. of points : ");
	scanf("%d",&n);
	A = (struct point*)malloc(n*sizeof(struct point));
	Ax = (struct point*)malloc(n*sizeof(struct point));
	for (i = 0; i <n ; ++i)
	{
		printf("\nEnter the x and y co-ordinates of the %dth point : ", (i+1));
		scanf("%d %d", &A[i].x, &A[i].y);
		Ax[i] = A[i];
	}
	if(n<=1)
	{
		printf("\nInvalid!");
		return;
	}
	quicksort(A, 0, n-1, 1);
	quicksort(Ax, 0, n-1, 0);
	struct pair sd = shortestDist(A, Ax, n, 0, n-1);
	printf("\nThe closest points are : (%d,%d) and (%d,%d)", sd.p1.x, sd.p1.y, sd.p2.x, sd.p2.y);
	printf("\nThe shortest distance between them : %f\n", sd.dist);
	return;
}