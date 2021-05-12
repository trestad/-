#ifndef GRAPH_H
#define GRAPH_H
#include <QVector>
#include <QLabel>
#include <QPropertyAnimation>
typedef struct Tnode
{
    int start;
    Tnode* next;
}node;

typedef struct tablenode//时刻表
{
    node* BusNext;
    node* TrainNext;
    node* PlaneNext;
}elem;

typedef struct pathnode//路径
{
    int BusWeight;
    int TrainWeight;
    int PlaneWeight;
}path;

typedef struct Tbestpath
{
    int curcity;
    int nextcity;
    int staytime;	//在cur滞留的时间
    int arrivetime;	//到达next时间
    int nextcost;   //下一次出发所需的行程
    char transport;	//离开cur的交通工具
    double risk;	//在当前城市的风险累计值
 }bestpath;

class person
{
public:
    person();
    person(int num,int start,int mid,int end ,int kind,int departTime,int limit,int vexnum,double * risklist,path** Map,elem** Table);
    int vex;                                //城市数
    elem** table;                           //时刻表
    path** map;                             //地图
    double* risklist;                       //城市风险
    QLabel* pic;                            //用图片显示当前位置
    QSequentialAnimationGroup *seqGroup;    //动画组
    QVector<QVector<bestpath>> bestplan;    //若有多个最优解
    QVector<bestpath> chosenplan;           //选出来的最优解
    QVector<QVector<int>> Path;             //初步DFS结果
    int departTime;                         //出发时间
    int pathexist = 0;                      //是否存在符合要求的路径
    bool moreplan;                          //是否有多条符合要求的路
    bool still = true;                      //旅客是否在某城市滞留
    bool done = false;                      //旅客是否旅行结束
    int loc;
private:
    int end;                                //旅客目的城市
    int mid;                                //旅客必经城市
    int start;                              //旅客起始城市
    int kind;                               //策略：1不限时，2限时
    int limittime;                          //旅行限时
    int PersonNum;                          //旅客编号
    void show(int time);                    //限时旅客任意时刻的状态信息
    int depart(node* first, int now);       //计算旅客在某城市何时出发
    void InitPath();                        //把地图信息传给旅客
    void BestPath();                        //旅客规划路径
};

class FindPath
{
public:

    QVector<QVector<int>> allPathsSource(QVector<QVector<int>>& graph, int start, int dest)
    {
        end = dest - 1;
        dfs(graph, start - 1);
        return res;
    }

private:

    int end;   //终点
    QVector<int> way;
    QVector<QVector<int>> res;

    void dfs(QVector<QVector<int>>& graph, int start)
    {
        way.push_back(start);  //将当前节点加入路径
        if (start == end) //是终点则将当前路径加入结果
        {
            res.push_back(way);
            way.pop_back();   //记得要把刚加入的节点删掉
            return;
        }
        for (int next : graph[start])  //遍历下一可达节点(未出现过path中的)
        {
            if (std::find(way.begin(), way.end(), next) != way.end())
            {
                continue;
            }
            else
                dfs(graph, next);
        }
        way.pop_back();    //完成一条路径后要把它删除
    }

};

#endif // GRAPH_H
