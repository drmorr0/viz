/*
 * David R. Morrison, March 2014
 */

#include "load.h"
#include "viz_tab.h"
#include "viz_canvas.h"
#include "viz_window.h"
#include "viz_prompt.h"
#include "viz_io.h"
#include "builder.h"

#include <graph.h>

#include <fstream>
#include <string>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

const CmdStructure<string> loadGraphCmd(
		vector<CmdHelp>{{"filename", "specify a file from which to load a new graph"}}
	);

const CmdStructure<string> loadScriptCmd(
		vector<CmdHelp>{{"filename", "specify a script file to execute"}}
	);

LoadCommand::LoadCommand() :
	Command("load", "Read in a new graph or script file",
			{{"graph", &loadGraphCmd},
			 {"script", &loadScriptCmd}})
{
	/* Do nothing */
}

bool LoadCommand::operator()(tok_iter& token, const tok_iter& end)
{
    if (token == end) { fpOutput->writeError("Too few arguments to load."); return false; }
    string subc = trim_copy(*token++);

	if (mSubCommands.count(subc) == 0)
	{
		fpOutput->writeError("Invalid subcommand " + subc);
		return false;
	}

	string subcName = mCmdName + " " + subc;
	string filename;
	
	if (subc == "graph")
	{
		loadGraphCmd.parse(subcName, fpOutput, token, end, filename);
		return loadGraph(filename);
	}
	else if (subc == "script")
	{
		loadScriptCmd.parse(subcName, fpOutput, token, end, filename);
		return loadScript(filename);
	}
	
    return true;
}

bool LoadCommand::loadGraph(const string& filename)
{
	Graph g;
	try { g = readJsonTree(filename); }
	catch (const Error& e) { fpOutput->writeError(e.what()); return false; }

	TheBuilder::getToplevel()->createTab(filename, g);
	return true;
}

bool LoadCommand::loadScript(const string& filename)
{
	ifstream scriptFile(filename);
	if (!scriptFile)
	{
		fpOutput->writeError(string("Could not open '") + filename + "' for input");
		return false;
	}

	Gtk::Entry* prompt = TheBuilder::get<Gtk::Entry>("viz_cmd_prompt");
	string line;
	while (scriptFile.good())
	{
		// '%' is a comment; erase everything following it (we can't use '#', because that's
		// how we specify colors in HTML syntax);
		getline(scriptFile, line);
		int hashPos = line.find("%");	
		if (hashPos != string::npos) line.erase(hashPos);
		if (line.empty()) continue;
		prompt->set_text(line);
		prompt->activate();
	}

	return true;
}







