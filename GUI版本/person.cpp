#include "Struct.h"
#include <QVector>
person::person(int num,int start,int mid,int end ,int kind,int departTime,int limit,int vexnum,double * risklist,path** Map,elem** Table)
{
   this->PersonNum = num;           //旅客序号
    this->start = start;            //起始城市
    this->mid = mid;                //必经城市
    this->end = end;                //目的地
    this->kind = kind;              //规划策略
    this->departTime = departTime;  //出发时间
    this->limittime = limit;    //时间限制
    this->vex = vexnum;         //城市数
    this->risklist = risklist;  //各城市风险值
    this->map = Map;            //地图信息
    this->table = Table;        //时刻表信息
    InitPath();                 //初始化解决方案
    BestPath();                 //得出最优解
}

person::person()
{

}

void person::InitPath()  //初始化得到所有可能的解
{
    QVector<QVector<int>> path(vex);
    if(this->map[start - 1][end - 1].BusWeight != -1 || this->map[start - 1][end - 1].TrainWeight != -1 || this->map[start - 1][end - 1].PlaneWeight != -1)
    {
        path[start - 1].push_back(end - 1);//如果有直达，直接输出直达路径。
    }
    else//没有直达再搜索。
    {
        for (int i = 0; i < vex; i++)
        {
            for (int j = 0; j < vex; j++)
            {
                if (this->map[i][j].BusWeight != -1 || this->map[i][j].TrainWeight != -1 || this->map[i][j].PlaneWeight != -1)
                {
                    path[i].push_back(j);
                }
            }
        }
    }

    Path = FindPath().allPathsSource(path,start,end);//所有能到达终点的路径
}

