#pragma once
#include "ClassicalForDFulkerson.h"


class ProposedFordFukerson :
	public ClassicalFordFulkerson
{
protected:
	int fordFulkerson();
public:
	ProposedFordFukerson(std::string filePath) :ClassicalFordFulkerson(filePath) {}
	~ProposedFordFukerson();
	void run();
};

