#include "pch.h"
#include "ClassicalForDFulkerson.h"
#include<utility>

using namespace std;

int ClassicalFordFulkerson::getEdgeIndex(int u, int v)
{
	int i;
	for ( i = 0; i < edges[u].size(); i++)
	{
		if (edges[u][i].first == v)
			break;
	}
	if (i < edges[u].size())
		return i;
	else
		return -1;
		
}

void ClassicalFordFulkerson::input()
{
	myfile >> V >> E;
	int u, v, w;
	for (int i = 0; i < V; i++)
	{
		edges[i].clear();
		backedges[i].clear();
		flow[i].clear();
	}
	for (int i = 0; i < E; i++)
	{
		myfile >> u >> v >> w;
		edges[u].push_back(make_pair(v, w));
		backedges[v].push_back(make_pair(u, w));
		flow[u].push_back(make_pair(v, 0));
	}
}

void ClassicalFordFulkerson::run()
{
	myfile >> numberOfCases;
	for (int it = 0; it < numberOfCases; it++)
	{
		input();
		for (int i = 0; i < V; i++)
			omp_init_lock(&lck[i]);

		cout << fordFulkerson()<<endl;

		for (int i = 0; i < V; i++)
			omp_destroy_lock(&lck[i]);
	}
	
}

void ClassicalFordFulkerson::init()
{
	for (int i = 0; i < V; i++)
	{
		state[i] = unlabeled;
		parent[i] = -1;
		label[i] = 0;
		labelType[i] = NULL;
	}

}

int ClassicalFordFulkerson::fordFulkerson()
{
	int flag = 1;
	int maxFlow=0;
	while (flag)
	{
		init(); // intializing
		state[0] = unscanned;
		label[0] = INF;
		while (state[V - 1] == unlabeled && flag)
		{
			flag = 0;
			for (int i = 0; i < V; i++)
				nextState[i] = state[i];

			for (int i = 0; i < V; i++)
			{

				for (int j = 0; j < edges[i].size(); j++)
				{
					int u = i, v = edges[i][j].first;
					grabLock(u, v);
					//	omp_set_lock(&lck[v]);
					if (state[u] == unscanned && state[v] == unlabeled && flow[u][j].second < edges[u][j].second)
					{
						label[v] = min(label[u], edges[u][j].second - flow[u][j].second);
						parent[v] = u;
						labelType[v] = '+';

						nextState[u] = scanned;
						nextState[v] = unscanned;
						flag = 1;
					}
					else if (state[v] == unscanned && state[u] == unlabeled && flow[u][j].second > 0)
					{
						label[u] = min(label[v], flow[u][j].second);
						parent[u] = v;
						labelType[u] = '-';

						nextState[v] = scanned;
						nextState[u] = unscanned;
						flag = 1;
					}
					omp_unset_lock(&lck[u]);
					omp_unset_lock(&lck[v]);
				}

			}

			for (int i = 0; i < V; i++)
				state[i] = nextState[i];
		}
		if (!flag)
			break;
		int x = V - 1;
		int y;
		maxFlow += label[V - 1];
		while (x != 0)
		{
			y = parent[x];
			if (labelType[x] == '+')
			{
				int j = getEdgeIndex(y, x);
				assert(j >=0);
				flow[y][j].second += label[V-1];
			}
			else if (labelType[x] == '-')
			{
				int j = getEdgeIndex(x, y);
				assert(j >= 0);
				flow[x][j].second -= label[V-1];
			}
			x = y;
		}

	}
	return maxFlow;
}

void ClassicalFordFulkerson::grabLock(int u, int v)
{
	if (u < v)
	{
		omp_set_lock(&lck[u]);
		omp_set_lock(&lck[v]);
	}
	else
	{
		omp_set_lock(&lck[v]);
		omp_set_lock(&lck[u]);
	}
		
}
