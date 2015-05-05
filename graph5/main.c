#include <stdio.h>
#include <stdlib.h>

#define MaxVerNum 20
#define DataType int


typedef struct Node //边节点
{
       int adjvex;  //这条边邻接顶点元素，其实是图中顶点节点数组的下标
       int info;    //这条边的权值
       struct Node * next;  //这条边后的下一条边
}EdgeNode;

typedef struct  //顶点节点
{
       int vertex;  //顶点元素的名称，这里用的是int型，最好用char型或string型
       EdgeNode * firstedge;  //该顶点所连接的第一条边
}VertexNode;

typedef struct  //图节点
{
       VertexNode adjlist[MaxVerNum];  //顶点节点数组，由此可见关键的还是顶点
       int Vnum, Enum;
}AlGraph, * Al;

typedef struct No  //队列节点
{
       DataType data;
       struct No * next;
}QNode;

typedef struct  //队列
{
       QNode * front;
       QNode * rear;
}LQueue;

typedef struct
{
       int fromVex;
       int endVex;
       int cost;
}EdgeType;

int visited[MaxVerNum];
int mark[MaxVerNum][MaxVerNum];



Al Create();
void DFSAL(AlGraph * G, int i);
void DFSTraverseAL(AlGraph * G);
LQueue * Init_LQueue();
void In_LQueue(LQueue * q, DataType x);
DataType Out_LQueue(LQueue * q);
void BFSM(AlGraph * G, int k);
void BFSTraverseAL(AlGraph * G);
void Prim(AlGraph * G);
void Edge(AlGraph * G, EdgeType edges[]);
int Find(int father[], int v);
void Kruskal(AlGraph * G, EdgeType edges[]);


Al Create()
{
       int i, j, k;
       AlGraph * G;
       G=(AlGraph *)malloc(sizeof(AlGraph));
       EdgeNode * s, * t;

       printf("Please input the number of vertex and edge(split with blank space,like15 18):");
       scanf("%d %d",&G->Vnum,&G->Enum);
       printf("Please input the info of vertexs(split with blank space):");
       for(i = 0; i < G->Vnum; i++)
       {
              scanf("%d",&G->adjlist[i].vertex);
              G->adjlist[i].firstedge = NULL;   //避免指针随意指向
       }

       printf("Please input the info of edges(split with blank space,like 1 2 10)\n");
       //FILE *input = fopen("data_notContinuous.txt", "r");
       FILE *input = fopen("data_notContinuous.txt", "r");
       for(k = 0; k < G->Enum; k++)
       {
              s=(EdgeNode *)malloc(sizeof(EdgeNode));
              t=(EdgeNode *)malloc(sizeof(EdgeNode));
              fscanf(input,"%d %d %d",&i,&j,&s->info);
              //scanf("%d %d %d",&i,&j,&s->info);
              //s->adjvex = j-1;      //这里还需要一个查找函数，来确定邻接顶点的下标，不能简单地j-1
              s->adjvex=searchIndex(j,G);
              s->next = G->adjlist[searchIndex(i,G)].firstedge;
              G->adjlist[searchIndex(i,G)].firstedge = s;

              t->info = s->info;
              t->adjvex = searchIndex(i,G);
              t->next = G->adjlist[searchIndex(j,G)].firstedge;
              G->adjlist[searchIndex(j,G)].firstedge = t;
       }
       fclose(input);
       return G;
}

int searchIndex(int n,AlGraph * G)
{
    int i=0;
    for(;i<G->Vnum;i++)
    {
        if(G->adjlist[i].vertex==n)
        {
            return i;
        }
    }
}
void DFSAL(AlGraph * G, int i)
{
       EdgeNode * p;       //这里仅仅是声明了一个边节点，由于后面该指针直接指向G->adjlist[i].firstedge,所以无需malloc
       printf("%d  ",G->adjlist[i].vertex);
       visited[i] = 1;
       p = G->adjlist[i].firstedge;
       while(p)
       {
              if(!visited[p->adjvex])
                     DFSAL(G, p->adjvex);
              p = p->next;
       }
}


void DFSTraverseAL(AlGraph * G)
{
       int i;
       for(i = 0; i< G->Vnum; i++)
              visited[i] = 0;
        printf("Depth_First Search\n");
       DFSAL(G, 0);
}


LQueue * Init_LQueue()                                        //初始化队列
{
       LQueue * q;
       QNode * p;   //p本身在栈中，但它所指向的内容在堆中
       q=(LQueue *)malloc(sizeof(LQueue));
       p=(QNode *)malloc(sizeof(QNode));
       p->next = NULL;
       q->front = q->rear = p;
       return q;
}

void In_LQueue(LQueue * q, DataType x)                       //进队列
{
       QNode * p;
       p=(QNode *)malloc(sizeof(QNode));
       p->data = x;
       p->next = NULL;
       q->rear->next = p;  //先把新加的节点p加上
       q->rear = p;      //再将队尾元素指向新加的节点
}

DataType Out_LQueue(LQueue * q)                               //出队列
{
       DataType i;
       QNode * p;
       p = q->front->next;
       q->front->next = p->next;  //队列出去一个元素后进行的自我调整
       i = p->data;
       if(q->front->next == NULL)  //判断队列是否为空
              q->rear = q->front;
       return i;
}

