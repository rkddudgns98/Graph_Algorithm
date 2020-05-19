#include <stdio.h>
#include <windows.h>
#pragma warning(disable: 4996)

#define MAX_NODE 100
#define MAX_EDGE 100
#define ONLY_FIND 0
#define UNION 1
#define UNSEEN (-INT_MAX)
#define INFINITE 1000

int name2int(char c)
{
	return c - 'A';
}
char int2name(int i)
{
	return i + 'A';
}

//file
FILE* fp;

//DFS BFS
int check[MAX_NODE] = { 0, };	//방문했는지 확인

//AP (Articulation Point)
int APcheck[MAX_NODE] = { 0, };	//방문순서, Spannig tree
int APorder = 0;	//순서
int son_of_root = 0;	//자식노드의 개수

//heap
int nheap = 0;
int heap[MAX_NODE] = { 0, };
int weighted[MAX_NODE] = { 0, };
int parent[MAX_NODE] = { 0, };

//Kruskal
int cost = 0;

//dijkstra
int start = 0;
int distance[MAX_NODE] = { 0, };

//floyd
int f_distance[MAX_NODE][MAX_NODE];	//floyd_distance

//Strongly connected
int SCcheck[MAX_NODE] = { 0, };	//방문순서, Spannig tree
int SCorder = 0;	//순서


//------------------------Stack-----------------------------
int stack[MAX_NODE] = { 0, };
int top = -1;
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


//------------------------Queue------------------------------
//Array based Queue (Circular)
int queue[MAX_NODE];
int front, rear;

void Init_Queue()
{
	front = 0;
	rear = 0;
}

void Clear_Queue()
{
	front = rear;
}

int Put(int k)
{
	if ((rear + 1) % MAX_NODE == front)
	{
		printf("  Queue overflow\n");
		return -1;
	}

	queue[rear] = k;
	rear = ++rear % MAX_NODE;

	return k;
}

int Get()
{
	int j;

	if (front == rear)
	{
		printf("  Queue underflow !!!\n");
		return -1;
	}

	j = queue[front];
	front = ++front % MAX_NODE;

	return j;
}

void Print_Queue()
{
	for (int i = front; i != rear; i = ++i % MAX_NODE)
	{
		printf("%3d", queue[i]);
	}
	printf("\n");
}

int Queue_Empty() 
{
	if (front == rear)
		return 1;
	else
		return 0;
}

//Double inked list based Queue
typedef struct DNode {
	int vertex;
	struct DNode* prev;
	struct DNode* next;
}DNode;

DNode* dhead, * dtail;

void Init_LLQueue()
{
	dhead = (DNode*)calloc(1, sizeof(DNode));
	dtail = (DNode*)calloc(1, sizeof(DNode));

	dhead->prev = dhead;
	dhead->next = dtail;
	dtail->prev = dhead;
	dtail->next = dtail;
}

//Input before tail
int LLPut(int k)
{
	DNode* t;

	if ((t = (DNode*)malloc(sizeof(DNode))) == NULL)
	{
		printf("Out of memory\n");
		return -1;
	}

	t->vertex = k;
	dtail->prev->next = t;
	t->prev = dtail->prev;
	dtail->prev = t;
	t->next = dtail;

	return k;
}

//Get after haed
int LLGet()
{
	DNode* t;
	int k;

	t = dhead->next;

	if (t == dtail)
	{
		printf("  LLQueue underflow\n");
		return -1;
	}

	k = t->vertex;
	dhead->next = t->next;
	t->next->prev = dhead;
	free(t);

	return k;
}

void Clear_LLQueue()
{
	DNode* t, * s;

	t = dhead->next;

	while (t != dtail)
	{
		s = t;
		t = t->next;
		free(s);
	}

	dhead->next = dtail;
	dtail->prev = dhead;
}

void Print_LLQueue()
{
	DNode* t;

	t = dhead->next;

	while (t != dtail)
	{
		printf("%3d", t->vertex);
		t = t->next;
	}
	printf("\n");
}

int LLQueue_Empty() 
{
	if (dhead->next == dtail && dtail->prev == dhead)
		return 1;
	else
		return 0;
}


//-----------------------adjmatrix----------------------------
int GM[MAX_NODE][MAX_NODE];

