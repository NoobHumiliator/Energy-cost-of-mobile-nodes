// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<math.h>
#include<iostream>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<list>
#include <stdlib.h>
#include <algorithm>
#include <cstdio>  
#include <cstdlib>  
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>
#include<conio.h> 

using namespace std;


const double pi = 3.141592654;
double L = 10;//模拟环境的大小


class packet {
public:
	int T;
	int destination;
	int current;
};

class node {
public:
	int number;
	double v;
	double x;
	double y;
	vector<int> reachable;//可到达的节点
	list<packet> queue;//数据包的队列
	double distance(node p);//传一个对象指针进去
	double energy;//节点的能量
	void move();
};

double node::distance(node p)
{
	return sqrt((x - p.x)*(x - p.x) + (y - p.y)*(y - p.y));

}
void node::move()
{
	double angel = (rand()*1.0 / double(RAND_MAX)) * 2 * pi - pi;
	x = fmod(cos(angel)*v + x + L, L);
	y = fmod(sin(angel)*v + y + L, L);
}

void main()
{
	srand((unsigned)time(NULL));
	int Tsum = 0;//统计所有包的生存时间
	int time = 0;//统计主程序循环了多少次
	int packet_sum = 0; //统计网络中有多少个数据包
	int allpacket = 0; //统计整个过程送达的多少数据包
	int nextloop=0;    //暂存下一跳
	int flag_arrive = 0;//用来标示是否已经送达
	int packet_push = 0;
	int packet_arrive = 0; //用来统计多少个数据包被送达
	int packet_arrive_all = 0;
	int arrivealltime = 0;//用来统计送达的数据包在网络中滞留的总时间
	double r = 1.0;//节点的接收半径
	const int N = 500;//节点个数
	int C = 1;//每一个时间节点可以发送包的个数
	int R = 25;//同一时间产生的包的多少
	double vl = 0.001;//节点的低速
	double vh = 1;//节点的高速
	double f = 0.00; //调节高低速度节点的比例
	double E = 1000;//节点的初始能量
	node* realnode[N];  //node的对象数组
	vector<packet> packet_under_go; //所有位于队列头上的packet（即将开始处理的数组）
	double cost = 0;//用来记录cost
	double rsum = 0;//所有邻居的距离之和
	double rneibor = 0;//到这位邻居的距离
	double esum = 0; //所有邻居的能量之和
	double costsum = 0; //cost总和
	double alpha = 0.00;//调节cost的阿尔法
	double costmin = 900000000;//记录最小cost
	double flag_end = 0;//算法结束的标志
	double rand_num = 0;//0-1之间的随机小数
	double degree_begin = 0; //有偏随机行走的概率分配 区间开始
	double degree_end = 0;   //有偏随机行走的概率分配 区间结束
	/*for(int start_node=0;start_node<N;start_node++)
	{
	realnode[start_node]=new node();//node的实例初始化
	realnode[start_node]->x=(rand()*1.0/double(RAND_MAX))*L;
	realnode[start_node]->y=(rand()*1.0/double(RAND_MAX))*L;
	f=rand()*1.0/double(RAND_MAX);
	if (f<2)//全部是高速
	{realnode[start_node]->v=vh;}
	else
	{realnode[start_node]->v=vl;}
	realnode[start_node]->energy=E;
	cout<<start_node<<"号节点初始分配的坐标是"<<realnode[start_node]->x<<" "<<realnode[start_node]->y<<"速度为"<<realnode[start_node]->v<<"能量为"<<realnode[start_node]->energy<<endl;

	}*/
	while (true)
	{
		Tsum = 0;//统计所有包的生存时间
		packet_sum = 0; //统计网络中有多少个数据包
		allpacket = 0; //统计整个过程送达的多少数据包
	    flag_arrive = 0;//用来标示是否已经送达
	    packet_push = 0;
	    packet_arrive = 0; //用来统计多少个数据包被送达
		cost = 0;//用来记录cost
		rsum = 0;//所有邻居的距离之和
		rneibor = 0;//到这位邻居的距离
		esum = 0; //所有邻居的能量之和
	    alpha = 0.1;//调节cost的阿尔法
		costmin = 900000000;//记录最小cost
	    flag_end = 0;//算法结束的标志
		while (alpha < 1.02)
		{
			for (int start_node = 0; start_node < N; start_node++)
			{
				realnode[start_node] = new node();//node的实例初始化
				realnode[start_node]->x = (rand()*1.0 / double(RAND_MAX))*L;
				realnode[start_node]->y = (rand()*1.0 / double(RAND_MAX))*L;
				f = rand()*1.0 / double(RAND_MAX);
				if (f < 2)//全部是高速 
				{
					realnode[start_node]->v = vh;
				}
				else
				{
					realnode[start_node]->v = vl;
				}
				realnode[start_node]->energy = E;
				//   	cout<<start_node<<"号节点初始分配的坐标是"<<realnode[start_node]->x<<" "<<realnode[start_node]->y<<"速度为"<<realnode[start_node]->v<<"能量为"<<realnode[start_node]->energy<<endl;

			}
			while (1)
			{
				for (int start_packet = 0; start_packet<R; start_packet++)
				{
					packet realpacket;
					realpacket.T = 0;
					while (1)
					{
						realpacket.current = (rand() / double(RAND_MAX))*(N - 1);
						realpacket.destination = (rand() / double(RAND_MAX))*(N - 1);
						if (realpacket.destination>-1 && realpacket.current > -1)
						{
							realnode[realpacket.current]->queue.push_back(realpacket);
							break;
						}
						else
						{
							continue;
						}
					}

				}
				time++;
				//	cout << "执行第" << time << "步" << endl;
				for (int move_node = 0; move_node < N; move_node++)
				{
					realnode[move_node]->move();
					//cout<<move_node<<"号节点的坐标是"<<realnode[move_node]->x<<" "<<realnode[move_node]->y<<"速度为"<<realnode[move_node]->v<<endl;
					realnode[move_node]->reachable.clear();
				}

				for (int process_node = 0; process_node < N; process_node++)
				{
					for (int toreach = 0; toreach < process_node; toreach++)
					{
						if (process_node != toreach)
						{
							if (r >= realnode[process_node]->distance(*realnode[toreach]))

							{
								realnode[process_node]->reachable.push_back(toreach);
								realnode[toreach]->reachable.push_back(process_node);
								//cout<<process_node<<"号节点可到达邻居为"<<toreach<<endl; 
								//只比较三角矩阵，节约时间
							}
							else{}
						}
						else {}
					}

				}

				for (int c_search = 0; c_search < C; c_search++)
				{
					for (int search = 0; search < N; search++)
					{
						//cout<<search<<"号节点有邻居"<<realnode[search]->reachable.size()<<"个"<<endl;
						if (realnode[search]->queue.size() != 0 && realnode[search]->reachable.size() != 0)//要求必须有邻居，并且队列里面有数据包
						{

							packet_under_go.push_back(realnode[search]->queue.front());
							realnode[search]->queue.pop_front();
						}
						else
						{
						}
					}
				}
				//      cout<<packet_under_go.size()<<"个位于队列第一个的包"<<endl;

				for (int process_packet = 0; process_packet < packet_under_go.size(); process_packet++)
				{   
					nextloop = 0;
					flag_arrive = 0;
					degree_begin = 0;
					degree_end = 0;
					rsum = 0;
					if (flag_end == 1)
					{
						break;
					}
					for (int reach = 0; reach < realnode[packet_under_go[process_packet].current]->reachable.size(); reach++)
					{
						if (realnode[packet_under_go[process_packet].current]->reachable[reach] == packet_under_go[process_packet].destination)
						{
							//   cout<<"送达用的距离"<<realnode[packet_under_go[process_packet].current]->distance(*realnode[packet_under_go[process_packet].destination])<<endl;
							realnode[packet_under_go[process_packet].current]->energy = realnode[packet_under_go[process_packet].current]->energy - 1;//送达数据包也要消耗一份能量
							if (realnode[packet_under_go[process_packet].current]->energy < 1)
							{
								flag_end = 1;
							}
							else
							{
							}
							packet_arrive++;
							packet_arrive_all++;
							arrivealltime = packet_under_go[process_packet].T + arrivealltime;
							flag_arrive = 1;
							break;
						}
						else
						{
						}
					}
					if (flag_arrive == 0)
					{
						
						for (int searchneibor = 0; searchneibor < realnode[packet_under_go[process_packet].current]->reachable.size(); searchneibor++)
						{
							rsum = realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) + rsum;//当前处理节点的邻居到当前包的目的地的距离
							//rsum = log((double)realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->distance(*realnode[packet_under_go[process_packet].destination])) / log((double)10) + rsum;
							esum = realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->energy + esum;
							//cout<<esum<<"能量和"<<endl;
							//cout<<rsum<<"距离和"<<endl;
						}
						//	cout << packet_under_go[process_packet].current<< "邻居度和为" << rsum << endl;
						for (int neibors = 0; neibors < realnode[packet_under_go[process_packet].current]->reachable.size(); neibors++)
						{
							cost = pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[neibors]]->energy) / esum, (1 - alpha))/ pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[neibors]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha);//cost的计算
							costsum = costsum + cost;
						}					
							rand_num = (double)rand() / RAND_MAX;
							for (int compareneibor = 0; compareneibor < realnode[packet_under_go[process_packet].current]->reachable.size(); compareneibor++)  //统计节点邻居的度
							{
								degree_begin = degree_end;
								degree_end = degree_end + pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->energy) / esum, (1 - alpha)) / pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha) / costsum;
								//cout << packet_under_go[process_packet].current << "邻居是" << realnode[packet_under_go[process_packet].current]->reachable[compareneibor] << "它的能量为" << pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->energy) / esum, (1 - alpha)) <<"他的距离"<< pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha) << "为其分配的区间" << degree_begin << "-" << degree_end << endl;
	 							if (rand_num>degree_begin&&rand_num<degree_end)
								{
									nextloop = compareneibor; break;
								}
								else
								{
								}
							}
							degree_begin = 0;
							degree_end = 0;
							cost = 0;
							rsum = 0;
							esum = 0;//暂时用的变量全部清零，只留一个nextloop；
							costsum = 0;
							//nextloop=(realnode[packet_under_go[process_packet].current]->reachable.size()-1)*(rand()/double(RAND_MAX));//随机挑选一个下一跳地址
						    //cout<<"下一跳的邻居编号为"<<nextloop<<"下一跳地址为"<<realnode[packet_under_go[process_packet].current]->reachable[nextloop]<<endl;
								realnode[packet_under_go[process_packet].current]->energy = realnode[packet_under_go[process_packet].current]->energy - 1;//转发数据包也要消耗能量
								if (realnode[packet_under_go[process_packet].current]->energy < 1)
								{
									flag_end = 1;
								}
								else
								{
								}
								packet_under_go[process_packet].current = realnode[packet_under_go[process_packet].current]->reachable[nextloop];
								realnode[packet_under_go[process_packet].current]->queue.push_back(packet_under_go[process_packet]);
								packet_push++;				
					}
					else
					{
					}
					nextloop = 0;
					flag_arrive = 0;
				}
				for (int count = 0; count < N; count++)

				{
					for (list<packet>::iterator iter = realnode[count]->queue.begin(); iter != realnode[count]->queue.end(); iter++)
					{
						//cout<<iter->T<<endl;
						Tsum = iter->T + Tsum;
						iter->T++;
						packet_sum++;
					}
					//cout<<"节点"<<count<<"的能量为"<<realnode[count]->energy<<endl;
				}
				allpacket = allpacket + packet_arrive;
				if (flag_end == 1)
				{
					cout << "网络生存了" << time << "步" << endl;
					ofstream ftest("TEST.txt", ios::app);
					ftest << alpha << " " << time << " " << allpacket << " " << packet_sum << " " << (double)arrivealltime / (double)packet_arrive_all << endl;//这里是你想要输出的内容
					ftest.close();
					break;
				}
				packet_under_go.clear();
				/*	cout<<"网络内数据包的总生存时间为"<<" "<<Tsum<<endl;
				cout<<"网络内数据包的总量为"<<" "<<packet_sum<<endl;
				cout<<"本轮送达的数据包数为"<<" "<<packet_arrive<<endl;
				cout<<"本未能送达的数据包数为"<<" "<<packet_push<<endl;
				*/
				Tsum = 0;
				packet_arrive = 0;
				packet_push = 0;
				packet_sum = 0;
			}
			arrivealltime = 0;
			packet_arrive_all = 0;
			packet_arrive = 0;
			packet_push = 0;
			packet_sum = 0;
			Tsum = 0;//统计所有包的生存时间
			time = 0;//统计主程序循环了多少次
			packet_sum = 0; //统计网络中有多少个数据包
			nextloop = 0;
			allpacket = 0;   //统计一共送达的多少包
			for (int delete_node = 0; delete_node < N; delete_node++)
			{
				delete realnode[delete_node];
			}
			packet_under_go.clear();
			flag_end = 0;//算法结束的标志
			alpha = alpha + 0.1;
		} //alpha变换括号
	}//重复试验
}//主程序