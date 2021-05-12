#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

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

bool check(int Vexnum, int edgeB, int edgeT, int edgeP)//输入图前的检验
{
	if (((Vexnum * (Vexnum - 1)) / 2) >= edgeB && ((Vexnum * (Vexnum - 1)) / 2) >= edgeT && ((Vexnum * (Vexnum - 1)) / 2) >= edgeP && Vexnum > 1)
		return true;
	else
		return false;
}

class Graph 
{
public:
	Graph()
	{
		cout << "输入城市个数和城市之间的直达汽车线路总数，直达火车线路总数，直达航班线路总数：" << endl;
		cin >> vexnum >> edgeB >> edgeT >> edgeP;
		while (!(check(vexnum, edgeB, edgeT, edgeP)))
		{
			cout << "输入的数值不合法，请重新输入" << endl;
			cin >> vexnum >> edgeB >> edgeT >> edgeP;
		}
		this->risklist = new double[this->vexnum];

		for(int i = 0;i<this->vexnum;i++)
		{
			cout << "输入第"<< i + 1 <<"个城市风险指数: ";
			cin >> risklist[i];
		}

		map = new path * [this->vexnum];
		table = new elem * [this->vexnum];

		for (int i = 0; i < this->vexnum; i++)
		{
			table[i] = new elem[this->vexnum];
			map[i] = new path[this->vexnum];

			for (int k = 0; k < this->vexnum; k++)
			{
				map[i][k].BusWeight = -1;
				map[i][k].TrainWeight = -1;
				map[i][k].PlaneWeight = -1;

				table[i][k].BusNext = NULL;
				table[i][k].PlaneNext = NULL;
				table[i][k].TrainNext = NULL;
			}
		}

		create();
		print();
	}
	
	~Graph()
	{
		for (int i = 0; i < this->vexnum; i++)
		{
			delete this->map[i];
			delete this->table[i];
		}
		delete[] map;
		delete[] table;
		delete risklist;
	}

	elem** GetTable()
	{
		return table;
	}

	path** GetMap()
	{
		return map;
	}

	int GetVex()
	{
		return vexnum;
	}

	double* GetRisk()
	{
		return risklist;
	}

