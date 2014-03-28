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

vector<int> match(const Graph& g, const string& filterText, MatchOp op, double value)
{
	vector<int> matched;
	for (auto v = g.begin(); v != g.end(); ++v)
	{
		BnbVertexData* data = (BnbVertexData*)g.vertexData(v->first);
		if (!data) continue;

		for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
		{
			if (prop->first.find(filterText) != string::npos)
			{
				double propVal;
				try { propVal = stod(prop->second); } catch (invalid_argument& e) { continue; }

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
