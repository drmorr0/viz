
#include "viz_window.h"
#include "scene_obj.h"

#include <graph_layout.h>

#include <map>

using namespace std;

GraphCanvas::GraphCanvas(const Graph& graph) :
	mCanvOffset(-400, -20),
	mZoom(1.0)
{
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | 
			   Gdk::POINTER_MOTION_MASK);

	GraphLayout treeLayout = graph::layoutTreeLevel(graph, 0, 0, 5, 50, 25);
	std::map<int, int> node2scene;
	for (auto node = treeLayout.begin(); node != treeLayout.end(); ++node)
	{
		int sceneId = mScene.addObject(
				new VertexSceneObject(node->second.first, node->second.second, 5));
		node2scene[node->first] = sceneId;
	}

	for (auto tail = graph.begin(); tail != graph.end(); ++tail)
		for (auto head = tail->second.begin(); head != tail->second.end(); ++head)
			mScene.addObject(new EdgeSceneObject(node2scene[tail->first], node2scene[*head]));
}

bool GraphCanvas::on_draw(const CairoContext& ctx)
{
	mScene.render(ctx, mCanvOffset, mZoom);
	return true;
}

bool GraphCanvas::on_button_press_event(GdkEventButton* evt)
{
	mDragPos = Vector2D(evt->x, evt->y);
	vector<int> selectedObjs = mScene.findObjects(1 / mZoom * (mDragPos - mCanvOffset));
	
	// If the user clicked on an object, start dragging it
	mDragItems.clear();
	for (int i = 0; i < selectedObjs.size(); ++i)
	{
		// Right now, we just drag the first object in the list that is marked as
		// "movable" -- TODO something more complicated with z-indices, perhaps?
		if (mScene.get(selectedObjs[i])->state() & MOVABLE)
		{
			mDragItems.push_back(selectedObjs[i]);
			break;
		}
	}

	return true;
}

bool GraphCanvas::on_button_release_event(GdkEventButton* evt)
{
	Vector2D clickPos(evt->x, evt->y);
	vector<int> selectedObjs = mScene.findObjects(1 / mZoom * (clickPos - mCanvOffset));

	for (int i = 0; i < selectedObjs.size(); ++i)
	{
		printf("User clicked on object %d\n", selectedObjs[i]);
	}
	return true;
}

bool GraphCanvas::on_scroll_event(GdkEventScroll* evt)
{
	Vector2D nodePos = 1 / mZoom * (Vector2D(evt->x, evt->y) - mCanvOffset);
	double oldZoom = mZoom;

	if (evt->direction == GDK_SCROLL_UP) mZoom *= 2;
	else if (evt->direction == GDK_SCROLL_DOWN) mZoom /= 2;

	mCanvOffset += nodePos * (oldZoom - mZoom);
	queue_draw();  // TODO only redraw visible part of scene graph
	return true;
}

bool GraphCanvas::on_motion_notify_event(GdkEventMotion* evt)
{
	if (!(evt->state & GDK_BUTTON1_MASK)) return false;
	else if (evt->is_hint) return true;
	else 
	{
		Vector2D newDragPos(evt->x, evt->y);
		Vector2D delta = newDragPos - mDragPos;
		if (mDragItems.size() == 0) mCanvOffset += delta;
		else for (int i = 0; i < mDragItems.size(); ++i)
			mScene.get(mDragItems[i])->move(1 / mZoom * delta);
		mDragPos = newDragPos;
		queue_draw();  // TODO only redraw visible part of scene graph
	}
	
	return true;
}

VizWindow::VizWindow(const Graph& graph, Gtk::WindowType wt) :
	Gtk::Window(wt)
{
	set_default_size(600, 400);
	set_position(Gtk::WIN_POS_CENTER);

	mCanvas = new GraphCanvas(graph);
	mCanvas->set_size_request(600,400);

	add(*mCanvas);
	mCanvas->show();
}


