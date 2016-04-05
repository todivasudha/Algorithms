#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

struct point{
    float x;
    float y;
};

struct event{
    int i;
    int j;
    int status;
    float time;
    int type;    //wall or another ball
};

struct node{
    struct event *e;
    struct node *next;
    
};

struct state{
    int color;
    int radius;
    struct point pos;
    struct point vel;
    float t;
    struct node *list;
};

float dot(struct point a, struct point b)
{
    return (a.x*b.x + a.y*b.y);
}

float modSquare(struct point a)
{
    return (a.x*a.x + a.y*a.y);
}

struct node *listInsert(struct node *head, struct event *a)
{
    struct node *l = (struct node *)malloc(sizeof(struct node));
    l->e = a;
    l->next = NULL;
    if(head == NULL)
    {
	head = l;
	return head;
    }
    l->next = head;
    head = l;
    return head;
}

int parent(int i)
{
    if(i%2 == 0)
	return (i/2 - 1);
    return i/2;
}

int left(int i)
{
    return (2*i + 1);
}

int right(int i)
{
    return (2*i + 2) ;
}

void heapify (struct event *A[], int heap_size, int i)
{
    int l, r;
    struct event *temp;
    l = left(i);
    r = right(i);
    int smallest = i;
    if((l < heap_size) && (A[l]->time < A[i]->time))
	smallest = l;
    if((r < heap_size) && (A[r]->time < A[smallest]->time))
	smallest = r;
    if(smallest != i)
    {   temp = A[smallest];
	A[smallest] = A[i];
	A[i] = temp;
	heapify(A, heap_size, smallest);
    }
}

void buildHeap(struct event *A[], int heap_size)
{
    int i;
    for(i = heap_size/2; i >= 0; i--)
    {
	heapify(A, heap_size, i);
    }
}
	
struct event *extractMin(struct event *A[], int *heap_size)
{
    struct event *min;
    if(*heap_size < 0)
    {
	min->i = -1;
	return min;
    }
    min = A[0];
    A[0] = A[*heap_size - 1];
    (*heap_size)--;
    heapify(A, *heap_size, 0);
    return min;
}
 
struct event *heapInsert(struct event *A[], int *heap_size, struct event *ele)
{
    (*heap_size)++;
    int i = *heap_size - 1;
    while(i > 0 && ele->time < A[parent(i)]->time)
    {   
	A[i] = A[parent(i)];
	i = parent(i);
    }
    A[i] = ele;
    ele = A[i];
    return ele;
}

float findCollisionTime(struct state a, struct state b, float t1)
{
    float t;
    float r;
    struct point p, v;
    r = a.radius + b.radius;
    a.pos.x = a.pos.x + a.vel.x*(t1 - a.t);
    a.pos.y = a.pos.y + a.vel.y*(t1 - a.t);
    b.pos.x = b.pos.x + b.vel.x*(t1 - b.t);
    b.pos.y = b.pos.y + b.vel.y*(t1 - b.t);
    p.x = a.pos.x - b.pos.x;
    p.y = a.pos.y - b.pos.y;
    v.x = a.vel.x - b.vel.x;
    v.y = a.vel.y - b.vel.y;
    if(modSquare(v) == 0)
		return -1;
    float det = dot(p,v)*dot(p,v) - modSquare(v)*modSquare(p) + r*r*modSquare(v);
    if(det < 0)
		return -1;
    det = sqrt(det);
    t = (-1*dot(p,v) - det)/modSquare(v);
    if(t<0)
		return -1;
    else 
		return (t + t1);
}

float findWallCollisionTime(struct state a, float dim[], float t, int *w)
{
    float t1, t2;
    a.pos.x = a.pos.x + a.vel.x*(t - a.t);
    a.pos.y = a.pos.y + a.vel.y*(t - a.t);
    if(a.vel.x == 0)
		t1 = FLT_MAX;
    else if(a.vel.x < 0)
		t1 = -1*(a.pos.x - a.radius)/a.vel.x;
    else
		t1 = (dim[0] - a.pos.x - a.radius)/a.vel.x;
    if(a.vel.y == 0)
		t2 = FLT_MAX;
    else if(a.vel.y < 0)
		t2 = -1*(a.pos.y - a.radius)/a.vel.y;
    else
		t2 = (dim[1] - a.pos.y - a.radius)/a.vel.y;
    if(t1 < t2)
    {
		*w = 0;
		return (t1 + t);
    }
    else
    {
		*w = 1;
		return (t2 + t);
    }
}

