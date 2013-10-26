// graph_utils.cpp: David R. Morrison, Sept. 2013
// Non-member non-friend utility functions for the graph class

#include "graph.h"

using namespace std;

namespace drm
{

namespace GraphUtils
{

// Density is computed as number of edges in G divided by total possible number of edges
double density(const Graph& g) 
{
    double density = g.size() / (g.order() * (g.order() - 1.0));
    return (g.isDirected() ? density : 2 * density);
}

// Test if a given set is an independent set in g
bool testIndSet(const Graph& g, const vector<int>& set) 
{
	if (g.isDirected()) return false;

	// If any edges exist, it's not an independent set
    for (int i = 0; i < set.size(); ++i)
        for (int j = 0; j < set.size(); ++j)
            if (i != j && g.hasEdge(set[i], set[j]))
                return false;
    return true;
}

// Test if a given set is a clique in g
bool testClique(const Graph& g, const vector<int>& set) 
{
	if (g.isDirected()) return false;

	// If any edges don't exist, it's not a clique
    for (int i = 0; i < set.size(); ++i)
        for (int j = 0; j < set.size(); ++j)
            if (i != j && !g.hasEdge(set[i], set[j]))
                return false;
    return true;
}

Graph complement(const Graph& g)
{
	Graph gc(g.type());
	for (auto i = g.begin(); i != g.end(); ++i)
	{
		for (auto j = g.begin(); j != g.end(); ++j)
		{
			int u = i->first;
			int v = j->first;
			if ((u != v || !g.isSimple()) && !g.hasEdge(u, v))
				gc.addEdge(u, v);
		}
	}
	return gc;
}

/*
// Compute a list of vertices ordered in descending order of degree
void Graph::Impl::computeSortedVertices() const
{
    mSortedVertices.clear();
    for (int i = 0; i < mAdjList.size(); ++i)
        if (mVertexExists[i]) mSortedVertices.push_back(i);
    sort(mSortedVertices.begin(), mSortedVertices.end(),
            [this] (int v1, int v2) { return (this->degree(v1) > this->degree(v2)); });
}*/


}; // namespace GraphUtils
}; // namespace drm

