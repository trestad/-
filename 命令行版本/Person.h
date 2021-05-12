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
	int staytime;	//��cur������ʱ��
	int arrivetime;	//����nextʱ��
	int nextcost;   //��һ�γ���������г�
	char transport;	//�뿪cur�Ľ�ͨ����
	double risk;	//�ڵ�ǰ���еķ����ۼ�ֵ
 }bestpath;

class Person
{
public:

	Person(Graph& map)
	{
		PersonNum++;

		cout << "�������"<<PersonNum<<"���ÿ���ʼ���У�";
		cin >> start;
		cout << "������ؾ�����";
		cin >> mid;
		cout << "��������Ŀ�ĳ��У�";
		cin >> end;
		cout << "�������" << PersonNum << "���ÿ����мƻ���1������ʱ��ͷ��գ�2����ʱ��ͷ��գ�";
		cin >> kind;
		cout << "�����������ʱ�䣺";
		cin >> time;
		this->map = map.GetMap();
		this->table = map.GetTable();
		this->vex = map.GetVex();
		this->risklist = map.GetRisk();
		if (kind == 2)
		{
			cout << "�������" << PersonNum << "���ÿ�������ʱ��";
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
		cout << "------------------------------�ÿ���Ϣ----------------------------------" << endl;
		string str;
		int flag;
		for (auto it = chosenplan.begin(); it != chosenplan.end(); it++)
		{
			flag = 1;
			int arrivetime = (*it).arrivetime;
			int depart = (*it).arrivetime - (*it).nextcost;
			int stay = (*it).arrivetime - (*it).staytime - (*it).nextcost;//ǰһ�����е���ʱ�䣬����ʼ������ʱ��
			double risk = 0;
			int rate = 0;
			if ((*it).transport == 'b')
			{
				str = "����";
				rate = 2;
			}
			if ((*it).transport == 't')
			{
				str = "��";
				rate = 5;
			}
			if ((*it).transport == 'p')
			{
				str = "�ɻ�";
				rate = 9;
			}
			if (time > arrivetime)
			{
				flag = 0;
				continue;
			}
			else if (time < this->time)
			{
				cout << "��ǰʱ�䣺��" << time / 24 << "��" << time % 24 << "��" << endl;
				cout << "��δ����" << endl;
				risk = 0;
				cout << "��ǰ�ۼƷ���ֵΪ" << risk << endl;
				break;
			}
			else if (time == arrivetime)
			{
				cout << "��ǰʱ�䣺��" << time / 24 << "��" << time % 24 << "��" << endl;
				cout << "�պõ������" << (*it).nextcity << endl;
				risk = (*it).risk;
				cout << "��ǰ�ۼƷ���ֵΪ" << risk << endl;
				break;
			}
			else if (time >= depart && time < arrivetime)
			{
				cout << "��ǰʱ�䣺��" << time / 24 << "��" << time % 24 << "��" << endl;
				cout << "�ӳ���" << (*it).curcity << "����" << str << "ǰ��������" << (*it).nextcity << endl;
				risk = (*it).risk - risklist[(*it).curcity - 1] * rate * (arrivetime - time);
				cout << "��ǰ�ۼƷ���ֵΪ" << risk << endl;
				break;
			}
			else if (time < depart && time >= stay)
			{
				cout << "��ǰʱ�䣺��" << time / 24 << "��" << time % 24 << "��" << endl;
				cout << "�ڳ���" << (*it).curcity << "������" << endl;
				risk = (*it).risk - risklist[(*it).curcity - 1] * ((arrivetime - time - (*it).nextcost) + rate * (*it).nextcost);
				cout << "��ǰ�ۼƷ���ֵΪ" << risk << endl;
				break;
			}

		}
		if (flag == 0)
		{
			cout << "�Ѿ�����Ŀ�ĵ�" << endl;
		}
		cout << "------------------------------------------------------------------------" << endl;
		cout << endl;
	}
	
private:
	int end;
	int time;		//����ʱ��
	int mid;
	int start;
	int kind;		//�ο����ͣ�1����ʱ��2��ʱ
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

		Path = FindPath().allPathsSource(path,start,end);//�����ܵ����յ��·��
	}

