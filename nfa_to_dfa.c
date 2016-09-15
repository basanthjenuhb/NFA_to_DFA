//Program to perform nfa to dfa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *outfile;

struct Node
{
	char c;
	int n0,n1;
	struct Node **link0;
	struct Node **link1;
};
typedef struct Node node;

int found;
struct Tree
{
	char c[10];
	int valid,n;
	struct Node **links;
	struct Tree *llink;
	struct Tree *rlink;
};
typedef struct Tree tree;

node * getnode(int i)
{
	node *x=(node *)malloc(sizeof(node));
	return x;
}

void getdata(node** start,int i)
{
	int j,num;
	start[i]->c=(char)i+48;
	printf("\nNode q%d:\n",i);
	
	printf("No. of 0 transitions: ");
	scanf("%d",&start[i]->n0);
	start[i]->link0=(node **)malloc(start[i]->n0*sizeof(node *));
	printf("\nEnter the nodes by 0:");
	for(j=0;j<start[i]->n0;j++)
	{
		scanf("%d",&num);
		start[i]->link0[j]=start[num];
	}
	
	printf("\nNo. of 1 transitions: ");
	scanf("%d",&start[i]->n1);
	start[i]->link1=(node **)malloc(start[i]->n1*sizeof(node *));
	printf("\nEnter the nodes by 1:");
	for(j=0;j<start[i]->n1;j++)
	{
		scanf("%d",&num);
		start[i]->link1[j]=start[num];
	}
}

void display(node** start,int n)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<start[i]->n0;j++)
			fprintf(outfile,"q%c -> q%c[label=\"0\",weight=\"0\"];\n",start[i]->c,start[i]->link0[j]->c);
		for(j=0;j<start[i]->n1;j++)
			fprintf(outfile,"q%c -> q%c[label=\"1\",weight=\"1\"];\n",start[i]->c,start[i]->link1[j]->c);
	}
}

int search(char c[],tree* parent)
{
	if(strcmp(c,parent->c)==0 && parent->valid==1)
	{	
		found=1;
		return 1;
	}
	if(parent->llink!=NULL)
		search(c,parent->llink);
	if(parent->rlink!=NULL)
		search(c,parent->rlink);
	return 0;
}

void construct(tree* parent,tree* cur,int z)
{
	int i,j=0,x=0,k=0,r=0;
	found=0;
	search(cur->c,parent);
	if(found==1)
		return;
	cur->valid=1;
	cur->llink=(tree *)malloc(sizeof(tree));
	cur->llink->llink=NULL;
	cur->llink->rlink=NULL;
	cur->llink->valid=0;
	
	for(i=0;i<cur->n;i++)
		x=x+cur->links[i]->n0;
	cur->llink->n=x;
	
	cur->llink->links=(node**)malloc(cur->llink->n*sizeof(node*));
	
	for(i=0;i<cur->n;i++)
		for(j=0;j<cur->links[i]->n0;j++)
			cur->llink->links[k++]=cur->links[i]->link0[j];

	for(i=0;i<x;i++)
		for(j=i+1;j<x;j++)
			if(cur->llink->links[j]==cur->llink->links[i])
			{
				for(r=j;r<x-1;r++)
					cur->llink->links[j]=cur->llink->links[j+1];
				cur->llink->n--;
				x--;
			}

	for(i=0;i<cur->llink->n;i++)
		cur->llink->c[i]=cur->llink->links[i]->c;
	

	cur->rlink=(tree *)malloc(sizeof(tree));
	cur->rlink->llink=NULL;
	cur->rlink->rlink=NULL;
	cur->rlink->valid=0;
	i=0;j=0;x=0,k=0;
	for(i=0;i<cur->n;i++)
		x=x+cur->links[i]->n1;
	cur->rlink->n=x;
	cur->rlink->links=(node**)malloc(cur->rlink->n*sizeof(node*));
	cur->rlink->valid=0;
	for(i=0;i<cur->n;i++)
		for(j=0;j<cur->links[i]->n1;j++)
			cur->rlink->links[k++]=cur->links[i]->link1[j];
	for(i=0;i<x;i++)
		for(j=i+1;j<x;j++)
			if(cur->rlink->links[j]==cur->rlink->links[i])
			{
				for(r=j;r<x-1;r++)
					cur->rlink->links[j]=cur->rlink->links[j+1];
				cur->rlink->n--;
				x--;
			}
	for(i=0;i<cur->rlink->n;i++)
		cur->rlink->c[i]=cur->rlink->links[i]->c;
	construct(parent,cur->llink,0);
	construct(parent,cur->rlink,1);
}

void finaldisplay(tree* parent)
{
	if(parent->llink==NULL && parent->rlink==NULL)
		return;
	int i,flag1=0,flag2=0,k=0;
	for(i=0;i<parent->n;i++)
	{
		fprintf(outfile,"q%c",parent->links[i]->c);
	}
	if(parent->n!=0)
	{
		fprintf(outfile," -> ");
		for(i=0;i<parent->llink->n;i++)
			fprintf(outfile,"q%c",parent->llink->links[i]->c);
		if(parent->llink->n==0)
			fprintf(outfile,"T");
		fprintf(outfile,"[label=\"0\",weight=\"0\"];\n");
	}
	for(i=0;i<parent->n;i++)
		fprintf(outfile,"q%c",parent->links[i]->c);
	if(parent->n!=0)
	{
		fprintf(outfile," -> ");
		for(i=0;i<parent->rlink->n;i++)
			fprintf(outfile,"q%c",parent->rlink->links[i]->c);
		if(parent->rlink->n==0)
			fprintf(outfile, "T" );
		fprintf(outfile,"[label=\"1\",weight=\"1\"];\n");
	}
	if(parent->llink!=NULL)
		finaldisplay(parent->llink);
	if(parent->rlink!=NULL)
		finaldisplay(parent->rlink);
}

int main()
{
	int n,i;
	printf("Enter number of nodes: ");
	scanf("%d",&n);
	node **start;
		
		
	start=(node **)malloc(n*sizeof(node *));
	
	for(i=0;i<n;i++)
		start[i]=getnode(i);
	
	for(i=0;i<n;i++)
		getdata(start,i);
	printf("\n");
	tree *cur,*parent=(tree *)malloc(sizeof(tree));
	parent->c[0]=start[0]->c;
	parent->valid=0;
	parent->n=1;
	parent->links=(node **)malloc(sizeof(node *));
	parent->links[0]=start[0];
	parent->llink=parent->rlink=NULL;
	cur=parent;
	construct(parent,cur,0);
	printf("\n\n\n");
	if(!( outfile = fopen("BTREE.dot","r")))
		outfile = fopen("BTREE.dot", "w");
	else
		outfile = fopen("BTREE.dot", "a");
	fprintf(outfile,"digraph{\n");
	display(start,n);
	fprintf(outfile,"\"\"-> q0;\n");
	fprintf(outfile,"\nrankdir=\"LR\"");
	fprintf(outfile,"\n}");
	fprintf(outfile,"digraph{\n");
	finaldisplay(parent);
	fprintf(outfile,"\nrankdir=\"LR\"");
	fprintf(outfile,"\n}");
	fclose (outfile);
	system("dot -Tps -O BTREE.dot ");
	system("evince BTREE.dot.ps &");
//	system("rm BTREE.dot");
}
