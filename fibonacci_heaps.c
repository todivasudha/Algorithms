#include <stdio.h>
#include <stdlib.h>

struct node{
	int a;
	struct node *parent, *child, *l, *r;
	int degree;
	int mark;
};

struct list{
	int b[600];
	struct list *next;
};

struct list *listInsert(struct list *head, int a[], int m)
{
	int i;
    struct list *l = (struct list *)malloc(sizeof(struct list));
    struct list *l1 = head;
    for (i = 0; i < m; ++i)
    {
    	l->b[i] = a[i];	
    }
    l->next = NULL;
    if(head == NULL)
    {
		head = l;
		return head;
    }
    while(l1->next !=NULL)
    {
    	l1 = l1->next;
    }
    l1->next = l;
    return head;
}

struct list *listDelete(struct list *l, struct list *l1, int m)
{
	int i;
	if(l == NULL)
	{
		printf("\nError");
		return l;
	}
	for (i = 0; i < m; ++i)
	{
		l1->b[i] = l->b[i];
	}
	l1->next = NULL;
	struct list *l2 = l;
	l = l->next;
	free(l2);
	return l;
}

struct node *makeHeap(int *n)
{
	struct node *H = NULL;
	*n = 0;
	return H;
}

struct node *createNode(int a)
{
	struct node* N= (struct node *)malloc(sizeof(struct node));
	N->a = a;
	N->parent = N->child = NULL;
	N->l = N->r = N;
	N->degree = N->mark = 0;
	return N;
}

struct node *insertCircular(struct node *head, struct node *x)
{
	if(head == NULL)
	{
		head = x;
		x->l = x->r = x;
		return head;
	}
	x->l = head->l;
	x->r = head;
	x->parent = head->parent;
	if(head->l != NULL)
		head->l->r = x;
	head->l = x;
	return head;
}

struct node *deleteCircular(struct node *H, struct node *y)
{
	if(y->r == y)
	{
		H = NULL;
		return H;
	}
	y->r->l = y->l;
	y->l->r = y->r;
	if(H == y)
		H = y->r;
	return H;
}

void print(struct node *H, int p)
{
	struct node *x = H, *y = H;
	int i;
	if(H == NULL)
		return;
	while(1)
	{
		printf("\n");
		for (i = 0; i < p; ++i)
		{
			printf(" ");
		}
		printf("%d->child %d %d", x->a, x->mark, x->degree);
		print(x->child, p+1);
		x = x->r;
		if(x == y)
			break;
	}
	return;
}

struct node *insertFibHeap(struct node *H, int a, int *n, struct node **y)
{
	struct node *x;
	x = createNode(a);
	H = insertCircular(H, x);
	if(H->a > x->a)
		H = x;
	*y = x;
	(*n)++;
	return H;
}

struct node *unionHeap(struct node *H1, struct node *H2, int n1, int n2, int *n)
{
	struct node *H;
	if(H1 == NULL)
	{
		H = H2;
		*n = n2;
		return H;
	}
	else if(H2 == NULL)
	{
		H = H1;
		*n = n1;
		return H;
	}
	H = H1;
	struct node *temp = H->r;
    H->r = H2;
    struct node *temp1 = H2->l;
    H2->l = H;
    temp1->r = temp;
    temp->l = temp1;
	if(H1->a > H2->a)
		H = H2;
	*n = n1 + n2;
	return H;
}

struct node *fibHeapLink(struct node *H, struct node *x, struct node *y)
{
	H = deleteCircular(H, y);
	x->child = insertCircular(x->child, y);
	y->parent = x;
	x->degree++;
	y->mark = 0;
	return H;
}

