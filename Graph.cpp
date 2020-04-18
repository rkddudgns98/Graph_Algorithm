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
int check[MAX_NODE] = { 0, };	//방문했는지 확인
int stack[MAX_NODE] = { 0, };	
int top = -1;

//AP (Articulation Point)
int APcheck[MAX_NODE] = { 0, };	//방문순서, Spannig tree
int APorder = 0;	//순서
int son_of_root = 0;	//자식노드의 개수


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
		printf("Input two node consist of edge ->");
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
		printf("\n");
	}

	printf("\n");
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
	for (int i = 0; i < V; i++)	//check 초기화
		check[i] = 0;

	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)	//방문안했을때
		{
			Node* t;
			Push(i);	//push
			check[i] = 1;	//방문했음
			while (!Stack_Empty())	//스택이 빌때까지
			{
				i = Pop();	//팝하고,
				printf("%3c", int2name(i));	//그 값을 출력

				//pop한 값의 연결된 노드들을 모두 push
				for (t = a[i]; t != NULL; t = t->next)
				{
					//이미 방문했다면, push하지 않는다.
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

	//연결된 노드들 검사
	for (t = a[i]; t != NULL; t = t->next)
	{
		//i번째 부모에대해 t->vertex번째의 자식이 있다면
		if (i == 0 && APcheck[t->vertex] == 0) 
			son_of_root++;	//자식의 개수 ++

		//tree-edge
		if (APcheck[t->vertex] == 0)	//방문했는가
		{
			m = AP_recur(a, t->vertex);

			if (m < min)
				min = m;

			if (m >= APcheck[i] && i != 0)	//AP이면 *표시
				printf(" * %c %3d : %d\n", int2name(i), APcheck[i], m); 
			else //AP가 아니면
				printf(" %3c %3d : %d\n", int2name(i), APcheck[i], m); 
		}
		//non-tree-edge
		else if (APcheck[t->vertex] < min)
			min = APcheck[t->vertex];	//min값 재설정
	} 

	return min;
}

void AP_search(Node* a[], int V) 
{
	int i, n = 0; 
	Node* t;

	for (i = 0; i < V; i++)//방문안함	
		APcheck[i] = 0; 

	APorder = son_of_root = 0; 

	AP_recur(a, 0);	//0, 'A'부터 시작

	if (son_of_root > 1) //자식이 여러개, 즉 AP이면 *표시
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
