// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "scene_obj.h"

#include <graph.h>

using namespace std;

VizWindow::VizWindow(const char* gladeFile, Gtk::WindowType wt) :
	Gtk::Window(wt),
	mBuilder(Gtk::Builder::create_from_file(gladeFile))
{
	maximize();
	Gtk::Widget* grid;
	Gtk::Box* box;
	mBuilder->get_widget("viz_main_grid", grid);
	mBuilder->get_widget("box1", box);
	add(*grid);
}

VizWindow::~VizWindow()
{
	for (int i = 0; i < mTabContents.size(); ++i)
		delete mTabContents[i];
}

void VizWindow::createTab(const char* tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs;
	mBuilder->get_widget("viz_tabs", vizTabs);

	Gtk::Frame* newTab = Gtk::manage(new Gtk::Frame);
	mTabContents.push_back(new Graph(tabContents));
	VizCanvas* tabCanvas = Gtk::manage(new VizCanvas(mTabContents.back()));
	newTab->add(*tabCanvas);
	vizTabs->append_page(*newTab, tabName);
	newTab->show_all();
}

	


