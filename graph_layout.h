#ifndef GRAPH_LAYOUT_H
#define GRAPH_LAYOUT_H
/*
 * graph_layout.h: David R. Morrison, Oct. 2013
 *
 * Contains different functions and algorithms for computing drawings of various types of graphs.
 * Right now I just have algorithms for trees, perhaps eventually more complicated graphs will be
 * supported.  Some algorithms here are from the Handbook of Graph Drawing and Visualization (2013),
 * ed. Tamassia.
 */

class Graph;
class Point;

namespace drm
{

namespace GraphUtils
{
	// The level-based recursive approach described in HGDV; this function assumes that the graph is
	// a tree, and that the root of the tree has node id 0.  For simplicity, it also assumes that
	// the graph is directed (so that arcs point from parent to child).  Undefined behavior results
	// if these assumptions are violated.
	void layoutTreeLevel(Graph& g, const Point& rootPos, int vSpace, int hSpace);

}; // namespace GraphUtils

}; // namespace drm

#endif // GRAPH_LAYOUT_H


