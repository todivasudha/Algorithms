#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct list{
    int a;
    struct list *next;
};

struct node{
    int l;
    int u;
    struct list *p;
    struct node *left;
    struct node *right;
};

struct list *list_insert(struct list *head, int a)
{
    struct list *l = (struct list *)malloc(sizeof(struct list));
    struct list *prev, *p = head;
    l->a = a;
    l->next = NULL;
    if(p == NULL)
    {
        p = l;
        return p;
    }
    if(p->a >= a)
    {
        l->next = p;
        head = l;
        while(p != NULL)
        {
            p = p->next;
        }
        return head;
    }
    while(p != NULL && p->a < a)
    {
        prev = p;
        p = p->next;
    }
    prev->next = l;
    l->next = p;
    p = head;
    while(p != NULL)
    {
        p = p->next;
    }
    return head;
}

struct list *insert_elements(struct node *n, struct list *l)
{
    struct list *l1 = n->p;
    while(l1 != NULL)
    {
        l = list_insert(l, l1->a);
        l1 = l1->next;    
    }
    if(n->left !=NULL)
        l = insert_elements(n->left, l);
    if(n->right != NULL)
        l = insert_elements(n->right, l);
    return l;
}

int get_height(struct node *tree)
{
    if(tree == NULL)
        return 0;
    int h1=0,h2=0;
    if(tree->left != NULL);
        h1 = get_height(tree->left);
    if(tree->right != NULL)
        h2 = get_height(tree->right);
    if(h1 > h2)
        return (h1+1);
    else
        return (h2+1);
}

struct node *create_interval_tree(int l, int u, int n)
{
    if(l>=u)
        return NULL;
    struct node *root;
    root = (struct node *)malloc(sizeof(struct node));
    root->p = NULL;
    int step = (u-l+1)/n;
    root->l = l + step*((n-1)/2 + (n-1)%2);
    root->u = root->l + step - 1;
    root->left = create_interval_tree(l, root->l-1, n/2);
    if(n%2==1)
        root->right = create_interval_tree(root->u+1, u, n/2);
    else
        root->right = create_interval_tree(root->u+1, u, n/2-1);
    return root;
}

void insert(struct node *tree, int i)
{
    struct list *q, *r, *s;
    q = (struct list *)malloc(sizeof(struct list));
    q->a = i;
    q->next = NULL;
    s = r = tree->p;
    if(i >= tree->l && i<= tree->u)
    {
        if(s == NULL)
        {
            tree->p = q;
            return;
        }
        if(s->a >= i)
        {
            q->next = tree->p;
            tree->p = q;
            return;
        }
        while(s!=NULL && s->a < i)
        {
            r = s;
            s = s->next;
        }
        r->next = q;
        q->next = s;
        return;
    }
    if(i < tree->l)
        insert(tree->left, i);
    else
        insert(tree->right, i);
}

