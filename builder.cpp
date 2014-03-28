// builder.cpp: David R. Morrison
// Implementation details for the builder interface

#include "builder.h"
#include "viz_canvas.h"

VizCanvas* TheBuilder::getCurrentTab()
{
	Gtk::Notebook* tabs = get<Gtk::Notebook>("viz_tabs");
	return (VizCanvas*)((Gtk::Frame*)tabs->get_nth_page(tabs->get_current_page()))->get_child();
}
