#include "graph.h"

// ���������ڽӱ��ͼ�����ݴ�filenameָ�����ļ��ж�ȡ����ʽĬ����ʵ����Ŀ��˵����һ��
void create_graph(ALGraph *G, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        printf("cannot open file!\n");
    int u, v, w;
    int vcnt, ecnt;
    fscanf(fp,"%d", &vcnt);
    fscanf(fp,"%d", &ecnt);
    printf("vcnt=%d,out-degree=%d\n",vcnt,ecnt);

    G->vcnt = vcnt;
    G->ecnt = ecnt*vcnt;
    G->vertex = (ALNode **)malloc(sizeof(ALNode*) * vcnt);
    for(int i=0;i<vcnt;i++)
        G->vertex[i] = (ALNode*)malloc(sizeof(ALNode));

    ALEdge *temp = NULL;
    for (int i = 0; i < vcnt; i++)
    {
        G->vertex[i]->seq = i + 1;
        G->vertex[i]->adj = NULL;
        for (int j = 0; j < ecnt; j++)
        {
            temp = (ALEdge *)malloc(sizeof(ALEdge));
            temp->next = G->vertex[i]->adj;
            G->vertex[i]->adj = temp; // ͷ�巨�����
            fscanf(fp,"%d%d%d", &u, &v, &w);
            temp->weight = w;
            temp->u = G->vertex[i];
            temp->v = G->vertex[v-1]; // vΪ�����ţ�G->vertex+v-1��Ϊ���v��ָ��
        }
    }
    fclose(fp);
}

// �ɳڲ���
void relax(ALNode *u, ALNode *v, int w)
{
    if(u->d!=INT_MAX)   //���u��d���������
        if (v->d > u->d + w)
        {
            v->d = u->d + w;
            v->pi = u;
        }
}

void relax_heap(ALNode* u, ALNode* v, int w, heap* h)
{//���öѵ��ɳڲ�����Dijkstra�㷨��ʹ��
    if (u->d != INT_MAX)   //���u��d���������
        if (v->d > u->d + w)
        {
            decrease_key(h, v, u->d + w);
           /* v->d = u->d + w;*/
            v->pi = u;
        }
}

//bellman-ford�㷨��ʼ��
void _init_single_source(ALGraph *G, int seq)
{
    for (int i = 0; i < G->vcnt; i++)
    {
        G->vertex[i]->d = INT_MAX;
        G->vertex[i]->pi = NULL;
        if (G->vertex[i]->seq == seq)
            G->vertex[i]->d = 0;
    }
}

// Bellman-ford�㷨
int bellman_ford(ALGraph *G, int seq)
{//�и���-->����1���޸���-->����0
    _init_single_source(G, seq);
    for (int i = 1; i < G->vcnt; i++)
    {

        // �������б�
        for (int j = 0; j < G->vcnt; j++)
        {
            if (j == G->vcnt-1) {
                j=G->vcnt-1;
            }
            for (ALEdge *temp = G->vertex[j]->adj; temp; temp = temp->next)
            {
                relax(G->vertex[j], temp->v, temp->weight);
            }
        }
    }
    // �������б�
    int u = -1, v = -1, flag = 0;
    for (int j = 0; j < G->vcnt; j++)
    {
        for (ALEdge *temp = G->vertex[j]->adj; temp; temp = temp->next)
        {
            if (temp->v->d > G->vertex[j]->d + temp->weight)
            {// ����ζ�Ŵ�Դ��s��vertex[j]�ٵ�v�䣬���ڸ���
                u = G->vertex[j]->seq;
                v = temp->v->seq;
                flag = 1;
                break;
                //return 0; // ���ڸ���
            }
        }
        if (flag)
            break;
    }
    if (flag) {
        int* x = (int*)malloc(sizeof(int) * G->vcnt);
        int cnt = 0, t, index = -1;
        x[cnt++] = v;
        x[cnt++] = t = u;
        while (1) {//����·�����ݣ����Ƿ��ж����ظ�����
            t = G->vertex[t - 1]->pi->seq;     //x[cnt-1]Ϊ���ݵĵ�ǰ�����ţ�vertex[x[cnt-1]-1]Ϊ�ö���ָ��
            for (int i = 0;i < cnt;i++) {
                if (x[i] == t) {    //�ҵ��ظ���㣬������
                    index = i;
                    break;
                }
            }
            if (index != -1) {//x[i]��x[cnt-1]��tΪ����ֻ�轫x[cnt-1]��x[cnt-2]��ı�ɾȥ
                ALNode *u = G->vertex[x[cnt - 1] - 1];
                ALNode *v = G->vertex[x[cnt - 2] - 1];
                ALEdge* d = NULL;
                if (u->adj->v==v) {
                    u->adj = u->adj->next;
                }
                else{
                    for (d = u->adj;d->next->v!=v;d = d->next);
                    d->next = d->next->next;
                }
                G->ecnt -= 1;
                break;
            }
            x[cnt++] = t;
        }
        return 1;
    }
    return 0;
}

