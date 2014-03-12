#ifndef VIZ_WINDOW_H
#define VIZ_WINDOW_H

#include "viz_tab.h"
#include <graph.h>
using graph::Graph;

#include <vector>
#include <gtkmm.h>

class VizTab;

class VizWindow : public Gtk::Window
{
public:
	VizWindow(const std::vector<Graph>& graphs, Gtk::WindowType wt = Gtk::WINDOW_TOPLEVEL);
	
	// Disable copy/assignment for now
	VizWindow(const VizWindow&) = delete;
	VizWindow& operator=(const VizWindow&) = delete;
	
private:
	std::vector<GtkWidgetPtr> mTabContents;
	std::vector<GtkWidgetPtr> mTabLabels;
	Gtk::Notebook mTabManager;
	
};

#endif // VIZ_WINDOW_H
