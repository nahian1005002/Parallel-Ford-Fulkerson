// Parallel Ford Fulkerson.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

struct Edge
{
	int u;
	int v;
	int w;
	Edge(int a, int b, int c)
	{
		u = a;
		v = b;
		w = c;
	}
};

int getHeadNode(int i,int V,int cardinality)
{
	int y = i + 1 + rand() % (cardinality);
	if (y >= V)
		return -1;
}

void generateInput()
{
	ofstream myfile("input.txt");
	if (!myfile.is_open())
	{
		cout << "failed to create file\n";
		return;
	}
	int numberOfTests = 2; 
	myfile << numberOfTests << endl;
	int V, cardinality=5;
	vector<Edge> edges;
	int lowestWeight = 5;
	set<int> headNodes;
	for (int it = 0; it < numberOfTests; it++)
	{
		edges.clear();
		V = 8 + it * 10 ;
		for (int i = 0; i < V-1; i++)
		{
			headNodes.clear();
			for (int j = 0; j < cardinality; j++)
			{
				int headNode = getHeadNode(i,V,cardinality);
				if (headNode != -1 && headNodes.find(headNode)==headNodes.end())
				{
					edges.push_back(Edge(i, headNode, lowestWeight + rand() % 50));
					headNodes.insert(headNode);
				}
				//myfile << i << ' ' << i+1+rand()%10 <<' '<<5+rand()%50<< endl;
			}
		}
		myfile << V << ' ' << edges.size() << endl;
		for (int i = 0; i < edges.size(); i++)
		{
			myfile << edges[i].u << ' ' << edges[i].v << ' ' << edges[i].w << endl;
		}
	}
	myfile.close();
}

int main()
{
    std::cout << "Hello World!\n";
	generateInput();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
