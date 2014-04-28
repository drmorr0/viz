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

#include <string>
#include <vector>
#include <map>
#include <gtkmm.h>

class Scene;
class SceneObject;
class VizTab;

class VizCanvas : public Gtk::DrawingArea
{
public:
	VizCanvas(Graph* graph, VizTab* parent);

	void showAll();
	void hide(const std::vector<int>& toHide);
	void format(const std::vector<int>& toFormat, const Gdk::Color& color, 
			const Gdk::Color& fill, double radius, double thickness);
	void render(const CairoContext& ctx, double scale);
	void render(const CairoContext& ctx, double scale, const Vector2D& offset, 
			bool renderSelected = false);
	BoundingBox bounds() const;
	VizTab* getTab() const { return fpParent; }

private:
	VizTab* fpParent;
	unique_ptr<Scene> pScene;
	Graph* fpGraph;

	// Canvas position information
	Vector2D mCanvOffset;
	const double mZoomStep;
	double mZoom;

	bool mInCurrSel, mDragSelection;
	Vector2D mClickPos, mDragPos;

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

	// Convert to and from screen coordinates to scene coordinates
	Vector2D toScenePos(const Vector2D& screenPos) const;
	double toSceneX(double screenX) const;
	double toSceneY(double screenY) const;
	Vector2D toScreenPos(const Vector2D& scenePos) const;
};

#endif // VIZ_CANVAS_H

