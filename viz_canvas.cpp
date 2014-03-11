// viz_canvas.cpp: David R. Morrison
// Implementation details for the VizCanvas class

#include "viz_tab.h"
#include "viz_canvas.h"
#include "scene.h"
#include "scene_obj.h"

#include <graph.h>
#include <graph_layout.h>
using graph::Graph;

#include <gdk/gdk.h>

using namespace std;

/*
 * The VizCanvas constuctor needs to do the following things:
 *  1) Set up the events that the canvas will accept
 *  2) Initialize the scene graph
 */
VizCanvas::VizCanvas(VizTab* parent) :
	mParent(parent),
	mCanvOffset(-400, -20),
	mZoom(1.0)
{
	mScene = new Scene;

	// I don't understand exactly how this works -- I can't get GTK to recognize custom event
	// handlers, nor does it seem to work with the Gdk::ALL_EVENTS_MASK (TODO)
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | 
			   Gdk::POINTER_MOTION_MASK);

	if (mParent)
	{
	// Compute an initial layout for the graph that we want to display
	GraphLayout treeLayout = graph::layoutTreeLevel(*(mParent->currGraph()), 0, 0, 5, 50, 25);
	std::map<int, int> node2scene;

	// First add all of the vertices to the scene
	for (auto node = treeLayout.begin(); node != treeLayout.end(); ++node)
	{
		int sceneId = mScene->addObject(
				new VertexSceneObject(node->second.first, node->second.second, 5));
		node2scene[node->first] = sceneId;
	}

	// Next add the edges to the scene
	for (auto tail = mParent->currGraph()->begin(); tail != mParent->currGraph()->end(); ++tail)
		for (auto head = tail->second.begin(); head != tail->second.end(); ++head)
			mScene->addObject(new EdgeSceneObject(node2scene[tail->first], node2scene[*head]));
	}
}

/**** EVENT HANDLERS *****/

/*
 * on_draw: redraw the canvas; we just use our scene manager to control rendering of SceneObjects
 * TODO: just render the currently-visible area
 */
bool VizCanvas::on_draw(const CairoContext& ctx)
{
	mScene->render(ctx, mCanvOffset, mZoom);
	return true;
}

/*
 * on_button_press_event:
 *  1) If no movable SceneObject is under the cursor, drag the canvas
 *  2) If a movable SceneObject is under the cursor, drag the object
 */
bool VizCanvas::on_button_press_event(GdkEventButton* evt)
{
	printf("ding!\n");
	mDragPos = Vector2D(evt->x, evt->y);
	vector<int> selectedObjs = mScene->findObjects(1 / mZoom * (mDragPos - mCanvOffset));
	
	// If the user clicked on an object, start dragging it
	mDragItems.clear();
	for (int i = 0; i < selectedObjs.size(); ++i)
	{
		// Right now, we just drag the first object in the list that is marked as
		// "movable" -- TODO something more complicated with z-indices, perhaps?
		if (mScene->get(selectedObjs[i])->state() & MOVABLE)
		{
			mDragItems.push_back(selectedObjs[i]);
			break;
		}
	}

	return true;
}

/*
 * on_button_release_event:
 *  1) TODO: display information about the object clicked on
 */
bool VizCanvas::on_button_release_event(GdkEventButton* evt)
{
/*	Vector2D clickPos(evt->x, evt->y);
	vector<int> selectedObjs = mScene.findObjects(1 / mZoom * (clickPos - mCanvOffset));

	for (int i = 0; i < selectedObjs.size(); ++i)
	{
		printf("User clicked on object %d\n", selectedObjs[i]);
	}*/
	return true;
}

/* 
 * on_scroll_event:
 *  1) Zoom in or out from the current canvas
 *  TODO: implement some sort of smooth scrolling/animation?
 */
bool VizCanvas::on_scroll_event(GdkEventScroll* evt)
{
	Vector2D nodePos = 1 / mZoom * (Vector2D(evt->x, evt->y) - mCanvOffset);
	double oldZoom = mZoom;

	if (evt->direction == GDK_SCROLL_UP) mZoom *= 2;
	else if (evt->direction == GDK_SCROLL_DOWN) mZoom /= 2;

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
		if (mDragItems.size() == 0) mCanvOffset += delta;
		else for (int i = 0; i < mDragItems.size(); ++i)
			mScene->get(mDragItems[i])->move(1 / mZoom * delta);
		mDragPos = newDragPos;
		queue_draw();  // TODO only redraw visible part of scene graph
	}
	
	return true;
}
