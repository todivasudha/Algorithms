#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node
{
	int t;
	int priority;
	int color;
	int l[20], u[20];
	int create, end, num;
	struct node *left, *right, *parent, *next;
};

struct node *createNode(int t, int p, int k,int c)
 {
 	struct node *n = (struct node *)malloc(sizeof(struct node));
 	int i;
 	for (i = 0; i <20; ++i)
 	{
 		n->l[i] = n->u[i] = -1;
  	}
 	n->t = t;
 	n->priority = p;
 	n->left = NULL;
 	n->right = NULL;
 	n->parent = NULL;
 	n->next = NULL;
 	n->color = 1;
 	n->create = c;
 	n->num = k;
 }
 
struct node *leftRotate(struct node *tree, struct node *x)
{
	 struct node *y = x->right;
	 x->right = y->left;
	 if(x->right != NULL)
	 	x->right->parent = x;
	 y->left = x;
	 y->parent = x->parent;
	 if(y->parent == NULL)
	 	tree = y;
	 else if(x == x->parent->left)
	 	x->parent->left = y;
	 else
	 	x->parent->right = y;
	 x->parent = y;
	 return tree;
}

struct node *rightRotate(struct node *tree, struct node *x)
{
	 struct node *y = x->left;
	 x->left = y->right;
	 if(x->left != NULL)
	 	x->left->parent = x;
	 y->right = x;
	 y->parent = x->parent;
	 if(y->parent == NULL)
	 	tree = y;
	 else if(x == x->parent->left)
	 	x->parent->left = y;
	 else
	 	x->parent->right = y;
	 x->parent = y;
	 return tree;
}

void pretty_print(struct node *tree, int k)
{
	int j;
    if(tree->left != NULL)
        pretty_print(tree->left, k+1);
    printf("\n");
    for(j=0; j< k; j++)
    {
            printf("\t");
    }
    printf("%d %d", tree->t, tree->color);
    if(tree->right != NULL)
        pretty_print(tree->right, k+1);
    return;
}

int findSame(struct node *root, struct node *n)
{
	if(root == NULL)
		return 0;
	if(root->t == n->t)
	{
		n->color = root->color;
		struct node *q = root;
		while(q->next != NULL)
			q = q->next;
		q->next = n;
		return 1;
	}
	if(n->t < root->t)
		return findSame(root->left, n);
	else
		return findSame(root->right, n);
}

struct node *bstInsert(struct node *root, struct node *n)
{
	if(root == NULL)
	{	root = n;
		n->color = 0;
		return root;
	}
	if(n->t < root->t)
	{
		if(root->left == NULL)
		{
			root->left = n;
			n->parent = root;
			return root;
		}
		root->left = bstInsert(root->left, n);
	}
	else
	{
		if(root->right == NULL)
		{
			root->right = n;
			n->parent = root;
			return root;
		}
		root->right = bstInsert(root->right, n);
	}
	return root;
}

struct node *rbInsert(struct node *root, struct node *n)
{
	root = bstInsert(root, n);
	root->color = 0;
	struct node *x = n;
	struct node *p = n->parent;
	while(x != root && p->color == 1)
	{
		if((p->parent->left != NULL && p->parent->right != NULL) && (p->parent->left->color == 1) && (p->parent->right->color == 1))
		{
			p->parent->left->color = p->parent->right->color = 0;
			p->parent->color = 1;
			x = p->parent;
		}
		else
		{	
			if(p == p->parent->left)
			{	
				if(x == p->right)
				{
					root = leftRotate(root, p);
					root = rightRotate(root, x->parent);
					x->color = 0;
					x->right->color = 1;
				}
				else
				{
					root = rightRotate(root, p->parent);
					p->color = 0;
					p->right->color = 1;
				}
				root->color = 0;
				return root;
				
			}
			else
			{
				if(x == p->left)	
				{
					root = rightRotate(root, p);
					root = leftRotate(root, x->parent);
					x->color = 0;
					x->left->color = 1;
				}
				else
				{
					root = leftRotate(root, p->parent);
					p->color = 0;
					p->left->color = 1;
				}
				root->color = 0;
				return root;
			}	
		}
	}
	root->color = 0;
	return root;
}

