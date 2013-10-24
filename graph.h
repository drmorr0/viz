#ifndef GRAPH_H
#define GRAPH_H
/*
 * graph.h: David R. Morrison, Sept. 2013
 *
 * Class that encompasses all of the graph data for the graph visualization software; the graph
 * class can read from a variety of different file types, or from an adjacency matrix or adjacency
 * list.
 */

#include "util.h"

#include <vector>
#include <memory>
#include <string>
#include <map>

namespace drm
{
using std::vector;
using std::string;
using std::unique_ptr;
using std::map;

enum GraphType { SimpleUndirected, SimpleDirected };
enum FileType { DIMACS, DOT, JSON_Tree };
enum BranchDir { Up, Down };

class Graph
{

public:
	
	struct VertexData
	{
		// Information about the vertex
		string name;
		int genTime, expTime;
		int branchVar;
		BranchDir branchDir;
		double lb, ub;
		string info;

		// Positioning information
		Point center;
		int radius;

		//color....
	};

	// Constructors, assignment operator, destructor
	Graph(GraphType type);
	Graph(const char* filename, FileType type);
	Graph(const Graph& g);
	Graph& operator=(Graph g);
	void swap(Graph& g) { using std::swap; swap(theImpl, g.theImpl); }
	~Graph();

	// Graph modification
	void addVertex(int u);
	void addEdge(int u, int v);
	void delVertex(int u);
	void delEdge(int u, int v);
	void setSource(int s);
	void setSink(int t);

	// Global graph properties
	int order() const;
	int size() const;
	bool isDirected() const;
	bool isSimple() const;
	bool isWeighted() const;
	GraphType type() const;

	// Vertex properties
	vector<int> neighbors(int u) const;
	int degree(int u) const;
	int outdegree(int u) const;
	int indegree(int u) const;
	int getSource() const;
	int getSink() const;
	VertexData* const vertexData(int u);

	// Edge properties
	bool hasEdge(int u, int v) const;

	// Output
	void print() const;
	void printShort() const;
	
private:
	class Impl;
	unique_ptr<Impl> theImpl;

public:
	// Iterators
	typedef map<int, vector<int>>::const_iterator graph_iterator;
	graph_iterator begin() const;
	graph_iterator end() const;
};


}; // namespace drm

#endif // GRAPH_H
