// viz_tab.cpp: David R. Morrison, April 2014
// Implementation details for a tab in the viz window

#include "viz_tab.h"
#include "viz_canvas.h"
#include "builder.h"
#include "util.h"

#include <vector>
#include <gtkmm.h>

using namespace std;

VizTab::VizTab(const Graph& g, const string& name) :
	pGraph(new Graph(g))
{
	Gtk::Notebook* notebook = TheBuilder::get<Gtk::Notebook>("viz_tabs");
	Gtk::Frame* tabFrame = Gtk::manage(new Gtk::Frame);
	fpCanvas = Gtk::manage(new VizCanvas(pGraph.get(), this));
	notebook->append_page(*tabFrame, name);
	tabFrame->add(*fpCanvas);
	tabFrame->show_all();

	notebook->set_current_page(-1);

	Gtk::Button* compareBtn = TheBuilder::get<Gtk::Button>("viz_compare_button");
	compareBtn->signal_clicked().connect(sigc::mem_fun(*this, &VizTab::displayComparisonDialog));
	
}

void VizTab::setSelection(const vector<int>& sel)
{
	mCurrSelection = sel;
	updateSelectionText();
}

void VizTab::addToSelection(const vector<int>& sel)
{
	for (int i = 0; i < sel.size(); ++i)
		if (!contains(mCurrSelection, sel[i]))
			mCurrSelection.push_back(sel[i]);
	updateSelectionText();
}

void VizTab::delFromSelection(const vector<int>& sel)
{
	for (int i = 0; i < sel.size(); ++i)
		remove(mCurrSelection, sel[i]);
	updateSelectionText();
}

void VizTab::clearSelection()
{
	mCurrSelection.clear();
	updateSelectionText();
}

// This is horribly inefficient right now, but I have other things I'd rather focus on.
void VizTab::updateSelectionText()
{
    string info = ""; 
    if (mCurrSelection.size() == 1)
    {   
        int gid = mCurrSelection.back();
        graph::VertexData* data = pGraph->vertexData(gid);
        for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
            info += prop->first + ": " + prop->second + "\n";
        TheBuilder::get<Gtk::Button>("viz_compare_button")->hide();
    }   
    else if (mCurrSelection.size() > 1)
    {   
        TheBuilder::get<Gtk::Button>("viz_compare_button")->show();
        info = "You have selected " + to_string(mCurrSelection.size()) + " items.";
    }   
    else TheBuilder::get<Gtk::Button>("viz_compare_button")->hide();
    Gtk::TextView* infoBox = TheBuilder::get<Gtk::TextView>("viz_sel_info_box");
    infoBox->get_buffer()->set_text(info);

	map<string, vector<string>> selectedProperties;
	for (int i = 0; i < mCurrSelection.size(); ++i)
	{
		graph::VertexData* data = pGraph->vertexData(mCurrSelection[i]);
		for (auto prop = data->properties.begin(); prop != data->properties.end(); ++prop)
			selectedProperties[prop->first];
	}

	Gtk::TreeModel::ColumnRecord colRec;
	vector<Gtk::TreeModelColumn<Glib::ustring>*> cols;
	for (int i = 0; i < selectedProperties.size(); ++i)
	{
		cols.push_back(new Gtk::TreeModelColumn<Glib::ustring>);
		colRec.add(*cols.back());
	}
	
	auto listStore = Gtk::ListStore::create(colRec);

	for (int i = 0; i < mCurrSelection.size(); ++i)
	{
		auto row = listStore->append();
		int j = 0;
		for (auto prop = selectedProperties.begin(); prop != selectedProperties.end(); ++prop)
		{
			string val;
			graph::VertexData* data = pGraph->vertexData(mCurrSelection[i]);
			if (data->properties.find(prop->first) != data->properties.end())
				val = data->properties[prop->first];
			row->set_value(j++, val);
		}
	}

	Gtk::TreeView* tv = TheBuilder::get<Gtk::TreeView>("compare_treeview");
	tv->set_model(listStore);
	int i = 0;
	for (auto prop = selectedProperties.begin(); prop != selectedProperties.end(); ++prop)
	{
		tv->append_column(prop->first, *(cols[i]));
		tv->get_column(i++)->set_reorderable();
	}
}

void VizTab::displayComparisonDialog()
{
	TheBuilder::get<Gtk::Dialog>("viz_compare_win")->show();
}




