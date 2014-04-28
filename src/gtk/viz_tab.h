#ifndef VIZ_TAB_H
#define VIZ_TAB_H
/*
 * viz_tab.h: David R. Morrison, April 2014
 *
 * XXXX
 */

#include <memory>
#include <string>
#include <graph.h>
using graph::Graph;

class VizCanvas;

class VizTab
{
public:
	VizTab(const Graph& g, const std::string& name);

	void setSelection(const std::vector<int>& sel);
	void addToSelection(const std::vector<int>& sel);
	void delFromSelection(const std::vector<int>& sel);
	void clearSelection();
	std::vector<int> getSelection() const { return mCurrSelection; }

	VizCanvas* canvas() const { return fpCanvas; }
	Graph* graph() const { return pGraph.get(); }

private:
	std::unique_ptr<Graph> pGraph;
	std::vector<int> mCurrSelection;
	VizCanvas* fpCanvas;

	// Selection dialog stuff
//	Glib::RefPtr<ListStore> compareListStore;

	void updateSelectionText();
	void displayComparisonDialog();
};

#endif // VIZ_TAB_H
