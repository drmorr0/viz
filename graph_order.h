/*
 * graph_order.h: David R. Morrison, Sept. 2013
 *
 * Vertex-ordering functions for the Graph class
 */

namespace drm
{

namespace GraphUtils
{
	vector<int> identityVertexOrder(const Graph& g);
	vector<int> randomVertexOrder(const Graph& g);
	vector<int> degreeListVertexOrder(const Graph& g);
	vector<int> outdegreeListVertexOrder(const Graph& g);
	vector<int> indegreeListVertexOrder(const Graph& g);
	vector<int> degreeSequence(const Graph& g);
	vector<int> outdegreeSequence(const Graph& g);
	vector<int> indegreeSequence(const Graph& g);

}; // namespace GraphUtils

}; // namespace drm



