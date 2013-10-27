// graph.cpp: David R. Morrison
//
// Implementation details for the Graph class; this class follows the pimpl design pattern, and 
// all of the implementation-specific code is contained within the subclass Graph::Impl.
//
// Internally, the graph is stored as an adjacency list; to distinguish between degree-0 vertices
// and non-existant vertices, an additional vector<bool> is stored to keep track of whether a vertex
// is ``real'' or not

#include "graph.h"
#include "graph_impl.h"
#include "graph_utils.h"
#include "util.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace drm
{

// Create an empty graph
Graph::Impl::Impl(GraphType type) :
	mType(type),
	mSource(-1),
	mSink(-1), 
	mNumEdges(0)
{
}

// Add a new vertex to the graph
void Graph::Impl::addVertex(int u)
{
	// operator[] adds an empty element to the map if the vertex doesn't
	// exist, and does nothing if it already does.  No messy resize needed!
	mAdjList[u];
}

// Add an edge to the graph (note that the endpoints to the edge do not 
// necessarily have to exist)
void Graph::Impl::addEdge(int u, int v)
{
	if (isSimple() && hasEdge(u, v)) return;

	addVertex(u);
	addVertex(v);

	++mNumEdges;
	mAdjList[u].push_back(v);
	if (!isDirected()) mAdjList[v].push_back(u);
	else mRevAdjList[v].push_back(u);
}

// Delete a vertex from the graph
void Graph::Impl::delVertex(int u)
{
	if (mAdjList.find(u) == mAdjList.end()) return;

	// Delete all incident edges
	for (int i = 0; i < mAdjList[u].size(); ++i)
		delEdge(u, mAdjList[u][i]);
	if (isDirected() && mRevAdjList.find(u) != mRevAdjList.end())	
	{
		// Delete incoming edges if the graph is directed
		vector<int> revNeighbors = mRevAdjList[u];
		for (int i = 0; i < revNeighbors.size(); ++i)
			delEdge(revNeighbors[i], u);
		mRevAdjList.erase(u);
	}

	mAdjList.erase(u);
	mVertexData.erase(u);
}

// Delete an edge from the graph
void Graph::Impl::delEdge(int u, int v)
{
	if (mAdjList.find(u) == mAdjList.end()) return;

	if (contains(mAdjList[u], v))
	{
		--mNumEdges;
		remove(mAdjList[u], v);
		if (!isDirected()) remove(mAdjList[v], u);
		else remove(mRevAdjList[v], u);
	}
}

// Return a list of neighbors of a vertex
vector<int> Graph::Impl::neighbors(int u) const
{
	auto iter = mAdjList.find(u);
	return iter != mAdjList.end() ? iter->second : vector<int>();
}

// Return the degree of a given vertex
int Graph::Impl::degree(int u) const
{
	auto iter = mAdjList.find(u);
	return iter != mAdjList.end() ? iter->second.size() : 0;
}

// Return the number of incoming edges incident to a vertex
int Graph::Impl::indegree(int u) const
{
	if (!isDirected()) return degree(u);
	auto iter = mRevAdjList.find(u);
	return iter != mRevAdjList.end() ? iter->second.size() : 0;
}

// Return a pointer to the vertex data associated with vertex u
Graph::VertexDataPtr const Graph::Impl::vertexData(int u) 
{
	if (mAdjList.count(u) == 0) addVertex(u);
	if (mVertexData.count(u) == 0)
		mVertexData[u].set(new Graph::VertexData);
	return mVertexData[u];
}

// Check to see if an edge exists
bool Graph::Impl::hasEdge(int u, int v) const
{
	auto iter = mAdjList.find(u);
	return (iter != mAdjList.end() && contains(iter->second, v));
}

/**********************************************************/
/* Handle functions redirecting from Graph to Graph::Impl */
/**********************************************************/

Graph::Graph(GraphType type) : theImpl(new Impl(type)) { }
Graph::Graph(const Graph& g) : theImpl(new Impl(*g.theImpl)) { }
Graph& Graph::operator=(Graph rhs) { swap(rhs); return *this; }
Graph::~Graph() { if (theImpl) delete theImpl; }		

void Graph::addVertex(int u) { theImpl->addVertex(u); }
void Graph::addEdge(int u, int v) { theImpl->addEdge(u, v); }
void Graph::delVertex(int u) { theImpl->delVertex(u); }
void Graph::delEdge(int u, int v) { theImpl->delEdge(u, v); }
void Graph::setSource(int s) { theImpl->setSource(s); }
void Graph::setSink(int t) { theImpl->setSink(t); }
void Graph::setType(GraphType t) { theImpl->setType(t); }

int Graph::order() const { return theImpl->order(); }
int Graph::size() const { return theImpl->size(); }
bool Graph::isDirected() const { return theImpl->isDirected(); }
bool Graph::isSimple() const { return theImpl->isSimple(); }
bool Graph::isWeighted() const { return theImpl->isWeighted(); }
GraphType Graph::type() const { return theImpl->type(); }

vector<int> Graph::neighbors(int u) const { return theImpl->neighbors(u); }
int Graph::degree(int u) const { return theImpl->degree(u); }
int Graph::outdegree(int u) const { return theImpl->outdegree(u); }
int Graph::indegree(int u) const { return theImpl->indegree(u); }
int Graph::source() const { return theImpl->source(); }
int Graph::sink() const { return theImpl->sink(); }
Graph::VertexDataPtr const Graph::vertexData(int u) { return theImpl->vertexData(u); }

bool Graph::hasEdge(int u, int v) const { return theImpl->hasEdge(u, v); }

Graph::graph_iterator Graph::begin() const { return theImpl->begin(); }
Graph::graph_iterator Graph::end() const { return theImpl->end(); }


}; // namespace drm
