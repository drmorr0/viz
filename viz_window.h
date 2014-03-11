#ifndef VIZ_WINDOW_H
#define VIZ_WINDOW_H

#include "scene.h"
#include "vector2d.h"

#include <graph.h>
using graph::Graph;

#include <vector>

#include <gdk/gdk.h>
#include <gtkmm.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>

class GraphCanvas : public Gtk::DrawingArea
{
public:
	GraphCanvas(const Graph& graph);

private:
	Scene mScene;

	// Canvas position information
	Vector2D mCanvOffset;
	double mZoom;

	Vector2D mDragPos;
	std::vector<int> mDragItems;

	// Signal handlers
	virtual bool on_draw(const CairoContext& ctx);
	virtual bool on_button_press_event(GdkEventButton* evt);
	virtual bool on_button_release_event(GdkEventButton* evt);
	virtual bool on_scroll_event(GdkEventScroll* evt);
	virtual bool on_motion_notify_event(GdkEventMotion* evt);
};

class VizWindow : public Gtk::Window
{
public:
	VizWindow(const Graph& graph, Gtk::WindowType wt = Gtk::WINDOW_TOPLEVEL);
	
private:
	GraphCanvas* mCanvas;
	
};

#endif // VIZ_WINDOW_H
