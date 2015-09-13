// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
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
double L = 10;//ģ�⻷���Ĵ�С


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
	vector<int> reachable;//�ɵ���Ľڵ�
	list<packet> queue;//���ݰ��Ķ���
	double distance(node p);//��һ������ָ���ȥ
	double energy;//�ڵ������
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
	int Tsum = 0;//ͳ�����а�������ʱ��
	int time = 0;//ͳ��������ѭ���˶��ٴ�
	int packet_sum = 0; //ͳ���������ж��ٸ����ݰ�
	int allpacket = 0; //ͳ�����������ʹ�Ķ������ݰ�
	int nextloop=0;    //�ݴ���һ��
	int flag_arrive = 0;//������ʾ�Ƿ��Ѿ��ʹ�
	int packet_push = 0;
	int packet_arrive = 0; //����ͳ�ƶ��ٸ����ݰ����ʹ�
	int packet_arrive_all = 0;
	int arrivealltime = 0;//����ͳ���ʹ�����ݰ�����������������ʱ��
	double r = 1.0;//�ڵ�Ľ��հ뾶
	const int N = 500;//�ڵ����
	int C = 1;//ÿһ��ʱ��ڵ���Է��Ͱ��ĸ���
	int R = 25;//ͬһʱ������İ��Ķ���
	double vl = 0.001;//�ڵ�ĵ���
	double vh = 1;//�ڵ�ĸ���
	double f = 0.00; //���ڸߵ��ٶȽڵ�ı���
	double E = 1000;//�ڵ�ĳ�ʼ����
	node* realnode[N];  //node�Ķ�������
	vector<packet> packet_under_go; //����λ�ڶ���ͷ�ϵ�packet��������ʼ��������飩
	double cost = 0;//������¼cost
	double rsum = 0;//�����ھӵľ���֮��
	double rneibor = 0;//����λ�ھӵľ���
	double esum = 0; //�����ھӵ�����֮��
	double costsum = 0; //cost�ܺ�
	double alpha = 0.00;//����cost�İ�����
	double costmin = 900000000;//��¼��Сcost
	double flag_end = 0;//�㷨�����ı�־
	double rand_num = 0;//0-1֮������С��
	double degree_begin = 0; //��ƫ������ߵĸ��ʷ��� ���俪ʼ
	double degree_end = 0;   //��ƫ������ߵĸ��ʷ��� �������
	/*for(int start_node=0;start_node<N;start_node++)
	{
	realnode[start_node]=new node();//node��ʵ����ʼ��
	realnode[start_node]->x=(rand()*1.0/double(RAND_MAX))*L;
	realnode[start_node]->y=(rand()*1.0/double(RAND_MAX))*L;
	f=rand()*1.0/double(RAND_MAX);
	if (f<2)//ȫ���Ǹ���
	{realnode[start_node]->v=vh;}
	else
	{realnode[start_node]->v=vl;}
	realnode[start_node]->energy=E;
	cout<<start_node<<"�Žڵ��ʼ�����������"<<realnode[start_node]->x<<" "<<realnode[start_node]->y<<"�ٶ�Ϊ"<<realnode[start_node]->v<<"����Ϊ"<<realnode[start_node]->energy<<endl;

	}*/
	while (true)
	{
		Tsum = 0;//ͳ�����а�������ʱ��
		packet_sum = 0; //ͳ���������ж��ٸ����ݰ�
		allpacket = 0; //ͳ�����������ʹ�Ķ������ݰ�
	    flag_arrive = 0;//������ʾ�Ƿ��Ѿ��ʹ�
	    packet_push = 0;
	    packet_arrive = 0; //����ͳ�ƶ��ٸ����ݰ����ʹ�
		cost = 0;//������¼cost
		rsum = 0;//�����ھӵľ���֮��
		rneibor = 0;//����λ�ھӵľ���
		esum = 0; //�����ھӵ�����֮��
	    alpha = 0.1;//����cost�İ�����
		costmin = 900000000;//��¼��Сcost
	    flag_end = 0;//�㷨�����ı�־
		while (alpha < 1.02)
		{
			for (int start_node = 0; start_node < N; start_node++)
			{
				realnode[start_node] = new node();//node��ʵ����ʼ��
				realnode[start_node]->x = (rand()*1.0 / double(RAND_MAX))*L;
				realnode[start_node]->y = (rand()*1.0 / double(RAND_MAX))*L;
				f = rand()*1.0 / double(RAND_MAX);
				if (f < 2)//ȫ���Ǹ��� 
				{
					realnode[start_node]->v = vh;
				}
				else
				{
					realnode[start_node]->v = vl;
				}
				realnode[start_node]->energy = E;
				//   	cout<<start_node<<"�Žڵ��ʼ�����������"<<realnode[start_node]->x<<" "<<realnode[start_node]->y<<"�ٶ�Ϊ"<<realnode[start_node]->v<<"����Ϊ"<<realnode[start_node]->energy<<endl;

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
				//	cout << "ִ�е�" << time << "��" << endl;
				for (int move_node = 0; move_node < N; move_node++)
				{
					realnode[move_node]->move();
					//cout<<move_node<<"�Žڵ��������"<<realnode[move_node]->x<<" "<<realnode[move_node]->y<<"�ٶ�Ϊ"<<realnode[move_node]->v<<endl;
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
								//cout<<process_node<<"�Žڵ�ɵ����ھ�Ϊ"<<toreach<<endl; 
								//ֻ�Ƚ����Ǿ��󣬽�Լʱ��
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
						//cout<<search<<"�Žڵ����ھ�"<<realnode[search]->reachable.size()<<"��"<<endl;
						if (realnode[search]->queue.size() != 0 && realnode[search]->reachable.size() != 0)//Ҫ��������ھӣ����Ҷ������������ݰ�
						{

							packet_under_go.push_back(realnode[search]->queue.front());
							realnode[search]->queue.pop_front();
						}
						else
						{
						}
					}
				}
				//      cout<<packet_under_go.size()<<"��λ�ڶ��е�һ���İ�"<<endl;

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
							//   cout<<"�ʹ��õľ���"<<realnode[packet_under_go[process_packet].current]->distance(*realnode[packet_under_go[process_packet].destination])<<endl;
							realnode[packet_under_go[process_packet].current]->energy = realnode[packet_under_go[process_packet].current]->energy - 1;//�ʹ����ݰ�ҲҪ����һ������
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
							rsum = realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) + rsum;//��ǰ����ڵ���ھӵ���ǰ����Ŀ�ĵصľ���
							//rsum = log((double)realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->distance(*realnode[packet_under_go[process_packet].destination])) / log((double)10) + rsum;
							esum = realnode[realnode[packet_under_go[process_packet].current]->reachable[searchneibor]]->energy + esum;
							//cout<<esum<<"������"<<endl;
							//cout<<rsum<<"�����"<<endl;
						}
						//	cout << packet_under_go[process_packet].current<< "�ھӶȺ�Ϊ" << rsum << endl;
						for (int neibors = 0; neibors < realnode[packet_under_go[process_packet].current]->reachable.size(); neibors++)
						{
							cost = pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[neibors]]->energy) / esum, (1 - alpha))/ pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[neibors]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha);//cost�ļ���
							costsum = costsum + cost;
						}					
							rand_num = (double)rand() / RAND_MAX;
							for (int compareneibor = 0; compareneibor < realnode[packet_under_go[process_packet].current]->reachable.size(); compareneibor++)  //ͳ�ƽڵ��ھӵĶ�
							{
								degree_begin = degree_end;
								degree_end = degree_end + pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->energy) / esum, (1 - alpha)) / pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha) / costsum;
								//cout << packet_under_go[process_packet].current << "�ھ���" << realnode[packet_under_go[process_packet].current]->reachable[compareneibor] << "��������Ϊ" << pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->energy) / esum, (1 - alpha)) <<"���ľ���"<< pow((realnode[realnode[packet_under_go[process_packet].current]->reachable[compareneibor]]->distance(*realnode[packet_under_go[process_packet].destination]) / rsum), alpha) << "Ϊ����������" << degree_begin << "-" << degree_end << endl;
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
							esum = 0;//��ʱ�õı���ȫ�����㣬ֻ��һ��nextloop��
							costsum = 0;
							//nextloop=(realnode[packet_under_go[process_packet].current]->reachable.size()-1)*(rand()/double(RAND_MAX));//�����ѡһ����һ����ַ
						    //cout<<"��һ�����ھӱ��Ϊ"<<nextloop<<"��һ����ַΪ"<<realnode[packet_under_go[process_packet].current]->reachable[nextloop]<<endl;
								realnode[packet_under_go[process_packet].current]->energy = realnode[packet_under_go[process_packet].current]->energy - 1;//ת�����ݰ�ҲҪ��������
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
					//cout<<"�ڵ�"<<count<<"������Ϊ"<<realnode[count]->energy<<endl;
				}
				allpacket = allpacket + packet_arrive;
				if (flag_end == 1)
				{
					cout << "����������" << time << "��" << endl;
					ofstream ftest("TEST.txt", ios::app);
					ftest << alpha << " " << time << " " << allpacket << " " << packet_sum << " " << (double)arrivealltime / (double)packet_arrive_all << endl;//����������Ҫ���������
					ftest.close();
					break;
				}
				packet_under_go.clear();
				/*	cout<<"���������ݰ���������ʱ��Ϊ"<<" "<<Tsum<<endl;
				cout<<"���������ݰ�������Ϊ"<<" "<<packet_sum<<endl;
				cout<<"�����ʹ�����ݰ���Ϊ"<<" "<<packet_arrive<<endl;
				cout<<"��δ���ʹ�����ݰ���Ϊ"<<" "<<packet_push<<endl;
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
			Tsum = 0;//ͳ�����а�������ʱ��
			time = 0;//ͳ��������ѭ���˶��ٴ�
			packet_sum = 0; //ͳ���������ж��ٸ����ݰ�
			nextloop = 0;
			allpacket = 0;   //ͳ��һ���ʹ�Ķ��ٰ�
			for (int delete_node = 0; delete_node < N; delete_node++)
			{
				delete realnode[delete_node];
			}
			packet_under_go.clear();
			flag_end = 0;//�㷨�����ı�־
			alpha = alpha + 0.1;
		} //alpha�任����
	}//�ظ�����
}//������