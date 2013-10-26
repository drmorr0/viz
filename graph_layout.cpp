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

SubtreeBlock computeSubtreeLayout(Graph& g, int rId, map<int, Point>& relVertexPos, 
		int vSpace, int hSpace)
{
	auto rData = g.vertexData(rId);
	int rDeg = g.outdegree(rId);

	// Base case -- we have no children, so our span is just the width of the single node
	if (rDeg == 0) return { 2 * rData->radius, rData->radius };

	// Loop through all the neighbors and recursively compute a layout for them
	int width = 0;
	for (int i = 0; i < rDeg; ++i)
	{
		int child = g.neighbors(rId)[i];
		SubtreeBlock block = computeSubtreeLayout(g, child, relVertexPos, vSpace, hSpace);

		// The x-coordinate is just the position of the root in the sub-block plus the width
		// of the current block
		relVertexPos[child].x = (width + block.rootX);
		relVertexPos[child].y = vSpace;

		// Increase the width of the current block; if this isn't the last subtree, add in spacing
		width += block.width;
		if (i != rDeg - 1) width += hSpace;
	}

	// Always place the root halfway across the block, then normalize the x-coordinates to be
	// relative to the root position
	int rootX = width / 2;
	for (int i = 0; i < rDeg; ++i)
		relVertexPos[g.neighbors(rId)[i]].x -= rootX;

	return { width, rootX };
}

void layoutTreeLevel(Graph& g, const Point& rootPos, int vSpace, int hSpace)
{
	// Initialize a map of vertex positions relative to the vertex's parent; i.e.,
	// relVertexPos[i].x is the position of vertex i relative to the parent of i
	map<int, Point> relVertexPos;

	// Initialize the root of the tree
	auto rootData = g.vertexData(0);
	rootData->center.x = rootPos.x;
	rootData->center.y = rootPos.y;

	// Compute the subtree layout in terms of relative vertex positions
	computeSubtreeLayout(g, 0, relVertexPos, vSpace, hSpace);

	// Translate relative positions into absolute positions using breadth-first search
	list<int> queue{0};
	while (!queue.empty())
	{
		int currNode = queue.front(); queue.pop_front();
		auto currData = g.vertexData(currNode);
		for (int i = 0; i < g.outdegree(currNode); ++i)
		{
			int child = g.neighbors(currNode)[i];
			auto childData = g.vertexData(child);
			childData->center.x = currData->center.x + relVertexPos[child].x;
			childData->center.y = currData->center.y + relVertexPos[child].y;
			queue.push_back(child);
		}
	}
}

}; // namespace GraphUtils

}; // namespace drm





