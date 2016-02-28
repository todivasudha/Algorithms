#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define N 10000
#define M 100

struct node
{
	int a;
	struct node *left;
	struct node *right;
};

struct node *createTree(int n, int *used, int *i)
{
  if(n==0)
  	return NULL;
  struct node *root;
  root = (struct node *)malloc(sizeof(struct node));
  root->a = rand()%(*i+1) - *i/2;
  if(used[root->a + *i/2])
  	root->a = *i - *i/2;
  used[root->a + *i/2] = 1;
  (*i)++;
  root->left = NULL;
  root->right = NULL;
  if(n==1)
  	return root;
  int d = rand()%21 + 40;
  d = n*d/100;
  root->left = createTree(d, used, i);
  root->right = createTree(n-d-1, used, i);
  return root;
}

void inorderTraversal(struct node *root)
{	if(root->left != NULL)
		inorderTraversal(root->left);
	printf("%d ", root->a);
	if(root->right != NULL)
		inorderTraversal(root->right);
}

void preorderTraversal(struct node *root)
{	
	printf("%d ", root->a);
	if(root->left != NULL)
		preorderTraversal(root->left);
	if(root->right != NULL)
		preorderTraversal(root->right);
}

void postorderTraversal(struct node *root)
{	if(root->left != NULL)
		postorderTraversal(root->left);
	if(root->right != NULL)
		postorderTraversal(root->right);
	printf("%d ", root->a);
}

int maxRootSumPath(struct node *root, int *path, int *c)
{
	if(root == NULL)
	{
		*c = 0;
		return 0;
	}
	int l, r, cl, cr, k;
	int left[M], right[M];
	l = maxRootSumPath(root->left, left, &cl);
	r = maxRootSumPath(root->right, right, &cr);
	if(l>r)
	{
		for (k = 0; k <= cl; ++k)
		{
			path[k] = left[k];
		}
		path[cl + 1] = root->a;
		*c = cl + 1;
		return (l + root->a);
	}
	else
	{
		for (k = 0; k <= cr; ++k)
		{
			path[k] = right[k];
		}
		path[cr + 1] = root->a;
		*c = cr + 1;
		return (r + root->a);
	}
}

int maxSumPath(struct node *root, int *max_path, int *side_path, int *c, int *max, int *cm)
{
	if(root == NULL)
		{
			*c = 0;
			return 0;
		}
	int l, r, cl, cr, side_max, left[M], right[M], k;
	l = maxSumPath(root->left, max_path, left, &cl, max, cm);
	r = maxSumPath(root->right, max_path, right, &cr, max, cm);
	if(l>r)
	{
		if(l>0)
		{
			side_max = l + root->a;
			for (k = 0; k <= cl; ++k)
			{
				side_path[k] = left[k];
			}
			side_path[cl + 1] = root->a;
			*c = cl + 1;
		}
		else
		{
			side_max = root->a;
			side_path[1] = root->a;
			*c = 1;
		}
	}
	else
	{
		if(r>0)
		{
			side_max = r + root->a;
			for (k = 0; k <= cr; ++k)
			{
				side_path[k] = right[k];
			}
			side_path[cr + 1] = root->a;
			*c = cr + 1;
		}
		else
		{
			side_max = root->a;
			side_path[1] = root->a;
			*c = 1;
		}
	}
	if(*max < (l + r + root->a) || *max < side_max)
	{
		if((l + r + root->a) > side_max)
		{
			*max = l + r +root->a;
			for (k = 1; k <= cl; ++k)
				max_path[k] = left[k];
			max_path[cl+1] = root->a;
			for (k = 1; k <= cr; ++k)
				max_path[k + cl + 1] = right[cr - k + 1];
			*cm = cl + cr + 1;
		}
		else
		{
			*max = side_max;
			for (k = 1; k <= *c ; ++k)
			{
				max_path[k] = side_path[k];
			}
			*cm = *c;
		}
	}
	return side_max;
}

int main()
{
	srand(time(NULL));
	struct node *tree;
	int used[N] = {0};
	int i = N - M;
	int path[M], max_path[M], side_path[M], max, j, c, cm;
	tree = createTree(M, used, &i);
	inorderTraversal(tree);
	printf("\n\n");
	preorderTraversal(tree);
	printf("\n\n");
	postorderTraversal(tree);
	printf("\n\n");
	max = maxRootSumPath(tree, path, &c);
	for (j = c; j >= 1; --j)
	{
		printf("%d ", path[j]);
	}
	printf("\n\n");
	int m = INT_MIN;
	max = maxSumPath(tree, max_path, side_path, &c, &m, &cm);
	for (j = 1; j <= cm ; ++j)
	{
		printf("%d ", max_path[j]);
	}
	printf("\n\n");
	return 0;
}