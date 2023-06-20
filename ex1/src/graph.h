#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//enum Color{
//    WHITE,
//    GRAY,
//    BLACK
//};

typedef struct Node{
    int seq;    //������
    int d;      //���·������
    struct Node *pi;     //���·���е�ǰ��
    //struct Edge* dfs_pi;    //��ȱ����е�ǰ��(��)
    //enum Color color;       
    struct Edge *adj;     //�ڽӱ�
}ALNode;
typedef struct ALGraph{
    ALNode **vertex;
    int vcnt;   //������
    int ecnt;   //����
}ALGraph;
typedef struct Edge{
    struct Edge *next;   //�ڽӱ��е���һ����
    ALNode  *u,*v;     //�ߵ������յ�
    int weight;
}ALEdge;


typedef struct{
    int size;       //��Ԫ�ظ���
    int length;     //����Ԫ�ظ���
    int *order;      //order[i]��¼seqΪi�Ķ���ָ����data�е�����
    ALNode **data;
}heap;

void create_graph(ALGraph *G, char *filename);  //�����ļ���ָ�����ļ�����ͼ
void relax(ALNode *u, ALNode *v, int w);    //�ɳڲ���
void relax_heap(ALNode* u, ALNode* v, int w, heap* h);  //������С�ѵ��ɳڲ���
void _init_single_source(ALGraph *G, int seq);   //��Դ���·��ʱ����ʼ��

int bellman_ford(ALGraph *G, int seq);
void dijkstra(ALGraph *G,int seq);

void init_heap(heap *h,ALGraph *G);     //��ʼ����
void build_heap(heap *h);       //������С��
void min_heapify(heap *h,int i);    //������С��
void decrease_key(heap* h, ALNode* v, int key);     //��Сĳ�ڵ�Ĺؼ���
struct Node *extract_min_heap(heap *h); //��ȡ��СԪ��

//void DFS(ALGraph* G);
//void DFS_rec(ALGraph* G, ALNode* s);

void johnson(ALGraph G, int** w, FILE *fp); //w�洢���·�����ȣ�fpΪ����ļ���ָ��
void _output_path(ALNode* t, FILE* fp);  //���ڵݹ������С·��







