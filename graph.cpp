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

#include <sstream>

namespace drm
{

// Create an empty graph
Graph::Impl::Impl(GraphType type, Graph* g) :
	theGraph(g),
	mType(type),
	mSource(-1),
	mSink(-1), 
	mNumEdges(0)
{
}

// Read in a graph from a file.  Currently-supported filetypes are DIMACS and GraphViz DOT (TODO)
Graph::Impl::Impl(const char* filename, FileType type, Graph* g) :
	Impl(SimpleUndirected, g)
{
	ifstream input(filename);
	if (!input) throw ERROR << "Unable to open file " << filename << " for reading.";

	switch (type)
	{
		case DIMACS: readDIMACS(input); break;
		case DOT:    readDot(input); break;
	}

	input.close();
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

// Check to see if an edge exists
bool Graph::Impl::hasEdge(int u, int v) const
{
	auto iter = mAdjList.find(u);
	return (iter != mAdjList.end() && contains(iter->second, v));
}

// Read in a graph from a DIMACS .col file
void Graph::Impl::readDIMACS(ifstream& input)
{
    string nextLine;

    // Read from file
    while (input.good())
    {
        getline(input, nextLine);
        if (nextLine.empty()) continue;

        int node1, node2;
        char type;

        stringstream lineStr(nextLine);
        lineStr >> type;
        if (type == 'a' || type == 'e')
        {
            lineStr >> node1 >> node2;
            addEdge(node1, node2);
			if (type == 'e')
				addEdge(node2, node1);
			else mType = SimpleDirected;
        }
    }
}

void Graph::Impl::readDot(ifstream& input)
{
	// TODO
}

/* Printing functions */
void Graph::Impl::print() const
{
    for (auto i = mAdjList.begin(); i != mAdjList.end(); ++i)
	{
        printf("%d: ", i->first);
        for (int j = 0; j < i->second.size(); ++j) 
            printf("%d ", i->second[j]);
        printf("\n");
    }
}

void Graph::Impl::printShort() const
{
    printf("Number of nodes: %d\n", order());
    printf("Number of arcs: %d\n", size());
    printf("Density: %0.2f\n", GraphUtils::density(*theGraph));
}


/**********************************************************/
/* Handle functions redirecting from Graph to Graph::Impl */
/**********************************************************/

Graph::Graph(GraphType type) : theImpl(new Impl(type, this)) { }
Graph::Graph(const char* filename, FileType type) : theImpl(new Impl(filename, type, this)) { }
Graph::Graph(const Graph& g) : theImpl(new Impl(*g.theImpl)) { }
Graph& Graph::operator=(Graph rhs) { swap(rhs); return *this; }
Graph::~Graph() { }		// Necessary so the unique_ptr destructor can be instantiated properly

void Graph::addVertex(int u) { theImpl->addVertex(u); }
void Graph::addEdge(int u, int v) { theImpl->addEdge(u, v); }
void Graph::delVertex(int u) { theImpl->delVertex(u); }
void Graph::delEdge(int u, int v) { theImpl->delEdge(u, v); }
void Graph::setSource(int s) { theImpl->setSource(s); }
void Graph::setSink(int t) { theImpl->setSink(t); }

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
int Graph::getSource() const { return theImpl->getSource(); }
int Graph::getSink() const { return theImpl->getSink(); }

bool Graph::hasEdge(int u, int v) const { return theImpl->hasEdge(u, v); }

void Graph::print() const { theImpl->print(); }
void Graph::printShort() const { theImpl->printShort(); }

Graph::graph_iterator Graph::begin() const { return theImpl->begin(); }
Graph::graph_iterator Graph::end() const { return theImpl->end(); }


}; // namespace drm