	void BestPath()
	{
		//int cnt = -1;
		int Time = time;//�ȳ�ʼ��Ϊ�˿ͳ�����ʱ��
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
				int timeBus = 9999, timeTrain = 9999, timePlane = 9999;//ÿ�ν�ѭ��ǰҪ����
				int staytimeB = 0, staytimeT = 0, staytimeP = 0;//ͬ��
				int cur = (*i)[j] + 1;
				int next = (*i)[j + 1] + 1;
				if (map[cur - 1][next - 1].BusWeight != -1)//���������·�ߵĻ�������һ�µ���һ�ڵ����ʱ��(�������ʱ��+�г�ʱ�䣩
				{
					disB = map[cur - 1][next - 1].BusWeight;
					staytimeB = depart(table[cur - 1][next - 1].BusNext, Time);//��ȥ��next����ʱ��cur���е�����ʱ��
					timeBus = staytimeB + disB;//�ӵ���cur��ʼ������next��ʱ��
					//cout << "������" << cur << "��" << next << "����ʱ��Ϊ" << staytimeB << "������" << next << "ʱ��Ϊ" << timeBus + Time<<endl;
				}
				if (map[cur - 1][next - 1].TrainWeight != -1)
				{
					disT = map[cur - 1][next - 1].TrainWeight;
					staytimeT = depart(table[cur - 1][next - 1].TrainNext, Time);
					timeTrain = staytimeT + disT;
					//cout << "����" << cur << "��" << next << "����ʱ��Ϊ" << staytimeT << "������" << next << "ʱ��Ϊ" << timeTrain + Time<< endl;
					
				}
				if (map[cur - 1][next - 1].PlaneWeight != -1)
				{
					disP = map[cur - 1][next - 1].PlaneWeight;
					staytimeP = depart(table[cur - 1][next - 1].PlaneNext, Time);
					timePlane = staytimeP + disP;
					//cout << "�ɻ���" << cur << "��" << next << "����ʱ��Ϊ" << staytimeP << "������"<< next<<"ʱ��Ϊ" << timePlane + Time<< endl;					
				}
				riskB = (disB * 2 + staytimeB) * risklist[cur - 1];		//����Ϊ�������з���ֵ*(����ʱ�� + ����һ�����е��г�ʱ�䣩
				riskT = (disT * 5 + staytimeT) * risklist[cur - 1];
				riskP = (disP * 9 + staytimeP) * risklist[cur - 1];

				//ѡ��������·������Time
				if (riskB < riskT && riskB < riskP)
				{
					Time += timeBus;
					risk += riskB;
					//cout << "ѡ��������������ǰ�ܷ���ֵΪ��" << risk << endl;
					temp->staytime = staytimeB;
					temp->nextcost = disB;
					temp->transport = 'b';
					temp->risk = risk;
				}
				else if (riskT < riskB && riskT < riskP)
				{
					Time += timeTrain;
					risk += riskT;
					//cout << "ѡ�������𳵣���ǰ�ܷ���ֵΪ��" << risk << endl;
					temp->staytime = staytimeT;
					temp->nextcost = disT;
					temp->transport = 't';
					temp->risk = risk;
				}
				else if (riskP < riskB && riskP < riskT)
				{
					Time += timePlane;
					risk += riskP;
					//cout << "ѡ�������ɻ�����ǰ�ܷ���ֵΪ��" << risk << endl;
					temp->staytime = staytimeP;
					temp->nextcost = disP;
					temp->transport = 'p';
					temp->risk = risk;
				}
				else if (riskB == riskT)//������ڷ�������ȵ����
				{
					if (riskB > riskP)
					{
						Time += timePlane;
						risk += riskP;
						//cout << "ѡ�������ɻ�����ǰ�ܷ���ֵΪ��" << risk << endl;
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
							//cout << "ѡ�������𳵣���ǰ�ܷ���ֵΪ��" << risk << endl;
							temp->staytime = staytimeT;
							temp->nextcost = disT;
							temp->transport = 't';
							temp->risk = risk;
						}
						else
						{
							Time += timeBus;
							risk += riskB;
							//cout << "ѡ��������������ǰ�ܷ���ֵΪ��" << risk << endl;
							temp->staytime = staytimeB;
							temp->nextcost = disB;
							temp->transport = 'b';
							temp->risk = risk;
						}
					}
				}
				else if (riskP == riskB)//������ڷ�������ȵ����
				{
					if (riskP > riskT)
					{
						Time += timeTrain;
						risk += riskT;
						//cout << "ѡ�������𳵣���ǰ�ܷ���ֵΪ��" << risk << endl;
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
							//cout << "ѡ��������������ǰ�ܷ���ֵΪ��" << risk << endl;
							temp->staytime = staytimeB;
							temp->nextcost = disB;
							temp->transport = 'b';
							temp->risk = risk;
						}
						else
						{
							Time += timePlane;
							risk += riskP;
							//cout << "ѡ�������ɻ�����ǰ�ܷ���ֵΪ��" << risk << endl;
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
						//cout << "ѡ��������������ǰ�ܷ���ֵΪ��" << risk << endl;
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
							//cout << "ѡ�������𳵣���ǰ�ܷ���ֵΪ��" << risk << endl;
							temp->staytime = staytimeT;
							temp->nextcost = disT;
							temp->transport = 't';
							temp->risk = risk;
						}
						else
						{
							Time += timePlane;
							risk += riskP;
							//cout << "ѡ�������ɻ�����ǰ�ܷ���ֵΪ��" << risk << endl;
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
					//cout << "ѡ��������������ǰ�ܷ���ֵΪ��" << risk << endl;
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
				cout << "�ÿ��޷��ڹ涨ʱ���ڵ����յ�";
			}
		}

