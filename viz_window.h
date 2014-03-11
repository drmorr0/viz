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
	
private:
	std::vector<VizTab*> mTabContents;
	std::vector<Gtk::Label*> mTabLabels;
	Gtk::Notebook mTabManager;
	
};

#endif // VIZ_WINDOW_H
