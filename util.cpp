// util.cpp: David R. Morrison
// Utility classes and functions

#include "util.h"

Gtk::Widget* TheBuilder::get(const char* name)
{
	static Glib::RefPtr<Gtk::Builder> mBuilder;
	if (!mBuilder) mBuilder = Gtk::Builder::create_from_file("layouts/viz_main.glade");

	Gtk::Widget* theWidget;
	mBuilder->get_widget(name, theWidget);
	return theWidget;
}

