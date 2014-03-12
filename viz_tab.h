#ifndef VIZ_TAB_H
#define VIZ_TAB_H
/*
 * viz_tab.h: David R. Morrison: March 2014
 *
 * This class describes a single pane or "tab" that is visible in the visualization software.  Since
 * each tab could be viewing a different file, this is a container interface that groups together
 * all of the relevant information about the current tab.
 *
 */

#include "types.h"

#include <graph.h>
using graph::Graph;

#include <gtkmm.h>

class VizWindow;

class VizTab : public Gtk::Frame
{
public:
	VizTab(const Graph& graph);
	VizTab(const VizTab& rhs);
	const Graph* const currGraph() const { return mGraph.get(); }

private:
	GraphPtr mGraph;	
	GtkWidgetPtr mCanvas;
};

#endif // VIZ_TAB_H
