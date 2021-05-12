#include <iostream>
#include <string>
#include<Windows.h>
#include <conio.h>
#include "Graph.h"
#include "Person.h"
using namespace std;

int main()
{
	int time = 0;
	char ch;
	cout << "------------------------------创建地图--------------------------------" << endl;
	cout << endl;
	Graph graph;
	vector<Person> namelist;//旅客名单
	cout << "按下回车开始" << endl;
	getchar();
	getchar();
	while (1)
	{
		if (_kbhit()) //如果有按键按下，则_kbhit()函数返回真
		{
			ch = _getch();//使用_getch()函数获取按下的键值

			if (ch == '1') //当按下1时创建旅客
			{
				cout << endl;
				cout << "-------------------------------新增旅客-------------------------------" << endl;
				cout << endl;
				cout << "当前时间为" << time << endl;
				cout << endl;
				namelist.push_back(Person(graph));
				cout << "按下回车返回" << endl;
				getchar();
				getchar();
			}
			if (ch == '2') //当按下2时查询地图信息
			{
				cout << endl;
				cout << "-------------------------------查询地图-------------------------------" << endl;
				graph.print();
				cout << "按下回车返回" << endl;
				getchar();
				getchar();
			}
			if (ch == '3')//当按下3时查询乘客信息
			{
				int j = 1;
				for (auto i = namelist.begin(); i != namelist.end(); i++)
				{
					cout << "第" << j << "号旅客信息" << endl;
					j++;
					(*i).show(time);
				}
				cout << "按下回车返回" << endl;
				getchar();
			}
			if (ch == '4')//添加城市
			{
				cout << "输入风险值" << endl;
				double b;
				cin >> b;
				graph.addcity(b);
			}
			if (ch == 27)//当按下ESC时退出
			{
				break;
			}
		}
		cout<< "当前时间为" << "第" << time/24 <<"天"<< time%24 <<"点" <<endl;
		Sleep(1000);
		time++;
	}
	return 0;
}
