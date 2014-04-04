// viz_canvas.cpp: David R. Morrison
// Implementation details for the VizCanvas class

#include "viz_canvas.h"
#include "scene.h"
#include "scene_obj.h"
#include "more_graph_utils.h"
#include "builder.h"

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
	mScene(new Scene),
	mGraph(graph),
	mCanvOffset(400, 50),
	mZoom(1.0)
{
	// I don't understand exactly how this works -- I can't get GTK to recognize custom event
	// handlers, nor does it seem to work with the Gdk::ALL_EVENTS_MASK (TODO)
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | 
			   Gdk::POINTER_MOTION_MASK);

	// Compute an initial layout for the graph that we want to display
	GraphLayout treeLayout = graph::improvedLayoutTreeLevel(*mGraph, 15, 25, 50);

	// First add all of the vertices to the scene
	for (auto node = treeLayout.begin(); node != treeLayout.end(); ++node)
	{
		int sceneId = mScene->addObject(
				new VertexSceneObject(node->second.first, node->second.second, 5));
		graph2scene[node->first] = sceneId;
		scene2graph[sceneId] = node->first;
	}

	// Next add the edges to the scene
	for (auto tail = mGraph->begin(); tail != mGraph->end(); ++tail)
		for (auto head = tail->second.begin(); head != tail->second.end(); ++head)
			mScene->addObject(new EdgeSceneObject(toSceneID(tail->first), toSceneID(*head)));
}

/* 
 * Mark all objects in the scene graph as visible, and then redraw
 */
void VizCanvas::showAll()
{
	mScene->showAll();
	queue_draw();
}

/*
 * Mark the specified vertices as invisible, and then redraw
 */
void VizCanvas::hide(const vector<int>& toHide)
{
	for (int i = 0; i < toHide.size(); ++i)
		mScene->get(toSceneID(toHide[i]))->state() &= ~VISIBLE;
	queue_draw();
}

void VizCanvas::format(const vector<int>& toFormat, const Gdk::Color& color, 
		double radius, double thickness)
{
	for (int i = 0; i < toFormat.size(); ++i)
	{
		VertexSceneObject* vertex = (VertexSceneObject*)mScene->get(toSceneID(toFormat[i]));
		vertex->setColor(color);
		if (radius >= 0) vertex->setRadius(radius);
		if (thickness >= 0) vertex->setThickness(thickness);
	}
	queue_draw();
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
	mDragPos = Vector2D(evt->x, evt->y);
	vector<SceneObject*> selected = mScene->findObjects(1 / mZoom * (mDragPos - mCanvOffset));
	
	// If the user clicked on an object, start dragging it
	mDragItems.clear(); mDragged = false;
	for (int i = 0; i < selected.size(); ++i)
	{
		// Right now, we just drag the first object in the list that is marked as
		// "movable" -- TODO something more complicated with z-indices, perhaps?
		if (selected[i]->state() & MOVABLE)
		{
			// If the 'shift' button is held down and we clicked on a vertex (right now, vertices
			// are the only MOVABLE objects; TODO), we want to be able to drag the entire subtree.
			// Since our tree is directed, this is just all the vertices reachable from what we
			// clicked on
			if (evt->state & GDK_SHIFT_MASK)
			{
				vector<int> subtree = graph::getReachable(*mGraph, {toGraphID(selected[i]->id())});
				for (int j = 0; j < subtree.size(); ++j)
					mDragItems.push_back(mScene->get(toSceneID(subtree[j])));
				break;
			}
			else
			{
				mDragItems.push_back(selected[i]);
				break;
			}
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
	// If we weren't dragging anything around, then check to see if we clicked on anything
	// and get its information.  Otherwise do nothing.
	if (!mDragged)
	{
		Vector2D clickPos(evt->x, evt->y);
		vector<SceneObject*> selected = mScene->findObjects(1 / mZoom * (clickPos - mCanvOffset));

		if (selected.size() > 0)
		{
			int gid = toGraphID(selected[0]->id());
			string info = "Subproblem ID: " + to_string(gid) + "\n";
			graph::VertexData* data = mGraph->vertexData(gid);
			for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
				info += prop->first + ": " + prop->second + "\n";
			
			Gtk::TextView* infoBox = TheBuilder::get<Gtk::TextView>("viz_info_box");
			infoBox->get_buffer()->set_text(info);
		}
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
		mDragged = true;
		Vector2D newDragPos(evt->x, evt->y);
		Vector2D delta = newDragPos - mDragPos;
		if (mDragItems.size() == 0) mCanvOffset += delta;
		else for (int i = 0; i < mDragItems.size(); ++i)
			mDragItems[i]->move(1 / mZoom * delta);
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
