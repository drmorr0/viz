// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "viz_cmd.h"
#include "scene_obj.h"
#include "builder.h"

#include <graph.h>

using namespace std;

VizWindow::VizWindow(Gtk::WindowType wt) :
	Gtk::Window(wt),
	mPrompt(new VizCmdPrompt)
{
	maximize();
	add(*TheBuilder::get("viz_main_grid"));
}

VizWindow::~VizWindow()
{
	delete mPrompt;
}

void VizWindow::createTab(const char* tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs = TheBuilder::get<Gtk::Notebook>("viz_tabs");

	Gtk::Frame* newTab = Gtk::manage(new Gtk::Frame);
	VizCanvas* tabCanvas = Gtk::manage(new VizCanvas(new Graph(tabContents)));
	vizTabs->append_page(*newTab, tabName);
	newTab->add(*tabCanvas);
	newTab->show_all();
}

	


