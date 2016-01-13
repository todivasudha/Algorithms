#include <stdio.h>
#include <stdlib.h>

void board(int **A, int i, int j, int n, int a, int b, int  c)
{
  int k, l;
  if(n==2)
  {
     for(k=i; k<i+2; k++)
     {
       for(l=j; l<j+2; l++)
       {
         if(!A[k][l])
          {
             A[k][l] = c;
          }
        }
      }
    return;
  }
  if(a < i+n/2)
    {
      A[i+n/2][j+n/2] =  A[i+n/2][j+n/2-1] = c;
      board(A, i+n/2, j, n/2, i+n/2, j+n/2-1, c*4-2);
      board(A, i+n/2, j+n/2, n/2, i+n/2, j+n/2, c*4-1);
      if(b < j+n/2)
        {
          A[i+n/2-1][j+n/2] = c;
          board(A, i, j+n/2, n/2, i+n/2-1, j+n/2, c*4);
          board(A, i, j, n/2, a, b, c*4+1);
        }
      else
        {
          A[i+n/2-1][j+n/2-1] = c;
          board(A, i, j, n/2, i+n/2-1, j+n/2-1, c*4);
          board(A, i, j+n/2, n/2, a, b, c*4+1);
        }
    }
  else
    {
      A[i+n/2-1][j+n/2] =  A[i+n/2-1][j+n/2-1] = c;
      board(A, i, j, n/2, i+n/2-1, j+n/2-1, c*4-2);
      board(A, i, j+n/2, n/2, i+n/2-1, j+n/2, c*4-1);
      if(b < j+n/2)
        {
          A[i+n/2][j+n/2] = c;
          board(A, i+n/2, j+n/2, n/2, i+n/2, j+n/2, c*4);
          board(A, i+n/2, j, n/2, a, b, c*4+1);
        }
      else
        {
          A[i+n/2][j+n/2-1] = c;
          board(A, i+n/2, j, n/2, i+n/2, j+n/2-1, c*4);
          board(A, i+n/2, j+n/2, n/2, a, b, c*4+1);
        }
    }
  return;
}

int main()
{
  int **A;
  int a, b, c, i, j, n;
  c=1;
  printf("\nEnter the value of n : ");
  scanf("%d", &n);
  if(!n)
  {
    printf("\nInvalid!");
    return;
  }
  if(n==1)
  {
    printf("\n-1");
    return;
  }
  A = (int**)calloc(n, sizeof(int*));
  for (i = 0; i < n; i++) {
    A[i] = (int*)calloc(n, sizeof(int));
  }
  printf("\nEnter the x and y co-ordinates of the missing cell : ");
  scanf("%d %d", &a, &b);
  A[a][b] = -1;
  printf("\n");
  board(A, 0, 0, n, a, b, c);
  for(i = 0; i<n; i++)
  {
  for(j=0; j<n; j++)
    {
      printf("%d\t", A[i][j]);
    }
  printf("\n"); 
  }
  printf("\n");
  return;
}







