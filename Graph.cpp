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
int check[MAX_NODE] = { 0, };	//�湮�ߴ��� Ȯ��

//AP (Articulation Point)
int APcheck[MAX_NODE] = { 0, };	//�湮����, Spannig tree
int APorder = 0;	//����
int son_of_root = 0;	//�ڽĳ���� ����

//heap
int nheap = 0;
int heap[MAX_NODE] = { 0, };
int weighted[MAX_NODE] = { 0, };
int parent[MAX_NODE] = { 0, };

//Kruskal
int cost = 0;


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
		a[i][i] = 0;	//�밢���
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
			a[i][j] = 0;	//2D array
		}
	}
	for (i = 0; i < *V; i++)
	{
		a[i][i] = 1;	//�밢���
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
		printf("%5c", int2name(i));		//ù��(A,B,C,D ...)
	}

	printf("\n");

	for (int i = 0; i < V; i++)
	{
		printf("%5c", int2name(i));	//ùĭ
		for (int j = 0; j < V; j++)
		{
			printf("%5d", a[i][j]);//matrix���
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

	char vertex[3];	//�Է¹��� ���ڿ� ����

	int i, j, w;	//w= weight

	//���� ������ �����Է¹ޱ�
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);

	//��� ������ Ÿ�� �迭 �ʱ�ȭ (�����Ͷ� NULL�� �ʱ�ȭ)
	for (i = 0; i < *V; i++)
	{
		a[i] = NULL;
	}

	printf("Input two node consist of edge & weight");

	for (int j = 0; j < *E; j++)
	{
		fscanf(fp,"%s %d", vertex, &w);	//AB�� �Է¹ް�,AC�� �Է¹ް�
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//������
		t->vertex = name2int(vertex[1]);	//B�� �־��ְ�, C���־��ְ�
		t->next = a[i];	//B->NULL�� ���ش�, C->B�����ش�
		a[i] = t;	//A->B, A->C
		t->weight = w;	//input weight

		//symetric(��Ī)
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

	char vertex[3];	//�Է¹��� ���ڿ� ����

	int i, j, w;	//w= weight

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
		printf("Input two node consist of edge & weight");
		scanf("%s %d", vertex, &w);	//AB�� �Է¹ް�,AC�� �Է¹ް�
		i = name2int(vertex[0]);	//A-> ,A->
		t = (Node*)malloc(sizeof(Node));	//������
		t->vertex = name2int(vertex[1]);	//B�� �־��ְ�, C���־��ְ�
		t->next = a[i];	//B->NULL�� ���ش�, C->B�����ش�
		a[i] = t;	//A->B, A->C
		t->weight = w;	//input weight

		//symetric(��Ī)
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

		printf("%3c", int2name(i));	//A,B,C,D ����������

		for (t = a[i]; t != NULL; t = t->next)
		{
			//t�� ->vertex�� ����Ѵ�.
			printf(" -> %c:%d ", int2name(t->vertex), t->weight);
		}
		//�ٹٲ�
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
	for (int i = 0; i < V; i++)	//check �ʱ�ȭ
		check[i] = 0;

	for (int i = 0; i < V; i++)
	{
		if (check[i] == 0)	//�湮��������
		{
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
	Init_Queue();	//queue�迭 �ʱ�ȭ
	for (i = 0; i < V; i++)	//��� �湮����
		check[i] = 0;
	for (i = 0; i < V; i++)	//��ü��忡 ���ؼ�
	{
		if (check[i] == 0)	//�湮�� ���ߴٸ�
		{
			Put(i);	//queue put
			check[i] = 1;	//�湮üũ
			while (!Queue_Empty())	//queue�� �����ʾ�����
			{
				i = Get();	//queue get
				printf("%3c", int2name(i));	//�湮������ ���
				for (j = 0; j < V; j++)	//��ü��忡����
					if (a[i][j] != 0)	//����Ǿ��ְ�
						if (check[j] == 0)	//�湮���ߴٸ�
						{
							Put(j);	//queue put
							check[j] = 1;	//�湮������ üũ
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

	//����� ���� �˻�
	for (int v = V-1; v >= 0; v--)
	{
		if (a[i][v] == 1)
		{
			//Root�θ𿡴��� t->vertex��°�� �ڽ��� �ִٸ�
			if (i == 0 && APcheck[v] == 0)
				son_of_root++;	//�ڽ��� ���� ++

			//tree-edge
			if (APcheck[v] == 0)	//�湮�ߴ°�
			{
				//m=����� ����� �ֻ������
				m = AP_recur_adjmatrix(a, v, V);

				if (m <= min)
					min = m;

				//m>=APcheck[i]�̸� i���� �������� ����Ǿ��ִ�
				if (m >= APcheck[i] && i != 0)	//AP�̸� *ǥ��
					printf(" * %c %3d : %d\n", int2name(i), APcheck[i], m);
				else //AP�� �ƴϸ�
					printf(" %3c %3d : %d\n", int2name(i), APcheck[i], m);
			}
			//non-tree-edge
			else if (APcheck[v] < min)
				min = APcheck[v];	//min�� �缳��
		}
	}

	return min;
}

void AP_adjmatrix(int a[][MAX_NODE], int V)
{
	printf("\n--AP_search(adjmatrix)--\n");
	int i, n = 0;

	for (i = 0; i < V; i++)//�湮����	
		APcheck[i] = 0;

	APorder = son_of_root = 0;

	AP_recur_adjmatrix(a, 0, V);	//0, 'A'���� ����

	if (son_of_root > 1) //�ڽ��� ������, �� AP�̸� *ǥ��
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

	//����� ���� �˻�
	for (t = a[i]; t != NULL; t = t->next)
	{
		//Root�θ𿡴��� t->vertex��°�� �ڽ��� �ִٸ�
		if (i == 0 && APcheck[t->vertex] == 0) 
			son_of_root++;	//�ڽ��� ���� ++

		//tree-edge
		if (APcheck[t->vertex] == 0)	//�湮�ߴ°�
		{
			//m=����� ����� �ֻ������
			m = AP_recur_adjlist(a, t->vertex);

			if (m < min)
				min = m;

			//m>=APcheck[i]�̸� i���� �������� ����Ǿ��ִ�
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

void AP_adjlist(Node* a[], int V) 
{
	printf("\n--AP_search(adjlist)--\n");
	int i, n = 0; 
	Node* t;

	for (i = 0; i < V; i++)//�湮����	
		APcheck[i] = 0; 

	APorder = son_of_root = 0; 

	AP_recur_adjlist(a, 0);	//0, 'A'���� ����

	if (son_of_root > 1) //�ڽ��� ������, �� AP�̸� *ǥ��
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
	nheap = 0;	//���� ���� = 0
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

//heap ����
void adjust_heap(int h[], int n)
{
	int k;
	for (k = n / 2; k >= 1; k--)
		downheap(h, k);
}

//heap ����
int pq_extract(int h[])
{
	int v = h[1];	//v�� heap queue�� ù index ����

	h[1] = h[nheap--];	//�������׸��� ù index��
	downheap(h, 1);	//����

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
		h[++nheap] = v;	//1����, v=index
		weighted[v] = p; //store the weight
		upheap(h, nheap);	//����
		return 1;	//������Ʈ ����
	}
	else
	{
		if (weighted[v] < p)	//Fringe Node
		{
			weighted[v] = p;	//�켱���� ����
			adjust_heap(h, nheap);	//�� ����
			return 1;	//������Ʈ ����
		}
		else
			return 0;	//������Ʈ �� �� ����
	}
}

void PFS_adjlist(Node* g[], int V)
{
	printf("\n--PFS_adjlist--\n");
	int i;
	Node* t;
	pq_init();	//���� ���� = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //����ġ = UNSEEN, �켱������ ����
		parent[i] = 0; //�θ���
	}

	for (i = 0; i < V; i++)
	{
		if (weighted[i] == UNSEEN)	//�湮 ��������
		{
			parent[i] = -1;
			pq_update(heap, i, UNSEEN); //�� ������(����ġ)

			while (!pq_empty())	//���� �����Ͱ� �����Ҷ�
			{
				print_heap(heap);	//heap���(index:weighted)
				i = pq_extract(heap);	//���� = ��������
				weighted[i] = -weighted[i];	//����� �ٲ���
				printf(" --> %c:%d\n", int2name(i), weighted[i]);

				for (t = g[i]; t != NULL; t = t->next)
				{
					if (weighted[t->vertex] < 0) //UNSEEN or Fringe
						//����ġ�� ������Ʈ �Ǹ�
						if (pq_update(heap, t->vertex, -t->weight))
							parent[t->vertex] = i;	//�θ�� i�̴�
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
	pq_init();	//���� ���� = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //����ġ = UNSEEN, �켱������ �ֻ�
		parent[i] = 0; //�θ���
	}

	for (i = 0; i < V; i++)
	{
		if (weighted[i] == UNSEEN)	//�湮 ��������
		{
			parent[i] = -1;
			pq_update(heap, i, UNSEEN); //�� ������(����ġ)

			while (!pq_empty())	//���� �����Ͱ� �����Ҷ�
			{
				print_heap(heap);	//heap���(index:weighted)
				i = pq_extract(heap);	//���� = ��������
				weighted[i] = -weighted[i];	//����� �ٲ���
				printf(" --> %c:%d\n", int2name(i), weighted[i]);

				for (int v = V; v >= 0; v--)
				{
					if (a[i][v] > 0) {
						if (weighted[v] < 0) //UNSEEN or Fringe
							//����ġ�� ������Ʈ �Ǹ�
							if (pq_update(heap, v, -a[i][v]))
								parent[v] = i;	//�θ�� i�̴�
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
	//node�� ������ŭ ���ĺ� �������
	printf("Son:    ");
	for (int i = 0; i < V; i++)
		printf("%c ", int2name(i));	

	//�ش� ����� �θ� ��� ���(ù��° ���� .)
	printf("\nParent: . ");
	for (int i = 1; i < V; i++)
		printf("%c ", int2name(p[i]));

	printf("\n");
}

int get_cost(int w[],int V)
{
	int cost = 0;
	//�ּҺ���� weight�� ��� ���� ��
	for (int i = 1; i < V; i++)	//�Ѹ����� ����
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
	int v = h[1];	//v�� heap queue�� ù index ����

	h[1] = h[nheap--];	//�������׸��� ù index��
	downKheap(h, 1);	//����

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

//�θ��� -1�� �ʱ�ȭ
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
{	//elem = �ڽ�, asso = �θ�
	int i = elem, j = asso; 
	while (parent[i] >= 0)
		i = parent[i]; 
	while (parent[j] >= 0) 
		j = parent[j]; 
	//�θ� �ٸ��ٸ�(�ٸ� �׷��̶��)
	if (flag == UNION && i != j) 
		union_set(i, j);//�θ�� ����
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
		val = pq_Kextract(heap);	//heap�� index�� ���ĺ�, ���ļ����� weight
		if (find_set(e[val].v1, e[val].v2, UNION))	//�θ� ���ٸ� �ٸ���Ʈ�� ����
		{ 
			visit(val); 
			n++;
		} 
		if (n == V - 1) //�ּ� V�� ��忡���� V-1������ �־�� ��ġ����
			break; 
	}
}


//----------------------Shortest_adjlist------------------------
void Shortest_adjlist(Node* g[], int start, int V)
{
	printf("\n--Shortest_adjlist--\n");
	int i;
	Node* t;
	pq_init();	//���� ���� = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //����ġ = UNSEEN, �켱������ ����
		parent[i] = 0; //�θ���
	}
	i = start;	//���۳��

	if (weighted[i] == UNSEEN)	//�湮 ��������
	{
		parent[i] = -1;
		pq_update(heap, i, 0); //�� ������(����ġ)

		while (!pq_empty())	//���� �����Ͱ� �����Ҷ�
		{
			print_heap(heap);	//heap���(index:weighted)
			i = pq_extract(heap);	//���� = ��������
			weighted[i] = -weighted[i];	//����� �ٲ���
			printf(" --> %c to %c:%d\n", int2name(start), int2name(i), weighted[i]);

			for (t = g[i]; t != NULL; t = t->next)
			{
				if (weighted[t->vertex] < 0) //UNSEEN or Fringe
					//����ġ(����)�� ������Ʈ �Ǹ�
					if (pq_update(heap, t->vertex, -t->weight - weighted[i]))
						parent[t->vertex] = i;	//�θ�� i�̴�
			}
		}
	}
	printf("\n");
}
void Shortest_adjmatrix(int a[][MAX_NODE], int start, int V)
{
	printf("\n--Shortest_adjmatrix--\n");
	int i;
	pq_init();	//���� ���� = 0

	//Init
	for (i = 0; i < V; i++)
	{
		weighted[i] = UNSEEN; //����ġ = UNSEEN, �켱������ �ֻ�
		parent[i] = 0; //�θ���
	}
	i = start;
	if (weighted[i] == UNSEEN)	//�湮 ��������
	{
		parent[i] = -1;
		pq_update(heap, i, 0); //�� ������(����ġ)

		while (!pq_empty())	//���� �����Ͱ� �����Ҷ�
		{
			print_heap(heap);	//heap���(index:weighted)
			i = pq_extract(heap);	//���� = ��������
			weighted[i] = -weighted[i];	//����� �ٲ���
			printf(" --> %c to %c:%d\n", int2name(start), int2name(i), weighted[i]);

			for (int v = V; v >= 0; v--)
			{
				if (a[i][v] > 0) {
					if (weighted[v] < 0) //UNSEEN or Fringe
						//����ġ�� ������Ʈ �Ǹ�
						if (pq_update(heap, v, -a[i][v] - weighted[i]))
							parent[v] = i;	//�θ�� i�̴�
				}
			}
		}
	}
	printf("\n");
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
	//Shortest_adjmatrix(GM, 5, V);
	//Shortest_adjlist(GL, 5, V);


	fclose(fp);

	return 0;
}