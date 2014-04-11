// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_canvas.h"
#include "viz_prompt.h"
#include "scene_obj.h"
#include "builder.h"

#include "cmd.h"
#include "format.h"
#include "help.h"
#include "hide.h"
#include "load.h"
#include "save.h"
#include "show.h"
#include "quit.h"

#include <graph.h>

using namespace std;

VizWindow::VizWindow(Gtk::WindowType wt) :
	Gtk::Window(wt),
	pCmdMgr(new CommandManager()),
	pCmdPrompt(new VizPrompt("viz_cmd_prompt", "viz_cmd_output", "viz_cmd_scroll", pCmdMgr.get()))
{
	maximize();
	add(*TheBuilder::get("viz_main_grid"));

	// Register the commands that are accepted by the main command prompt
	pCmdMgr->registerCommand("exit", QuitCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("format", FormatCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("help", HelpCommand(pCmdMgr.get()), pCmdPrompt.get());
	pCmdMgr->registerCommand("hide", HideCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("load", LoadCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("save", SaveCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("show", ShowCommand(), pCmdPrompt.get());
	pCmdMgr->registerCommand("quit", QuitCommand(), pCmdPrompt.get());
}

void VizWindow::load(const string& filename)
{
	((LoadCommand*)pCmdMgr->get("load"))->loadGraph(filename);
}

/*
 * Create a new tab and add it to the notebook.
 */
void VizWindow::createTab(const string& tabName, const Graph& tabContents)
{
	Gtk::Notebook* vizTabs = TheBuilder::get<Gtk::Notebook>("viz_tabs");

	Gtk::Frame* newTab = Gtk::manage(new Gtk::Frame);
	VizCanvas* tabCanvas = Gtk::manage(new VizCanvas(new Graph(tabContents)));
	vizTabs->append_page(*newTab, tabName);
	newTab->add(*tabCanvas);
	newTab->show_all();

	// Negative numbers set to the last page
	vizTabs->set_current_page(-1);
}

	


