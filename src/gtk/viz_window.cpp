// viz_window.cpp: David R. Morrison, March 2014
// Implementation details for the VizWindow class

#include "viz_window.h"
#include "viz_prompt.h"
#include "viz_tab.h"
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

VizWindow::~VizWindow()
{
	for (int i = 0; i < mpTabs.size(); ++i)
		delete mpTabs[i];
}

void VizWindow::load(const string& filename)
{
	((LoadCommand*)pCmdMgr->get("load"))->loadGraph(filename);
}

void VizWindow::createTab(const string& name, const Graph& g)
{
	mpTabs.push_back(new VizTab(g, name));
}


	


