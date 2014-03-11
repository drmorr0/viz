
#include "viz_window.h"
#include "scene_obj.h"

#include <map>

using namespace std;

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

GraphCanvas::GraphCanvas(const Graph& graph) :
	mCanvOffset(-400, -20),
	mZoom(1.0)
{
	add_events(Gdk::ALL_EVENTS_MASK);

	std::map<int, int> node2scene;
	for (auto node = graph.begin(); node != graph.end(); ++node)
	{
		auto nodeData = graph.vertexData(node->first);
		int sceneId = mScene.addObject(
				new VertexSceneObject(nodeData->x, nodeData->y, nodeData->radius));
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
	mPanPos = Vector2D(evt->x, evt->y);
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
		Vector2D newPanPos(evt->x, evt->y);
		Vector2D delta = Vector2D(evt->x, evt->y) - mPanPos;
		mCanvOffset += delta;
		mPanPos = newPanPos;
		queue_draw();  // TODO only redraw visible part of scene graph
	}
	
	return true;
}
