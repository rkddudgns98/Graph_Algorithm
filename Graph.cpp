#include <stdio.h>
#include <windows.h>
#pragma warning(disable: 4996)

#define MAX_NODE 100
#define MAX_EDGE 100
#define ONLY_FIND 0
#define UNION 1
#define UNSEEN (-INT_MAX)

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

//DFS
int check[MAX_NODE] = { 0, };	//방문했는지 확인
int stack[MAX_NODE] = { 0, };	
int top = -1;

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


//------------------------Stack-----------------------------
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


//-----------------------adjmatrix----------------------------
int GM[MAX_NODE][MAX_NODE];


void input_adjmatrix(int a[][MAX_NODE], int* V, int* E)
{
	char vertex[3];
	int i, j, k;
	printf("Input number of node & edge\n");
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


//------------------------adjlist----------------------------
typedef struct Node {
	int vertex;
	int weight;
	struct Node* next;
}Node;

//Node
Node* GL[MAX_NODE];

void input_adjlist(Node* a[], int* V, int* E)
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

void print_adjlist(Node* a[], int V)
{
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


//----------------------Find AP----------------------------
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


//----------------------PFS--------------------------------
Node* G[MAX_NODE];

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
	printf("\n");
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
				printf(" --> %c", int2name(i));

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

	printf("\n\n");
}

//print parents and son
void print_tree(int p[],int V)
{
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

void print_cost(int w[],int V)
{
	int cost = 0;
	//최소비용은 weight를 모두 더한 값
	for (int i = 1; i < V; i++)
	{
		cost += w[i];
	}
	printf("\'%d'\n", cost);
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

//input edge ( v1,v2,weight)
void input_edge(Edge e[], int* V, int* E)
{
	char vertex[3];
	int i, j, w;

	printf("\nInput number of nodes and edges\n"); 
	printf("Input two nodes consisting of edge and its weight");
	fscanf(fp, "%d %d", V, E);
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
void find_init(int elem)
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
	find_init(V);
	pq_init();
	for (n = 0; n < E; n++)
		pq_insert(heap, n);
	n = 0; 
	while (!pq_empty())
	{
		val = pq_Kextract(heap);
		if (find_set(e[val].v1, e[val].v2, UNION))
		{ 
			visit(val); 
			n++;
		} 
		if (n == V - 1) 
			break; 
	}
}

int main()
{
	int V, E;
	fp = fopen("graph.txt", "rt");

	/*
	//------------for adjacency matrix---------------
	//input_adjmatrix(GM, &V, &E);
	//print_adjmatrix(GM, V);

	//-------------for adjacency list-----------------
	//17 18 AB AC AD BE CF DH EF FH EG GI HI JK JL MN MO NP NQ OQ
	//input_adjlist(GL, &V, &E);
	//print_adjlist(GL, V);


	//--------traversse the given graph----------------
	//DFS_adjlist(GL, V);
	//nrDFS_adjlist(GL, V);

	//----------------AP searching---------------------
	//AP_search(GL, V);
	
	

	//--------------for PFS searching-------------------
	//11 17 AB 4 AC 1 AD 2 AE 3 CD 2 DF 4 EF 4 BF 4
	//DG 4 GH 3 HI 2 GI 3 GJ 4 IJ 2 JK 1 FJ 2 FK 4
	input_adjlist(G, &V, &E);
	
	printf("\nOriginal graph\n");
	print_adjlist(G, V);

	printf("\nVisit order of Minimum Spanning Tree");
	PFS_adjlist(G, V);
	
	print_tree(parent,V);
	printf("\nMinimum Cost is ");
	print_cost(weighted,V);
	*/

	input_edge(edge, &V, &E);
	printf("\n\nVisited edge of minimum spanning tree\n");
	kruskal(edge, V, E);
	printf("\n\nMinimum cost is \'%d'\n",cost);

	fclose(fp);


	return 0;
}