void updateState(struct state *a, struct state *b, float t)
{
    (*a).pos.x = (*a).pos.x + (*a).vel.x*(t - (*a).t);
    (*a).pos.y = (*a).pos.y + (*a).vel.y*(t - (*a).t);
    (*b).pos.x = (*b).pos.x + (*b).vel.x*(t - (*b).t);
    (*b).pos.y = (*b).pos.y + (*b).vel.y*(t - (*b).t);
    struct point tangent;
    if(modSquare((*a).vel) == 0)
    {
		(*a).vel = (*b).vel;
		(*b).vel.x = 0;
		(*b).vel.y = 0;
		return;
    }
    if(modSquare((*b).vel) == 0)
    {
		(*b).vel = (*a).vel;
		(*a).vel.x = 0;
		(*a).vel.y = 0;
		return;
    }
    tangent.x = ((*a).vel.x/sqrt(modSquare((*a).vel)) + (*b).vel.x/sqrt(modSquare((*b).vel)));
    tangent.y = ((*a).vel.y/sqrt(modSquare((*a).vel)) + (*b).vel.y/sqrt(modSquare((*b).vel)));
    if(modSquare(tangent) == 0)
    {
		(*a).vel.x = -1*(*a).vel.x;
		(*a).vel.y = -1*(*a).vel.y;
		(*b).vel.x = -1*(*b).vel.x;
		(*b).vel.y = -1*(*b).vel.y;
	return;
    }
    tangent.x /= sqrt(modSquare(tangent));
    tangent.y /= sqrt(modSquare(tangent));
    struct point vt1, vt2, vn1, vn2;
    vt1.x = (dot((*a).vel, tangent))*tangent.x;
    vt1.y = (dot((*a).vel, tangent))*tangent.y;
    vt2.x = (dot((*b).vel, tangent))*tangent.x;
    vt2.y = (dot((*b).vel, tangent))*tangent.y;
    vn1.x = (*a).vel.x - vt1.x;
    vn1.y = (*a).vel.y - vt1.y;
    vn2.x = (*b).vel.x - vt2.x;
    vn2.y = (*b).vel.y - vt2.y;
    (*a).vel.x = vt1.x + vn2.x;
    (*a).vel.y = vt1.y + vn2.y;
    (*b).vel.x = vt2.x + vn1.x;
    (*b).vel.y = vt2.y + vn1.y;
    return;
}

void updateWallState(struct state *a, int w, float t)
{
    (*a).pos.x = (*a).pos.x + (*a).vel.x*(t - (*a).t);
    (*a).pos.y = (*a).pos.y + (*a).vel.y*(t - (*a).t);
    if(w==0)
		(*a).vel.x = -1*((*a).vel.x);
    else
		(*a).vel.y = -1*((*a).vel.y);
    return;
}

