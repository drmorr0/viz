
#include "viz_window.h"
#include "vertex.h"

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
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
	for (auto node = graph.begin(); node != graph.end(); ++node)
	{
		auto nodeData = graph.vertexData(node->first);
		mScene.addObject(new VertexSceneObject(nodeData->x, nodeData->y, nodeData->radius));
	}
}

bool GraphCanvas::on_draw(const CairoContext& ctx)
{
	mScene.render(ctx, mCanvOffset, mZoom);
/*	for (auto node = mGraph->begin(); node != mGraph->end(); ++node)
	{
		auto nodeData = mGraph->vertexData(node->first);

		// Translate from the position in graph-embedding space to canvas space
		Vector2D nodePos(nodeData->x, nodeData->y);
		Vector2D nodeCanvPos = mCanvOffset + mZoom * nodePos;
		double nodeCanvRadius = nodeData->radius * mZoom;

		// Draw a circle for the node
		cr->arc(nodeCanvPos.x, nodeCanvPos.y, nodeCanvRadius, 0, 2 * M_PI);
		cr->stroke();

		for (auto child = node->second.begin(); child != node->second.end(); ++child)
		{
			auto childData = mGraph->vertexData(*child);
			Vector2D childPos(childData->x, childData->y);
			Vector2D childCanvPos = mCanvOffset + mZoom * childPos;
			double childCanvRadius = childData->radius * mZoom;
			
			Vector2D connector = childCanvPos - nodeCanvPos;
			normalize(connector); 
			Vector2D edgeStart = nodeCanvRadius * connector;
			Vector2D edgeEnd = -1 * childCanvRadius * connector;

			cr->move_to(nodeCanvPos.x + edgeStart.x, nodeCanvPos.y + edgeStart.y);
			cr->line_to(childCanvPos.x + edgeEnd.x, childCanvPos.y + edgeEnd.y);
			cr->stroke();
		}
	}*/

	return true;
}

bool GraphCanvas::on_button_press_event(GdkEventButton* evt)
{
	mPanPos = Vector2D(evt->x, evt->y);
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