void person::BestPath()//选出最优解
{
    int cnt = -1;
    int Time = departTime;//先初始化为乘客出发的时间
    double risk = 0;
    QVector<QVector<bestpath>> collection;
    for (auto i = Path.begin(); i < Path.end(); ++i)
    {
        cnt++;
        Time = departTime;
        risk = 0;
        QVector<bestpath> intemp((*i).size());
        for (int j = 0; j < (*i).size() && j + 1 < (*i).size(); j++)
        {
            bestpath* temp = new bestpath;
            double riskB = 0, riskT = 0, riskP = 0;
            double disB = 999, disT = 999, disP = 999;
            int timeBus = 9999, timeTrain = 9999, timePlane = 9999;//每次进循环前要更新
            int staytimeB = 0, staytimeT = 0, staytimeP = 0;//同上
            int cur = (*i)[j] + 1;
            int next = (*i)[j + 1] + 1;
            if (map[cur - 1][next - 1].BusWeight != -1)//如果有汽车路线的话，计算一下到下一节点最短时间(最早出发时间+行程时间）
            {
                disB = map[cur - 1][next - 1].BusWeight;
                staytimeB = depart(table[cur - 1][next - 1].BusNext, Time);//欲去往next城市时在cur城市的滞留时间
                timeBus = staytimeB + disB;//从到达cur开始到到达next的时间
            }
            if (map[cur - 1][next - 1].TrainWeight != -1)
            {
                disT = map[cur - 1][next - 1].TrainWeight;
                staytimeT = depart(table[cur - 1][next - 1].TrainNext, Time);
                timeTrain = staytimeT + disT;
            }
            if (map[cur - 1][next - 1].PlaneWeight != -1)
            {
                disP = map[cur - 1][next - 1].PlaneWeight;
                staytimeP = depart(table[cur - 1][next - 1].PlaneNext, Time);
                timePlane = staytimeP + disP; 
            }
            riskB = (disB * 2 + staytimeB) * risklist[cur - 1];		//风险为滞留城市风险值*(滞留时间 + 到下一个城市的行程时间）
            riskT = (disT * 5 + staytimeT) * risklist[cur - 1];
            riskP = (disP * 9 + staytimeP) * risklist[cur - 1];

            //选出走哪条路，更新Time
            if (riskB < riskT && riskB < riskP)
            {
                Time += timeBus;
                risk += riskB;
                temp->staytime = staytimeB;
                temp->nextcost = disB;
                temp->transport = 'b';
                temp->risk = risk;
            }
            else if (riskT < riskB && riskT < riskP)
            {
                Time += timeTrain;
                risk += riskT;
                temp->staytime = staytimeT;
                temp->nextcost = disT;
                temp->transport = 't';
                temp->risk = risk;
            }
            else if (riskP < riskB && riskP < riskT)
            {
                Time += timePlane;
                risk += riskP;
                temp->staytime = staytimeP;
                temp->nextcost = disP;
                temp->transport = 'p';
                temp->risk = risk;
            }
            else if (riskB == riskT)//如果存在风险有相等的情况
            {
                if (riskB > riskP)
                {
                    Time += timePlane;
                    risk += riskP;
                    temp->staytime = staytimeP;
                    temp->nextcost = disP;
                    temp->transport = 'p';
                    temp->risk = risk;
                }
                else
                {
                    if (disT + staytimeT < disB + staytimeB)
                    {
                        Time += timeTrain;
                        risk += riskT;
                        temp->staytime = staytimeT;
                        temp->nextcost = disT;
                        temp->transport = 't';
                        temp->risk = risk;
                    }
                    else
                    {
                        Time += timeBus;
                        risk += riskB;
                        temp->staytime = staytimeB;
                        temp->nextcost = disB;
                        temp->transport = 'b';
                        temp->risk = risk;
                    }
                }
            }
            else if (riskP == riskB)//如果存在风险有相等的情况
            {
                if (riskP > riskT)
                {
                    Time += timeTrain;
                    risk += riskT;
                    temp->staytime = staytimeT;
                    temp->nextcost = disT;
                    temp->transport = 't';
                    temp->risk = risk;
                }
                else
                {
                    if (disB + staytimeB <= disP + staytimeP)
                    {
                        Time += timeBus;
                        risk += riskB;
                        temp->staytime = staytimeB;
                        temp->nextcost = disB;
                        temp->transport = 'b';
                        temp->risk = risk;
                    }
                    else
                    {
                        Time += timePlane;
                        risk += riskP;
                        temp->staytime = staytimeP;
                        temp->nextcost = disP;
                        temp->transport = 'p';
                        temp->risk = risk;
                    }
                }
            }
            else if (riskP == riskT)
            {
                if (riskP > riskB)
                {
                    Time += timeBus;
                    risk += riskB;
                    temp->staytime = staytimeB;
                    temp->nextcost = disB;
                    temp->transport = 'b';
                    temp->risk = risk;
                }
                else
                {
                    if (disT + staytimeT < disP + staytimeP)
                    {
                        Time += timeTrain;
                        risk += riskT;
                        temp->staytime = staytimeT;
                        temp->nextcost = disT;
                        temp->transport = 't';
                        temp->risk = risk;
                    }
                    else
                    {
                        Time += timePlane;
                        risk += riskP;
                        temp->staytime = staytimeP;
                        temp->nextcost = disP;
                        temp->transport = 'p';
                        temp->risk = risk;
                    }
                }
            }
            else
            {
                Time += timeBus;
                risk += riskB;
                temp->staytime = staytimeB;
                temp->nextcost = disB;
                temp->transport = 'b';
                temp->risk = risk;
            }
            temp->curcity = cur;
            temp->nextcity = next;
            temp->arrivetime = Time;
            intemp[j] = *temp;
        }
        intemp.pop_back();



        //检查找到的通路中是否经过必经城市
        bool Throughmid = false;
        for (auto i = intemp.begin(); i != intemp.end(); i++)
        {
            if (i->curcity == mid)
            {
                Throughmid = true;
                break;
            }
        }
        if (Throughmid == true)//经过必经城市就加入解集
        {
            collection.push_back(intemp);
        }
    }


    //清空Path释放空间
    Path.clear();
    double minrisk = 999;//最小风险
    pathexist = 0;//存在解的标记



    if (kind == 1)//不限时策略
    {
        for (auto it = collection.begin(); it != collection.end(); it++)
        {
            if (((*it).back()).risk < minrisk)//找最小风险值
            {
                pathexist = 1;
                minrisk = (*it).back().risk;
            }
        }
        for (auto it = collection.begin(); it != collection.end(); it++)
        {
            if (((*it).back()).risk == minrisk)
            {
                bestplan.push_back((*it));//插入到解集中
            }
        }
    }
    else//限时策略
    {
        for (auto it = collection.begin(); it != collection.end(); it++)
        {            
            if (((*it).back()).arrivetime <= limittime + departTime)//是否满足时间限制
            {
                if (((*it).back()).arrivetime < minrisk)//找最小风险值
                {                    
                    pathexist = 1;
                    minrisk = (*it).back().risk;
                }
            }
        }
        for (auto it = collection.begin(); it != collection.end(); it++)
        {
            if (((*it).back()).arrivetime <= limittime + departTime)//是否满足时间限制
            {
                if (((*it).back()).risk == minrisk)
                {
                    bestplan.push_back((*it));//插入到解集中
                }
            }
        }
    }
    chosenplan.resize(0);
    if(bestplan.size() == 0)
        pathexist = 0;
    if(bestplan.size() == 1)
        chosenplan = bestplan.first(); //只有一个直接选
}

int person::depart(node* first, int now)
{
    now = now % 24;
    node* p = first->next;	//时刻表上第一个车次，不用判断p是否为空
                            //运行到这个函数里的话，p至少是有一个车次的
    node* head = p;
    if (p->start < now)//如果当前时间比第一个始发时间大，只能等今天下一班车
    {
        if (p->next)//如果时刻表上有下一班车
        {
            p = p->next;//从下一班车开始查表
            while (p)
            {
                if (p->start < now )
                {
                    p = p->next;
                }
                else if (p->start >= now)//终于找到一班车比现在时刻晚或相等
                {
                    return p->start - now;
                }
            }
            //运行到这还没return，说明当前时刻比今天所有车次发车时间都晚
            //只能等明天第一班车了
            return head->start + 24 - now ;
        }
        else//如果今天没有下一班车了
        {
            return p->start + 24 - now;//坐明天第一班车的等待时间
        }
    }

    else if (p->start == now)//现在刚好有车出发
    {
        return 0;
    }

    else if (p->start > now)//第一班车比现在时刻晚
    {
        return p->start - now;//等车时间
    }
    return p->start + 24 - now;//等待比较稳妥
}
