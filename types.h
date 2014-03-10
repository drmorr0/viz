#ifndef TYPES_H
#define TYPES_H

#include <graph.h>

enum BranchDir { Down = -1, Up = 1 };

struct BnbVertexData : public graph::VertexData
{
	int expTime;
	int pruneTime;
	int genTime;
	double lb, ub;
	double estimate;
	int contour;
	string branchVar;
	BranchDir branchDir;

	BnbVertexData* clone() { return new BnbVertexData(*this); }
	~BnbVertexData() { }
};


#endif // TYPES_H



