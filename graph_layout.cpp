// graph_layout.cpp: David R. Morrison
// Implementations of various graph drawing algorithms

#include "graph.h"
#include "graph_layout.h"

#include <list>
#include <map>

using namespace std;

namespace drm
{

namespace GraphUtils
{

struct SubtreeBlock
{
	int width, rootX;
};

SubtreeBlock layoutTreeLevelHelper(const Graph& g, int rId, map<int, Point>& relVertexPos, 
		int xOffset, int vSpace, int hSpace)
{
	printf("computing layout for subtree at %d\n", rId);
	Graph::VertexData* rData = g.vertexData(rId);
	int rDeg = g.outdegree(rId);

	// Base case -- we have no children, so our span is just the width of the single node
	if (rDeg == 0) 
	{
		return { 2 * rData->radius, rData->radius + xOffset };
	}

	// Loop through all the neighbors and recursively compute a layout for them
	vector<SubtreeBlock> blocks;
	int xOffsetChild = 0;
	for (int i = 0; i < rDeg; ++i)
	{
		blocks.push_back(layoutTreeLevelHelper(g, g.neighbors(rId)[i], relVertexPos, 
					xOffsetChild, vSpace, hSpace));
		xOffsetChild += blocks[i].width;
		if (i != rDeg - 1) xOffsetChild += hSpace;
	}

	// Always place the root halfway across the block, and then modify by the x-offset
	int rootX = blocks[rDeg].width / 2 + xOffset;

	for (int i = 0; i < rDeg; ++i)
	{
		int vertex = g.neighbors(rId)[i];
		relVertexPos[vertex].x = blocks[i].rootX - rootX;
		relVertexPos[vertex].y = vSpace;
	}

	return { xOffsetChild, rootX };
}

void layoutTreeLevel(const Graph& g, const Point& rootPos, int vSpace, int hSpace)
{
	map<int, Point> relVertexPos;
	relVertexPos[0] = rootPos;
	layoutTreeLevelHelper(g, 0, relVertexPos, 0, vSpace, hSpace);

	list<int> queue{0};
	while (!queue.empty())
	{
		int currNode = queue.front(); queue.pop_front();
		Graph::VertexData* currData = g.vertexData(currNode);
		for (int i = 0; i < g.outdegree(currNode); ++i)
		{
			int child = g.neighbors(currNode)[i];
			Graph::VertexData* childData = g.vertexData(child);
			childData->center.x = currData->center.x + relVertexPos[child].x;
			childData->center.y = currData->center.y + relVertexPos[child].y;
		}
	}
}

}; // namespace GraphUtils

}; // namespace drm