	void showTable()
	{
		cout << endl;
		cout << "----------------------------交通工具时刻表----------------------------" << endl;
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (table[i][j].BusNext != NULL)
				{
					cout<<"第"<<i+1<<"到第"<<j+1<< "的汽车每日发车时刻" <<endl;
					node* show = (table[i][j].BusNext)->next;
					while (show!=NULL)
					{
						cout << show->start<<":00;";
						show = show->next;
					}
					cout << endl;
					cout << endl;
				}
			}
		}
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (table[i][j].TrainNext != NULL)
				{
					cout << "第" << i + 1 << "到第" << j + 1 << "的火车每日发车时刻" << endl;
					node* show = (table[i][j].TrainNext)->next;
					while (show != NULL)
					{
						cout << show->start << ":00; ";
						show = show->next;
					}
					cout << endl;
					cout << endl;
				}
			}
		}
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (table[i][j].PlaneNext != NULL)
				{
					cout << "第" << i + 1 << "到第" << j + 1 << "的航班每日起飞时刻" << endl;
					node* show = (table[i][j].PlaneNext)->next;
					while (show != NULL)
					{
						cout << show->start << ":00; ";
						show = show->next;
					}
					cout << endl;
					cout << endl;
				}
			}
		}
		cout << "----------------------------------------------------------------------" << endl;
		cout << endl;
	}

	void print()
	{
		cout << endl;
		cout << "-----------------------------各城市风险值-----------------------------" << endl;
		cout << endl;
		for (int i = 0; i < this->vexnum; i++)
		{
			cout << "第" << i + 1 << "个城市的风险值为" << this->risklist[i] << endl;
		}
		cout << endl;
		showTable();
		int count_row = 0;
		int count_col = 0;
		cout << endl;
		cout << "---------------------------汽车线路邻接矩阵---------------------------" << endl;
		while (count_row != this->vexnum)
		{
			count_col = 0;
			while (count_col != this->vexnum)
			{
				if (map[count_row][count_col].BusWeight == -1)
					cout << "∞" << " ";
				else
					cout << map[count_row][count_col].BusWeight << " ";
				++count_col;
			}
			cout << endl;
			++count_row;
		}
		count_row = 0;
		cout << endl;
		cout << "---------------------------火车线路邻接矩阵---------------------------" << endl;
		while (count_row != this->vexnum)
		{
			count_col = 0;
			while (count_col != this->vexnum)
			{
				if (map[count_row][count_col].TrainWeight == -1)
					cout << "∞" << " ";
				else
					cout << map[count_row][count_col].TrainWeight << " ";
				++count_col;
			}
			cout << endl;
			++count_row;
		}
		count_row = 0;
		cout << endl;
		cout << "---------------------------飞机航班邻接矩阵---------------------------" << endl;
		while (count_row != this->vexnum)
		{
			count_col = 0;
			while (count_col != this->vexnum)
			{
				if (map[count_row][count_col].PlaneWeight == -1)
					cout << "∞" << " ";
				else
					cout << map[count_row][count_col].PlaneWeight << " ";
				++count_col;
			}
			cout << endl;
			cout << endl;
			++count_row;
		}
	}

	void addcity(double risk)
	{
		double* temp = risklist;
		this->vexnum++;
		this->risklist = new double[this->vexnum];

		for (int i = 0; i < vexnum - 1; i++)
		{
			this->risklist[i] = temp[i];
		}
		this->risklist[this->vexnum - 1] = risk;

		path** tempmap = map;
		this->map = new path * [this->vexnum];
		elem** temptable = table;
		this->table = new elem * [this->vexnum];
		for (int i = 0; i < this->vexnum; i++)
		{
			this->table[i] = new elem[this->vexnum];
			this->map[i] = new path[this->vexnum];
			if (i != this->vexnum-1)
			{
				for (int k = 0; k < this->vexnum; k++)
				{
					if (k != this->vexnum-1)
					{
						this->map[i][k].BusWeight = tempmap[i][k].BusWeight;
						this->map[i][k].TrainWeight = tempmap[i][k].TrainWeight;
						this->map[i][k].PlaneWeight = tempmap[i][k].PlaneWeight;
						this->table[i][k].BusNext = temptable[i][k].BusNext;
						this->table[i][k].TrainNext = temptable[i][k].PlaneNext;
						this->table[i][k].PlaneNext = temptable[i][k].TrainNext;
					}
					else
					{
						this->map[i][k].BusWeight = -1;
						this->map[i][k].TrainWeight = -1;
						this->map[i][k].PlaneWeight = -1;
						this->table[i][k].BusNext = NULL;
						this->table[i][k].PlaneNext = NULL;
						this->table[i][k].TrainNext = NULL;
					}
				}
			}
			else
			{
				for (int k = 0; k < this->vexnum; k++)
				{
					this->map[i][k].BusWeight = -1;
					this->map[i][k].TrainWeight = -1;
					this->map[i][k].PlaneWeight = -1;
					this->table[i][k].BusNext = NULL;
					this->table[i][k].PlaneNext = NULL;
					this->table[i][k].TrainNext = NULL;
				}
			}
		}
	
		for (int i = 0; i < vexnum; i++)
		{
			cout<< this->risklist[i];
		}
	}
