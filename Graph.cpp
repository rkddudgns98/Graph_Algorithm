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
		a[i][i] = 1;	//대각행렬
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
		printf("%3c", int2name(i));		//첫줄(A,B,C,D ...)
	}

	printf("\n");

	for (int i = 0; i < V; i++)
	{
		printf("%3c", int2name(i));	//첫칸
		for (int j = 0; j < V; j++)
		{
			printf("%3d", a[i][j]);//matrix출력
		}
		printf("\n");
	}
}


//adjlist
typedef struct Node {
	int vertex;
	struct Node* next;
}Node;

Node* GL[MAX_NODE];

void input_adjlist(Node* a[], int* V, int* E)
{
	Node* t;

	char vertex[3];	//입력받을 문자열 공간

	int i, j;

	//노드와 엣지의 개수입력받기
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);

	//노드 포인터 타입 배열 초기화 (포인터라 NULL로 초기화)
	for (i = 0; i < *V; i++)
	{
		a[i] = NULL;
	}

	for (int j = 0; j < *E; j++)
	{
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);	//AB를 입력받고,AC를 입력받고
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//노드생성
		t->vertex = name2int(vertex[1]);	//B를 넣어주고, C를넣어주고
		t->next = a[i];	//B->NULL를 해준다, C->B를해준다
		a[i] = t;	//A->B, A->C

		//symetric(대칭)
		i = name2int(vertex[1]);
		t = (Node*)malloc(sizeof(Node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
	}
}

void print_adjlist(Node* a[], int V)
{
	Node* t;

	for (int i = 0; i < V; i++) {

		printf("%3c", int2name(i));	//A,B,C,D 순서대로출력

		for (t = a[i]; t != NULL; t = t->next)
		{
			//t의 ->vertex를 출력한다.
			printf("->%3c ", int2name(t->vertex));
		}
		//줄바꿈
		printf("\n");
	}
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

	//17 18 AB AC AD BE CF DH EF FH EG GI HI JK JL MN MO NP NQ OQ

	return 0;
}
