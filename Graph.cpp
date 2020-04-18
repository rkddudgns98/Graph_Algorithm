#include <stdio.h>
#include <windows.h>
#pragma warning(disable: 4996)

#define MAX_NODE 100

int name2int(char c)
{
	return c - 'A';
}
char int2name(int i)
{
	return i + 'A';
}


//DFS
int check[MAX_NODE] = { 0, };	//�湮�ߴ��� Ȯ��
int stack[MAX_NODE] = { 0, };	
int top = -1;

//AP (Articulation Point)
int APcheck[MAX_NODE] = { 0, };	//�湮����, Spannig tree
int APorder = 0;	//����
int son_of_root = 0;	//�ڽĳ���� ����


//Stack
int Push(int t)
{
	if (top >= MAX_NODE - 1)
	{
		printf("Stack overflow\n");
		return -1;
	}

	stack[++top] = t;
	return t;
}

int Pop()
{
	if (top < 0)
	{
		printf("Stack underflow\m");
		return -1;
	}

	return stack[top--];
}

void Init_Stack()
{
	top = -1;
}

int Stack_Empty()
{
	return (top < 0);
}


//adjmatrix
int GM[MAX_NODE][MAX_NODE];
FILE* fp;

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
	char vertex[3];
	int i, j, k;
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);

	for (i = 0; i < *V; i++)
	{
		for (j = 0; j < *V; j++)
		{
			a[i][j] = 0;	//2D array
		}
	}
	for (i = 0; i < *V; i++)
	{
		a[i][i] = 1;	//�밢���
	}
	for (k = 0; k < *E; k++)
	{
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = 1;
		a[j][i] = 1;	//symetric
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("   ");
	for (int i = 0; i < V; i++)
	{
		printf("%3c", int2name(i));		//ù��(A,B,C,D ...)
	}

	printf("\n");

	for (int i = 0; i < V; i++)
	{
		printf("%3c", int2name(i));	//ùĭ
		for (int j = 0; j < V; j++)
		{
			printf("%3d", a[i][j]);//matrix���
		}
		printf("\n\n");
	}
}


//adjlist
typedef struct Node {
	int vertex;
	struct Node* next;
}Node;

//Node
Node* GL[MAX_NODE];

void input_adjlist(Node* a[], int* V, int* E)
{
	Node* t;

	char vertex[3];	//�Է¹��� ���ڿ� ����

	int i, j;

	//���� ������ �����Է¹ޱ�
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);

	//��� ������ Ÿ�� �迭 �ʱ�ȭ (�����Ͷ� NULL�� �ʱ�ȭ)
	for (i = 0; i < *V; i++)
	{
		a[i] = NULL;
	}

	for (int j = 0; j < *E; j++)
	{
		printf("Input two node consist of edge ->");
		scanf("%s", vertex);	//AB�� �Է¹ް�,AC�� �Է¹ް�
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//������
		t->vertex = name2int(vertex[1]);	//B�� �־��ְ�, C���־��ְ�
		t->next = a[i];	//B->NULL�� ���ش�, C->B�����ش�
		a[i] = t;	//A->B, A->C

		//symetric(��Ī)
		i = name2int(vertex[1]);
		t = (Node*)malloc(sizeof(Node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
		printf("\n");
	}

	printf("\n");
}

void print_adjlist(Node* a[], int V)
{
	Node* t;

	for (int i = 0; i < V; i++) {

		printf("%3c", int2name(i));	//A,B,C,D ����������

		for (t = a[i]; t != NULL; t = t->next)
		{
			//t�� ->vertex�� ����Ѵ�.
			printf("->%3c ", int2name(t->vertex));
		}
		//�ٹٲ�
		printf("\n");
	}
	printf("\n");
}

//recursive version DFS
void DFS_recur_list(Node* a[], int V, int i)
{
	Node* t;
	check[i] = 1;
	printf("%3c", int2name(i));
	for (t = a[i]; t != NULL; t = t->next)
	{
		if (check[t->vertex] == 0)
			DFS_recur_list(a, V, t->vertex);
	}
}

void DFS_adjlist(Node* a[], int V)
{
	int i;
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
			DFS_recur_list(a, V, i);
	}
	printf("\n");
}


//non-recursive version DFS
void nrDFS_adjlist(Node* a[], int V)
{
	Init_Stack();	//top = -1;
	for (int i = 0; i < V; i++)	//check �ʱ�ȭ
		check[i] = 0;

	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)	//�湮��������
		{
			Node* t;
			Push(i);	//push
			check[i] = 1;	//�湮����
			while (!Stack_Empty())	//������ ��������
			{
				i = Pop();	//���ϰ�,
				printf("%3c", int2name(i));	//�� ���� ���

				//pop�� ���� ����� ������ ��� push
				for (t = a[i]; t != NULL; t = t->next)
				{
					//�̹� �湮�ߴٸ�, push���� �ʴ´�.
					if (check[t->vertex] == 0)
					{
						Push(t->vertex);
						check[t->vertex] = 1;
					}
				}
			}
		}
	}
	printf("\n\n");
}


//Find AP
int AP_recur(Node* a[], int i)
{
	Node* t; 
	int min, m; 

	APcheck[i] = min = ++APorder; 

	//����� ���� �˻�
	for (t = a[i]; t != NULL; t = t->next)
	{
		//i��° �θ𿡴��� t->vertex��°�� �ڽ��� �ִٸ�
		if (i == 0 && APcheck[t->vertex] == 0) 
			son_of_root++;	//�ڽ��� ���� ++

		//tree-edge
		if (APcheck[t->vertex] == 0)	//�湮�ߴ°�
		{
			m = AP_recur(a, t->vertex);

			if (m < min)
				min = m;

			if (m >= APcheck[i] && i != 0)	//AP�̸� *ǥ��
				printf(" * %c %3d : %d\n", int2name(i), APcheck[i], m); 
			else //AP�� �ƴϸ�
				printf(" %3c %3d : %d\n", int2name(i), APcheck[i], m); 
		}
		//non-tree-edge
		else if (APcheck[t->vertex] < min)
			min = APcheck[t->vertex];	//min�� �缳��
	} 

	return min;
}

void AP_search(Node* a[], int V) 
{
	int i, n = 0; 
	Node* t;

	for (i = 0; i < V; i++)//�湮����	
		APcheck[i] = 0; 

	APorder = son_of_root = 0; 

	AP_recur(a, 0);	//0, 'A'���� ����

	if (son_of_root > 1) //�ڽ��� ������, �� AP�̸� *ǥ��
		printf(" * %c son : %d\n\n", int2name(0), son_of_root);
	else  
		printf(" %3c son : %d\n\n", int2name(0), son_of_root);
}


int main()
{
	int V, E;

	//for adjacency matrix
	//input_adjmatrix(GM, &V, &E);
	//print_adjmatrix(GM, V);

	//for adjacency list
	input_adjlist(GL, &V, &E);
	print_adjlist(GL, V);


	//traversse the given graph
	//DFS_adjlist(GL, V);
	//nrDFS_adjlist(GL, V);

	AP_search(GL, V);
	
	//17 18 AB AC AD BE CF DH EF FH EG GI HI JK JL MN MO NP NQ OQ
	//DJ JM

	return 0;
}