void BFSTraverseAL(AlGraph * G)
{
       int i,j;
       EdgeNode *l;
       LQueue *q;
       for(i = 0; i < G->Vnum; i++)  //刷新一下全局变量，全部置为0
              visited[i] = 0;
       printf("\nBreadth_First Search\n");

       q=Init_LQueue();   //首先得有一个队列，内容为空
       for(i=0;i<G->Vnum;i++) //对图中的每一个vertext都要进行测试，测试其是否已经访问过
       {
           if(!visited[i])
           {
               visited[i]=1;
               In_LQueue(q,i);    //先设置其已被访问过，再让其进队列
               while(q->front!=q->rear)  //只要队列不为空
               {
                   j=Out_LQueue(q);
                   //printf("%d  ",j+1);
                   printf("%d  ",G->adjlist[j].vertex);
                   for(l=G->adjlist[j].firstedge;l!=NULL;l=l->next) //将这个顶点的所有相邻边列出，也即将它的下一层的所有顶点全部列出来
                   {
                       if(!visited[l->adjvex])   //如果它下一层的某个顶点未被访问过..
                       {
                           visited[l->adjvex]=1;
                           In_LQueue(q,l->adjvex);
                       }
                   }  //至此，该顶点下一层的所有顶点均被收入队列中
               }      //至此，实际上已经完成图的遍历
           }
       }
}

void Prim(AlGraph * G)
{
       int lowcost[MaxVerNum], mincost;    //最小代价的边的长度，如果不是邻接的边则置为1000，已经加入的边设为0
       int closevex[MaxVerNum];   //邻接顶点的标号，也就是那个顶点的名字，只有显示只用，算法中用不到
       int i, j, k;
       EdgeNode * p;
       p = G->adjlist[0].firstedge;  //第一个顶点随意取，这里取第一个
       for(i = 1; i < G->Vnum; i++)
       {
              lowcost[i] = 1000;  //除了第一个点外，其他都不是邻接边
              closevex[i] = G->adjlist[0].vertex;
       }
       while(p)
       {
              //lowcost[searchIndex(p->adjvex,G)] = p->info;  //先将第一个顶点的所有邻接边的代价找出,p->adjvex本来就是下标，吸取教训
              lowcost[p->adjvex]=p->info;
              //printf("%d--%d\n",p->adjvex,p->info);  //首先看看第一个顶点的各个邻接边的代价是否正确得出
              p = p->next;
       }
       lowcost[0] = 0;
       /*printf("\n*********************test******************\n");
       for(i=0;i<G->Vnum;i++)
       {
           printf("%d  ",lowcost[i]);
       }
       printf("\n*********************test******************\n");*/
       closevex[0] = G->adjlist[0].vertex;
       for(i = 1; i < G->Vnum; i++)  //因为有n-1个节点需要加入，所以需要进行n-1次循环
       {
              mincost = 1000;
              j = 1;
              k = 1;
              while(j < G->Vnum)  //整个这个循环用来求这一步从U到V-U最小代价的边的下标
              {
                     if(lowcost[j] != 0 && lowcost[j] < mincost)
                     {
                        mincost = lowcost[j];
                            k = j;
                     }
                     j++;
              }
              printf("(%d,%d)",G->adjlist[k].vertex,closevex[k]);    //打印的语句，从新添加的节点到它所连接的节点
              lowcost[k] = 0;
              p = G->adjlist[k].firstedge;
              while(p)
              {
                     if(p->info < lowcost[p->adjvex])
                     {
                            lowcost[p->adjvex] = p->info;
                            closevex[p->adjvex] = G->adjlist[k].vertex;
                     }
                     p = p->next;
              }

       }
}


void Edge(AlGraph * G, EdgeType edges[])
{
       EdgeNode * p;
       int k, m = 1000, i, j, q;

       for(i = 0; i < G->Vnum; i++)
       {
              for(j = 0; j < G->Vnum; j++)
              {
                     mark[i][j] = 0;
              }
       }

       for(j = 0; j < G->Enum; j++)
       {
              for(i = 0; i < G->Vnum; i++)
              {
                     p = G->adjlist[i].firstedge;
                     while(p)
                     {
                            if(mark[i][p->adjvex] == 0 && mark[p->adjvex][i] == 0 && p->info < m)
                            {
                                   m = p->info;
                                   q = p->adjvex;
                                   k = i;
                            }
                            p = p->next;
                     }
              }
              edges[j].fromVex = k;
              edges[j].endVex = q;
              edges[j].cost = m;
              mark[k][q] = mark[q][k] = 1;
              m = 1000;
       }
}


int Find(int father[], int v)
{
       int t;
       t = v;
       while(father[t] >= 0)
              t = father[t];
       return (t);
}


void Kruskal(AlGraph * G, EdgeType edges[])
{
       int father[MaxVerNum];
       int i, j, vf1, vf2;
       for(i = 0; i < G->Vnum; i++)
              father[i] = -1;

       i = 0;
       j = 0;
       while(j < G->Vnum-1)
       {
              vf1 = Find(father, edges[i].fromVex);
              vf2 = Find(father, edges[i].endVex);
              if(vf1 != vf2)
              {
                     father[vf2] = vf1;
                     j++;
                     printf("(%d,%d)",edges[i].fromVex+1,edges[i].endVex+1);
              }
              i++;
       }
}



int main()
{
       AlGraph * G;
       G = Create();
       DFSTraverseAL(G);
       BFSTraverseAL(G);
       printf("\nMinimum Cost Spanning Tree(Prim): \n");
       Prim(G);
       EdgeType edges[MaxVerNum];
       printf("\nMinimum Cost Spanning Tree(Kruskal):\n");
       Edge(G, edges);
       Kruskal(G, edges);
       return 1;
}
