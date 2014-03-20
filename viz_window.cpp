// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "viz_tab.h"
#include "scene_obj.h"

#include <graph.h>

using namespace std;

VizWindow::VizWindow(const char* gladeFile, Gtk::WindowType wt) :
	Gtk::Window(wt),
	builder(Gtk::Builder::create_from_file(gladeFile))
{
	maximize();
	Gtk::Widget* grid;
	builder->get_widget("viz_main_grid", grid);
	add(*grid);
}

void VizWindow::createTab(const char* tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs;
	builder->get_widget("viz_tabs", vizTabs);

	Gtk::Widget* newTab = Gtk::manage(new VizTab(tabContents));
	vizTabs->append_page(*newTab, tabName);
	newTab->show_all();
}

	


