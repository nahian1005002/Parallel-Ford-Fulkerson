#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<algorithm>
#include<omp.h>
#include<cassert>
#include<string>

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
protected:
	std::ifstream myfile;
	std::string filePath;
	int numberOfCases;
	int V, E;
	std::vector<std::pair<int,int> > edges[MAXNODES];
	std::vector<std::pair<int, int> > backedges[MAXNODES];
	std::vector<std::pair<int, int> > flow[MAXNODES];
	omp_lock_t lck[MAXNODES];
	int parent[MAXNODES];
	int label[MAXNODES];
	char labelType[MAXNODES];
	int state[MAXNODES];
	int nextState[MAXNODES];
	int getEdgeIndex(int u, int v);
	void input();
	void init();
	int fordFulkerson();
	void grabLock(int u, int v);
public:
	ClassicalFordFulkerson(std::string filePath) {
		this->filePath = filePath;
	}
	void run();
};