void dijkstra(ALGraph *G,int seq)
{
    _init_single_source(G,seq);
    heap Q;
    init_heap(&Q,G);
    build_heap(&Q);
    ALNode *u;
    while(Q.size!=0){
        u=extract_min_heap(&Q);
        // ������u���������б�
        for (ALEdge *temp = u->adj; temp; temp = temp->next)
        {
            relax_heap(u,temp->v,temp->weight,&Q);
        }
    }
}

//��ͼG�Ķ����ʼ����
void init_heap(heap *h,ALGraph *G)
{
    h->length=G->vcnt;
    h->data=(ALNode**)malloc(sizeof(ALNode*)*(h->length+1));
    h->order = (int*)malloc(sizeof(int) * (h->length + 1));
    h->size=0;
    for(int i=1;i<=h->length;i++){
        h->order[i] = i;        //��G->vertex[i-1]->seq
        h->data[i]=G->vertex[i-1];
    }
}
void build_heap(heap *h)
{
    h->size=h->length;
    for(int i=h->size/2;i>=1;i--){
        min_heapify(h,i);
    }
}
void min_heapify(heap *h,int i)
{
    int l=2*i;
    int r=2*i+1;
    int smallest;
    if(l<=h->size && h->data[l]->d<h->data[i]->d)
        smallest=l;
    else smallest=i;
    if(r<=h->size && h->data[r]->d<h->data[smallest]->d)
        smallest=r;
    if (smallest!=i){
        ALNode* temp=h->data[i];
        int t = h->order[i];
        h->data[i]=h->data[smallest];
        h->order[i] = h->order[smallest];
        h->data[smallest]=temp;
        h->order[smallest] = t;
        min_heapify(h,smallest);
    }
    
}
void decrease_key(heap* h, ALNode* v, int key)
{
    if (key > v->d) {
        printf("error!");
        exit(0);
    }
    v->d = key;
    int i = h->order[v->seq];
    while (i > 1 && h->data[i / 2]->d > h->data[i]->d) {
        ALNode* temp=h->data[i];
        h->data[i] = h->data[i / 2];
        h->data[i / 2] = temp;
        h->order[h->data[i]->seq] = i / 2;
        h->order[h->data[i / 2]->seq] = i;
        i /= 2;     //��i=PARENT(i)
    }
}
ALNode *extract_min_heap(heap *h)
{
    if(h->size<1){
        printf("error!");
        exit(0);
    }
    ALNode *min=h->data[1];
    h->data[1]=h->data[h->size];
    h->order[h->data[h->size]->seq] = 1;    //ͬʱ����order
    h->size-=1;
    min_heapify(h,1);
    return min;
}

