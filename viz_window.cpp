// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "scene_obj.h"

#include <graph_layout.h>
#include <map>

using namespace std;

VizWindow::VizWindow(const vector<Graph>& graphs, Gtk::WindowType wt) :
	Gtk::Window(wt)
{
	set_default_size(600, 400);
	set_position(Gtk::WIN_POS_CENTER);

	for (int i = 0; i < graphs.size(); ++i)
	{
		// TODO - clean up this memory madness!  (smart pointers?)
		mTabContents.push_back(new VizTab(new Graph(graphs[i])));
		mTabContents[i]->show_all();
		mTabLabels.push_back(new Gtk::Label("A tab"));
		mTabManager.append_page(*mTabContents[i], *mTabLabels[i]);
	}
	mTabManager.show_all();
	add(mTabManager);
}

	


