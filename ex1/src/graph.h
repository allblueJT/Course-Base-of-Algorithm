#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//enum Color{
//    WHITE,
//    GRAY,
//    BLACK
//};

typedef struct Node{
    int seq;    //结点序号
    int d;      //最短路径估计
    struct Node *pi;     //最短路径中的前驱
    //struct Edge* dfs_pi;    //深度遍历中的前驱(边)
    //enum Color color;       
    struct Edge *adj;     //邻接表
}ALNode;
typedef struct ALGraph{
    ALNode **vertex;
    int vcnt;   //顶点数
    int ecnt;   //边数
}ALGraph;
typedef struct Edge{
    struct Edge *next;   //邻接表中的下一条边
    ALNode  *u,*v;     //边的起点和终点
    int weight;
}ALEdge;


typedef struct{
    int size;       //堆元素个数
    int length;     //数组元素个数
    int *order;      //order[i]记录seq为i的顶点指针在data中的索引
    ALNode **data;
}heap;

void create_graph(ALGraph *G, char *filename);  //根据文件名指定的文件创建图
void relax(ALNode *u, ALNode *v, int w);    //松弛操作
void relax_heap(ALNode* u, ALNode* v, int w, heap* h);  //基于最小堆的松弛操作
void _init_single_source(ALGraph *G, int seq);   //求单源最短路径时，初始化

int bellman_ford(ALGraph *G, int seq);
void dijkstra(ALGraph *G,int seq);

void init_heap(heap *h,ALGraph *G);     //初始化堆
void build_heap(heap *h);       //建立最小堆
void min_heapify(heap *h,int i);    //调整最小堆
void decrease_key(heap* h, ALNode* v, int key);     //减小某节点的关键字
struct Node *extract_min_heap(heap *h); //提取最小元素

//void DFS(ALGraph* G);
//void DFS_rec(ALGraph* G, ALNode* s);

void johnson(ALGraph G, int** w, FILE *fp); //w存储最短路径长度，fp为输出文件的指针
void _output_path(ALNode* t, FILE* fp);  //用于递归输出最小路径







