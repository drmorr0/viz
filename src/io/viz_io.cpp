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
Graph readJsonTree(const string& filename)
{
	Graph g(graph::SimpleDirected);
	ifstream input(filename);
	if (!input) throw MY_ERROR << "Unable to open file " << filename << " for reading.";
	
	json::Value val;
	while (json::read(input, val))
	{
		if (val.type() != json::obj_type)
			throw MY_ERROR << "Invalid JSON format for " << filename;
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
		bool feasible = false;

		for (int i = 0; i < obj.size(); ++i)
		{
			if      (obj[i].name_ == "node_id")       nodeId = obj[i].value_.get_int();
			else if (obj[i].name_ == "explored_at")   exploredAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "pruned_at")     prunedAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "generated_at")  generatedAt = obj[i].value_.get_int();
			else if (obj[i].name_ == "lower_bound")   lowerBound = obj[i].value_.get_real();
			else if (obj[i].name_ == "upper_bound" ||
					 obj[i].name_ == "obj_value")     upperBound = obj[i].value_.get_real();
			else if (obj[i].name_ == "branching_var") branchingVar = obj[i].value_.get_str();
			else if (obj[i].name_ == "child")         child = obj[i].value_.get_int();
			else if (obj[i].name_ == "branch_dir")    branchDir = obj[i].value_.get_int();
			else if (obj[i].name_ == "estimate")      estimate = obj[i].value_.get_real();
			else if (obj[i].name_ == "contour")       contour = obj[i].value_.get_int();
			else if (obj[i].name_ == "is_int_feas")   feasible = true;
		}

		if (nodeId == -1) continue; // Every JSON line MUST specify a node_id to be parsed

		// Set up the vertex data struct for the current node if one doesn't already exist
		graph::VertexData* nodeData = nullptr;
		if (g.vertexData(nodeId))
			nodeData = g.vertexData(nodeId);
		else nodeData = g.setVertexData(nodeId, new graph::VertexData);

		// Add an edge to the child node, and set up its vertex data, if it doesn't already exist
		graph::VertexData* childData = nullptr;
		if (child != -1) 
		{
			g.addEdge(nodeId, child);
			if (g.vertexData(child)) 
				childData = g.vertexData(child);
			else childData = g.setVertexData(child, new graph::VertexData);
		}

		// Fill in the current node vertex data
		nodeData->properties["Node ID"] = to_string(nodeId);
		if (childData)
			childData->properties["Node ID"] = to_string(child);
		if (exploredAt != -1)        
			nodeData->properties["Explored"] = to_string(exploredAt);
		if (prunedAt != -1)          
			nodeData->properties["Pruned"] = to_string(prunedAt);
		if (generatedAt != -1)       
			nodeData->properties["Generated"] = to_string(generatedAt);
		if (lowerBound != -Infinity) 
			nodeData->properties["Lower bound"] = to_string(lowerBound);
		if (upperBound != Infinity && nodeData->properties.count("Feasible solution") == 0)  
			nodeData->properties["Upper bound"] = to_string(upperBound);
		if (branchingVar != "")	  
			nodeData->properties["Branching variable"] = branchingVar;
		if (feasible)
			nodeData->properties["Feasible solution"] = "1";
		if (branchDir != 0)          
		{
			if (branchDir == -1)
				childData->properties["Branching direction"] = "Down";
			else childData->properties["Branching direction"] = "Up";
		}
		if (estimate != -1)		  
			childData->properties["Node estimate"] = to_string(estimate);
		if (contour != MaxInt)		  
		{
			if (childData) childData->properties["Contour label"] = to_string(contour);
			else if (nodeData) nodeData->properties["Contour label"] = to_string(contour);
		}

	}
	return g;
}





