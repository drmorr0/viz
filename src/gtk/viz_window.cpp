// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "scene_obj.h"
#include "builder.h"

#include <graph.h>

using namespace std;

VizWindow::VizWindow(Gtk::WindowType wt) :
	Gtk::Window(wt)
{
	VizPrompt::init();
	maximize();
	add(*TheBuilder::get("viz_main_grid"));
}

/*
 * Create a new tab and add it to the notebook.
 */
void VizWindow::createTab(const string& tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs = TheBuilder::get<Gtk::Notebook>("viz_tabs");

	Gtk::Frame* newTab = Gtk::manage(new Gtk::Frame);
	VizCanvas* tabCanvas = Gtk::manage(new VizCanvas(new Graph(tabContents)));
	vizTabs->append_page(*newTab, tabName);
	newTab->add(*tabCanvas);
	newTab->show_all();

	// Negative numbers set to the last page
	vizTabs->set_current_page(-1);
}

	