struct node *consolidate(struct node *H, int n)
{
	int i, d, b;
	struct node *A[n];
	for (i = 0; i < n; ++i)
	{
		A[i] = NULL;
	}
	struct node *temp;
	int start = H->l->a;
	struct node *x = H;
	struct node *p = H;
	while(1)
	{
		p = x->r;
		b = x->a;
		d = x->degree;
		while(A[d])
		{
			struct node *y = A[d];
			//printf("\nhi %d %d",x->a, y->a);
			//getchar();
			if(x->a > y->a)
			{
				temp = x;
				x = y;
				y = temp;
			}
			H = fibHeapLink(H, x, y);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
		if(b == start)
			break;
		x = p;
	}
	H = NULL;
	for (i = 0; i < n; ++i)
	{
		if(A[i])
		{
			H = insertCircular(H, A[i]);
			if(H == NULL || A[i]->a < H->a)
				H = A[i];
		}
	}
	return H;
}

struct node *extractMinFibHeap(struct node *H, int *n, int *a)
{
	if(H != NULL)
	{
		if(H->r == H && H->child == NULL)
		{
			*a = H->a;
			H = NULL;
			return H;
		}
		struct node *x = H->child, *start = H->child;
		*a = H->a;
		if(x != NULL)
		{
			struct node *temp = H->r;
    		H->r = x;
    		struct node *temp1 = x->l;
    		x->l = H;
    		temp1->r = temp;
    		temp->l = temp1;
    		while(1)
    		{
    			x->parent = NULL;
    			x = x->r;
    			if(x == start)
    				break;
    		}
    		H->child = NULL;
		}
		if(H == NULL)
			printf("\nEmpty!");
		H = deleteCircular(H, H);
		H = consolidate(H, *n);
		(*n)--;
	}
	return H;
}

struct node *cut(struct node *H, struct node *x, struct node *y)
{
	y->child = deleteCircular(y->child, x);
	y->degree--;
	H = insertCircular(H, x);
	x->parent = NULL;
	x->mark = 0;
	return H;
}

struct node *cascadingCut(struct node *H, struct node *y)
{
	struct node *z = y->parent;
	if(z != NULL)
	{
		if(y->mark == 0)
			y->mark = 1;
		else
		{
			H = cut(H, y, z);
			H = cascadingCut(H, z);
		}
	}
	return H;
}

struct node *decreaseKey(struct node *H, struct node *x, int k)
{
	if(k > x->a)
	{
		printf("\nNew key is greater than current key!");
		return H;
	}
	x->a = k;
	struct node *y = x->parent;
	if(y != NULL && x->a < y->a)
	{
		H = cut(H, x, y);
		H = cascadingCut(H, y);
	}
	if(H->a > x->a)
		H = x;
	return H;
}

int main()
{
	int m, k, i, j, p, n, n1, m1,q;
	n = 0;
	struct node *H = makeHeap(&n);
	struct node *H1 = makeHeap(&n1);
	struct node *large = (struct node *)malloc(sizeof(struct node));
	printf("\nEnter k and m : ");
	scanf("%d %d", &k, &m);
	int temp[m];
	struct list *l[k], *l1 = (struct list *)malloc(sizeof(struct list));
	printf("\nNo. of blocks in a list :");
	scanf("%d", &m1);
	struct list *l2 = NULL;
	int a[600];
	for (i = 0; i < k; ++i)
	{
		l[i] = NULL;
		printf("\nEnter elements of %dth list : ", (i+1));
		for (j = 0; j < m1; ++j)
		{
			for (p = 0; p < m; ++p)
			{
				scanf("%d", &a[p]);
			}
			l[i] = listInsert(l[i], a, m);
		}
	}
	for (q = 0; q < m1; ++q)
	{
		for (i = 0; i < k; ++i)
		{
			l[i] = listDelete(l[i], l1, m);
			H1 = NULL;
			n1 = 0;
			for (j = 0; j < m; ++j)
			{
				H1 = insertFibHeap(H1, l1->b[j], &n1, &large);
			}
			H = unionHeap(H, H1, n, n1, &n);
		}
		H = decreaseKey(H, large, (H->a - 1));
		for (i = 0; i < k; ++i)
		{
			for (j = 0; j < m; ++j)
			{
				H = extractMinFibHeap(H, &n, &p);
				temp[j] = p;
			}
			l2 = listInsert(l2, temp, m);
		}
	}
	struct list *l3  = l2;
	printf("\nThe final list :");
	while(l3 != NULL)
	{
		printf("\n");
		for (i = 0; i < m; ++i)
		{
			printf("%d ", l3->b[i]);
		}
		l3 = l3->next;
	}
	return;
}