void input_Fadjmatrix(int a[][MAX_NODE], int* V, int* E)
{
	char vertex[3];
	int i, j, k, w;
	printf("Input number of node & edge\n");
	fscanf(fp,"%d %d", V, E);

	for (i = 0; i < *V; i++)
	{
		for (j = 0; j < *V; j++)
		{
			a[i][j] = INFINITE;	//2D array
		}
	}
	for (i = 0; i < *V; i++)
	{
		a[i][i] = 0;	//대각행렬
	}
	printf("Input two node consist of edge & weight\n");
	for (k = 0; k < *E; k++)
	{
		fscanf(fp,"%s %d", vertex, &w);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = w;
		a[j][i] = w;	//symetric
	}
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
	printf("\n--Input_adjmatrix--\n");
	char vertex[3];
	int i, j, k, w;
	printf("Input number of node & edge\n");
	scanf("%d %d", V, E);

	for (i = 0; i < *V; i++)
	{
		for (j = 0; j < *V; j++)
		{
			a[i][j] = INFINITE;	//2D array
		}
	}
	for (i = 0; i < *V; i++)
	{
		a[i][i] = 0;	//대각행렬
	}
	printf("Input two node consist of edge & weight\n");
	for (k = 0; k < *E; k++)
	{
		scanf("%s %d", vertex, &w);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = w;
		a[j][i] = w;	//symetric
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--Print_matrix--\n");
	printf("   ");
	for (int i = 0; i < V; i++)
	{
		printf("%5c", int2name(i));		//첫줄(A,B,C,D ...)
	}

	printf("\n");

	for (int i = 0; i < V; i++)
	{
		printf("%5c", int2name(i));	//첫칸
		for (int j = 0; j < V; j++)
		{
			printf("%5d", a[i][j]);//matrix출력
		}
		printf("\n");
	}
	printf("\n");
}


//------------------------adjlist----------------------------
typedef struct Node {
	int vertex;
	int weight;
	struct Node* next;
}Node;

//Node
Node* GL[MAX_NODE];

void input_Fadjlist(Node* a[], int* V, int* E)
{
	Node* t;

	char vertex[3];	//입력받을 문자열 공간

	int i, j, w;	//w= weight

	//노드와 엣지의 개수입력받기
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);

	//노드 포인터 타입 배열 초기화 (포인터라 NULL로 초기화)
	for (i = 0; i < *V; i++)
	{
		a[i] = NULL;
	}

	printf("Input two node consist of edge & weight");

	for (int j = 0; j < *E; j++)
	{
		fscanf(fp,"%s %d", vertex, &w);	//AB를 입력받고,AC를 입력받고
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//노드생성
		t->vertex = name2int(vertex[1]);	//B를 넣어주고, C를넣어주고
		t->next = a[i];	//B->NULL를 해준다, C->B를해준다
		a[i] = t;	//A->B, A->C
		t->weight = w;	//input weight

		//symetric(대칭)
		i = name2int(vertex[1]);
		t = (Node*)malloc(sizeof(Node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
		t->weight = w;
	}
	printf("\n\n");
}

void input_adjlist(Node* a[], int* V, int* E)
{
	printf("\n--Input_adjlist--\n");
	Node* t;

	char vertex[3];	//입력받을 문자열 공간

	int i, j, w;	//w= weight

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
		printf("Input two node consist of edge & weight");
		scanf("%s %d", vertex, &w);	//AB를 입력받고,AC를 입력받고
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//노드생성
		t->vertex = name2int(vertex[1]);	//B를 넣어주고, C를넣어주고
		t->next = a[i];	//B->NULL를 해준다, C->B를해준다
		a[i] = t;	//A->B, A->C
		t->weight = w;	//input weight

		//symetric(대칭)
		i = name2int(vertex[1]);
		t = (Node*)malloc(sizeof(Node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
		t->weight = w;
	}
	printf("\n\n");
}

void print_adjlist(Node* a[], int V)
{
	printf("\n--Print_adjlist--\n");
	Node* t;

	for (int i = 0; i < V; i++) {

		printf("%3c", int2name(i));	//A,B,C,D 순서대로출력

		for (t = a[i]; t != NULL; t = t->next)
		{
			//t의 ->vertex를 출력한다.
			printf(" -> %c:%d ", int2name(t->vertex), t->weight);
		}
		//줄바꿈
		printf("\n");
	}
	printf("\n");
}


//------------------------DFS------------------------------
//---------adjmatrix-----------
//recursive version DFS adjmatrix
void DFS_recur_adjmatrix(int a[][MAX_NODE], int V, int i) 
{ 
	check[i] = 1;
	printf("%3c", int2name(i));
	for (int j = V - 1; j >= 0; j--)
		if (a[i][j] != 0)
			if (check[j] == 0)
				DFS_recur_adjmatrix(a, V, j);
}

void DFS_adjmatrix(int a[][MAX_NODE], int V)
{ 
	printf("\n--DFS_adjmatrix--\n");
	int i;
	for (i = 0; i < V; i++) 
		check[i] = 0;
	for (i = 0; i < V; i++)
		if (check[i] == 0)
			DFS_recur_adjmatrix(a, V, i);

	printf("\n");
}

//non-recursive version DFS adjmatrix
void nrDFS_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--nrDFS_adjmatrix--\n");
	Init_Stack();
	for (int i = 0; i < V; i++)
		check[i] = 0;
	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			Push(i);
			check[i] = 1;
			while (!Stack_Empty())
			{
				i = Pop();
				printf("%3c", int2name(i));
				for (int j = 0; j < V; j++)
					if (a[i][j] != 0)
						if (check[j] == 0)
						{
							Push(j);
							check[j] = 1;
						}
			}
		}
	}
	printf("\n");
}

//----------adjlist------------
//recursive version DFS adjlist
void DFS_recur_adjlist(Node* a[], int V, int i)
{
	Node* t;
	check[i] = 1;
	printf("%3c", int2name(i));
	for (t = a[i]; t != NULL; t = t->next)
		if (check[t->vertex] == 0)
			DFS_recur_adjlist(a, V, t->vertex);
	
}

void DFS_adjlist(Node* a[], int V)
{
	printf("\n--DFS_adjlist--\n");
	int i;
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
		if (check[i] == 0)
			DFS_recur_adjlist(a, V, i);
	printf("\n");
}

