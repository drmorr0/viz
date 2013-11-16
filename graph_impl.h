#ifndef GRAPH_IMPL_H
#define GRAPH_IMPL_H
/*
 * graph_impl.h: David R. Morrison, Sept. 2013
 *
 * Implementation class for Graph objects
 */

#include "util.h"

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>

namespace std
{
template<>
void swap<drm::Graph>(drm::Graph& a, drm::Graph& b) { a.swap(b); }
}

using namespace std;

namespace drm
{

class Graph::Impl
{
public:
	
	Impl(GraphType type);
	~Impl();

	void addVertex(int u);
	void addEdge(int u, int v);
	void delVertex(int u);
	void delEdge(int u, int v);
	void setSource(int s) { mSource = s; }
	void setSink(int t) { mSink = t; }
	void setType(GraphType t) { mType = t; }

	int order() const { return mAdjList.size(); }
	int size() const { return mNumEdges; }
	bool isDirected() const { return mType == SimpleDirected; }
	bool isSimple() const { return true; /* TODO */ }
	bool isWeighted() const { return false; /* TODO */ }
	GraphType type() const { return mType; }

	vector<int> neighbors(int u) const;
	int degree(int u) const; 
	int outdegree(int u) const { return degree(u); }
	int indegree(int u) const;
	int source() const { return mSource; }
	int sink() const { return mSink; }
	VertexData* const vertexData(int u);

	bool hasEdge(int u, int v) const; 

private:

	// Should never assign Graph::Impl
	Impl& operator=(const Impl& other);

	// Graph structure
	GraphType mType;
	map<int, vector<int>> mAdjList;
	map<int, vector<int>> mRevAdjList;
	map<int, VertexData*> mVertexData;

	int mSource, mSink;
	int mNumEdges;

	// NOTE: If more members added, make sure to update copy constructor

public:
	// Iterators
    graph_iterator begin() const { return mAdjList.cbegin(); }
    graph_iterator end() const { return mAdjList.cend(); }

	// Copy constructor -- update when new members added
	Impl(const Impl& other);
};


}; // namespace drm

#endif // GRAPH_IMPL_H
