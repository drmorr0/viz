// graph_io.cpp: David R. Morrison, Oct. 2013
// Implementation for graph input/output functions

#include "graph.h"
#include "graph_io.h"
#include "graph_utils.h"
#include "util.h"

#include <fstream>

using namespace std;

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

void readJsonTree(const char* filename, Graph& g)
{
/*	using namespace json_spirit;
	mType = SimpleDirected;

	string nextLine;

	while (input.good())
	{
		getline(input, nextLine);
		if (nextLine.empty()) continue;

		Value val;
		read(nextLine, val);

		if (val.type() != json_spirit::object_type)
			throw DRM_ERROR << "Invalid format string: \n    " << nextLine;

		Object obj val.get_obj();

		int nId = -1;
		int child = -1;
		double startTime, endTime;
		if (obj.size() == 0)
			throw DRM_ERROR << "Invalid format string: \n    " << nextLine;
		else if (obj[0].name_ == "node_id") 
			nId = obj[i].value_.get_int();
		else if (obj[0].name_ == "start_time") 
			startTime = obj[i].value_.get_int();
		else if (obj[0].name_ == "end_time")
			endTime = obj[i].value_.get_int();
		else throw DRM_ERROR << "First parameter is not valid: \n    " << nextLine;

		VertexData data;

		for (int i = 1; i < obj.size(); ++i)
		{
			string name = obj[i].name_;
			Value val = obj[i].value_;

			if (name == "explored_at") 
				data->expTime = val.get_int();  
			else if (name == "branching_var") 
				data->branchVar = val.get_int();
			else if (name == "lower_bound") 
				data->lb = val.get_real();
			else if (name == "upper_bound") 
				data->ub = val.get_real();
			else if (name == "estimate") { } // TODO }
			else if (name == "contour") { } // TODO }
			else if (name == "child") 
			{
				child = val.get_int();
				data->genTime = vertexData(nId)->expTime;
				if (data->genTime == -1)
					throw ERROR << "Parent was not explored before child:\n    " << nextLine;
			}
			else if (name == "branch_dir") 
			{
				if (val.get_int() > 0) data->branchDir = Up;
				else if (val.get_int() < 0) data->branchDir = Down;
				else throw ERROR << "Invalid branch direction:\n    " << nextLine;
			}
			else if (name == "obj_value") { }

			else if (name == "clocks_per_sec") { }
			else if (name == "num_vars") { } 
			else if (name == "num_int_vars") { }
			else if (name == "num_bin_vars") { }
			else if (name == "total_nodes") { }
		}

		if (nId != -1)
		{
			if (child != -1) 
			{
				addEdge(nId, child);
				vertexData(child)->genTime = data->genTime;
				vertexData(child)->branchDir = data->branchDir;
			}
			else
			{
				vertexData(nId)->expTime = data->expTime;
				vertexData(nId)->branchVar = data->branchVar;
				vertexData(nId)->lb = data->lb;
				vertexData(nId)->ub = data->ub;
			}
		}
	} */
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


