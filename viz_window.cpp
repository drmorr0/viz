// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "viz_cmd.h"
#include "scene_obj.h"
#include "util.h"

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
	for (int i = 0; i < mTabContents.size(); ++i)
		delete mTabContents[i];
	delete mPrompt;
}

void VizWindow::createTab(const char* tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs = TheBuilder::get<Gtk::Notebook>("viz_tabs");

	Gtk::Frame* newTab = Gtk::manage(new Gtk::Frame);
	mTabContents.push_back(new Graph(tabContents));
	VizCanvas* tabCanvas = Gtk::manage(new VizCanvas(mTabContents.back()));
	vizTabs->append_page(*newTab, tabName);
	newTab->add(*tabCanvas);
	newTab->show_all();
}

	


