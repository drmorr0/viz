// util_templates.h: David R. Morrison
// Utility classes and functions

template<typename T>
T* TheBuilder::get(const char* name)
{
	Glib::RefPtr<Gtk::Builder> builder = getBuilder();
	T* theWidget;
	builder->get_widget(name, theWidget);
	return theWidget;
}