		if (bestplan.size() == 0)
			cout << "û·";
		else
			chosenplan = *bestplan.begin();
	}

	
	int depart(node* first, int now)
	{
		now = now % 24;
		node* p = first->next;	//ʱ�̱��ϵ�һ�����Σ������ж�p�Ƿ�Ϊ��
								//���е����������Ļ���p��������һ�����ε�
		node* head = p;
		if (p->start < now)//�����ǰʱ��ȵ�һ��ʼ��ʱ���ֻ�ܵȽ�����һ�೵
		{
			if (p->next)//���ʱ�̱�������һ�೵
			{
				p = p->next;//����һ�೵��ʼ���
				while (p)
				{
					if (p->start < now )
					{
						p = p->next;
					}
					else if (p->start >= now)//�����ҵ�һ�೵������ʱ��������
					{
						return p->start - now;
					}
				}
				//���е��⻹ûreturn��˵����ǰʱ�̱Ƚ������г��η���ʱ�䶼��
				//ֻ�ܵ������һ�೵��
				return head->start + 24 - now ;
			}
			else//�������û����һ�೵��
			{
				return p->start + 24 - now;//�������һ�೵�ĵȴ�ʱ��
			}
		}

		else if (p->start == now)//���ڸպ��г�����
		{
			return 0;
		}

		else if (p->start > now)//��һ�೵������ʱ����
		{
			return p->start - now;//�ȳ�ʱ��
		}
	}

	/*
	void showPath()
	{
		if (!Path.empty())
		{
			cout << "�˿��ܵ���Ŀ�ĵص�����·��" << endl;
			for (auto i = Path.begin(); i < Path.end(); ++i)
			{
				for (int j = 0; j < (*i).size() && j + 1 < (*i).size(); j++)
				{
					int cur = (*i)[j] + 1;
					int next = (*i)[j + 1] + 1;
					cout << cur << "-->" << next;
					if (map[cur - 1][next - 1].BusWeight != -1)
						cout << "���� ";
					if (map[cur - 1][next - 1].TrainWeight != -1)
						cout << "�� ";
					if (map[cur - 1][next - 1].PlaneWeight != -1)
						cout << "�ɻ� ";
					cout << endl;
				}
				cout << endl;
			}
		}
		else
		{
			cout << "������������·��" << endl;
		}
	}
	*/
	
	void showplan()
	{
		string str;
		cout << "ѡ���·����" << endl;
		for (auto it = chosenplan.begin(); it != chosenplan.end(); it++)
		{
			if ((*it).transport == 'b')
			{
				str = "����";
			}
			if ((*it).transport == 't')
			{
				str = "��";
			}
			if ((*it).transport == 'p')
			{
				str = "�ɻ�";
			}
			cout << "�ڳ���" <<(*it).curcity << "����" << (*it).staytime << "Сʱ����" << str << "ǰ��" << (*it).nextcity << "������ʱ��Ϊ��" << (*it).arrivetime / 24 <<"��"<< (*it).arrivetime % 24 <<"�㣬��ʱ�ܷ���Ϊ" << (*it).risk << endl;
		}
		cout<<endl;
	}
	
};
int Person::PersonNum = 0;