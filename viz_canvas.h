#ifndef VIZ_CANVAS_H
#define VIZ_CANVAS_H
/*
 * viz_canvas.h: David R. Morrison, March 2014
 *
 * This file describes a subclass of the Gtk::DrawingArea widget that manages things like mouse
 * interactivity, and interactions with the Scene object for the current display.
 *
 */

#include "types.h"
#include "vector2d.h"

#include <gtkmm.h>

class Scene;
class VizTab;

class VizCanvas : public Gtk::DrawingArea
{
public:
	VizCanvas(VizTab* parent);

private:
	VizTab* mParent;
	Scene* mScene;

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

#endif // VIZ_CANVAS_H

