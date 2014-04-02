// builder.cpp: David R. Morrison
// Implementation details for the builder interface

#include "builder.h"
#include "viz_canvas.h"

/*
 * Get the top-level window
 */
VizWindow* TheBuilder::getToplevel()
{
	Gtk::Widget* tl = get("viz_main_grid")->get_toplevel();
	if (tl && tl->get_is_toplevel())
		return (VizWindow*)tl;
	else return nullptr;
}

/*
 * Returns the currently-selected tab of the interface.  TODO this will break if there are no tabs
 */
VizCanvas* TheBuilder::getCurrentTab()
{
	Gtk::Notebook* tabs = get<Gtk::Notebook>("viz_tabs");
	return (VizCanvas*)((Gtk::Frame*)tabs->get_nth_page(tabs->get_current_page()))->get_child();
}