private:
	double* risklist;
	int vexnum;   
	int edgeB;     
	int edgeT;		
	int edgeP;		
	elem** table;
	path** map;
	
	bool check_edge_value(int start, int end, int weight) 
	{
		if (start<1 || end<1 || start>vexnum || end>vexnum || weight < 0) //顶点从1开始编号，判断我们每次输入的的边的信息是否合法
		{
			return false;
		}
		return true;
	}
	void create()
	{
		int start;
		int end;
		int weight;
		int time;
		int count = 0;
		time = 0;
		while (count != edgeB)
		{
			cout << "请输入每条汽车直达线路的起点和终点（城市编号从1开始）以及其距离：" << endl;
			cin >> start >> end >> weight;
			//首先判断边的信息是否合法
			while (!this->check_edge_value(start, end, weight))
			{
				cout << "输入的信息不合法，请重新输入" << endl;
				cin >> start >> end >> weight;
			}
			//对邻接矩阵对应上的点赋值
			map[start - 1][end - 1].BusWeight = weight;
			//map[end - 1][start - 1].BusWeight = weight;//无向图
			++count;
		}
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (map[i][j].BusWeight != -1)
				{
					cout << "输入从" << i + 1 << "到" << j + 1 << "每日的汽车发车次数" << endl;
					cin >> time;
					while (time<1)
					{
						cout << "输入0会使现有的路线上不存在交通工具通勤，请重新输入：";
						cin >> time;
					}
					table[i][j].BusNext = new node;
					table[i][j].BusNext->start = -1;
					table[i][j].BusNext->next = NULL;
					for (int k = 0; k < time; k++)
					{
						node* ptr = table[i][j].BusNext;
						int cur_start;
						cout << "请输入首发时刻（0-23）：" << endl;
						cin >> cur_start;
						node* cur = new node;
						node* last = NULL;
						while (ptr&& ptr->start < cur_start)
						{
							last = ptr;
							ptr = ptr->next;
						}
						if (ptr == NULL)
						{
							last->next = cur;
							cur->next = NULL;
							cur->start = cur_start;
						}
						else
						{
							last->next = cur;
							cur->next = ptr;
							cur->start = cur_start;
						}
					}
				}
			}
		}
		count = 0;
		time = 0;
		while (count != edgeT)
		{
			cout << "请输入每条火车直达线路的起点和终点（城市编号从1开始）以及其距离：" << endl;
			cin >> start >> end >> weight;
			//首先判断边的信息是否合法
			while (!this->check_edge_value(start, end, weight))
			{
				cout << "输入的信息不合法，请重新输入" << endl;
				cin >> start >> end >> weight;
			}
			//对邻接矩阵对应上的点赋值
			map[start - 1][end - 1].TrainWeight = weight;
			//map[end - 1][start - 1].TrainWeight = weight;
			++count;
		}
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (map[i][j].TrainWeight != -1)
				{
					cout << "输入从" << i + 1 << "到" << j + 1 << "每日的火车发车次数" << endl;
					cin >> time;
					while (time < 1)
					{
						cout << "输入0会使现有的路线上不存在交通工具通勤，请重新输入：";
						cin >> time;
					}
					table[i][j].TrainNext = new node;
					table[i][j].TrainNext->start = -1;
					table[i][j].TrainNext->next = NULL;
					for (int k = 0; k < time; k++)
					{
						node* ptr = table[i][j].TrainNext;
						int cur_start;
						cout << "请输入首发时刻（0-23）：" << endl;
						cin >> cur_start;
						node* cur = new node;
						node* last = NULL;
						while (ptr && ptr->start < cur_start)
						{
							last = ptr;
							ptr = ptr->next;
						}
						if (ptr == NULL)
						{
							last->next = cur;
							cur->next = NULL;
							cur->start = cur_start;
						}
						else
						{
							last->next = cur;
							cur->next = ptr;
							cur->start = cur_start;
						}
					}
				}
			}
		}
		count = 0;
		time = 0;
		while (count != edgeP)
		{
			cout << "请输入每条航班直达线路的起点和终点（城市编号从1开始）以及其距离：" << endl;
			cin >> start >> end >> weight;
			//首先判断边的信息是否合法
			while (!this->check_edge_value(start, end, weight))
			{
				cout << "输入的信息不合法，请重新输入" << endl;
				cin >> start >> end >> weight;
			}
			//对邻接矩阵对应上的点赋值
			map[start - 1][end - 1].PlaneWeight = weight;
			//map[end - 1][start - 1].PlaneWeight = weight;
			++count;
		}
		for (int i = 0; i < vexnum; i++)
		{
			for (int j = 0; j < vexnum; j++)
			{
				if (map[i][j].PlaneWeight != -1)
				{
					cout << "输入从" << i + 1 << "到" << j + 1 << "每日的航班起飞次数" << endl;
					cin >> time;
					while (time < 1)
					{
						cout << "输入0会使现有的路线上不存在交通工具通勤，请重新输入："<< endl;
						cin >> time;
					}
					table[i][j].PlaneNext = new node;
					table[i][j].PlaneNext->start = -1;
					table[i][j].PlaneNext->next = NULL;
					for (int k = 0; k < time; k++)
					{
						node* ptr = table[i][j].PlaneNext;
						int cur_start;
						cout << "请输入首发时刻（0-23）：" << endl;
						cin >> cur_start;
						node* cur = new node;
						node* last = NULL;
						while (ptr && ptr->start < cur_start)
						{
							last = ptr;
							ptr = ptr->next;
						}
						if (ptr == NULL)
						{
							last->next = cur;
							cur->next = NULL;
							cur->start = cur_start;
						}
						else
						{
							last->next = cur;
							cur->next = ptr;
							cur->start = cur_start;
						}
					}
				}
			}
		}
	}
};

class FindPath
{
public:

	vector<vector<int>> allPathsSource(vector<vector<int>>& graph, int start, int dest)
	{
		end = dest - 1;
		dfs(graph, start - 1);
		return res;
	}

private:

	int end;   //终点
	vector<int> way;
	vector<vector<int>> res;

	void dfs(vector<vector<int>>& graph, int start)
	{
		//cout << start<<" " << end<<" ";
		way.push_back(start);  //将当前节点加入路径
		if (start == end) //是终点则将当前路径加入结果
		{
			res.push_back(way);
			way.pop_back();   //记得要把刚加入的节点删掉
			return;
		}
		for (int next : graph[start])  //遍历下一可达节点(未出现过path中的)
		{
			//cout << next <<" "<< graph[start].size()<<" " <<endl;
			if (find(way.begin(), way.end(), next) != way.end())
			{
				continue;
			}
			else
				dfs(graph, next);
		}
		way.pop_back();    //完成一条路径后要把它删除
	}

};