//non-recursive version DFS adjlist
void nrDFS_adjlist(Node* a[], int V)
{
	printf("\n--nrDFS_adjlist--\n");
	Node* t;
	Init_Stack();	//top = -1;
	for (int i = 0; i < V; i++)	//check 초기화
		check[i] = 0;

	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)	//방문안했을때
		{
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
	printf("\n");
}



//-------------------------BFS------------------------------
//-----------adjmatrix----------
//recursive version BFS adjmatrix is same DFS, just dif <j>
//truly it is not BFS
void BFS_recur_adjmatrix(int a[][MAX_NODE], int V, int i)
{
	check[i] = 1;
	printf("%3c", int2name(i));
	for (int j = 0; j < V; j++)
		if (a[i][j] != 0)
			if (check[j] == 0)
				DFS_recur_adjmatrix(a, V, j);
}

void BFS_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--BFS_adjmatrix--\n");
	int i;
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
		if (check[i] == 0)
			BFS_recur_adjmatrix(a, V, i);

	printf("\n");
}

//non-recursive version BFS adjmatrix
void nrBFS_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--nrBFS_adjmatrix--\n");
	int i, j;
	Init_Queue();	//queue배열 초기화
	for (i = 0; i < V; i++)	//모두 방문안함
		check[i] = 0;
	for (i = 0; i < V; i++)	//전체노드에 대해서
	{
		if (check[i] == 0)	//방문을 안했다면
		{
			Put(i);	//queue put
			check[i] = 1;	//방문체크
			while (!Queue_Empty())	//queue가 비지않았을때
			{
				i = Get();	//queue get
				printf("%3c", int2name(i));	//방문했음을 출력
				for (j = 0; j < V; j++)	//전체노드에대해
					if (a[i][j] != 0)	//연결되어있고
						if (check[j] == 0)	//방문안했다면
						{
							Put(j);	//queue put
							check[j] = 1;	//방문했음을 체크
						}
			}
		}
	}
	printf("\n");
}

//-----------adjlist--------------
//BFS can't be recursive

//non-recursive version BFS adjlist
void nrBFS_adjlist(Node* a[], int V)
{
	printf("\n--nrBFS_adjlist--\n");
	int i;
	Node* t;
	Init_LLQueue();	
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			LLPut(i);
			check[i] = 1;
			while (!LLQueue_Empty())
			{
				i = LLGet();
				//visit
				printf("%3c", int2name(i));
				for (t = a[i]; t != NULL; t = t->next)
					if (check[t->vertex] == 0)
					{
						LLPut(t->vertex);
						check[t->vertex] = 1;
					}
			}
		}
	}
	printf("\n");
}


//-------------------Count Componenets-----------------------
void Count_Components_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--Count_Componenets--\n");
	int count = 0;
	Init_Stack();
	for (int i = 0; i < V; i++)
		check[i] = 0;
	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			count++;
			Push(i);
			check[i] = 1;
			while (!Stack_Empty())
			{
				i = Pop();
				printf("%3c", int2name(i));
				for (int j = 0; j < V; j++)
					if (a[i][j] != 0)
						if (check[j] == 0)
						{
							Push(j);
							check[j] = 1;
						}
			}
		}
	}
	printf("	# of graph: %d\n", count);
}

//------------------------Find AP----------------------------
//------------adjmatrix-----------------
int AP_recur_adjmatrix(int a[][MAX_NODE], int i,int V)
{
	int min, m;

	APcheck[i] = min = ++APorder;

	//연결된 노드들 검사
	for (int v = V-1; v >= 0; v--)
	{
		if (a[i][v] == 1)
		{
			//Root부모에대해 t->vertex번째의 자식이 있다면
			if (i == 0 && APcheck[v] == 0)
				son_of_root++;	//자식의 개수 ++

			//tree-edge
			if (APcheck[v] == 0)	//방문했는가
			{
				//m=연결된 노드의 최상위노드
				m = AP_recur_adjmatrix(a, v, V);

				if (m <= min)
					min = m;

				//m>=APcheck[i]이면 i보다 상위노드와 연결되어있다
				if (m >= APcheck[i] && i != 0)	//AP이면 *표시
					printf(" * %c %3d : %d\n", int2name(i), APcheck[i], m);
				else //AP가 아니면
					printf(" %3c %3d : %d\n", int2name(i), APcheck[i], m);
			}
			//non-tree-edge
			else if (APcheck[v] < min)
				min = APcheck[v];	//min값 재설정
		}
	}

	return min;
}

void AP_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--AP_search(adjmatrix)--\n");
	int i, n = 0;

	for (i = 0; i < V; i++)//방문안함	
		APcheck[i] = 0;

	APorder = son_of_root = 0;

	AP_recur_adjmatrix(a, 0, V);	//0, 'A'부터 시작

	if (son_of_root > 1) //자식이 여러개, 즉 AP이면 *표시
		printf(" * %c son : %d\n\n", int2name(0), son_of_root);
	else
		printf(" %3c son : %d\n\n", int2name(0), son_of_root);
}


