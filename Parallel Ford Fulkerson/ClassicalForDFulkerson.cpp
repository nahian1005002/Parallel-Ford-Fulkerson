#include "pch.h"
#include "ClassicalForDFulkerson.h"
#include<utility>

using namespace std;

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
	}
	
}

void ClassicalFordFulkerson::init()
{
	for (int i = 0; i < V; i++)
	{
		state[i] = unlabeled;
		parent[i] = -1;
		label[i] = 0;
	}

}

void ClassicalFordFulkerson::fordFulkerson()
{
	init(); // intializing
	state[0] = unscanned;
	label[0] = INF;
	while (state[V - 1] == unlabeled)
	{
		for (int i = 0; i < V; i++)
			nextState[i] = state[i];

		for (int i = 0; i < V; i++)
		{
		/*	if (!omp_test_lock(&lck[i]))
				continue;
			int flag = 0;
			omp_set_lock(&lck[i]);*/

			for (int j = 0; j < edges[i].size(); j++)
			{
				int u = i, v = edges[i][j].first;
				grabLock(u, v);
			//	omp_set_lock(&lck[v]);
				if (state[u] == unscanned && state[v] == unlabeled && flow[u][j].second < edges[u][j].second)
				{
					label[v] = min(label[u], edges[u][j].second - flow[u][j].second);
					parent[v] = u;

				//	flag = 1;
				//	state[v] = unscanned;
					nextState[u] = scanned;
					nextState[v] = unscanned;
				}
				else if (state[v] == unscanned && state[u] == unlabeled && flow[u][j].second > 0)
				{
					label[u] = -min(label[v], flow[u][j].second);
					parent[u] = v;

					nextState[v] = scanned;
					nextState[u] = unscanned;
				//	state[u] = unscanned;
				//	state[v] = scanned;
				//	break;
				}
			}

		/*	if (flag == 1 && state[i] == unscanned)
			{
				state[i] = scanned;
			}*/
		}

		for (int i = 0; i < V; i++)
			state[i] = nextState[i];
	}
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
