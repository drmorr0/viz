// viz_tab.cpp: David R. Morrison, March 2014
// Implementation for a single tab of the visualization tool

#include "viz_tab.h"
#include "viz_canvas.h"

VizTab::VizTab(Graph* graph) :
	mGraph(graph)
{
	mCanvas = new VizCanvas(this);
	add(*mCanvas);
}


