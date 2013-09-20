// graph_order.cpp: David R. Morrison, Sept. 2013
// Functions to return permutations of the graph vertices

#include "graph.h"

#include <vector>
#include <list>
#include <algorithm>
#include <stdlib.h>

using namespace std;

namespace drm
{

namespace GraphUtils
{

int myrandom(int i) { return lrand48() % i; }

vector<int> identityVertexOrder(const Graph& g)
{
	vector<int> verts;
	for (auto i = g.begin(); i != g.end(); ++i)
		verts.push_back(i->first);

	return verts;
}

vector<int> randomVertexOrder(const Graph& g)
{
	vector<int> verts = identityVertexOrder(g);
	random_shuffle(verts.begin(), verts.end(), [] (int i) { return lrand48() % i; });

	return verts;
}

vector<int> degreeListVertexOrder(const Graph& g)
{
	vector<int> verts = identityVertexOrder(g);
	sort(verts.begin(), verts.end(), [&g] (int u, int v) { return g.degree(u) > g.degree(v); });

	return verts;
}

vector<int> outdegreeListVertexOrder(const Graph& g) { return degreeListVertexOrder(g); }

vector<int> indegreeListVertexOrder(const Graph& g)
{
	vector<int> verts = identityVertexOrder(g);
	sort(verts.begin(), verts.end(), [&g] (int u, int v) { return g.indegree(u) > g.indegree(v); });

	return verts;
}

vector<int> degreeSequence(const Graph& g)
{
	vector<int> seq;
	for (auto i = g.begin(); i != g.end(); ++i)
		seq.push_back(i->second.size());
	sort(seq.begin(), seq.end(), [] (int d1, int d2) { return d1 > d2; });
	return seq;
}

vector<int> outdegreeSequence(const Graph& g) { return degreeSequence(g); }

vector<int> indegreeSequence(const Graph& g)
{
	vector<int> seq;
	for (auto i = g.begin(); i != g.end(); ++i)
		seq.push_back(g.indegree(i->first));
	sort(seq.begin(), seq.end(), [] (int d1, int d2) { return d1 > d2; });
	return seq;
}

}; // namespace GraphUtils

}; // namespace drm



