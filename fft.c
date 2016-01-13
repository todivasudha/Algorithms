#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.14159265358979323846

struct complex{
 double a;
 double b;
};

struct complex multiply( struct complex x, struct complex y)
{
  struct complex z;
  z.a = x.a*y.a - x.b*y.b;
  z.b = x.a*y.b + x.b*y.a;
  return z;
}

struct complex add( struct complex x, struct complex y)
{
  struct complex z;
  z.a = x.a + y.a;
  z.b = x.b + y.b;
  return z;
}

struct complex subtract( struct complex x, struct complex y)
{
  struct complex z;
  z.a = x.a - y.a;
  z.b = x.b - y.b;
  return z;
}

void random_number(int n, struct complex* a)
{
  int i;
 for (i = 0; i < n; ++i)
 {
   a[i].a = rand()%10;
 }
}

void fft(int n, struct complex A[], struct complex F[])
{
  int k,j=0;
  struct complex omegak, omega;
  int sft = n/2;
  if(n==1)
  {
    F[0] = A[0];
    return;
  }
  struct complex EF[n/2], OF[n/2], E[n/2], O[n/2];
  for(k=0; k<n; k+=2)
  {
   E[j]=A[k];
   O[j]=A[k+1];
   j++;
  }
  fft(n/2, E, EF);
  fft(n/2, O, OF);
  omega.a = cos(2*PI/n);
  omega.b = sin(2*PI/n);
  omegak.a = 1.0;
  omegak.b = 0.0;
  for (k=0; k<n/2; k++, omegak = multiply(omegak,omega))
  { 
    F[k] = add(EF[k] , multiply(omegak,OF[k]));
    F[k + sft] = subtract(EF[k] , multiply(omegak,OF[k]));
  }
  
}

void ifft(int n, struct complex A[], struct complex F[])
{
  int k,j=0;
  struct complex omegak, omega;
  int sft = n/2;
  if(n==1)
  {
    F[0] = A[0];
    return;
  }
  struct complex E[n/2], O[n/2], EF[n/2], OF[n/2];
  for(k=0; k<n; k+=2)
  {
   E[j]=A[k];
   O[j]=A[k+1];
   j++;
  }
  ifft(n/2, E, EF);
  ifft(n/2, O, OF);
  omega.a = cos(-2*PI/n);
  omega.b = sin(-2*PI/n);
  omegak.a = 1.0;
  omegak.b = 0.0;
  for (k=0; k<n/2; k++, omegak = multiply(omegak,omega))
  { 
    F[k] = add(EF[k] , multiply(omegak,OF[k]));
    F[k + sft] = subtract(EF[k] , multiply(omegak,OF[k]));
  }
}


int main()
{
  int i, m, n, flag=0, d, p;
  struct complex *FA, *FB, *A, *B, *C;
  srand(time(NULL));
  printf("\nEnter number of digits in the first number :");
  scanf("%d", &m);
  printf("\nEnter number of digits in the second number :");
  scanf("%d", &n);
  int l = m + n;
  l = ceil(log(l)/log(2));
  l = pow(2,l);
  A = (struct complex*)calloc((l),sizeof(struct complex));
  B = (struct complex*)calloc((l),sizeof(struct complex));
  C = (struct complex*)calloc((l),sizeof(struct complex));
  FA = (struct complex*)calloc((l),sizeof(struct complex));
  FB = (struct complex*)calloc((l),sizeof(struct complex));
  random_number(m, A);
  random_number(n, B);
  if(m==0 && n==0)
  {
    printf("\nThe result is : 0");
    return 0;
  }
  fft((l), A, FA);
  fft((l), B, FB);
  struct complex FC[l];
  for (i = 0; i < (l); ++i)
  {
    FC[i] = multiply(FA[i], FB[i]);
  }
  ifft((l), FC, C);
  printf("\nThe first number is : ");
  for (i = m-1; i >= 0; --i)
  {
    printf("%d", (int)A[i].a);
  }
  if(!m)
    printf("0");
  printf("\nThe second number is : ");
  for (i = n-1; i >= 0; --i)
  {
    printf("%d", (int)B[i].a);
  }
  if(!n)
    printf("0");
  d = 0;
  for (i = 0; i <l; ++i)
  {
    C[i].a /= l;
    C[i].a += 0.5;
    p = (int)C[i].a + d;
    C[i].a = p%10;
    d = p/10;
  }

  printf("\nThe result is : ");
  for (i = l-1; i >= 0; --i)
  {
    if(C[i].a)
      flag = 1;
    if(flag)
      printf("%d", (int)C[i].a);
  }
  if(!flag)
    printf("0");
  return 0;
}

 
   
 
 


