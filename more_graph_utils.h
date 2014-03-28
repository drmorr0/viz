/*
 * more_graph_utils.h: David R. Morrison, March 2014
 *
 * Additional utilities for graphs
 */

#include "types.h"

#include <vector>
#include <string>

namespace graph
{
	class Graph;

	// Subgraph stuff
	std::vector<int> getReachable(const Graph& g, const std::vector<int>& start);
	std::vector<int> match(const Graph& g, const std::string& filterText, MatchOp op, double value);
};