struct node *merge(struct node *T, int l, int u)
{
    struct node *N;
    N = (struct node *)malloc(sizeof(struct node));
    N->p = NULL;
    N->l = l;
    N->u = u;
    N->left = N->right = NULL;
    struct list *p = N->p;
    struct node *b, *c, *prev, *prev0;
    prev0 = b = T;
    while(b!=NULL)
    {
        if((l < b->l) && (u >= b->l))
        {
            prev = b;
            c = b->left;
            while(c != NULL)
            {
                if(l > c->u)
                    {
                        prev = c;
                        c = c->right;
                    }
                else if(l < c->l)
                {   
                    struct list *temp = c->p;
                    struct node *del = c;
                    while(temp != NULL)
                    {
                        p = list_insert(p, temp->a);
                        temp = temp->next;
                    }
                    if(c->right != NULL)
                        p = insert_elements(c->right, p);
                    c = c->left;
                    if(del == prev->left)
                        prev->left = c;
                    else
                        prev->right = c;
                    free(del);
                }
                else
                {
                    if(c->right != NULL)
                        p = insert_elements(c->right, p);
                    c->right = NULL;
                    struct list *temp = c->p;
                    struct list *prev1 = c->p;
                    while(temp != NULL && temp->a < l)
                    {
                        prev1 = temp;
                        temp = temp->next;
                    }
                    if(prev1 == c->p)
                        c->p = NULL;
                    else if(prev1 != NULL)
                        prev1->next = NULL;
                    while(temp != NULL)
                    {
                        p = list_insert(p, temp->a);
                        temp = temp->next;
                    }
                    if(c->l == l)
                    {
                        struct node *del = c;
                        c = c->left;
                        if(del != NULL && del == prev->left)
                            prev->left = c;
                        else if(del != NULL && del == prev->right)
                            prev->right = c;
                        free(del);
                    }
                    else
                    {
                        c->u = l-1;
                    }
                    N->left = b->left;
                    break;
                }
            }
            N->left = b->left;
        }
        if((u > b->u) && (l <= b->u))
        {
            prev = b;
            c = b->right;
            while(c != NULL)
            {
                if(u < c->l)
                    {
                        prev = c;
                        c = c->left;
                    }
                else if(u > c->u)
                {
                    struct list *temp = c->p;
                    struct node *del = c;
                    while(temp != NULL)
                    {
                        p = list_insert(p, temp->a);
                        temp = temp->next;
                    }
                    if(c->left != NULL)
                        p = insert_elements(c->left, p);
                    c = c->right;
                    if(del == prev->left)
                        prev->left = c;
                    else
                        prev->right = c;
                    free(del);
                }
                else
                {
                    if(c->left != NULL)
                        p = insert_elements(c->left, p);
                    c->left = NULL;
                    struct list *temp = c->p;
                    struct list *prev1 = temp;
                    while(temp != NULL && temp->a <= u)
                    {
                        p = list_insert(p, temp->a);
                        prev1 = temp;
                        temp = temp->next;
                    }
                    c->p = temp;
                    if(c->u == u)
                    {
                        struct node *del = c;
                        c = c->right;
                        if(del != NULL && del == prev->left)
                            prev->left = c;
                        else if(del != NULL && del == prev->right)
                            prev->right = c;
                        free(del);
                    }
                    else
                    {
                        c->l = u+1;
                    }
                    N->right = b->right;
                    break;
                }
            }
            N->right = b->right;
        }
        struct list *temp = b->p;
        if(l >= b->l && l <= b->u)
        {
            if(l > b->l)
            {
                N->left = (struct node *)malloc(sizeof(struct node));
                N->left->p = NULL;
                N->left->l = b->l;
                N->left->u = l-1;
                while(temp != NULL && temp->a < l)
                {
                    N->left->p = list_insert(N->left->p, temp->a);
                    temp = temp->next;
                }
                N->left->left = b->left;
                N->left->right = NULL;  
            }
            else
                N->left = b->left;
        }
        while(temp !=NULL && temp->a >= l && temp->a <= u)
        {
            p = list_insert(p, temp->a);
            temp = temp->next;
        }
        if(u >= b->l && u <= b->u)
        {
            if(u < b->u)
            {
                N->right = (struct node *)malloc(sizeof(struct node));
                N->right->p = NULL;
                N->right->u = b->u;
                N->right->l = u+1;
                while(temp != NULL)
                {
                    N->right->p = list_insert(N->right->p, temp->a);
                    temp = temp->next;
                }
                N->right->right = b->right;
                N->right->left = NULL;  
            }
            else
                N->right = b->right;
        }
        if(u < b->l)
        {
            prev0 = b;
            b = b->left;
        }
        else if(l > b->u)
        {
            prev0 = b;
            b = b->right;
        }
        else
        {
            if(T == b)
            {
                T = N;
                break;
            }
            else if(b == prev0->left)
                prev0->left = N;
            else if(b == prev0->right)
                prev0->right = N;
            free(b);
            break;
        }
    }
    N->p = p;
    return T;
}

void pretty_print(struct node *tree, int k)
{
    int j;
    if(tree->left != NULL)
        pretty_print(tree->left, k-1);
    printf("\n");
    for(j=0; j< k-1; j++)
    {
            printf("\t");
    }
    printf("[%d,%d] ", tree->l, tree->u);
    struct list *q;
    q = tree->p;
    while(q!=NULL)
    {
        printf("%d, ", q->a);
        q = q->next;
    }
    if(tree->right != NULL)
        pretty_print(tree->right, k-1);
    return;
}

int main()
{
    int l, u, n, i, k, j, m, height;
    srand(time(NULL));
    printf("\nEnter the values of l, u and n : ");
    scanf("%d %d %d", &l, &u, &n);
    struct node *tree = create_interval_tree(l,u,n);
    printf("\nEnter the number of integers i : ");
    scanf("%d", &i);
    for (k = 0; k < i; ++k)
    {
        j = rand()%(u-l+1) + l;
        insert(tree, j);
    }
    height = get_height(tree);
    printf("\nThe original tree : ");
    pretty_print(tree, height);
    printf("\n\nEnter the value of m (< %d) : ", n);
    scanf("%d", &m);
    int step = (u-l+1)/m;
    k = l;
    i=0;
    while(i < m)
    {
        if((k+step-1) <= u && (i != m-1))
            tree = merge(tree, k, (k+step-1));
        else
            tree = merge(tree, k, u);
        k += step;
        i++;
        height = get_height(tree);
        printf("\n\nThe tree after the %dth iteration : ", i);
        pretty_print(tree, height);
    }
    return 0;
}
    