void process(struct state S[5], struct event *E[], float dim[], int N, float t, float T, FILE *fptr, FILE *fptrs[])
{
	int i;
    int k, w;
    float t1, tf;
    struct point c1;
    struct event *min = extractMin(E, &N);
    struct node *l = S[min->i].list;
    if(min->i == -1)
		return;
    t = min->time;
    printf("\nT = %f", t);
    if(t >=T)
		return;
    if(min->status == 1)
    {
	 process(S, E, dim, N, t, T, fptr, fptrs);
	 return;
    }
    if(min->type == 0)
    {
    	for (i = 0; i < 10; ++i)
		{
			tf = S[min->i].t + i*(t - S[min->i].t)/10;
			c1.x = S[min->i].pos.x + S[min->i].vel.x*(tf - S[min->i].t);
			c1.y = S[min->i].pos.y + S[min->i].vel.y*(tf - S[min->i].t);
			fprintf(fptrs[S[min->i].color], "\n%f %f %f", tf, c1.x, c1.y);

		}
		for (i = 0; i < 10; ++i)
		{
			tf = S[min->j].t + i*(t - S[min->j].t)/10;
			c1.x = S[min->j].pos.x + S[min->j].vel.x*(tf - S[min->j].t);
			c1.y = S[min->j].pos.y + S[min->j].vel.y*(tf - S[min->j].t);
			fprintf(fptrs[S[min->j].color], "\n%f %f %f", tf, c1.x, c1.y);

		}
		updateState(&S[min->i], &S[min->j], t);
		S[min->i].t = S[min->j].t = t;
		fprintf(fptr, "\n\nAt time : %f", t);
		fprintf(fptr, "\nCollision between two particles.");
		fprintf(fptr, "\nColor of particle 1 : %d", S[min->i].color);
		fprintf(fptr, "\nPosition of particle 1 : (%f,%f)", S[min->i].pos.x, S[min->i].pos.y);
		fprintf(fptr, "\nVelocity of particle 1 : (%f,%f)", S[min->i].vel.x, S[min->i].vel.y);
		fprintf(fptr, "\nColor of particle 2 : %d", S[min->j].color);
		fprintf(fptr, "\nPosition of particle 2 : (%f,%f)", S[min->j].pos.x, S[min->j].pos.y);
		fprintf(fptr, "\nVelocity of particle 2 : (%f,%f)", S[min->j].vel.x, S[min->j].vel.y);
		printf("\n\n%d = (%f, %f) at %f vel : (%f, %f) ", S[min->i].color, S[min->i].pos.x, S[min->i].pos.y, t, S[min->i].vel.x, S[min->i].vel.y);
		printf("\n%d = (%f, %f) at %f vel : (%f, %f) ", S[min->j].color, S[min->j].pos.x, S[min->j].pos.y, t, S[min->j].vel.x, S[min->j].vel.y);
		struct node *l1 = S[min->i].list;
		while(l1 != NULL)
		{
		    l1->e->status = 1;
		    l1 = l1->next;
		}
		S[min->i].list = NULL;
		l1 = S[min->j].list;
		while(l1 != NULL)
		{	
		    l1->e->status = 1;
		    l1 = l1->next;
		}
		S[min->j].list = NULL;
		struct event *e;
		for(k =0; k<5; k++)
		{
		    if(k != min->i && k != min->j)
		    {
			t1 = findCollisionTime(S[min->i], S[k], t);
			if(t1 >= 0)
			{
				e = (struct event *)malloc(sizeof(struct event));
			 e->i = min->i;
			 e->j = k;
			 e->status = 0;
			 e->time = t1;
			 e->type = 0;
			 e = heapInsert(E, &N, e);
			 S[min->i].list = listInsert(S[min->i].list, e);
			 S[k].list = listInsert(S[k].list, e);
			}
			t1 = findCollisionTime(S[min->j], S[k], t);
			if(t1 >= 0)
			{
				e = (struct event *)malloc(sizeof(struct event));
			 e->i = min->j;
			 e->j = k;
			 e->status = 0;
			 e->time = t1;
			 e->type = 0;
			 e = heapInsert(E, &N, e);
			 S[min->j].list = listInsert(S[min->j].list, e);
			 S[k].list = listInsert(S[k].list, e);
			}
		    }
		}
		t1 = findWallCollisionTime(S[min->i], dim, t, &w);
		e = (struct event *)malloc(sizeof(struct event));
		e->i = min->i;
		e->status = 0;
		e->time = t1;
		if(w == 0)
		    e->type = 1;
		else
		    e->type = 2;
		e = heapInsert(E, &N, e);
		S[min->i].list = listInsert(S[min->i].list, e);
		t1 = findWallCollisionTime(S[min->j], dim, t, &w);
		e = (struct event *)malloc(sizeof(struct event));
		e->i = min->j;
		e->status = 0;
		e->time = t1;
		if(w == 0)
		    e->type = 1;
		else
		    e->type = 2;
		e = heapInsert(E, &N, e);
		S[min->j].list = listInsert(S[min->j].list, e);
    	}
    	else
    	{
    		for (i = 0; i < 10; ++i)
			{
				tf = S[min->i].t + i*(t - S[min->i].t)/10;
				c1.x = S[min->i].pos.x + S[min->i].vel.x*(tf - S[min->i].t);
				c1.y = S[min->i].pos.y + S[min->i].vel.y*(tf - S[min->i].t);
				fprintf(fptrs[S[min->i].color], "\n%f %f %f", tf, c1.x, c1.y);

			}
		if(min->type == 1)
		    updateWallState(&S[min->i], 0, t);
		else
		    updateWallState(&S[min->i], 1, t);
		S[min->i].t = t;
		fprintf(fptr, "\n\nAt time : %f", t);
		fprintf(fptr, "\nCollision with wall.");
		fprintf(fptr, "\nColor of particle  : %d", S[min->i].color);
		fprintf(fptr, "\nPosition of particle : (%f,%f)", S[min->i].pos.x, S[min->i].pos.y);
		fprintf(fptr, "\nVelocity of particle : (%f,%f)", S[min->i].vel.x, S[min->i].vel.y);
		printf("\n\n%d = (%f, %f) at %f vel : (%f, %f) ", S[min->i].color, S[min->i].pos.x, S[min->i].pos.y, t, S[min->i].vel.x, S[min->i].vel.y);
		struct node *l1 = S[min->i].list;
		while(l1 != NULL)
		{
		    l1->e->status = 1;
		    l1 = l1->next;
		}
		S[min->i].list = NULL;
		struct event *e;
		for(k =0; k<5; k++)
		{
		    if(k != min->i)
		    {
				t1 = findCollisionTime(S[min->i], S[k], t);
				if(t1 >= 0)
				{
					e = (struct event *)malloc(sizeof(struct event));
				 e->i = min->i;
				 e->j = k;
				 e->status = 0;	
				 e->time = t1;
				 e->type = 0;
				 e = heapInsert(E, &N, e);
				 S[min->i].list = listInsert(S[min->i].list, e);
				 S[k].list = listInsert(S[k].list, e);
				}
		    }
		}
		t1 = findWallCollisionTime(S[min->i], dim, t, &w);
		e = (struct event *)malloc(sizeof(struct event));
		e->i = min->i;
		e->status = 0;
		e->time = t1;
		if(w == 0)
		    e->type = 1;
		else
		    e->type = 2;
		e = heapInsert(E, &N, e);
		S[min->i].list = listInsert(S[min->i].list, e);
   	 }
	process(S, E, dim, N, t, T, fptr, fptrs);
   	return;
}

