#include "graph.h"
#include <windows.h>


//注：若要正常运行程序，请注意文件路径字符串变量的设置，相关设置在代码第15、16、22、30、31行。
int main()
{
    ALGraph G;
    LARGE_INTEGER begin;
    LARGE_INTEGER end;	//记录排序开始和结束时的时间
    LARGE_INTEGER freq; //时钟频率
    char *number[] = { "11","12","21","22","31","32","41","42"};     //输入文件的编号
    //  char input[] = "../../../tools/C/5-PB20151734-project4/input/input11.txt";      //初始输入路径
    //  char output[] = "../../../tools/C/5-PB20151734-project4/output/result11.txt";   //初始输出路径
    char input[] = "../input/input11.txt";      //初始输入路径
    char output[] = "../output/result11.txt";   //初始输出路径
    double time[8] = { 0, };    //8次运行的时间数据
    int cnt = 0;    //第几次运行

    //时间文件
    //  FILE *ft=fopen("../../../tools/C/5-PB20151734-project4/output/time.txt","w");   //时间文件路径
    FILE *ft=fopen("../output/time.txt","w");   //时间文件路径
    if (!ft) {
        printf("cannot open file!"); exit(0);
    }

    QueryPerformanceFrequency(&freq);
    for (int i = 0;i < 8;i++) {
        //更改输入、输出路径的编号
        input[14] = output[16] = number[i][0];
        input[15] = output[17] = number[i][1];

        //根据输入文件input创建图
        create_graph(&G, input);
        //消除负环
        //先添加源结点s
        G.vertex = (ALNode**)realloc(G.vertex, sizeof(ALNode*) * (G.vcnt + 1));
        G.vertex[G.vcnt] = (ALNode*)malloc(sizeof(ALNode));
        G.vertex[G.vcnt]->seq = 0;
        G.vertex[G.vcnt]->adj = NULL;
        ALEdge* temp;
        for (int j = 0;j < G.vcnt;j++) {    //s到每个顶点添加一条权为0的边
            temp = (ALEdge*)malloc(sizeof(ALEdge));
            temp->next = G.vertex[G.vcnt]->adj;
            G.vertex[G.vcnt]->adj = temp;
            temp->v = G.vertex[j];
            temp->u = G.vertex[G.vcnt];
            temp->weight = 0;
        }
        G.vcnt += 1;
        printf("%d edges originally.\n", G.ecnt);
        QueryPerformanceCounter(&begin);	//计时开始
        while (1)//不断调用bellman_ford消除负环，直到没有负环
            if (!bellman_ford(&G, 0))
                break;
        QueryPerformanceCounter(&end);		  //计时结束
        printf("Time for deleting negative cycle:%lfms\n",(double)(end.QuadPart - begin.QuadPart) / (double)freq.QuadPart * 1000 / 100); //记录算法用时,单位:ms
        G.vcnt -= 1;    //相当于删除源点s
        printf("%d edges left.\n", G.ecnt);

        //w为最短路径长度的矩阵
        int** w = (int**)malloc(sizeof(int*) * (G.vcnt + 1));
        for (int i = 1;i <= G.vcnt;i++)
            w[i] = (int*)malloc(sizeof(int) * (G.vcnt + 1));
        //打开输出文件
        FILE* fp = fopen(output,"w");   
        if (!fp) {
            printf("cannot open file!");
            exit(0);
        }
        //运行johnson算法，并计时
        QueryPerformanceCounter(&begin);	//计时开始
        johnson(G, w, fp);
        QueryPerformanceCounter(&end);		  //计时结束
        time[cnt++] = (double)(end.QuadPart - begin.QuadPart) / (double)freq.QuadPart * 1000 / 100; //记录算法用时,单位:ms
        fprintf(ft,"%lf\n",time[cnt-1]);    //时间数据写入time.txt
        fclose(fp);     //每轮结束后关闭输出文件

        //在终端输出点对的最短路径
        //for (int i = 1;i <= G.vcnt;i++) {
        //    for (int j = 1;j <= G.vcnt;j++) {
        //        if (w[i][j] != INT_MAX)
        //            printf("%d->%d,%d\n", i, j, w[i][j]);
        //    }
        //}
    }
    fclose(ft);
}