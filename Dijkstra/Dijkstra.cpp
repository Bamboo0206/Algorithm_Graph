/*使用了STL，不用自己写栈了嘻嘻*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
#include<vector>
#include<bitset>
using namespace std;

#define MAX_NODE_NUM 9999
#define INFINITE 99999999


/*邻接矩阵，矩阵内存的int为边的权重*/
int G_size;
int G[MAX_NODE_NUM][MAX_NODE_NUM];
//vector<vector<int>> G;//二维vector//由于二维vector不是方方正正的矩阵，不太好用可能越界，故弃之不用


/*Dijkstra，传入起点和终点即可获得最短路径。
传入path容器用于保存路径，最终结果为正序。
	注意，path内保存的是节点的编号。
图为全局变量，不用当作参数传入，调用dijkstra之前请初始化图使其每条边有唯一确定的权重。
*/
void Dijkstra(int src, int dest, vector<int> &path)
{
	/*初始化*/
	/*bool collected[MAX_NODE_NUM] = { false };
	int dist[MAX_NODE_NUM];*/
	bitset<MAX_NODE_NUM> collected;//无参构造，长度为MAX_NODE_NUM，默认每一位为０
	vector<int> dist(G_size);//dist和path是定长数组，还要支持随机存取，用vector或数组都一样啦
	vector<int> path_temp(G_size);

	//收录起点src
	dist[src] = 0;
	collected.set(src, true);

	//初始化dist和path_temp数组
	for (int i = 0; i < G_size; i++)//G[src].size应该是节点个数
	{
		/*假设是有权有向图*/
		if (G[src][i] < INFINITE) //将 从src出的边赋值
		{
			dist[i] = G[src][i];
			path_temp[i] = src;
		}
		else //????
		{
			dist[i] = INFINITE;
			path_temp[i] = -1;
			//dist.insert(dist.begin() + i, INFINITE);//将dist数组元素初始化为无穷;对未被收录的，dist[v]为起点到源点的最短距离
			//path_temp.insert(path_temp.begin() + i, -1);//将path_temp数组元素初始化为-1，path_temp数组元素为起点到节点i的路径的上一个节点
		}
	}

	/*Dijkstra*/
	int V = src;
	int minV, mindist;
	while (1)
	{
		/*收录未收录顶点中dist最小者*/
		mindist = INFINITE;
		minV = -1;
		for (int V = 0; V < G_size; V++)//遍历dist数组，找到dist最小的节点
		{
			if (collected[V] == false)//bitset支持[]运算符，返回bool
			{
				if (dist[V] < mindist)
				{
					mindist = dist[V];
					minV = V;
				}
			}
		}

		if (minV == -1) break;//若这样的顶点不存在，break（已收录所有顶点或图不连通）
		/*如果不用算完整个图还可以早一点就break*/

		V = minV;//收录该最小dist顶点
		collected[V] = true;

		/*更新dist和path_temp*/
		for (int W = 0; W < G_size; W++)
		{
			if (collected[W] == false)
			{
				if (dist[V] + G[V][W] < dist[W])//如果V到W有一条边 并且W节点未被收录到path_temp中
				{
					dist[W] = dist[V] + G[V][W];//起点到W的距离等于起点到W的距离+VW边的距离
					path_temp[W] = V;//路径中W的上一个节点是V
				}
			}
		}
	}


	/*把路径变为正序的（栈）*/

	/*从终点开始把逆序路径压入栈*/
	V = dest;
	stack<int> s;
	s.push(V);//终点压入栈
	while (path_temp.at(V) != -1)
	{
		s.push(path_temp.at(V));//V的上一个点压入栈
		V = path_temp.at(V);
	}

	/*弹栈，得到正序*/
	int i = 0;
	while (!s.empty())//栈非空
	{
		path.push_back(s.top());
		//path.at(i) = s.top();//返回栈顶的元素，但不删除该元素  
		s.pop();//删除栈顶元素但不返回其值  
		++i;
	}

	return ;
}

int main()
{
	/*cin重定向*/
	freopen("directed_weighted_graph.txt","r", stdin);

	cin >> G_size;
	for (int i = 0; i < G_size; i++)
	{
		for (int j = 0; j < G_size; j++)
		{
			G[i][j] = INFINITE;
			G[j][i] = INFINITE;
		}
	}

	int src, dest,weight;
	while ((cin >> src >> dest >> weight).good())
	{
		G[src][dest] = weight;
		G[dest][src] = weight;//无向图
	}
	cin.clear();
	freopen("CON", "r", stdin);//重定向到控制台
	cin >> src >> dest;
	vector<int> path;//path是变长数组，用vector比较好
	Dijkstra(src, dest, path);
	for (int i = 0; i < path.size(); i++)
	{
		cout << path.at(i) << " ";
	}
	return 0;
}