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

#include <graph.h>
using graph::Graph;

#include <map>
#include <gtkmm.h>

class Scene;
class SceneObject;
class VizTab;

class VizCanvas : public Gtk::DrawingArea
{
public:
	VizCanvas(Graph* graph);

private:
	unique_ptr<Scene> mScene;
	Graph* mGraph;

	// Canvas position information
	Vector2D mCanvOffset;
	double mZoom;

	Vector2D mDragPos;
	std::vector<SceneObject*> mDragItems;
	bool mDragged;

	// Signal handlers
	virtual bool on_draw(const CairoContext& ctx);
	virtual bool on_button_press_event(GdkEventButton* evt);
	virtual bool on_button_release_event(GdkEventButton* evt);
	virtual bool on_scroll_event(GdkEventScroll* evt);
	virtual bool on_motion_notify_event(GdkEventMotion* evt);

	// Convert from graph ids to scene ids and vice versa (TODO make more memory efficient)
	int toGraphID(int sceneID) const;
	int toSceneID(int graphID) const;
	std::map<int, int> graph2scene;
	std::map<int, int> scene2graph;
};

#endif // VIZ_CANVAS_H

