/*
 * more_graph_utils.h: David R. Morrison, March 2014
 *
 * Additional utilities for graphs
 */

#include <vector>

namespace graph
{
	class Graph;

	// Subgraph stuff
	std::vector<int> getReachable(const Graph& g, const std::vector<int>& start);
};