//-------------adjlist-------------------
int AP_recur_adjlist(Node* a[], int i)
{
	Node* t; 
	int min, m; 

	APcheck[i] = min = ++APorder; 

	//연결된 노드들 검사
	for (t = a[i]; t != NULL; t = t->next)
	{
		//Root부모에대해 t->vertex번째의 자식이 있다면
		if (i == 0 && APcheck[t->vertex] == 0) 
			son_of_root++;	//자식의 개수 ++

		//tree-edge
		if (APcheck[t->vertex] == 0)	//방문했는가
		{
			//m=연결된 노드의 최상위노드
			m = AP_recur_adjlist(a, t->vertex);

			if (m < min)
				min = m;

			//m>=APcheck[i]이면 i보다 상위노드와 연결되어있다
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

void AP_adjlist(Node* a[], int V) 
{
	printf("\n--AP_search(adjlist)--\n");
	int i, n = 0; 
	Node* t;

	for (i = 0; i < V; i++)//방문안함	
		APcheck[i] = 0; 

	APorder = son_of_root = 0; 

	AP_recur_adjlist(a, 0);	//0, 'A'부터 시작

	if (son_of_root > 1) //자식이 여러개, 즉 AP이면 *표시
		printf(" * %c son : %d\n\n", int2name(0), son_of_root);
	else  
		printf(" %3c son : %d\n\n", int2name(0), son_of_root);
}


//---------------------------PFS-----------------------------
int pq_empty()
{
	if (nheap == 0)
		return 1;
	return 0;
}

void pq_init()
{
	nheap = 0;	//힙의 길이 = 0
}

void upheap(int h[], int k)
{
	int v; //index
	v = h[k];

	while (weighted[h[k / 2]] <= weighted[v] && k / 2 > 0)
	{
		h[k] = h[k / 2];
		k /= 2;
	}

	h[k] = v;
}

void downheap(int h[], int k)
{
	int i, v;	//v=index
	v = h[k];

	while (k <= nheap / 2)
	{
		i = k << 1;

		if (i < nheap && weighted[h[i]] < weighted[h[i + 1]])
			i++;
		if (weighted[v] >= weighted[h[i]])
			break;
		h[k] = h[i];
		k = i;

	}

	h[k] = v;
}

//heap 정렬
void adjust_heap(int h[], int n)
{
	int k;
	for (k = n / 2; k >= 1; k--)
		downheap(h, k);
}

//heap 추출
int pq_extract(int h[])
{
	int v = h[1];	//v에 heap queue의 첫 index 추출

	h[1] = h[nheap--];	//마지막항목을 첫 index로
	downheap(h, 1);	//정렬

	return v;
}

void print_heap(int h[])
{
	for (int i = 1; i <= nheap; i++)
		printf("  %c:%d ", int2name(h[i]), weighted[h[i]]);
}

int pq_update(int h[], int v, int p)	//v=index, p=weight
{
	if (weighted[v] == UNSEEN)	//UNSEEN Node
	{
		h[++nheap] = v;	//1부터, v=index
		weighted[v] = p; //store the weight
		upheap(h, nheap);	//정렬
		return 1;	//업데이트 성공
	}
	else
	{
		if (weighted[v] < p)	//Fringe Node
		{
			weighted[v] = p;	//우선순위 변경
			adjust_heap(h, nheap);	//힙 정렬
			return 1;	//업데이트 성공
		}
		else
			return 0;	//업데이트 할 거 없음
	}
}

void PFS_adjlist(Node* g[], int V)
{
	printf("\n--PFS_adjlist--\n");
	int i;
	Node* t;
	pq_init();	//힙의 길이 = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //가중치 = UNSEEN, 우선순위가 최하
		parent[i] = 0; //부모노드
	}

	for (i = 0; i < V; i++)
	{
		if (weighted[i] == UNSEEN)	//방문 안했을때
		{
			parent[i] = -1;
			pq_update(heap, i, UNSEEN); //힙 재조정(가중치)

			while (!pq_empty())	//힙에 데이터가 존재할때
			{
				print_heap(heap);	//heap출력(index:weighted)
				i = pq_extract(heap);	//추출 = 나무정점
				weighted[i] = -weighted[i];	//양수로 바꿔줌
				printf(" --> %c:%d\n", int2name(i), weighted[i]);

				for (t = g[i]; t != NULL; t = t->next)
				{
					if (weighted[t->vertex] < 0) //UNSEEN or Fringe
						//가중치가 업데이트 되면
						if (pq_update(heap, t->vertex, -t->weight))
							parent[t->vertex] = i;	//부모는 i이다
				}
			}
		}
	}
	printf("\n");
}

void PFS_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--PFS_adjmatrix--\n");
	int i;
	pq_init();	//힙의 길이 = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //가중치 = UNSEEN, 우선순위가 최상
		parent[i] = 0; //부모노드
	}

	for (i = 0; i < V; i++)
	{
		if (weighted[i] == UNSEEN)	//방문 안했을때
		{
			parent[i] = -1;
			pq_update(heap, i, UNSEEN); //힙 재조정(가중치)

			while (!pq_empty())	//힙에 데이터가 존재할때
			{
				print_heap(heap);	//heap출력(index:weighted)
				i = pq_extract(heap);	//추출 = 나무정점
				weighted[i] = -weighted[i];	//양수로 바꿔줌
				printf(" --> %c:%d\n", int2name(i), weighted[i]);

				for (int v = V; v >= 0; v--)
				{
					if (a[i][v] > 0) {
						if (weighted[v] < 0) //UNSEEN or Fringe
							//가중치가 업데이트 되면
							if (pq_update(heap, v, -a[i][v]))
								parent[v] = i;	//부모는 i이다
					}
				}
			}
		}
	}
	printf("\n");
}

