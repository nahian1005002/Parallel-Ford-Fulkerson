#pragma once
#include "ClassicalForDFulkerson.h"


class ProposedFordFukerson :
	public ClassicalFordFulkerson
{
protected:
	int scanFrontier[2][MAXNODES];
	int frontierSize[2];
	int fordFulkerson();
public:
	ProposedFordFukerson(std::string filePath) :ClassicalFordFulkerson(filePath) {}
	~ProposedFordFukerson();
	void run();
};

