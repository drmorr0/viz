
#include "viz_window.h"

VizWindow::VizWindow(Graph* graph, Gtk::WindowType wt) :
	Gtk::Window(wt)
{
	set_default_size(600, 400);
	set_position(Gtk::WIN_POS_CENTER);

	mCanvas = new GraphCanvas(graph);
	mCanvas->set_size_request(600,400);

	add(*mCanvas);
	mCanvas->show();
}

GraphCanvas::GraphCanvas(Graph* graph) :
	mCanvasX(-400),
	mCanvasY(-20),
	mZoom(1.0),
	mGraph(graph)
{
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK | Gdk::SCROLL_MASK);
}

bool GraphCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	for (auto node = mGraph->begin(); node != mGraph->end(); ++node)
	{
		auto nodeData = mGraph->vertexData(node->first);

		double nodeCanvasX = mCanvasX + nodeData->center.x * mZoom;
		double nodeCanvasY = mCanvasY + nodeData->center.y * mZoom;
		double nodeCanvasRadius = nodeData->radius * mZoom;
		cr->arc(nodeCanvasX, nodeCanvasY, nodeCanvasRadius, 0, 2 * M_PI);
		cr->stroke();

		for (auto child = node->second.begin(); child != node->second.end(); ++child)
		{
			auto childData = mGraph->vertexData(*child);
			double childCanvasX = mCanvasX + childData->center.x * mZoom;
			double childCanvasY = mCanvasY + childData->center.y * mZoom;
			double childCanvasRadius = childData->radius * mZoom;
			
			double connectorX = childCanvasX - nodeCanvasX;
			double connectorY = childCanvasY - nodeCanvasY;
			double length = sqrt(connectorX * connectorX + connectorY * connectorY);
			connectorX /= length; connectorY /= length;

			cr->move_to(nodeCanvasX + connectorX * nodeCanvasRadius,
					    nodeCanvasY + connectorY * nodeCanvasRadius);
			cr->line_to(childCanvasX - connectorX * childCanvasRadius,
						childCanvasY - connectorY * childCanvasRadius);
			cr->stroke();
		}
	}

	return true;
}

bool GraphCanvas::on_button_press_event(GdkEventButton* evt)
{
	mScrollX = evt->x;
	mScrollY = evt->y;
	return true;
}

bool GraphCanvas::on_scroll_event(GdkEventScroll* evt)
{
	double graphX = 1 / mZoom * (evt->x - mCanvasX);
	double graphY = 1 / mZoom * (evt->y - mCanvasY);
	double oldZoom = mZoom;

	if (evt->direction == GDK_SCROLL_UP) mZoom *= 2;
	else if (evt->direction == GDK_SCROLL_DOWN) mZoom /= 2;

	mCanvasX += graphX * (oldZoom - mZoom);
	mCanvasY += graphY * (oldZoom - mZoom);
	queue_draw();
	return true;
}

bool GraphCanvas::on_motion_notify_event(GdkEventMotion* evt)
{
	if (!(evt->state & GDK_BUTTON1_MASK)) return false;
	else if (evt->is_hint) return true;
	else 
	{
		double deltaX = evt->x - mScrollX;
		double deltaY = evt->y - mScrollY;
		mCanvasX += deltaX; mCanvasY += deltaY;
		queue_draw();
		mScrollX = evt->x;
		mScrollY = evt->y;	
	}
	
	return true;
}