struct node *rbSearch(struct node *root, int t)
{
	if(root == NULL)
		return NULL;
	if(t == root->t)
		return root;
	if(t < root->t)
		return rbSearch(root->left, t);
	else
		return rbSearch(root->right, t);

}

struct node *findMin(struct node *root)
{
	if(root->left == NULL)
		return root;
	else
		return findMin(root->left);
}

struct node *rbDelete(struct node *root, struct node *n)
{
	
	if(n->left != NULL && n->left->color == 1)
	{
		n->left->color = 0;
		n->left->right = n->right;
		if(n->right != NULL)
			n->right->parent = n->left;
		n->left->parent = n->parent;
		if(n == root)
			root = n->left;
		else if(n == n->parent->left)
			n->parent->left = n->left;
		else
			n->parent->right = n->left;
		root->parent = NULL;
	}
	else if(n->right != NULL)
	{
		n->right->color = 0;
		n->right->parent = n->parent;
		if(n == root)
			root = n->right;
		else if(n == n->parent->left)
			n->parent->left = n->right;
		else
			n->parent->right = n->right;
		root->parent = NULL;
	}
	else if(n == root)
		root = NULL;
	else if(n->color == 1)
	{
		if(n == n->parent->left)
			n->parent->left = NULL;
		else 
			n->parent->right = NULL;
	}
	else
	{
		struct node *x = n;
		struct node *s, *r;
		while(x != root && x != NULL)
		{
			if(x != NULL && x == x->parent->left)
			{
				s = x->parent->right;
				if(s != NULL && s->color == 0 && s->right != NULL && s->right->color == 1)
				{
					r = s->right;
					root = leftRotate(root, x->parent);
					r->color = 0;
					if(x->parent->color == 1)
					{
						x->parent->color = 0;
						s->color = 1;
					}
					x = NULL;
				}
				else if(s != NULL && s->color == 0 && s->left != NULL && s->left->color == 1)
				{
					r = s->left;
					root = rightRotate(root, s);
					r->color = 0;
					root = leftRotate(root, x->parent);
					if(x->parent->color == 1)
					{
						x->parent->color = 0;
						s->color = 1;
					}
					x = NULL;
				}
				else if(s == NULL || s->color == 0)
				{
					if(s != NULL)
						s->color = 1;
					if(x->parent->color == 0)
						x = x->parent;
					else
					{
						x->parent->color = 0;
						x = NULL;
					}
				}
				else
				{
					root = leftRotate(root, x->parent);
					s->color = 0;
					if(s->left->right != NULL)
						s->left->right->color = 1;
					x = NULL;
				}
			}
			else if(x != NULL && x == x->parent->right)
			{
				s = x->parent->left;
				if(s != NULL && s->color == 0 && s->left != NULL && s->left->color == 1)
				{
					r = s->left;
					root = rightRotate(root, x->parent);
					r->color = 0;
					if(x->parent->color == 1)
					{
						x->parent->color = 0;
						s->color = 1;
					}
					x = NULL;
				}
				else if(s != NULL && s->color == 0 && s->right != NULL && s->right->color == 1)
				{
					r = s->right;
					root = leftRotate(root, s);
					r->color = 0;
					root = rightRotate(root, x->parent);
					if(x->parent->color == 1)
					{
						x->parent->color = 0;
						s->color = 1;
					}
					x = NULL;
				}
				else if(s == NULL || s->color == 0)
				{
					if(s != NULL)
						s->color = 1;
					if(x->parent->color == 0)
						x = x->parent;
					else
					{
						x->parent->color = 0;
						x = NULL;
					}
				}
				else
				{
					root = rightRotate(root, x->parent);
					s->color = 0;
					if(s->right->left != NULL)
						s->right->left->color = 1;
					x = NULL;
				}
			}
		}
		if(n == n->parent->left)
			n->parent->left = NULL;
		else 
			n->parent->right = NULL;
	}
	n->parent = NULL;
	free(n);
	return root;
}

struct node *processCreator(struct node *S, int T, int count)
{
	int t, p, flag = 0;
	t = rand()%1000 + 1;
	p = rand()%4 + 1;
	struct node *n = createNode(t, p, count, T);
	flag = findSame(S, n);
	if(!flag)
		S = rbInsert(S, n);
	return S;
}

