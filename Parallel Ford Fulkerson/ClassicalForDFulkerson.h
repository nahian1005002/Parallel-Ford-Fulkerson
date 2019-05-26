#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<algorithm>
#include<omp.h>

#define MAXNODES 1000
#define INF 99999999

enum States
{
	scanned,
	unscanned,
	unlabeled
};

class ClassicalFordFulkerson
{
	std::ifstream myfile;
	int numberOfCases;
	int V, E;
	std::vector<std::pair<int,int> > edges[MAXNODES];
	std::vector<std::pair<int, int> > backedges[MAXNODES];
	std::vector<std::pair<int, int> > flow[MAXNODES];
	omp_lock_t lck[MAXNODES];
	int parent[MAXNODES];
	int label[MAXNODES];
	int state[MAXNODES];
	int nextState[MAXNODES];
	ClassicalFordFulkerson() {
		myfile.open("input.txt");
	}
	void input();
	void run();
	void init();
	void fordFulkerson();
	void grabLock(int u, int v);
};
