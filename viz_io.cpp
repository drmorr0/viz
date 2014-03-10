// viz_io.cpp: David R. Morrison, Oct. 2013
// Implementation for visualizer input/output functions

#include "viz_io.h"
#include "util.h"
#include "types.h"

#include <graph.h>
#include <json_spirit.h>
#include <fstream>

using namespace std;
namespace json = json_spirit;
using graph::Graph;

// Read in a sample graph from a JSON file (this is still in flux)
Graph readJsonTree(const char* filename)
{
	Graph g(graph::SimpleDirected);
	ifstream input(filename);
	if (!input) throw ERROR << "Unable to open file " << filename << " for reading.";
	
	json::Value val;
	while (json::read(input, val))
	{
		if (val.type() != json::obj_type)
			throw ERROR << "Invalid JSON format for " << filename;
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

		// Set up the vertex data struct for the current node if one doesn't already exist
		BnbVertexData* nodeData;
		if (g.vertexData(nodeId))
			nodeData = (BnbVertexData*) g.vertexData(nodeId);
		else nodeData = (BnbVertexData*) g.setVertexData(nodeId, new BnbVertexData);

		// Add an edge to the child node, and set up its vertex data, if it doesn't already exist
		if (child != -1) 
		{
			g.addEdge(nodeId, child);
			if (!g.vertexData(child)) 
				g.setVertexData(child, new BnbVertexData);
		}

		// Fill in the current node vertex data
		if      (exploredAt != -1)        nodeData->expTime = exploredAt;
		else if (prunedAt != -1)          nodeData->pruneTime = prunedAt;
		else if (generatedAt != -1)       nodeData->genTime = generatedAt;
		else if (lowerBound != -Infinity) nodeData->lb = lowerBound;
		else if (upperBound != Infinity)  nodeData->ub = upperBound;
		else if (branchingVar != "")	  nodeData->branchVar = branchingVar;
		else if (branchDir != 0)          nodeData->branchDir=(BranchDir)branchDir;
		else if (estimate != -1)		  nodeData->estimate = estimate;
		else if (contour != MaxInt)		  nodeData->contour = contour;

	}
	return g;
}





