// viz_canvas.cpp: David R. Morrison
// Implementation details for the VizCanvas class

#include "viz_canvas.h"
#include "scene.h"
#include "vertex_obj.h"
#include "edge_obj.h"
#include "more_graph_utils.h"
#include "builder.h"
#include "util.h"

#include <graph_utils.h>
#include <graph_layout.h>

#include <string>
#include <gdk/gdk.h>

using namespace std;

/*
 * The VizCanvas constuctor needs to do the following things:
 *  1) Set up the events that the canvas will accept
 *  2) Initialize the scene graph
 */
VizCanvas::VizCanvas(Graph* graph) :
	pScene(new Scene),
	fpGraph(graph),
	mCanvOffset(400, 50),
	mZoomStep(1.1),
	mZoom(1.0),
	mInCurrSel(false)
{
	// I don't understand exactly how this works -- I can't get GTK to recognize custom event
	// handlers, nor does it seem to work with the Gdk::ALL_EVENTS_MASK (TODO)
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

	// Compute an initial layout for the graph that we want to display
	int nodeRadius = 10;
	GraphLayout treeLayout = graph::improvedLayoutTreeLevel(*fpGraph, nodeRadius, 30, 75);

	// First add all of the vertices to the scene
	for (auto node = treeLayout.begin(); node != treeLayout.end(); ++node)
	{
		int sceneId = pScene->addObject(
				new VertexSceneObject(node->second.first, node->second.second, nodeRadius));
		graph2scene[node->first] = sceneId;
		scene2graph[sceneId] = node->first;
	}

	// Next add the edges to the scene
	for (auto tail = fpGraph->begin(); tail != fpGraph->end(); ++tail)
		for (auto head = tail->second.begin(); head != tail->second.end(); ++head)
			pScene->addObject(new EdgeSceneObject(toSceneID(tail->first), toSceneID(*head)));
}

/* 
 * Mark all objects in the scene graph as visible, and then redraw
 */
void VizCanvas::showAll()
{
	pScene->showAll();
	queue_draw();
}

/*
 * Mark the specified vertices as invisible, and then redraw
 */
void VizCanvas::hide(const vector<int>& toHide)
{
	for (int i = 0; i < toHide.size(); ++i)
		pScene->get(toSceneID(toHide[i]))->state() &= ~VISIBLE;
	queue_draw();
}

void VizCanvas::format(const vector<int>& toFormat, const Gdk::Color& color, 
		const Gdk::Color& fill, double radius, double thickness)
{
	for (int i = 0; i < toFormat.size(); ++i)
	{
		VertexSceneObject* vertex = (VertexSceneObject*)pScene->get(toSceneID(toFormat[i]));
		vertex->setColor(color);
		if (fill != Gdk::Color("#deaded")) vertex->setFill(fill); // TODO Stupid hack
		if (radius >= 0) vertex->setRadius(radius);
		if (thickness >= 0) vertex->setThickness(thickness);
	}
	queue_draw();
}

void VizCanvas::render(const CairoContext& ctx, double scale)
{
	// Need to negate the top-left corner to provide the canvas offset amount
	render(ctx, scale, -1 * Vector2D(bounds().left, bounds().top));
}
void VizCanvas::render(const CairoContext& ctx, double scale, const Vector2D& offset, 
		bool renderSelected)
{
	pScene->render(ctx, offset, scale);

	// Draw dashed lines around each selected object
	if (renderSelected)
	{
		ctx->save();
		ctx->set_dash( vector<double>{ 1, 2 }, 0);
		ctx->set_line_width(1);
		for (int i = 0; i < mfpSelectedItems.size(); ++i)
		{
			BoundingBox box = mfpSelectedItems[i]->bounds();
			ctx->rectangle(mCanvOffset.x + mZoom * box.left, mCanvOffset.y + mZoom * box.top, 
					mZoom * box.width(), mZoom * box.height());
			ctx->stroke();
		}
		ctx->restore();
	}
}

BoundingBox VizCanvas::bounds() const
{
	return pScene->bounds();
}

/**** EVENT HANDLERS *****/

/*
 * on_draw: redraw the canvas; we just use our scene manager to control rendering of SceneObjects
 * TODO: just render the currently-visible area
 */
