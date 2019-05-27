#include "pch.h"
#include "ProposedFordFukerson.h"
#include <chrono>

using namespace std;


int ProposedFordFukerson::fordFulkerson()
{
	cout << "inside proposed fordfulkerson" << endl;
	int flag = 1;
	int maxFlow = 0;
	while (flag)
	{
		init(); // intializing
		state[0] = unscanned;
		label[0] = INF;
		while (state[V - 1] == unlabeled && flag)
		{
			flag = 0;
#pragma omp parallel for shared(nextState,state,V) private(i)
			for (int i = 0; i < V; i++)
				nextState[i] = state[i];

#pragma omp parallel for shared(nextState,state,V,edges,flow,label,parent,labelType,flag,lck) private(i,j,u,v)
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

#pragma omp parallel for shared(nextState,state,V) private(i)
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
				assert(j >= 0);
				flow[y][j].second += label[V - 1];
			}
			else if (labelType[x] == '-')
			{
				int j = getEdgeIndex(x, y);
				assert(j >= 0);
				flow[x][j].second -= label[V - 1];
			}
			x = y;
		}

	}
	return maxFlow;
}

ProposedFordFukerson::~ProposedFordFukerson()
{
}

void ProposedFordFukerson::run()
{
	myfile.open(filePath);
	cout << "inside proposed run" << endl;
	myfile >> numberOfCases;
	for (int it = 0; it < numberOfCases; it++)
	{
		input();
		omp_set_num_threads(1);
		for (int i = 0; i < V; i++)
			omp_init_lock(&lck[i]);

		auto start = std::chrono::high_resolution_clock::now();
		cout << fordFulkerson() << endl;
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		cout << "Time:" << duration.count() << " micro sec" << endl;
		for (int i = 0; i < V; i++)
			omp_destroy_lock(&lck[i]);
	}
	myfile.close();
}
