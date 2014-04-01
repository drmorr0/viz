#ifndef VIZ_WINDOW_H
#define VIZ_WINDOW_H
/*
 * viz_window.h: David R. Morrison, March 2014
 *
 * Top-level window class for Viz.  This class manages all of the other custom widgets, event
 * handlers, and the like, that are needed in order for the program to run
 */

#include "types.h"

#include <graph.h>
using graph::Graph;

#include <vector>
#include <gtkmm.h>

class VizCmdPrompt;

class VizWindow : public Gtk::Window
{
public:
	VizWindow(Gtk::WindowType wt = Gtk::WINDOW_TOPLEVEL);
	~VizWindow();
	
	void createTab(const char* tabName, const Graph& tabContents);
	
private:
	VizCmdPrompt* mPrompt;

public:
	// Disable copy/assignment for now
	VizWindow(const VizWindow&) = delete;
	VizWindow& operator=(const VizWindow&) = delete;
};

#endif // VIZ_WINDOW_H