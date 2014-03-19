// more_graph_utils.cpp: David R. Morrison
// Additional utility functions for working with graphs

#include "more_graph_utils.h"

#include <graph.h>

using namespace std;

namespace graph
{

// Run DFS to find all vertices reachable from a given input set
vector<int> getReachable(const Graph& g, const vector<int>& start)
{
	vector<int> reachable;
	vector<int> stack(start);
	map<int, bool> visited;

	while (!stack.empty())
	{
		int u = stack.back(); stack.pop_back();
		visited[u] = true;
		reachable.push_back(u);
		vector<int> n = g.neighbors(u);
		for (int i = 0; i < n.size(); ++i)
			if (visited.count(n[i]) == 0)
				stack.push_back(n[i]);
	}

	return reachable;
}

}; // namespace graph
