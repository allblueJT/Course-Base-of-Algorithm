#include "graph.h"
#include <windows.h>


//ע����Ҫ�������г�����ע���ļ�·���ַ������������ã���������ڴ����15��16��22��30��31�С�
int main()
{
    ALGraph G;
    LARGE_INTEGER begin;
    LARGE_INTEGER end;	//��¼����ʼ�ͽ���ʱ��ʱ��
    LARGE_INTEGER freq; //ʱ��Ƶ��
    char *number[] = { "11","12","21","22","31","32","41","42"};     //�����ļ��ı��
    //  char input[] = "../../../tools/C/5-PB20151734-project4/input/input11.txt";      //��ʼ����·��
    //  char output[] = "../../../tools/C/5-PB20151734-project4/output/result11.txt";   //��ʼ���·��
    char input[] = "../input/input11.txt";      //��ʼ����·��
    char output[] = "../output/result11.txt";   //��ʼ���·��
    double time[8] = { 0, };    //8�����е�ʱ������
    int cnt = 0;    //�ڼ�������

    //ʱ���ļ�
    //  FILE *ft=fopen("../../../tools/C/5-PB20151734-project4/output/time.txt","w");   //ʱ���ļ�·��
    FILE *ft=fopen("../output/time.txt","w");   //ʱ���ļ�·��
    if (!ft) {
        printf("cannot open file!"); exit(0);
    }

    QueryPerformanceFrequency(&freq);
    for (int i = 0;i < 8;i++) {
        //�������롢���·���ı��
        input[14] = output[16] = number[i][0];
        input[15] = output[17] = number[i][1];

        //���������ļ�input����ͼ
        create_graph(&G, input);
        //��������
        //�����Դ���s
        G.vertex = (ALNode**)realloc(G.vertex, sizeof(ALNode*) * (G.vcnt + 1));
        G.vertex[G.vcnt] = (ALNode*)malloc(sizeof(ALNode));
        G.vertex[G.vcnt]->seq = 0;
        G.vertex[G.vcnt]->adj = NULL;
        ALEdge* temp;
        for (int j = 0;j < G.vcnt;j++) {    //s��ÿ���������һ��ȨΪ0�ı�
            temp = (ALEdge*)malloc(sizeof(ALEdge));
            temp->next = G.vertex[G.vcnt]->adj;
            G.vertex[G.vcnt]->adj = temp;
            temp->v = G.vertex[j];
            temp->u = G.vertex[G.vcnt];
            temp->weight = 0;
        }
        G.vcnt += 1;
        printf("%d edges originally.\n", G.ecnt);
        QueryPerformanceCounter(&begin);	//��ʱ��ʼ
        while (1)//���ϵ���bellman_ford����������ֱ��û�и���
            if (!bellman_ford(&G, 0))
                break;
        QueryPerformanceCounter(&end);		  //��ʱ����
        printf("Time for deleting negative cycle:%lfms\n",(double)(end.QuadPart - begin.QuadPart) / (double)freq.QuadPart * 1000 / 100); //��¼�㷨��ʱ,��λ:ms
        G.vcnt -= 1;    //�൱��ɾ��Դ��s
        printf("%d edges left.\n", G.ecnt);

        //wΪ���·�����ȵľ���
        int** w = (int**)malloc(sizeof(int*) * (G.vcnt + 1));
        for (int i = 1;i <= G.vcnt;i++)
            w[i] = (int*)malloc(sizeof(int) * (G.vcnt + 1));
        //������ļ�
        FILE* fp = fopen(output,"w");   
        if (!fp) {
            printf("cannot open file!");
            exit(0);
        }
        //����johnson�㷨������ʱ
        QueryPerformanceCounter(&begin);	//��ʱ��ʼ
        johnson(G, w, fp);
        QueryPerformanceCounter(&end);		  //��ʱ����
        time[cnt++] = (double)(end.QuadPart - begin.QuadPart) / (double)freq.QuadPart * 1000 / 100; //��¼�㷨��ʱ,��λ:ms
        fprintf(ft,"%lf\n",time[cnt-1]);    //ʱ������д��time.txt
        fclose(fp);     //ÿ�ֽ�����ر�����ļ�

        //���ն������Ե����·��
        //for (int i = 1;i <= G.vcnt;i++) {
        //    for (int j = 1;j <= G.vcnt;j++) {
        //        if (w[i][j] != INT_MAX)
        //            printf("%d->%d,%d\n", i, j, w[i][j]);
        //    }
        //}
    }
    fclose(ft);
}