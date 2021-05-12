#pragma once
#include <iostream>
#include <string>
#include<vector>
#include "Graph.h"
using namespace std;

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

class Person
{
public:

	Person(Graph& map)
	{
		PersonNum++;

		cout << "请输入第"<<PersonNum<<"个旅客起始城市：";
		cin >> start;
		cout << "请输入必经城市";
		cin >> mid;
		cout << "请输入其目的城市：";
		cin >> end;
		cout << "请输入第" << PersonNum << "个旅客旅行计划：1、不限时最低风险；2、限时最低风险：";
		cin >> kind;
		cout << "请输入其出发时间：";
		cin >> time;
		this->map = map.GetMap();
		this->table = map.GetTable();
		this->vex = map.GetVex();
		this->risklist = map.GetRisk();
		if (kind == 2)
		{
			cout << "请输入第" << PersonNum << "个旅客旅行限时：";
			cin >> limittime;
		}
		else
		{
			limittime = 0;
		}
		InitPath();
		BestPath();
		showplan();
	}
	void show(int time)
	{
		cout << endl;
		cout << "------------------------------旅客信息----------------------------------" << endl;
		string str;
		int flag;
		for (auto it = chosenplan.begin(); it != chosenplan.end(); it++)
		{
			flag = 1;
			int arrivetime = (*it).arrivetime;
			int depart = (*it).arrivetime - (*it).nextcost;
			int stay = (*it).arrivetime - (*it).staytime - (*it).nextcost;//前一个城市到达时间，即开始滞留的时间
			double risk = 0;
			int rate = 0;
			if ((*it).transport == 'b')
			{
				str = "汽车";
				rate = 2;
			}
			if ((*it).transport == 't')
			{
				str = "火车";
				rate = 5;
			}
			if ((*it).transport == 'p')
			{
				str = "飞机";
				rate = 9;
			}
			if (time > arrivetime)
			{
				flag = 0;
				continue;
			}
			else if (time < this->time)
			{
				cout << "当前时间：第" << time / 24 << "天" << time % 24 << "点" << endl;
				cout << "还未出发" << endl;
				risk = 0;
				cout << "当前累计风险值为" << risk << endl;
				break;
			}
			else if (time == arrivetime)
			{
				cout << "当前时间：第" << time / 24 << "天" << time % 24 << "点" << endl;
				cout << "刚好到达城市" << (*it).nextcity << endl;
				risk = (*it).risk;
				cout << "当前累计风险值为" << risk << endl;
				break;
			}
			else if (time >= depart && time < arrivetime)
			{
				cout << "当前时间：第" << time / 24 << "天" << time % 24 << "点" << endl;
				cout << "从城市" << (*it).curcity << "乘坐" << str << "前往城市中" << (*it).nextcity << endl;
				risk = (*it).risk - risklist[(*it).curcity - 1] * rate * (arrivetime - time);
				cout << "当前累计风险值为" << risk << endl;
				break;
			}
			else if (time < depart && time >= stay)
			{
				cout << "当前时间：第" << time / 24 << "天" << time % 24 << "点" << endl;
				cout << "在城市" << (*it).curcity << "滞留中" << endl;
				risk = (*it).risk - risklist[(*it).curcity - 1] * ((arrivetime - time - (*it).nextcost) + rate * (*it).nextcost);
				cout << "当前累计风险值为" << risk << endl;
				break;
			}

		}
		if (flag == 0)
		{
			cout << "已经到达目的地" << endl;
		}
		cout << "------------------------------------------------------------------------" << endl;
		cout << endl;
	}
	
private:
	int end;
	int time;		//出发时间
	int mid;
	int start;
	int kind;		//游客类型：1不限时，2限时
	int limittime;
	int vex;
	double* risklist;
	static int PersonNum;
	elem** table;
	path** map;
	vector<vector<bestpath>> bestplan;
	vector<bestpath> chosenplan;
	vector<vector<int>> Path;

	void InitPath()
	{
		vector<vector<int>> path(vex);
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

		Path = FindPath().allPathsSource(path,start,end);//所有能到达终点的路径
	}