//print parents and son
void print_tree(int p[],int V)
{
	printf("\n--Parent-Sons--\n");
	//node의 개수만큼 알파벳 순차출력
	printf("Son:    ");
	for (int i = 0; i < V; i++)
		printf("%c ", int2name(i));	

	//해당 노드의 부모 노드 출력(첫번째 노드는 .)
	printf("\nParent: . ");
	for (int i = 1; i < V; i++)
		printf("%c ", int2name(p[i]));

	printf("\n");
}

int get_cost(int w[],int V)
{
	int cost = 0;
	//최소비용은 weight를 모두 더한 값
	for (int i = 1; i < V; i++)	//뿌리노드는 ㄴㄴ
	{
		cost += w[i];
	}
	return cost;
}


//---------------------Kruskal--------------------------------
typedef struct _Edge{
	int v1, v2; 
	int weight;
}Edge;

Edge edge[MAX_EDGE];

void upKheap(int h[], int k)
{
	int v; //index
	v = h[k];

	while (edge[h[k / 2]].weight > edge[v].weight && k / 2 > 0)
	{
		h[k] = h[k / 2];
		k /= 2;
	}

	h[k] = v;
}

void downKheap(int h[], int k)
{
	int i, v;	//v=index
	v = h[k];

	while (k <= nheap / 2)
	{
		i = k << 1;

		if (i < nheap && edge[h[i]].weight >= edge[h[i + 1]].weight)
			i++;
		if (edge[v].weight < edge[h[i]].weight)
			break;
		h[k] = h[i];
		k = i;

	}

	h[k] = v;
}

int pq_Kextract(int h[])
{
	int v = h[1];	//v에 heap queue의 첫 index 추출

	h[1] = h[nheap--];	//마지막항목을 첫 index로
	downKheap(h, 1);	//정렬

	return v;
}

//input edge (v1,v2,weight)
void input_edge(Edge e[], int* V, int* E)
{
	char vertex[3];
	int i, j, w;

	printf("\nInput number of nodes and edges\n"); 
	fscanf(fp, "%d %d", V, E);
	printf("Input two nodes consisting of edge and its weight");
	for (j = 0; j < *E; j++)
	{
		fscanf(fp, "%s %d", vertex, &w);
		vertex[2] = NULL;
		e[j].v1 = name2int(vertex[0]);
		e[j].v2 = name2int(vertex[1]);
		e[j].weight = w;
	}
}

//부모노드 -1로 초기화
void init_find(int elem)
{ 
	int i;
	for (i = 0; i < elem; i++) 
		parent[i] = -1; 
}

void union_set(int elem, int asso)
{	//asso is elem's parent
	parent[elem] = asso;
}

int find_set(int elem, int asso, int flag)
{	//elem = 자식, asso = 부모
	int i = elem, j = asso; 
	while (parent[i] >= 0)
		i = parent[i]; 
	while (parent[j] >= 0) 
		j = parent[j]; 
	//부모가 다르다면(다른 그룹이라면)
	if (flag == UNION && i != j) 
		union_set(i, j);//부모로 해줌
	return (i != j); 
}

void visit(int e)
{
	printf(" %c%c:%d ", int2name(edge[e].v1), int2name(edge[e].v2), edge[e].weight);
	cost += edge[e].weight;
}

void pq_insert(int h[], int v)
{
	h[++nheap] = v;
	upKheap(h, nheap);
}

void kruskal(Edge e[], int V, int E)
{
	int n, val = 0;
	init_find(V);
	pq_init();
	for (n = 0; n < E; n++)
		pq_insert(heap, n);
	n = 0; 
	while (!pq_empty())
	{
		val = pq_Kextract(heap);	//heap의 index는 알파벳, 정렬순서는 weight
		if (find_set(e[val].v1, e[val].v2, UNION))	//부모가 같다면 다른루트가 있음
		{ 
			visit(val); 
			n++;
		} 
		if (n == V - 1) //최소 V개 노드에대해 V-1엣지가 있어야 서치가능
			break; 
	}
}


//----------------------Shortest_adjlist------------------------
void Shortest_adjlist(Node* g[], int start, int V)
{
	printf("\n--Shortest_adjlist--\n");
	int i;
	Node* t;
	pq_init();	//힙의 길이 = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //가중치 = UNSEEN, 우선순위가 최하
		parent[i] = 0; //부모노드
	}
	i = start;	//시작노드

	if (weighted[i] == UNSEEN)	//방문 안했을때
	{
		parent[i] = -1;
		pq_update(heap, i, 0); //힙 재조정(가중치)

		while (!pq_empty())	//힙에 데이터가 존재할때
		{
			print_heap(heap);	//heap출력(index:weighted)
			i = pq_extract(heap);	//추출 = 나무정점
			weighted[i] = -weighted[i];	//양수로 바꿔줌
			printf(" --> %c to %c:%d\n", int2name(start), int2name(i), weighted[i]);

			for (t = g[i]; t != NULL; t = t->next)
			{
				if (weighted[t->vertex] < 0) //UNSEEN or Fringe
					//가중치(누적)가 업데이트 되면
					if (pq_update(heap, t->vertex, -t->weight - weighted[i]))
						parent[t->vertex] = i;	//부모는 i이다
			}
		}
	}
	printf("\n");
}

