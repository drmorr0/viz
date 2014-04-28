#ifndef BUILDER_H
#define BUILDER_H
/*
 * builder.h: David R. Morrison, March 2014
 *
 * Static interface to the Gtk::Builder class; this class will also include methods for retrieving
 * commonly-used widgets (for example, the currently-selected tab)
 */

#include <gtkmm.h>

class VizWindow;
class VizTab;

/*** Gtk Builder Class ***/
class TheBuilder
{
public:
	template<typename T = Gtk::Widget>
	static T* get(const char* name);
	static VizWindow* getToplevel();
	static VizTab* getCurrentTab();

private:
	TheBuilder();
	TheBuilder(const TheBuilder&);
	TheBuilder& operator=(const TheBuilder&);
	static Glib::RefPtr<Gtk::Builder> getBuilder()
	{
		static Glib::RefPtr<Gtk::Builder> builder;
		if (!builder) builder = Gtk::Builder::create_from_file("../layouts/viz_main.glade");
		return builder;
	}
};

// Get a widget with a specified type
template<typename T>
T* TheBuilder::get(const char* name)
{
	Glib::RefPtr<Gtk::Builder> builder = getBuilder();
	T* theWidget;
	builder->get_widget(name, theWidget);
	return theWidget;
}

#endif // BUILDER_H