	void BestPath()
	{
		//int cnt = -1;
		int Time = time;//先初始化为乘客出发的时间
		double risk = 0;
		vector<vector<bestpath>> collection;
		for (auto i = Path.begin(); i < Path.end(); ++i)
		{
			//cnt++;
			Time = time;
			risk = 0;
			vector<bestpath> intemp((*i).size());
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
					//cout << "汽车在" << cur << "到" << next << "滞留时间为" << staytimeB << "，到达" << next << "时间为" << timeBus + Time<<endl;
				}
				if (map[cur - 1][next - 1].TrainWeight != -1)
				{
					disT = map[cur - 1][next - 1].TrainWeight;
					staytimeT = depart(table[cur - 1][next - 1].TrainNext, Time);
					timeTrain = staytimeT + disT;
					//cout << "火车在" << cur << "到" << next << "滞留时间为" << staytimeT << "，到达" << next << "时间为" << timeTrain + Time<< endl;
					
				}
				if (map[cur - 1][next - 1].PlaneWeight != -1)
				{
					disP = map[cur - 1][next - 1].PlaneWeight;
					staytimeP = depart(table[cur - 1][next - 1].PlaneNext, Time);
					timePlane = staytimeP + disP;
					//cout << "飞机在" << cur << "到" << next << "滞留时间为" << staytimeP << "，到达"<< next<<"时间为" << timePlane + Time<< endl;					
				}
				riskB = (disB * 2 + staytimeB) * risklist[cur - 1];		//风险为滞留城市风险值*(滞留时间 + 到下一个城市的行程时间）
				riskT = (disT * 5 + staytimeT) * risklist[cur - 1];
				riskP = (disP * 9 + staytimeP) * risklist[cur - 1];

				//选出走哪条路，更新Time
				if (riskB < riskT && riskB < riskP)
				{
					Time += timeBus;
					risk += riskB;
					//cout << "选择了坐汽车，当前总风险值为：" << risk << endl;
					temp->staytime = staytimeB;
					temp->nextcost = disB;
					temp->transport = 'b';
					temp->risk = risk;
				}
				else if (riskT < riskB && riskT < riskP)
				{
					Time += timeTrain;
					risk += riskT;
					//cout << "选择了坐火车，当前总风险值为：" << risk << endl;
					temp->staytime = staytimeT;
					temp->nextcost = disT;
					temp->transport = 't';
					temp->risk = risk;
				}
				else if (riskP < riskB && riskP < riskT)
				{
					Time += timePlane;
					risk += riskP;
					//cout << "选择了坐飞机，当前总风险值为：" << risk << endl;
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
						//cout << "选择了坐飞机，当前总风险值为：" << risk << endl;
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
							//cout << "选择了坐火车，当前总风险值为：" << risk << endl;
							temp->staytime = staytimeT;
							temp->nextcost = disT;
							temp->transport = 't';
							temp->risk = risk;
						}
						else
						{
							Time += timeBus;
							risk += riskB;
							//cout << "选择了坐汽车，当前总风险值为：" << risk << endl;
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
						//cout << "选择了坐火车，当前总风险值为：" << risk << endl;
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
							//cout << "选择了坐汽车，当前总风险值为：" << risk << endl;
							temp->staytime = staytimeB;
							temp->nextcost = disB;
							temp->transport = 'b';
							temp->risk = risk;
						}
						else
						{
							Time += timePlane;
							risk += riskP;
							//cout << "选择了坐飞机，当前总风险值为：" << risk << endl;
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
						//cout << "选择了坐汽车，当前总风险值为：" << risk << endl;
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
							//cout << "选择了坐火车，当前总风险值为：" << risk << endl;
							temp->staytime = staytimeT;
							temp->nextcost = disT;
							temp->transport = 't';
							temp->risk = risk;
						}
						else
						{
							Time += timePlane;
							risk += riskP;
							//cout << "选择了坐飞机，当前总风险值为：" << risk << endl;
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
					//cout << "选择了坐汽车，当前总风险值为：" << risk << endl;
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
			bool Throughmid = false;
			for (auto i = intemp.begin(); i != intemp.end(); i++)
			{
				if (i->curcity == mid)
				{
					Throughmid = true;
					break;
				}
			}
			if (Throughmid == true)
			{
				collection.push_back(intemp);
			}
			cout << endl;
		}

		int minrisk = 999;
		int pathexist = 0;
		if (kind == 1)
		{
			for (auto it = collection.begin(); it != collection.end(); it++)
			{
				if (((*it).back()).risk <= minrisk)
				{
					pathexist = 1;
					bestplan.push_back((*it));
					minrisk = (*it).back().risk;
				}
			}
		}
		else
		{
			for (auto it = collection.begin(); it != collection.end(); it++)
			{
				if (((*it).back()).arrivetime <= limittime + time)
				{
					if (((*it).back()).arrivetime <= minrisk)
					{
						pathexist = 1;
						bestplan.push_back((*it));
						minrisk = (*it).back().risk;
					}
				}
			}
			
			if (pathexist == 0)
			{
				cout << "旅客无法在规定时间内到达终点";
			}
		}

		if (bestplan.size() == 0)
			cout << "没路";
		else
			chosenplan = *bestplan.begin();
	}

	
	int depart(node* first, int now)
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
	}

	/*
	void showPath()
	{
		if (!Path.empty())
		{
			cout << "乘客能到达目的地的所有路径" << endl;
			for (auto i = Path.begin(); i < Path.end(); ++i)
			{
				for (int j = 0; j < (*i).size() && j + 1 < (*i).size(); j++)
				{
					int cur = (*i)[j] + 1;
					int next = (*i)[j + 1] + 1;
					cout << cur << "-->" << next;
					if (map[cur - 1][next - 1].BusWeight != -1)
						cout << "汽车 ";
					if (map[cur - 1][next - 1].TrainWeight != -1)
						cout << "火车 ";
					if (map[cur - 1][next - 1].PlaneWeight != -1)
						cout << "飞机 ";
					cout << endl;
				}
				cout << endl;
			}
		}
		else
		{
			cout << "不存在这样的路径" << endl;
		}
	}
	*/
	
	void showplan()
	{
		string str;
		cout << "选择的路径：" << endl;
		for (auto it = chosenplan.begin(); it != chosenplan.end(); it++)
		{
			if ((*it).transport == 'b')
			{
				str = "汽车";
			}
			if ((*it).transport == 't')
			{
				str = "火车";
			}
			if ((*it).transport == 'p')
			{
				str = "飞机";
			}
			cout << "在城市" <<(*it).curcity << "滞留" << (*it).staytime << "小时后搭乘" << str << "前往" << (*it).nextcity << "，到达时间为第" << (*it).arrivetime / 24 <<"天"<< (*it).arrivetime % 24 <<"点，此时总风险为" << (*it).risk << endl;
		}
		cout<<endl;
	}
	
};
int Person::PersonNum = 0;