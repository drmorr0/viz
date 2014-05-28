/*
 * more_graph_utils.h: David R. Morrison, March 2014
 *
 * Additional utilities for graphs
 */

#include "types.h"
#include "match.h"
#include <graph_layout.h>

#include <vector>
#include <string>

namespace graph
{
	class Graph;

	std::vector<int> match(const Graph& g, const std::string& filterText, Match::Operator op, 
			double value);
	GraphLayout improvedLayoutTreeLevel(const Graph&, int radius, int hSpace, int vSpace);
};