void Shortest_adjmatrix(int a[][MAX_NODE], int start, int V)
{
	printf("\n--Shortest_adjmatrix--\n");
	int i;
	pq_init();	//힙의 길이 = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //가중치 = UNSEEN, 우선순위가 최상
		parent[i] = 0; //부모노드
	}
	i = start;
	if (weighted[i] == UNSEEN)	//방문 안했을때
	{
		parent[i] = -1;
		pq_update(heap, i, 0); //힙 재조정(가중치)

		while (!pq_empty())	//힙에 데이터가 존재할때
		{
			print_heap(heap);	//heap출력(index:weighted)
			i = pq_extract(heap);	//추출 = 나무정점
			weighted[i] = -weighted[i];	//양수로 바꿔줌
			printf(" --> %c to %c:%d\n", int2name(start), int2name(i), weighted[i]);

			for (int v = V; v >= 0; v--)
			{
				if (a[i][v] > 0) {
					if (weighted[v] < 0) //UNSEEN or Fringe
						//가중치가 업데이트 되면
						if (pq_update(heap, v, -a[i][v] - weighted[i]))
							parent[v] = i;	//부모는 i이다
				}
			}
		}
	}
	printf("\n");
}


//----------------------dijkstra algorithm-----------------------
void Print_dijkstra(int a[], int start, int V)
{
	printf("%5c", int2name(start));

	for (int j = 0; j < V; j++)
	{
		printf("%5d", a[j]);	//distance출력
	}
	printf("\n");
}

void Dijkstra(int a[][MAX_NODE], int start, int V)
{
	printf("--dijkstra--\n");
	int x = 0, y, d;
	int i, checked = 0;
	for (x = 0; x < V; x++)	//모든정점 x에 대해
	{
		distance[x] = a[start][x];	//start node와 거리저장
		if (x != start) 
			parent[x] = start;	//부모는 start node이다
	}
	check[start] = 1;	//start node 방문
	checked++;	//모두 방문함을 세어줄 카운트

	//print_distance(distance, start, V) for visual
	printf("     ");
	for (int i = 0; i < V; i++)
	{
		printf("%5c", int2name(i));		//첫줄(A,B,C,D ...)
	}
	printf("\n");
	Print_dijkstra(distance, start, V);

	while (checked < V)	//모두 방문할때까지
	{
		x = 0;	//A부터
		while (check[x]) 
			x++;	//방문했다면 다음으로
		for (i = x; i < V; i++)	//x부터 마지막노드까지
			if (check[i] == 0 && distance[i] < distance[x])	//아직 방문안했고, 가중치가 작으면
				x = i;	//가중치가 최소인 node
		check[x] = 1;	//x node 방문
		checked++;	

		for (y = 0; y < V; y++)	
		{
			if (x == y || a[x][y] >= INFINITE || check[y])	
				continue;
			d = distance[x] + a[x][y];	//누적된 가중치에 새로운 가중치 누적
			if (d < distance[y]) //누적된 가중치가 기존 가중치보다 작다면(최단루트라면)
			{
				distance[y] = d; //누적가중치 변경
				parent[y] = x;	//부모변경
			}
		}
		//print_distance(distance, x, V)
		Print_dijkstra(distance, x, V);
	}
	//print result for visual
	printf("Result from %c to @\n", int2name(start));
	Print_dijkstra(distance, start, V);
}

void Print_dijkstra_parent(int p[],int start, int V)
{
	printf("\n--Parent-Sons--\n");
	//node의 개수만큼 알파벳 순차출력
	printf("Son:    ");
	for (int i = 0; i < V; i++)
		printf("%c ", int2name(i));

	//해당 노드의 부모 노드 출력(첫번째 노드는 .)
	printf("\nParent: ");
	for (int i = 0; i < V; i++)
	{
		if (i == start)
			printf(". ");
		else
			printf("%c ", int2name(p[i]));
	}
	printf("\n");
}



//-----------------------directed graph--------------------------
//----------dirmatrix----------
void input_dirmatrix(int a[][MAX_NODE], int* V, int* E)
{
	char vertex[3];
	int i, j, k, w;
	printf("Input number of node & edge\n");
	fscanf(fp, "%d %d", V, E);

	for (i = 0; i < *V; i++)
	{
		for (j = 0; j < *V; j++)
		{
			a[i][j] = INFINITE;	//2D array
		}
	}
	for (i = 0; i < *V; i++)
	{
		a[i][i] = 0;	//대각행렬
	}
	printf("Input two node consist of edge & weight\n");
	for (k = 0; k < *E; k++)
	{
		fscanf(fp, "%s %d", vertex, &w);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = w;
	}
}
//-----------dirlist-----------
void input_dirlist(Node* a[], int* V, int* E)
{
	Node* t;

	char vertex[3];	//입력받을 문자열 공간

	int i, j, w;	//w= weight

	//노드와 엣지의 개수입력받기
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);

	//노드 포인터 타입 배열 초기화 (포인터라 NULL로 초기화)
	for (i = 0; i < *V; i++)
	{
		a[i] = NULL;
	}

	printf("Input two node consist of edge & weight");

	for (int j = 0; j < *E; j++)
	{
		fscanf(fp, "%s %d", vertex, &w);	//AB를 입력받고,AC를 입력받고
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//노드생성
		t->vertex = name2int(vertex[1]);	//B를 넣어주고, C를넣어주고
		t->next = a[i];	//B->NULL를 해준다, C->B를해준다
		a[i] = t;	//A->B, A->C
		t->weight = w;	//input weight
	}
	printf("\n\n");
}