bool VizCanvas::on_draw(const CairoContext& ctx)
{
	render(ctx, mZoom, mCanvOffset, true);
	return true;
}

/*
 * on_button_press_event:
 *  If a SceneObj is under the cursor, we select it; otherwise, we just mark the location in case
 *  the user is trying to pan
 */
bool VizCanvas::on_button_press_event(GdkEventButton* evt)
{
	mDragPos = Vector2D(evt->x, evt->y);
	vector<SceneObject*> objs = pScene->findObjects(toScenePos(mDragPos));

	mInCurrSel = false;
	if (objs.size() > 0)
	{
		// Check to see if we clicked on anything in our current selection list
		for (int i = 0; i < mfpSelectedItems.size(); ++i)
			if (mfpSelectedItems[i]->contains(toScenePos(mDragPos)))
				{ mInCurrSel = true; break; }

		// If we didn't, then either add to the selection (if shift is held) or change 
		// the selection (if shift isn't held).
		if (!mInCurrSel)
		{
			if ((evt->state & GDK_SHIFT_MASK) != GDK_SHIFT_MASK)
				mfpSelectedItems.clear();

			// Only use the first item in objs (TODO - some sort of z-index?)
			mfpSelectedItems.push_back(objs[0]);

			// Only need to redraw if the selection actually changed...
			queue_draw();
		}

		// Regardless, we are now inside the current selection, so set mInCurrSel = true
		mInCurrSel = true;
	}

	if (mfpSelectedItems.size() > 0)
	{
		int gid = toGraphID(mfpSelectedItems.back()->id());
		string info = "";
		graph::VertexData* data = fpGraph->vertexData(gid);
		for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
			info += prop->first + ": " + prop->second + "\n";
		
		Gtk::TextView* infoBox = TheBuilder::get<Gtk::TextView>("viz_sel_info_box");
		infoBox->get_buffer()->set_text(info);
	}

	
	return true;
}

/* 
 * on_scroll_event:
 *  1) Zoom in or out from the current canvas
 *  TODO: implement some sort of smooth scrolling/animation?
 */
bool VizCanvas::on_scroll_event(GdkEventScroll* evt)
{
	Vector2D nodePos = toScenePos(Vector2D(evt->x, evt->y));
	double oldZoom = mZoom;

	if (evt->direction == GDK_SCROLL_UP) mZoom *= mZoomStep;
	else if (evt->direction == GDK_SCROLL_DOWN) mZoom /= mZoomStep;

	mCanvOffset += nodePos * (oldZoom - mZoom);
	queue_draw();  // TODO only redraw visible part of scene graph
	return true;
}

/*
 * on_motion_notify_event: If the primary mouse button is pressed,
 *  1) If one or more items are being dragged, update their position
 *  2) Otherwise update the position of the canvas
 */
bool VizCanvas::on_motion_notify_event(GdkEventMotion* evt)
{
	if (!(evt->state & GDK_BUTTON1_MASK)) return false;
	else if (evt->is_hint) return true;
	else 
	{
		Vector2D newDragPos(evt->x, evt->y);
		Vector2D delta = newDragPos - mDragPos;
		if (!mInCurrSel) mCanvOffset += delta;
		else for (int i = 0; i < mfpSelectedItems.size(); ++i)
			mfpSelectedItems[i]->move(1 / mZoom * delta);
		mDragPos = newDragPos;
		queue_draw();  // TODO only redraw visible part of scene graph
	}
	
	return true;
}

/***** End event handlers *****/

/*
 * toGraphID: convert a scene ID to a graph ID
 */
int VizCanvas::toGraphID(int sceneID) const
{
	if (scene2graph.count(sceneID) > 0)
		return scene2graph.find(sceneID)->second;
	else return -1;
}

/*
 * toSceneID: convert a graph ID to a scene ID
 */
int VizCanvas::toSceneID(int graphID) const
{
	if (graph2scene.count(graphID) > 0)
		return graph2scene.find(graphID)->second;
	else return -1;
}

Vector2D VizCanvas::toScenePos(const Vector2D& screenPos)
{
	return 1 / mZoom * (screenPos - mCanvOffset);
}

Vector2D VizCanvas::toScreenPos(const Vector2D& scenePos)
{
	return mCanvOffset + mZoom * scenePos;
}