int main()
{
    srand(time(NULL));
    struct state S[5];
    struct event *E[1000];
    float dim[2];
    int N = 0, i, j;
    FILE *fptr, *fptrs[5];
	fptr = fopen("Heap.txt", "w");
	if(fptr == NULL)
	{
		printf("\nError in opening file!");
		exit(1);
	}
	fptrs[0] = fopen("ball[1].txt", "w");
	fptrs[1] = fopen("ball[2].txt", "w");
	fptrs[2] = fopen("ball[3].txt", "w");
	fptrs[3] = fopen("ball[4].txt", "w");
	fptrs[4] = fopen("ball[5].txt", "w");
    for (i = 0; i < 1000; ++i)
    {
    	E[i] = (struct event *)malloc(sizeof(struct event));
    }
    float T = 0.0;
    printf("\nEnter maximum time : ");
    scanf("%f", &T);
    printf("\nEnter dimensions (length, breadth) :");
    scanf("%f %f", &dim[0], &dim[1]);
    for (i = 0; i < 5; ++i)
    {
		S[i].pos.x = dim[0]/(i+1.5);
		S[i].pos.y = dim[1]/2;
		S[i].vel.x = rand()%3 + 1;
		S[i].vel.y = rand()%3 + 1;
		S[i].t = 0;
		S[i].color = i;
		S[i].list = NULL;
		S[i].radius = rand()%5 + 1;
    }
    int c = 0, w;
    for (i = 0; i < 5; ++i)
    {
	for (j = 0; j < i; ++j)
	{
	       E[c]->time = findCollisionTime(S[i], S[j], 0);
	       if(E[c]->time < 0)
		    continue;
	       E[c]->i = i;
	       E[c]->j = j;
	       E[c]->status = 0;
	       E[c]->type = 0;
	       S[i].list = listInsert(S[i].list, E[c]);
	       S[j].list = listInsert(S[j].list, E[c]);
	       c++;
	}
	E[c]->i = i;
	E[c]->status = 0;
	E[c]->time = findWallCollisionTime(S[i], dim, 0, &w);
	if(E[c]->time < 0)
	    continue;
	if(w == 0)
	    E[c]->type = 1;
	else
	    E[c]->type = 2;
	S[i].list = listInsert(S[i].list, E[c]);
	c++;
    }
    N = c;
    buildHeap(E, N);
    printf("\nRadius : ");
    for (i = 0; i < 5; ++i)
    {
    	printf("\n%d ", S[i].radius);
    }
    process(S, E, dim, N, 0, T, fptr, fptrs);
    for (i = 0; i < 5; ++i)
    {
  		S[i].pos.x = S[i].pos.x + S[i].vel.x*(100 - S[i].t);
		S[i].pos.y = S[i].pos.y + S[i].vel.y*(100 - S[i].t);
		fprintf(fptrs[S[i].color], "\n100 %f %f", S[i].pos.x, S[i].pos.y);
    }
	fclose(fptr);
	for (i = 0; i < 5; ++i)
	{
		fclose(fptrs[i]);
	}
	return;
}