//---------------------------DFS---------------------------------
//-----------dirlist-----------
void DFS_dirlist(Node* a[], int V)
{
	printf("\n--DFS_dirlist--\n");
	int i, j;
	Node* t;
	Init_Stack();
	for (i = 0; i < V; i++)
	{
		for (j = 0; j < V; j++)
			check[j] = 0;
		Push(i);
		check[i] = 1;
		printf("%3c:", int2name(i));
		while (!Stack_Empty())
		{
			j = Pop();
			printf("%3c", int2name(j));
			for (t = a[j]; t != NULL; t = t->next)
			{
				if (check[t->vertex] == 0)
				{
					Push(t->vertex);
					check[t->vertex] = 1;
				}
			}
		}
		printf("\n");
	}
}

//--------------------------floyd--------------------------------
void Copy_matrix(int a[][MAX_NODE], int b[][MAX_NODE])
{
	for (int y = 0; y < MAX_NODE; y++)
	{
		for (int x = 0; x < MAX_NODE; x++)
		{
			b[y][x] = a[y][x];
		}
	}
}

void Floyd(int a[][MAX_NODE], int V)
{
	for (int y = 0; y < V; y++)
		for (int x = 0; x < V; x++)
			for (int k = 0; k < V; k++)
				//x-k 연결보다 x-y-k연결이 짧을경우
				if (f_distance[x][y] + f_distance[y][k] < f_distance[x][k])
					//x-k 가중치를 바꿔준다
					f_distance[x][k] = f_distance[x][y] + f_distance[y][k];
}

//-----------------------Topological sort------------------------
typedef struct _topol{
	int count = 0;
	struct _topol* next;
}topol;

topol network[MAX_NODE];

void set_count_indegree(Node* a[], topol net[], int V)
{
	printf("\n--set_count_indegree--\n");
	int i, j;
	int count;
	Node* t;
	for (i = 0; i < V; i++)	//모든노드
	{
		count = 0;
		for (j = 0; j < V; j++)	//A부터
			for (t = a[j]; t != NULL; t = t->next)	//연결된 노드탐색
				if (t->vertex == i)	//선행작업이 있으면
					count++;	//내차수 증가
		net[i].count = count;
	}

	for (int k = 0; k < V; k++)	//내차수를 출력해 확인
	{
		printf(" %c:%d ", int2name(k), net[k].count);
	}

	printf("\n");
}

int Topol_sort(Node* a[], topol net[], int V)
{
	int i, j, k;
	Node* ptr;
	Init_Stack();
	set_count_indegree(a,net, V);	//내차수 계산
	printf("\n--Topol_sort--\n");

	for (i = 0; i < V; i++)
		if (!net[i].count)	//내차수가 0이면
			Push(i);	//Push (선행작업을 모두함)
	for (i = 0; i < V; i++)
	{
		if (Stack_Empty())
			return -1;	//끝
		else
		{
			j = Pop();	//해야할 작업순서
			printf("%3c ", int2name(j));
			for (ptr = a[j]; ptr != NULL; ptr = ptr->next)
			{
				k = ptr->vertex;	//선행작업의 다음작업에 대해
				net[k].count--;	//내차수 -1
				if (!net[k].count)
					Push(k);	//선행작업이 모두끝났다면
			}
		}
	}
	printf("\n");
}

void set_count_outdegree(Node* a[], topol net[], int V)
{
	printf("\n--set_count_outdegree--\n");
	int i, j;
	int count;
	Node* t;
	for (i = 0; i < V; i++)	//모든노드
	{
		count = 0;
		for (t = a[i]; t != NULL; t = t->next)	//연결된 노드탐색
			count++;	//외차수 증가
		net[i].count = count;
	}

	for (int k = 0; k < V; k++)	//외차수를 출력해 확인
	{
		printf(" %c:%d ", int2name(k), net[k].count);
	}

	printf("\n");
}

int rev_Topol_sort(Node* a[], topol net[], int V)
{
	int i, j, k;
	Node* ptr;
	Init_Stack();
	set_count_outdegree(a, net, V);	//내차수 계산
	printf("\n--rev_Topol_sort--\n");

	for (i = 0; i < V; i++)
		if (!net[i].count)	//내차수가 0이면
			Push(i);	//Push (선행작업을 모두함)
	for (i = 0; i < V; i++)
	{
		if (Stack_Empty())
			return -1;	//끝
		else
		{
			j = Pop();	//해야할 작업순서
			printf("%3c ", int2name(j));
			for (k = 0; k < V; k++) {
				for (ptr = a[k]; ptr != NULL; ptr = ptr->next)
				{
					if (ptr->vertex == j) 
					{
						net[k].count--;	//외차수 -1
						if (!net[k].count)
							Push(k);	//선행작업이 모두끝났다면
					}
				}
			}
		}
	}
	printf("\n");
}


