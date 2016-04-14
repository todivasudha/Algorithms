#include <stdio.h>

int parent(int i, int p)
{
    i -= p;
    if(i%2 == 0)
	return (p + i/2 - 1);
    return (p + i/2);
}

int left(int i, int p)
{
    i -= p;
    return (p + 2*i + 1);
}

int right(int i, int p)
{
    i -= p;
    return (p + 2*i + 2) ;
}

void minHeapify (int A[], int p, int q, int i)
{
    int l, r;
    int temp;
    l = left(i, p);
    r = right(i, p);
    int smallest = i;
    if((l <= q) && (A[l] < A[i]))
	    smallest = l;
    if((r <= q) && (A[r] < A[smallest]))
	    smallest = r;
    if(smallest != i)
    {   
        temp = A[smallest];
	    A[smallest] = A[i];
	    A[i] = temp;
	    minHeapify(A, p, q, smallest);
    }
}

void maxHeapify (int A[], int p, int q, int i)
{
    int l, r;
    int temp;
    l = left(i, p);
    r = right(i, p);
    int largest = i;
    if((l <= q) && (A[l] > A[i]))
	    largest = l;
    if((r <= q) && (A[r] > A[largest]))
	    largest = r;
    if(largest != i)
    {   
        temp = A[largest];
	    A[largest] = A[i];
	    A[i] = temp;
	    maxHeapify(A, p, q, largest);
    }
}

void buildHeap(int A[], int p, int q, int flag)
{
    int i;
    if(flag)
    {
        for(i = p+(q-p+2)/2; i >= p; i--)
        {
    	    minHeapify(A, p, q, i);
        }
    }
    else
    {
        for(i = p+(q-p+2)/2; i >= p; i--)
        {
    	    maxHeapify(A, p, q, i);
        }
    }
    return;
}
	
int extract(int A[], int p, int *q, int flag)
{
    int ele;
    ele = A[p];
    A[p] = A[*q];
    (*q)--;
    if(flag)
        minHeapify(A, p, *q, p);
    else
        maxHeapify(A, p, *q, p);
    return ele;
}

int partition(int A[], int p, int q, int pivot)
{
    int i, temp, j;
    j = p;
    for(i = p; i <= q; i++)
    {
        if(A[i] < pivot)
        {
            temp = A[j];
            A[j] = A[i];
            A[i] = temp;
            j++;
        }
        if(A[i] == pivot && A[p] < pivot)
        {
            temp = A[p];
            A[p] = A[i];
            A[i] = A[j];
            A[j] = temp;
            j++;
        }
        else if(A[i] == pivot)
        {
            temp = A[p];
            A[p] = A[i];
            A[i] = temp;
            j++;
        }
    }
    temp = A[j-1];
    A[j-1] = A[p];
    A[p] = temp;
    return (j-1-p);
}

int medianOfMedians(int A[], int p, int q, int k, int *pos)
{
    if(p == q)
    {
        *pos = p;
        return A[p];
    }
    int i, m, n, B[(q-p+1)/5+1], small, index, b = 0, j;
    for(i = p; i <= q; i+=5)
    {
        for(m = p; m < p+5 && m <=q; m++)
        {
            small = A[m];
            index = m;
            for(n = m+1; n < p+5 && n <= q; n++)
            {
                if(small > A[n])
                {
                    small = A[n];
                    index = n;
                }
            }
            if(m != index)
            {
                A[index] = A[m];
                A[m] = small;        
            }
        }
        if(i+5 > q)
        {
            B[b] = A[(i+q)/2];
        }
        else
        {
            B[b] =  A[i+2];
        }
        b++;
    }
    int pivot = medianOfMedians(B, 0, b-1, (b-1)/2, pos);
    int rank = partition(A, p, q, pivot);
    *pos = p+rank;
    if(k == rank)
        return pivot;
    else if(k < rank)
        return medianOfMedians(A, p, p+rank-1, k, pos);
    else
        return medianOfMedians(A, p+rank+1, q, (k-rank-1), pos);
}

int process(int A[], int B[], int n, int k)
{
    int pos1, pos2, k1, k2, min, max, a, b, i;
    if(k == 1)
    {
        k1 = medianOfMedians(A, 0, n-1, 0, &pos1);
        k2 = medianOfMedians(A, 0, n-1, 0, &pos2);
        if(k1 < k2)
            return k1;
        else
            return k2;
    }
    k1 = medianOfMedians(A, 0, n-1, k/2-1, &pos1);
    getchar();
    if(k%2 == 0)
        k2 = medianOfMedians(B, 0, n-1, k/2-1, &pos2);
    else
        k2 = medianOfMedians(B, 0, n-1, k/2, &pos2);
    if(k1 > k2)
    {
        a = pos1-1;
        b = n-1;
        if(b < (pos2+1))
        {
            return k1;
        }
        buildHeap(A, 0, (pos1-1), 0);
        buildHeap(B, (pos2+1), n-1, 1);
        while(k1 > k2)
        {
            if(b == pos2+1)
            {
                if(k1 > B[b])
                {
                    if(a >= 0)
                    {
                        max = extract(A, 0, &a, 0);
                        if(B[b] < max)
                            return max;
                        else
                            return B[b];
                    }
                    else B[b];
                }
                else
                    return k1;
            }
            if(a < 0)
            {
                min = extract(B, (pos2+1), &b, 1);
                if(min < k1)
                    return min;
                else
                    return k1;
            }
            max = extract(A, 0, &a, 0);
            min = extract(B, (pos2+1), &b, 1);
            if(min < k1)
            {
                k1 = max;
                k2 = min;
            }
            else
                return k1;
        }
        return k2;
    }
    if(k1 < k2)
    {
        a = n-1;
        b = pos2-1;
        if(a < (pos1+1))
        {
            return k2;
        }
        buildHeap(A, pos1+1, n-1, 1);
        buildHeap(B, 0, pos2-1, 0);
        while(k1 < k2)
        {
            if(a == pos1+1)
            {
                if(k2 > A[a])
                {
                    if(b >= 0)
                    {
                        max = extract(B, 0, &b, 0);
                        if(A[a] < max)
                            return max;
                        else
                            return A[a];
                    }
                    else A[a];

                }
                else
                    return k2;
            }
            if(b < 0)
            {
                min = extract(A, (pos1+1), &a, 1);
                if(min < k2)
                    return min;
                else
                    return k2;
            }
            max = extract(B, 0, &b, 0);
            min = extract(A, (pos1+1), &a, 1);
            if(min < k2)
            {
                k2 = max;
                k1 = min;
            }
            else
                return k2;
        }
        return k1;
    }
}

int main()
{
    int n, k, A[1000], B[1000], i;
    printf("\nEnter k n p1 p2 ... pn q1 q2 ... qn\n");
    scanf("%d %d", &k, &n);
    for (i = 0; i < n; ++i)
    {
        scanf("%d", &A[i]);
    }
    for (i = 0; i < n; ++i)
    {
        scanf("%d", &B[i]);
    }
    if(k <= 0 || k > 2*n)
    {
        printf("\nNo such index!");
        return 0;
    }
    printf("\nThe two lists are :\n");
    for (i = 0; i < n; ++i)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
    for (i = 0; i < n; ++i)
    {
        printf("%d ", B[i]);
    }
    int result = process(A, B, n, k);
    printf("\nThe %dth ranked element is : %d\n", k, result);
    return 0;
}
