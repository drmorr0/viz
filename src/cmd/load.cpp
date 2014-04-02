/*
 * David R. Morrison, March 2014
 */

#include "load.h"
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

LoadCommand::LoadCommand() :
	Command("load [script|graph] Filename")
{
	/* Do nothing */
}

bool LoadCommand::operator()(tok_iter& token, const tok_iter& end)
{
    if (token == end) { VizPrompt::displayError("Too few arguments to load."); return false; }
    string subcommand = trim_copy(*token++);

	if (subcommand == "graph")
	{
		if (token == end) 
			{ VizPrompt::displayError("Too few arguments to load script."); return false; }
		string filename = trim_copy(*token++);
		return loadGraph(filename);
	}
	else if (subcommand == "script")
	{
		if (token == end) 
			{ VizPrompt::displayError("Too few arguments to load script."); return false; }
		string filename = trim_copy(*token++);
		return loadScript(filename);
	}
	else 
	{
		VizPrompt::displayError(string("Unrecognized subcommand to load: ") + subcommand);
		return false;
	}
    return true;
}

bool LoadCommand::loadGraph(const string& filename)
{
	Graph g;
	try { g = readJsonTree(filename); }
	catch (const Error& e) { VizPrompt::displayError(e.what()); return false; }

	TheBuilder::getToplevel()->createTab(filename, g);
	return true;
	VizPrompt::displayError("The load graph subcommand is currently not supported.");
}

bool LoadCommand::loadScript(const string& filename)
{
	ifstream scriptFile(filename);
	if (!scriptFile)
	{
		VizPrompt::displayError(string("Could not open '") + filename + "' for input");
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