////��DFS���ж��Ƿ��и���
//void DFS(ALGraph* G)
//{
//    for (int i = 0;i < G->vcnt;i++)
//        G->vertex[i]->dfs_pi = NULL;
//    for (int i = 0;i < G->vcnt;i++) {
//        if (G->vertex[i]->color == WHITE)
//            DFS_rec(G, G->vertex[i]);
//    }
//    printf("%d edges left.\n", G->ecnt);
//}
////�ݹ�
//void DFS_rec(ALGraph* G, ALNode* s)
//{
//    s->color = GRAY;
//    for (ALEdge* temp = s->adj;temp;temp=temp->next) {
//        if (temp->v->color == WHITE) {
//            temp->v->dfs_pi = temp;
//            DFS_rec(G, temp->v);
//        }
//        else if (temp->v->color == GRAY) {      //�л�
//            //�ж��Ƿ�Ϊ����
//            ALNode* cur=NULL;
//            int sum = temp->weight;
//            for (ALEdge* t = temp; t->u!=temp->v;) {    //���ϻ��ݣ����ۼ�Ȩ�أ����ٴε����ظ��Ľ��temp->vʱ��sum��Ϊ��·��Ȩ�غ�
//                cur = t->u;
//                t = cur->dfs_pi;
//                sum += t->weight;
//            }
//            if (sum < 0) {      //������ɾ����temp
//                ALEdge* r = s->adj;
//                if (s->adj == temp) {
//                    s->adj = temp->next;
//                }
//                else {
//                    while (r->next != temp)
//                        r = r->next;
//                    r->next = temp->next;
//                }
//                G->ecnt -= 1;      //������1
//                //r = temp;
//                //temp = temp->next;
//                //free(r);
//            }
//        }
//    }
//    s->color = WHITE;
//}

void johnson(ALGraph G,int** w,FILE *fp)
{
    //���Դ���s
    G.vertex = (ALNode**)realloc(G.vertex, sizeof(ALNode*) * (G.vcnt + 1));
    G.vertex[G.vcnt] = (ALNode*)malloc(sizeof(ALNode));
    G.vertex[G.vcnt]->seq = 0;
    G.vertex[G.vcnt]->adj = NULL;
    
    ALEdge* temp;
    for (int i = 0;i < G.vcnt;i++) {
        temp = (ALEdge*)malloc(sizeof(ALEdge));
        temp->next = G.vertex[G.vcnt]->adj;
        G.vertex[G.vcnt]->adj = temp;
        temp->v = G.vertex[i];
        temp->u = G.vertex[G.vcnt];
        temp->weight = 0;
    }
    G.vcnt += 1;

    if (bellman_ford(&G, 0) == 1) {
        printf("Negative Cycle!");
        exit(0);
    }
    else {
        //d�����bellman-ford�õ���dֵ(��Ϊdijkstra�㷨�����dֵ)
        int* d = (int*)malloc(sizeof(int*) * G.vcnt);
        for (int i = 1;i < G.vcnt;i++) {
            d[i] = G.vertex[i - 1]->d;    //d[1,...,n]Ϊԭͼ��sԴ���·������
        }
        //������Ȩ��
        for (int i = 0;i < G.vcnt;i++) {
            for (ALEdge* temp = G.vertex[i]->adj;temp;temp = temp->next){
                temp->weight = temp->weight + temp->u->d - temp->v->d;
            }
        }
        
        //wΪ�����صĽ���������������·������    ע��G.vcnt-1Ϊԭͼ�Ľ����
        for (int i = 1;i <= G.vcnt - 1;i++) {
            for (int j = 1;j <= G.vcnt - 1;j++)
                w[i][j] = INT_MAX;
        }
        //��ԭͼ��ÿ����㣬����dijkstra�㷨
        for (int i = 1;i < G.vcnt;i++) {
            dijkstra(&G, i);
            for (int j = 1;j < G.vcnt;j++) {
                if (G.vertex[j - 1]->d == INT_MAX) {
                    w[i][j] = INT_MAX;
                    fprintf(fp, "(%d-/->%d),Not Connected\n", G.vertex[i - 1]->seq, G.vertex[j - 1]->seq);
                    continue;
                }               
                w[i][j] = G.vertex[j - 1]->d + d[j] - d[i];   //��α������d[u][v]=��(u,v)+h(v)-h(u)
                fprintf(fp, "(");
                _output_path(G.vertex[j - 1], fp);
                fprintf(fp, " %d)\n", w[i][j]);
            }
        }
    }
}
void _output_path(ALNode* t, FILE* fp)
{
    //���·��
    if (t->pi) {
        _output_path(t->pi, fp);
        fprintf(fp,",%d", t->seq);
    }
    else {
        fprintf(fp, "%d", t->seq);
    }
    
}

//����1���ɳ�ʱ��u->d+w���