//-----------------Strongly connected component------------------
int Strong_recur(Node* g[], int i)
{
	int m, min, k, flag;	//min == 연결되어있는 최상위노드
	Node* t;
	SCcheck[i] = min = ++SCorder;
	Push(i); //i 노드 Push

	for (t = g[i]; t != NULL; t = t->next)	//i노드와 연결된 노드 탐색
	{
		if (SCcheck[t->vertex] == 0)	//방문안했다면
			m = Strong_recur(g, t->vertex);	//재귀함수실행
		else	//방문했다면
			m = SCcheck[t->vertex];		//해당노드의 순번	
		if (m < min)	//상위노드가 있다면
			min = m;	//변경
	}
	if (min == SCcheck[i])	//하위노드와 비간선연결로 연결되어있다면
	{
		flag = 0;
		while ((k = Pop()) != i)	//i노드부터 하위노드까지
		{
			printf("%3c", int2name(k));	//화면에 출력
			SCcheck[k] = MAX_NODE + 1;
			flag = 1;
		}
		if (flag)
			printf("%3c\n", int2name(k));
	}
	return min;
}

void Strong_Connected(Node* a[], int V)
{
	printf("\n--Strong_Connected--\n");
	int y;
	SCorder = 0;
	for (y = 0; y < V; y++)
	{
		SCcheck[y] = 0;
	}
	for (y = 0; y < V; y++)
	{
		if (SCcheck[y] == 0)	//방문안했을때
			Strong_recur(a, y);	//Strong_recur함수실행
	}
}


int main()
{
	int V, E;
	fp = fopen("graph.txt", "rt");

	//17 18 AB 1 AC 1 AD 1 BE 1 CF 1 DH 1
	//EF 1 FH 1 EG 1 GI 1 HI 1 JK 1 JL 1 MN 1 MO 1 NP 1 NQ 1 OQ 1
	
	//------------for adjacency matrix---------------
	//input_Fadjmatrix(GM, &V, &E);
	//print_adjmatrix(GM, V);

	//-------------for adjacency list-----------------
	//input_Fadjlist(GL, &V, &E);
	//print_adjlist(GL, V);

	//------------------------DFS-----------------------
	//-------adjmatrix--------
	//DFS_adjmatrix(GM, V);
	//nrDFS_adjmatrix(GM, V);

	//--------adjlist---------
	//DFS_adjlist(GL, V);
	//nrDFS_adjlist(GL, V);


	//------------------------BFS-----------------------
	//--------adjmatirx-------
	//BFS_adjmatrix(GM, V);
	//nrBFS_adjmatrix(GM,V);

	//---------adjlist--------
	//recursive is impossible
	//nrBFS_adjlist(GL,V);

	//---------------------Count Components-------------
	//Count_Components_adjmatrix(GM, V);

	//----------------AP searching---------------------
	//DJ JM
	//AP_adjmatrix(GM, V);
	//AP_adjlist(GL, V);
	
	//--------------for PFS searching-------------------
	//11 17 AB 4 AC 1 AD 2 AE 3 CD 2 DF 4 EF 4 BF 4
	//DG 4 GH 3 HI 2 GI 3 GJ 4 IJ 2 JK 1 FJ 2 FK 4

	//printf("\nOriginal graph\n");
	//print_adjmatrix(GM, V);
	//print_adjlist(GL, V);

	//printf("\nVisit order of Minimum Spanning Tree");
	//PFS_adjmatrix(GM, V);
	//PFS_adjlist(GL, V);
	
	//printf("Parents-Sons\n");
	//print_tree(parent,V);
	//printf("\nMinimum Cost is \'%d'\n",get_cost(weighted, V));
	
	//---------------------Kruskal-----------------------
	//input_edge(edge, &V, &E);
	//printf("\n\nVisited edge of minimum spanning tree\n");
	//kruskal(edge, V, E);
	//printf("\n\nMinimum cost is \'%d'\n",cost);

	//---------------Shortest path adjlist---------------
	//start = 5;
	//Shortest_adjmatrix(GM, start, V);
	//Shortest_adjlist(GL, start, V);

	//----------------dijkstra algorithm------------------
	//Dijkstra(GM, 5, V);
	//Print_dijkstra_parent(parent, start, V);

	//-------------------directed graph-------------------
	//-------matrix-------
	//input_dirmatrix(GM, &V, &E);
	//print_adjmatrix(GM, V);
	//--------list--------
	input_dirlist(GL, &V, &E);
	print_adjlist(GL, V);

	//------------DFS_dirlist--------------
	//DFS_dirlist(GL, V);

	//-------------------------Floyd----------------------
	//Copy_matrix(GM, f_distance);
	//Floyd(f_distance, V);
	//print_adjmatrix(f_distance, V);

	//------------------topoligical sort------------------
	//Topol_sort(GL, network, V);
	//rev_Topol_sort(GL, network, V);

	//----------------Strongly connected------------------
	Strong_Connected(GL, V);


	fclose(fp);

	return 0;
}