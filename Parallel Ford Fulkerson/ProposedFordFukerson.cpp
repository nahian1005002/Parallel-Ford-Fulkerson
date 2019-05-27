#include "pch.h"
#include "ProposedFordFukerson.h"
#include <chrono>
#include <atomic>

using namespace std;


int ProposedFordFukerson::fordFulkerson()
{
	cout << "inside proposed fordfulkerson" << endl;
	int flag = 1;
	int maxFlow = 0;
	int turn = 0,turnPrev=1;
	while (flag)
	{
		init(); // intializing
		state[0] = unscanned;
		label[0] = INF;
		scanFrontier[turn][0] = 0;
		frontierSize[turn] = 1;
		while (state[V - 1] == unlabeled && flag)
		{
			flag = 0;
			turn = 1 - turn;
			turnPrev = 1 - turnPrev;
			std::atomic<int> idx = 0;


			for (int i = 0; i < frontierSize[turnPrev]; i++)
			{
				int u = scanFrontier[turnPrev][i];
				for (int j = 0; j < edges[u].size(); j++)
				{
					int v = edges[u][j].first;
					grabLock(u, v);

					if ( state[v] == unlabeled && flow[u][j].second < edges[u][j].second)
					{
						label[v] = min(label[u], edges[u][j].second - flow[u][j].second);
						parent[v] = u;
						labelType[v] = '+';

						state[v] = unscanned;
						flag = 1;
						scanFrontier[turn][idx++] = v;
					}
					omp_unset_lock(&lck[u]);
					omp_unset_lock(&lck[v]);
				}
				for (int j = 0; j < backedges[u].size(); j++)
				{
					int w = backedges[u][j].first;
					int k = backedges[u][j].second;
					grabLock(u, w);
					if (state[w] == unlabeled && flow[w][k].second > 0)
					{
						label[w] = min(label[u], flow[w][k].second);
						parent[w] = u;
						labelType[w] = '-';

						state[w] = unscanned;
						flag = 1;
						scanFrontier[turn][idx++] = w;
					}
					omp_unset_lock(&lck[u]);
					omp_unset_lock(&lck[w]);
				}

				omp_set_lock(&lck[u]);
				state[u] = scanned;
				omp_unset_lock(&lck[u]);
				frontierSize[turn] = idx;
			}

//			for (int i = 0; i < V; i++)
//				state[i] = nextState[i];
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
