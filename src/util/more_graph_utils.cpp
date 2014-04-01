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

/*
 * Loop through all vertices of the specified graph and return a list of the ones that match
 * the specified rule.  I'm not exactly sure how efficient this is going to be for large graphs...
 */
vector<int> match(const Graph& g, const string& filterText, MatchOp op, double value)
{
	vector<int> matched;
	for (auto v = g.begin(); v != g.end(); ++v)
	{
		BnbVertexData* data = (BnbVertexData*)g.vertexData(v->first);
		if (!data) continue;	// If a vertex has no data associated, skip it

		for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
		{
			// Check to see if the property contains the specified filter text.  Right now it must
			// match exactly, though being able to do something like a regex might be cool...
			if (prop->first.find(filterText) != string::npos)
			{
				double propVal;
				try { propVal = stod(prop->second); } catch (invalid_argument& e) { continue; }

				// How to handle non-numeric properties? TODO
				if ((op == Less 	 && propVal <  value) ||
					(op == LessEq	 && propVal <= value) ||
					(op == Eq		 && propVal == value) ||
					(op == NotEq	 && propVal != value) ||
					(op == GreaterEq && propVal >= value) ||
					(op == Greater	 && propVal >  value))
					matched.push_back(v->first);
			}
		}
	}

	return matched;
}

}; // namespace graph
