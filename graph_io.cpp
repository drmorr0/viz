// graph_io.cpp: David R. Morrison, Oct. 2013
// Implementation for graph input/output functions

#include "graph.h"
#include "graph_io.h"
#include "graph_utils.h"
#include "util.h"

#include <json_spirit.h>

#include <fstream>

using namespace std;
namespace json = json_spirit;

namespace drm
{

// Read in a graph from a DIMACS .col file
void readDIMACS(const char* filename, Graph& g)
{
	ifstream input(filename);
	if (!input) throw DRM_ERROR << "Unable to open file " << filename << " for reading.";
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
			if (type == 'a') g.setType(SimpleDirected);
			lineStr >> node1 >> node2;
			g.addEdge(node1, node2);
			if (type == 'e') g.addEdge(node2, node1);
		}
	}
}

Graph readJsonTree(const char* filename)
{
	Graph g(SimpleDirected);
	ifstream input(filename);
	if (!input) throw DRM_ERROR << "Unable to open file " << filename << " for reading.";
	
	json::Value val;
	while (json::read(input, val))
	{
		if (val.type() != json::obj_type)
			throw DRM_ERROR << "Invalid JSON format for " << filename;
		json::Object obj = val.get_obj();

		int nodeId = -1;
		int exploredAt = -1;
		int prunedAt = -1;
		int generatedAt = -1;
		double lowerBound = -Infinity;
		double upperBound = Infinity;
		string branchingVar;
		int child = -1;
		int branchDir = 0;
		double estimate = -1;
		int contour = MaxInt;

		for (int i = 0; i < obj.size(); ++i)
		{
			if      (obj[i].name_ == "node_id")       nodeId = obj[i].value_.get_int();
			else if (obj[i].name_ == "explored_at")   exploredAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "pruned_at")     prunedAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "generated_at")  generatedAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "lower_bound")   lowerBound = obj[i].value_.get_real();
			else if (obj[i].name_ == "upper_bound")   upperBound = obj[i].value_.get_real();
			else if (obj[i].name_ == "branching_var") branchingVar = obj[i].value_.get_str();
			else if (obj[i].name_ == "child")         child = obj[i].value_.get_int();
			else if (obj[i].name_ == "branch_dir")    branchDir = obj[i].value_.get_int();
			else if (obj[i].name_ == "estimate")      estimate = obj[i].value_.get_real();
			else if (obj[i].name_ == "contour")       contour = obj[i].value_.get_int();
		}

		if (nodeId == -1) continue; // Every JSON line MUST specify a node_id to be parsed

		if      (exploredAt != -1)        g.vertexData(nodeId)->expTime = exploredAt;
		else if (prunedAt != -1)          g.vertexData(nodeId)->pruneTime = prunedAt;
		else if (generatedAt != -1)       g.vertexData(nodeId)->genTime = generatedAt;
		else if (lowerBound != -Infinity) g.vertexData(nodeId)->lb = lowerBound;
		else if (upperBound != Infinity)  g.vertexData(nodeId)->ub = upperBound;
		else if (branchingVar != "")	  g.vertexData(nodeId)->branchVar = branchingVar;
		else if (child != -1)		      g.addEdge(nodeId, child);
		else if (branchDir != 0)          g.vertexData(nodeId)->branchDir=(drm::BranchDir)branchDir;
		else if (estimate != -1)		  g.vertexData(nodeId)->estimate = estimate;
		else if (contour != MaxInt)		  g.vertexData(nodeId)->contour = contour;
	}
	return g;
}

/* Printing functions */
void print(const Graph& g) 
{
    for (auto i = g.begin(); i != g.end(); ++i)
    {
        printf("%d: ", i->first);
        for (int j = 0; j < i->second.size(); ++j)
            printf("%d ", i->second[j]);
        printf("\n");
    }
}

void printShort(const Graph& g) 
{
    printf("Number of nodes: %d\n", g.order());
    printf("Number of arcs: %d\n", g.size());
    printf("Density: %0.2f\n", GraphUtils::density(g));
}



};