void scheduler(int N, int M)
{
	int count = 0;
	int c=0;
	long int T = 0;
	int p, t, i, k=0;
	int no = 0;
	FILE *fptr;
	fptr = fopen("Red_Black_Tree.txt", "w");
	if(fptr == NULL)
	{
		printf("\nError in opening file!");
		exit(1);
	}
	fprintf(fptr, "\n%-14s\t%-14s\t%-8s\t%-30s\t\t%-30s\t\t%-15s", "Process number", "Creation time", "Priority", "Scheduled time stamps", "Preemption time stamps", "Completion time" );
	struct node *S;
	S = NULL;
	while(!(count == M && S == NULL))
	{
		k = 0;
		if((no < N) && (count < M))
		{
			no++;
			count++;
			S = processCreator(S, T, count);
		}
		struct node *process = findMin(S);
		while(process->l[k] != -1)
			k++;
		process->l[k] = T;
		if(process->t <= process->priority*50)
		{
			T += process->t;
			no--;
			process->end = T;
			process->u[k] = T;
			fprintf(fptr, "\n\n");
			for (i = 0; i < 140; ++i)
			{
				fprintf(fptr, "%s", "-");
			}
			c++;
			fprintf(fptr, "\n%-14d\t%-14d\t%-8d\t", c, process->create, process->priority);
			for (i = 0; i < 5; ++i)
			{
				if(k >= i)
					fprintf(fptr, "%-5d,", process->l[i]);
				else
					fprintf(fptr, "%-6s", " ");
			}
			fprintf(fptr, "\t\t");
			for (i = 0; i < 5; ++i)
			{
				if(k >= i)
					fprintf(fptr, "%-5d,", process->u[i]);
				else
					fprintf(fptr, "%-6s", " ");
			}
			fprintf(fptr, "\t\t%-15d", process->end);
			int j;
			while(k >= i)
			{
				fprintf(fptr, "\n%-14s\t%-14s\t%-8s\t", " ", " ", " ");
				for (j = i; j < i+5; ++j)
				{
					if(k >= j)
						fprintf(fptr, "%-5d,", process->l[j]);
					else
						fprintf(fptr, "%-6s", " ");
				}
				fprintf(fptr, "\t\t");
				for (j = i; j < i+5; ++j)
				{
					if(k >= j)
						fprintf(fptr, "%-5d,", process->u[j]);
					else
						fprintf(fptr, "%-6s", " ");
				}
				fprintf(fptr, "\t\t%-15s", " ");
				i +=5;
			}
			if(process->next != NULL)
			{
				process->next->left = process->left;
				process->next->right = process->right;
				process->next->parent = process->parent;
				if(S == process)
					S = process->next;
				else if(process == process->parent->left)
					process->parent->left = process->next;
				else
					process->parent->right = process->next;
				if(process->left !=NULL)
					process->left->parent = process->next;
				if(process->right != NULL)
					process->right->parent = process->next;
				free(process);
				
			}
			else
				S = rbDelete(S, process);
		}
		else
		{
			struct node *temp = (struct node *)malloc(sizeof(struct node));
			*temp = *process;
			temp->left = temp->right = temp->parent = temp->next = NULL;
 			if(process->next != NULL)
			{
				process->next->left = process->left;
				process->next->right = process->right;
				process->next->parent = process->parent;
				if(S == process)
					S = process->next;
				else if(process == process->parent->left)
					process->parent->left = process->next;
				else
					process->parent->right = process->next;
				if(process->left !=NULL)
					process->left->parent = process->next;
				if(process->right != NULL)
					process->right->parent = process->next;
				free(process);		
			}
			else
				S = rbDelete(S, process);
			temp->t -= temp->priority*50;
			T += temp->priority*50;
			temp->u[k] = T;
			S = rbInsert(S, temp);
		}
	}
	fclose(fptr);
}


int main()
{
	int N, M;
	srand(time(NULL));
	printf("\nEnter the maximum number of processes in the scheduler tree (N) : ");
	scanf("%d", &N);
	printf("\nEnter the total number of processes (M) :");
	scanf("%d", &M);
	scheduler(N, M);
	return;
}